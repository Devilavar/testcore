/*
 * Copyright (C) 2005-2011 MaNGOS <http://getmangos.com/>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include "PathGenerator.h"
#include "Map.h"
#include "Creature.h"
#include "MMapFactory.h"
#include "MMapManager.h"
#include "Log.h"

#include "DetourCommon.h"
#include "DetourNavMeshQuery.h"

////////////////// PathGenerator //////////////////
PathGenerator::PathGenerator(const Unit* owner) :
    _polyLength(0), _type(PATHFIND_BLANK), _useStraightPath(false),
    _forceDestination(false), _pathLengthLimit(MAX_PATH_DISTANCE), _straightLine(false),
    _endPosition(G3D::Vector3::zero()), _sourceUnit(owner), _navMesh(NULL),
    _navMeshQuery(NULL)
{
    memset(_pathPolyRefs, 0, sizeof(_pathPolyRefs));

    TC_LOG_DEBUG("maps", "++ PathGenerator::PathGenerator for %u \n", _sourceUnit->GetGUIDLow());

    uint32 mapId = _sourceUnit->GetMapId();
    if (MMAP::MMapFactory::IsPathfindingEnabled(mapId))
    {
        MMAP::MMapManager* mmap = MMAP::MMapFactory::createOrGetMMapManager();
        _navMesh = mmap->GetNavMesh(mapId);
        _navMeshQuery = mmap->GetNavMeshQuery(mapId, _sourceUnit->GetInstanceId());
    }

    CreateFilter();
}

PathGenerator::~PathGenerator()
{
    TC_LOG_DEBUG("maps", "++ PathGenerator::~PathGenerator() for %u \n", _sourceUnit->GetGUIDLow());
}

bool PathGenerator::CalculatePath(float destX, float destY, float destZ, bool forceDest, bool straightLine)
{
    float x, y, z;
    _sourceUnit->GetPosition(x, y, z);

    if (!Trinity::IsValidMapCoord(destX, destY, destZ) || !Trinity::IsValidMapCoord(x, y, z))
        return false;

    G3D::Vector3 dest(destX, destY, destZ);
    SetEndPosition(dest);

    G3D::Vector3 start(x, y, z);
    SetStartPosition(start);

    _forceDestination = forceDest;
    _straightLine = straightLine;

    TC_LOG_DEBUG("maps", "++ PathGenerator::CalculatePath() for %u \n", _sourceUnit->GetGUIDLow());

    // make sure navMesh works - we can run on map w/o mmap
    // check if the start and end point have a .mmtile loaded (can we pass via not loaded tile on the way?)
    if (!_navMesh || !_navMeshQuery || _sourceUnit->HasUnitState(UNIT_STATE_IGNORE_PATHFINDING) ||
        !HaveTile(start) || !HaveTile(dest) || DisableMgr::IsDisabledFor(DISABLE_TYPE_MMAP, _sourceUnit->GetMapId(), NULL) ||
        ((_sourceUnit->GetMap()->IsInAir(_sourceUnit->GetPhaseMask(), x, y, z) ||
        _sourceUnit->GetMap()->IsInAir(_sourceUnit->GetPhaseMask(), destX, destY, destZ)) && (_sourceUnit->CanFly() || _sourceUnit->HasUnitMovementFlag(MOVEMENTFLAG_CAN_FLY))))
    {
        BuildShortcut();
        _type = PathType(PATHFIND_NORMAL | PATHFIND_NOT_USING_PATH);
        return true;
    }

    UpdateFilter();

    BuildPolyPath(start, dest);
    return true;
}

dtPolyRef PathGenerator::GetPathPolyByPosition(dtPolyRef const* polyPath, uint32 polyPathSize, float const* point, float* distance) const
{
    if (!polyPath || !polyPathSize)
        return INVALID_POLYREF;

    dtPolyRef nearestPoly = INVALID_POLYREF;
    float minDist2d = FLT_MAX;
    float minDist3d = 0.0f;

    for (uint32 i = 0; i < polyPathSize; ++i)
    {
        float closestPoint[VERTEX_SIZE];
        if (dtStatusFailed(_navMeshQuery->closestPointOnPoly(polyPath[i], point, closestPoint, NULL)))
            continue;

        float d = dtVdist2DSqr(point, closestPoint);
        if (d < minDist2d)
        {
            minDist2d = d;
            nearestPoly = polyPath[i];
            minDist3d = dtVdistSqr(point, closestPoint);
        }

        if (minDist2d < 1.0f) // shortcut out - close enough for us
            break;
    }

    if (distance)
        *distance = dtSqrt(minDist3d);

    return (minDist2d < 3.0f) ? nearestPoly : INVALID_POLYREF;
}

dtPolyRef PathGenerator::GetPolyByLocation(float const* point, float* distance) const
{
    // first we check the current path
    // if the current path doesn't contain the current poly,
    // we need to use the expensive navMesh.findNearestPoly
    dtPolyRef polyRef = GetPathPolyByPosition(_pathPolyRefs, _polyLength, point, distance);
    if (polyRef != INVALID_POLYREF)
        return polyRef;

    // we don't have it in our old path
    // try to get it by findNearestPoly()
    // first try with low search box
    float extents[VERTEX_SIZE] = {3.0f, 5.0f, 3.0f};    // bounds of poly search area
    float closestPoint[VERTEX_SIZE] = {0.0f, 0.0f, 0.0f};
    if (dtStatusSucceed(_navMeshQuery->findNearestPoly(point, extents, &_filter, &polyRef, closestPoint)) && polyRef != INVALID_POLYREF)
    {
        *distance = dtVdist(closestPoint, point);
        return polyRef;
    }

    // still nothing ..
    // try with bigger search box
    // Note that the extent should not overlap more than 128 polygons in the navmesh (see dtNavMeshQuery::findNearestPoly)
    extents[1] = 50.0f;

    if (dtStatusSucceed(_navMeshQuery->findNearestPoly(point, extents, &_filter, &polyRef, closestPoint)) && polyRef != INVALID_POLYREF)
    {
        *distance = dtVdist(closestPoint, point);
        return polyRef;
    }

    return INVALID_POLYREF;
}

void PathGenerator::BuildPolyPath(G3D::Vector3 const& startPos, G3D::Vector3 const& endPos)
{
    // *** getting start/end poly logic ***

    float distToStartPoly, distToEndPoly;
    float startPoint[VERTEX_SIZE] = {startPos.y, startPos.z, startPos.x};
    float endPoint[VERTEX_SIZE] = {endPos.y, endPos.z, endPos.x};

    dtPolyRef startPoly = GetPolyByLocation(startPoint, &distToStartPoly);
    dtPolyRef endPoly = GetPolyByLocation(endPoint, &distToEndPoly);

    // we have a hole in our mesh
    // make shortcut path and mark it as NOPATH ( with flying exception )
    // its up to caller how he will use this info
    if (startPoly == INVALID_POLYREF || endPoly == INVALID_POLYREF)
    {
        TC_LOG_DEBUG("maps", "++ BuildPolyPath :: (startPoly == 0 || endPoly == 0)\n");
        BuildShortcut();
        bool path = _sourceUnit->GetTypeId() == TYPEID_UNIT && _sourceUnit->ToCreature()->CanFly();

        _type = path ? PathType(PATHFIND_NORMAL | PATHFIND_NOT_USING_PATH) : PATHFIND_NOPATH;
        return;
    }

    /*
    // If either current position, or end position is under water, then build a shortcut path
    ZLiquidStatus status = _sourceUnit->GetBaseMap()->getLiquidStatus(endPos.x, endPos.y, endPos.z, MAP_ALL_LIQUIDS, NULL);
    if (_sourceUnit->GetBaseMap()->IsUnderWater(_sourceUnit->GetPositionX(), _sourceUnit->GetPositionY(), _sourceUnit->GetPositionZ()) || status == LIQUID_MAP_UNDER_WATER)
    {
        _buildShortcut();
        _type = PathType(PATHFIND_NORMAL | PATHFIND_NOT_USING_PATH);
        return;
    }
    */

    // we may need a better number here
    bool farFromPoly = (distToStartPoly > 7.0f || distToEndPoly > 7.0f);
    if (farFromPoly)
    {
        TC_LOG_DEBUG("maps", "++ BuildPolyPath :: farFromPoly distToStartPoly=%.3f distToEndPoly=%.3f\n", distToStartPoly, distToEndPoly);

        float closestPoint[VERTEX_SIZE];
        // we may want to use closestPointOnPolyBoundary instead
        if (dtStatusSucceed(_navMeshQuery->closestPointOnPoly(endPoly, endPoint, closestPoint, NULL)))
        {
            dtVcopy(endPoint, closestPoint);
            SetActualEndPosition(G3D::Vector3(endPoint[2],endPoint[0],endPoint[1]));
        }

        _type = PATHFIND_INCOMPLETE;
    }

    // *** poly path generating logic ***

    // start and end are on same polygon
    // just need to move in straight line
    if (startPoly == endPoly)
    {
        TC_LOG_DEBUG("maps", "++ BuildPolyPath :: (startPoly == endPoly)\n");

        BuildShortcut();

        _pathPolyRefs[0] = startPoly;
        _polyLength = 1;

        _type = farFromPoly ? PATHFIND_INCOMPLETE : PATHFIND_NORMAL;
        TC_LOG_DEBUG("maps", "++ BuildPolyPath :: path type %d\n", _type);
        return;
    }

    // look for startPoly/endPoly in current path
    // TODO: we can merge it with getPathPolyByPosition() loop
    bool startPolyFound = false;
    bool endPolyFound = false;
    uint32 pathStartIndex = 0;
    uint32 pathEndIndex = 0;

    if (_polyLength)
    {
        for (; pathStartIndex < _polyLength; ++pathStartIndex)
        {
            // here to carch few bugs
            ASSERT(_pathPolyRefs[pathStartIndex] != INVALID_POLYREF);

            if (_pathPolyRefs[pathStartIndex] == startPoly)
            {
                startPolyFound = true;
                break;
            }
        }

        for (pathEndIndex = _polyLength-1; pathEndIndex > pathStartIndex; --pathEndIndex)
            if (_pathPolyRefs[pathEndIndex] == endPoly)
            {
                endPolyFound = true;
                break;
            }
    }

    if (startPolyFound && endPolyFound)
    {
        TC_LOG_DEBUG("maps", "++ BuildPolyPath :: (startPolyFound && endPolyFound)\n");

        // we moved along the path and the target did not move out of our old poly-path
        // our path is a simple subpath case, we have all the data we need
        // just "cut" it out

        _polyLength = pathEndIndex - pathStartIndex + 1;
        memmove(_pathPolyRefs, _pathPolyRefs+pathStartIndex, _polyLength*sizeof(dtPolyRef));
    }
    else
    {
        TC_LOG_DEBUG("maps", "++ BuildPolyPath :: (!startPolyFound && !endPolyFound)\n");

        // either we have no path at all -> first run
        // or something went really wrong -> we aren't moving along the path to the target
        // just generate new path

        // free and invalidate old path data
        _clear();

        dtStatus dtResult = _navMeshQuery->findPath(
                startPoly,          // start polygon
                endPoly,            // end polygon
                startPoint,         // start position
                endPoint,           // end position
                &_filter,           // polygon search filter
                _pathPolyRefs,     // [out] path
                (int*)&_polyLength,
                MAX_POINT_PATH_POINTS);   // max number of polygons in output path

        if (!_polyLength || dtResult != DT_SUCCESS)
        {
            // only happens if we passed bad data to findPath(), or navmesh is messed up
            TC_LOG_DEBUG("maps", "%u's Path Build failed: 0 length path", _sourceUnit->GetGUIDLow());
            BuildShortcut();
            _type = PATHFIND_NOPATH;
            return;
        }
    }

    // by now we know what type of path we can get
    if (_pathPolyRefs[_polyLength - 1] == endPoly && !(_type & PATHFIND_INCOMPLETE))
        _type = PATHFIND_NORMAL;
    else
        _type = PATHFIND_INCOMPLETE;

    // generate the point-path out of our up-to-date poly-path
    BuildPointPath(startPoint, endPoint);
}

void PathGenerator::BuildPointPath(const float *startPoint, const float *endPoint)
{
    float pathPoints[MAX_POINT_PATH_POINTS*VERTEX_SIZE];
    uint32 pointCount = 0;
    dtStatus dtResult = DT_FAILURE;
    if (_straightLine)
    {
        // if the path is a straight line then start and end position are enough
        dtResult = DT_SUCCESS;
        pointCount = 2;
        memcpy(&pathPoints[0], startPoint, sizeof(float)* 3);
        memcpy(&pathPoints[3], endPoint, sizeof(float)* 3);
    }
    else
    {
        dtResult = FindSmartPath(
                startPoint,         // start position
                endPoint,           // end position
                _pathPolyRefs,     // current path
                _polyLength,       // length of current path
                pathPoints,         // [out] path corner points
                (int*)&pointCount,
                _pathLengthLimit);    // maximum length of path
    }

    if (pointCount < 2 || dtStatusFailed(dtResult))
    {
        // only happens if pass bad data to findStraightPath or navmesh is broken
        // single point paths can be generated here
        /// @todo check the exact cases
        TC_LOG_DEBUG("maps", "++ PathGenerator::BuildPointPath FAILED! path sized %d returned\n", pointCount);
        BuildShortcut();
        _type = PATHFIND_NOPATH;
        return;
    }
    else if (dtResult & DT_BUFFER_TOO_SMALL)
        _type = PATHFIND_SHORT;

    _pathPoints.resize(pointCount);

    for (uint32 i = 0; i < pointCount; ++i)
        _pathPoints[i] = G3D::Vector3(pathPoints[i*VERTEX_SIZE + 2], pathPoints[i*VERTEX_SIZE], pathPoints[i*VERTEX_SIZE + 1]);

    //NormalizePath();

    // first point is always our current location - we need the next one
    SetActualEndPosition(_pathPoints[pointCount-1]);

    // force the given destination, if needed
    if (_forceDestination &&
        (!(_type & PATHFIND_NORMAL) || !InRange(GetEndPosition(), GetActualEndPosition(), 1.0f, 1.0f)))
    {
        // we may want to keep partial subpath
        if (Dist3DSqr(GetActualEndPosition(), GetEndPosition()) < 0.3f * Dist3DSqr(GetStartPosition(), GetEndPosition()))
        {
            SetActualEndPosition(GetEndPosition());
            _pathPoints[_pathPoints.size()-1] = GetEndPosition();
        }
        else
        {
            SetActualEndPosition(GetEndPosition());
            BuildShortcut();
        }

        _type = PathType(PATHFIND_NORMAL | PATHFIND_NOT_USING_PATH);
    }

    TC_LOG_DEBUG("maps", "++ PathGenerator::BuildPointPath path type %d size %d poly-size %d\n", _type, pointCount, _polyLength);
}

void PathGenerator::NormalizePath()
{
    for (uint32 i = 0; i < _pathPoints.size(); ++i)
        _sourceUnit->UpdateAllowedPositionZ(_pathPoints[i].x, _pathPoints[i].y, _pathPoints[i].z);
}

void PathGenerator::NormalizePath(float* path, uint32 pathSize)
{
    for (uint32 i = 0; i < pathSize; ++i)
        _sourceUnit->UpdateAllowedPositionZ(path[i*VERTEX_SIZE+2], path[i*VERTEX_SIZE+0], path[i*VERTEX_SIZE+1]);
}

void PathGenerator::BuildShortcut()
{
    TC_LOG_DEBUG("maps", "++ BuildShortcut :: making shortcut\n");

    Clear();

    // make two point path, our curr pos is the start, and dest is the end
    _pathPoints.resize(2);

    // set start and a default next position
    _pathPoints[0] = GetStartPosition();
    _pathPoints[1] = GetActualEndPosition();

    NormalizePath();

    _type = PATHFIND_SHORTCUT;
}

void PathGenerator::CreateFilter()
{
    uint16 includeFlags = 0;
    uint16 excludeFlags = 0;

    if (_sourceUnit->GetTypeId() == TYPEID_UNIT)
    {
        Creature* creature = (Creature*)_sourceUnit;
        if (creature->CanWalk())
            includeFlags |= NAV_GROUND;          // walk

        // creatures don't take environmental damage
        if (creature->CanSwim())
            includeFlags |= (NAV_WATER | NAV_MAGMA | NAV_SLIME);           // swim
    }
    else // assume Player
    {
        // perfect support not possible, just stay 'safe'
        includeFlags |= (NAV_GROUND | NAV_WATER | NAV_MAGMA | NAV_SLIME);
    }

    _filter.setIncludeFlags(includeFlags);
    _filter.setExcludeFlags(excludeFlags);

    UpdateFilter();
}

void PathGenerator::UpdateFilter()
{
    // allow creatures to cheat and use different movement types if they are moved
    // forcefully into terrain they can't normally move in
    if (_sourceUnit->IsInWater() || _sourceUnit->IsUnderWater())
    {
        uint16 includedFlags = _filter.getIncludeFlags();
        includedFlags |= GetNavTerrain(_sourceUnit->GetPositionX(),
                                       _sourceUnit->GetPositionY(),
                                       _sourceUnit->GetPositionZ());

        _filter.setIncludeFlags(includedFlags);
    }
}

NavTerrain PathGenerator::GetNavTerrain(float x, float y, float z)
{
    LiquidData data;
    ZLiquidStatus liquidStatus = _sourceUnit->GetBaseMap()->getLiquidStatus(x, y, z, MAP_ALL_LIQUIDS, &data);
    if (liquidStatus == LIQUID_MAP_NO_WATER)
        return NAV_GROUND;

    switch (data.type_flags)
    {
        case MAP_LIQUID_TYPE_WATER:
        case MAP_LIQUID_TYPE_OCEAN:
            return NAV_WATER;
        case MAP_LIQUID_TYPE_MAGMA:
            return NAV_MAGMA;
        case MAP_LIQUID_TYPE_SLIME:
            return NAV_SLIME;
        default:
            return NAV_GROUND;
    }
}

bool PathGenerator::HaveTile(const G3D::Vector3& p) const
{
    int tx = -1, ty = -1;
    float point[VERTEX_SIZE] = {p.y, p.z, p.x};

    _navMesh->calcTileLoc(point, &tx, &ty);

    /// Workaround
    /// For some reason, often the tx and ty variables wont get a valid value
    /// Use this check to prevent getting negative tile coords and crashing on getTileAt
    if (tx < 0 || ty < 0)
        return false;

    return (_navMesh->getTileAt(tx, ty, 0) != NULL);
}

uint32 PathGenerator::FixupCorridor(dtPolyRef* path, uint32 npath, uint32 maxPath, dtPolyRef const* visited, uint32 nvisited)
{
    int32 furthestPath = -1;
    int32 furthestVisited = -1;

    // Find furthest common polygon.
    for (int32 i = npath-1; i >= 0; --i)
    {
        bool found = false;
        for (int32 j = nvisited-1; j >= 0; --j)
        {
            if (path[i] == visited[j])
            {
                furthestPath = i;
                furthestVisited = j;
                found = true;
            }
        }
        if (found)
            break;
    }

    // If no intersection found just return current path.
    if (furthestPath == -1 || furthestVisited == -1)
        return npath;

    // Concatenate paths.

    // Adjust beginning of the buffer to include the visited.
    uint32 req = nvisited - furthestVisited;
    uint32 orig = uint32(furthestPath + 1) < npath ? furthestPath + 1 : npath;
    uint32 size = npath > orig ? npath - orig : 0;
    if (req + size > maxPath)
        size = maxPath-req;

    if (size)
        memmove(path + req, path + orig, size * sizeof(dtPolyRef));

    // Store visited
    for (uint32 i = 0; i < req; ++i)
        path[i] = visited[(nvisited - 1) - i];

    return req+size;
}

bool PathGenerator::GetSteerTarget(float const* startPos, float const* endPos,
                              float minTargetDist, dtPolyRef const* path, uint32 pathSize,
                              float* steerPos, unsigned char& steerPosFlag, dtPolyRef& steerPosRef)
{
    // Find steer target.
    static const uint32 MAX_STEER_POINTS = 3;
    float steerPath[MAX_STEER_POINTS*VERTEX_SIZE];
    unsigned char steerPathFlags[MAX_STEER_POINTS];
    dtPolyRef steerPathPolys[MAX_STEER_POINTS];
    uint32 nsteerPath = 0;
    dtStatus dtResult = _navMeshQuery->findStraightPath(startPos, endPos, path, pathSize,
                                                steerPath, steerPathFlags, steerPathPolys, (int*)&nsteerPath, MAX_STEER_POINTS);
    if (!nsteerPath || dtStatusFailed(dtResult))
        return false;

    // Find vertex far enough to steer to.
    uint32 ns = 0;
    while (ns < nsteerPath)
    {
        // Stop at Off-Mesh link or when point is further than slop away.
        if ((steerPathFlags[ns] & DT_STRAIGHTPATH_OFFMESH_CONNECTION) ||
            !InRangeYZX(&steerPath[ns*VERTEX_SIZE], startPos, minTargetDist, 1000.0f))
            break;
        ns++;
    }
    // Failed to find good point to steer to.
    if (ns >= nsteerPath)
        return false;

    dtVcopy(steerPos, &steerPath[ns*VERTEX_SIZE]);
    steerPos[1] = startPos[1];  // keep Z value
    steerPosFlag = steerPathFlags[ns];
    steerPosRef = steerPathPolys[ns];

    return true;
}

dtStatus PathGenerator::FindSmoothPath(float const* startPos, float const* endPos,
                                     dtPolyRef const* polyPath, uint32 polyPathSize,
                                     float* smoothPath, int* smoothPathSize, uint32 maxSmoothPathSize)
{
    *smoothPathSize = 0;
    uint32 nsmoothPath = 0;

    dtPolyRef polys[MAX_POINT_PATH_POINTS];
    memcpy(polys, polyPath, sizeof(dtPolyRef)*polyPathSize);
    uint32 npolys = polyPathSize;

    float iterPos[VERTEX_SIZE], targetPos[VERTEX_SIZE];
    if (dtStatusFailed(_navMeshQuery->closestPointOnPolyBoundary(polys[0], startPos, iterPos)))
        return DT_FAILURE;

    if (dtStatusFailed(_navMeshQuery->closestPointOnPolyBoundary(polys[npolys-1], endPos, targetPos)))
        return DT_FAILURE;

    dtVcopy(&smoothPath[nsmoothPath*VERTEX_SIZE], iterPos);
    nsmoothPath++;

    // Move towards target a small advancement at a time until target reached or
    // when ran out of memory to store the path.
    while (npolys && nsmoothPath < maxSmoothPathSize)
    {
        // Find location to steer towards.
        float steerPos[VERTEX_SIZE];
        unsigned char steerPosFlag;
        dtPolyRef steerPosRef = INVALID_POLYREF;

        if (!GetSteerTarget(iterPos, targetPos, SMOOTH_PATH_SLOP, polys, npolys, steerPos, steerPosFlag, steerPosRef))
            break;

        bool endOfPath = (steerPosFlag & DT_STRAIGHTPATH_END);
        bool offMeshConnection = (steerPosFlag & DT_STRAIGHTPATH_OFFMESH_CONNECTION);

        // Find movement delta.
        float delta[VERTEX_SIZE];
        dtVsub(delta, steerPos, iterPos);
        float len = dtSqrt(dtVdot(delta, delta));
        // If the steer target is end of path or off-mesh link, do not move past the location.
        if ((endOfPath || offMeshConnection) && len < SMOOTH_PATH_STEP_SIZE)
            len = 1.0f;
        else
            len = SMOOTH_PATH_STEP_SIZE / len;

        float moveTgt[VERTEX_SIZE];
        dtVmad(moveTgt, iterPos, delta, len);

        // Move
        float result[VERTEX_SIZE];
        const static uint32 MAX_VISIT_POLY = 16;
        dtPolyRef visited[MAX_VISIT_POLY];

        uint32 nvisited = 0;
        _navMeshQuery->moveAlongSurface(polys[0], iterPos, moveTgt, &_filter, result, visited, (int*)&nvisited, MAX_VISIT_POLY);
        npolys = FixupCorridor(polys, npolys, MAX_POINT_PATH_POINTS, visited, nvisited);

        _navMeshQuery->getPolyHeight(polys[0], result, &result[1]);
        result[1] += 0.5f;
        dtVcopy(iterPos, result);

        // Handle end of path and off-mesh links when close enough.
        if (endOfPath && InRangeYZX(iterPos, steerPos, SMOOTH_PATH_SLOP, 1.0f))
        {
            // Reached end of path.
            dtVcopy(iterPos, targetPos);
            if (nsmoothPath < maxSmoothPathSize)
            {
                dtVcopy(&smoothPath[nsmoothPath*VERTEX_SIZE], iterPos);
                nsmoothPath++;
            }
            break;
        }
        else if (offMeshConnection && InRangeYZX(iterPos, steerPos, SMOOTH_PATH_SLOP, 1.0f))
        {
            // Advance the path up to and over the off-mesh connection.
            dtPolyRef prevRef = INVALID_POLYREF;
            dtPolyRef polyRef = polys[0];
            uint32 npos = 0;
            while (npos < npolys && polyRef != steerPosRef)
            {
                prevRef = polyRef;
                polyRef = polys[npos];
                npos++;
            }

            for (uint32 i = npos; i < npolys; ++i)
                polys[i-npos] = polys[i];

            npolys -= npos;

            // Handle the connection.
            float startPos[VERTEX_SIZE], endPos[VERTEX_SIZE];
            if (dtStatusSucceed(_navMesh->getOffMeshConnectionPolyEndPoints(prevRef, polyRef, startPos, endPos)))
            {
                if (nsmoothPath < maxSmoothPathSize)
                {
                    dtVcopy(&smoothPath[nsmoothPath*VERTEX_SIZE], startPos);
                    nsmoothPath++;
                }
                // Move position at the other side of the off-mesh link.
                dtVcopy(iterPos, endPos);
                _navMeshQuery->getPolyHeight(polys[0], iterPos, &iterPos[1]);
                iterPos[1] += 0.5f;
            }
        }

        // Store results.
        if (nsmoothPath < maxSmoothPathSize)
        {
            dtVcopy(&smoothPath[nsmoothPath*VERTEX_SIZE], iterPos);
            nsmoothPath++;
        }
    }

    *smoothPathSize = nsmoothPath;

    // this is most likely a loop
    return nsmoothPath < MAX_POINT_PATH_POINTS ? DT_SUCCESS : DT_FAILURE;
}

bool PathGenerator::InRangeYZX(const float* v1, const float* v2, float r, float h) const
{
    const float dx = v2[0] - v1[0];
    const float dy = v2[1] - v1[1]; // elevation
    const float dz = v2[2] - v1[2];
    return (dx * dx + dz * dz) < r * r && fabsf(dy) < h;
}

static float GetAngle(float* p1, float*p2, float* p3, bool is3D)
{
    float dasq, dbsq, dcsq, ang;
    if (!is3D)
    {
        dasq = ((p2[2] - p3[2]) * (p2[2] - p3[2])) + ((p2[0] - p3[0]) * (p2[0] - p3[0]));
        dbsq = ((p3[2] - p1[2]) * (p3[2] - p1[2])) + ((p3[0] - p1[0]) * (p3[0] - p1[0]));
        dcsq = ((p1[2] - p2[2]) * (p1[2] - p2[2])) + ((p1[0] - p2[0]) * (p1[0] - p2[0]));
        ang = acos((dcsq + dasq - dbsq) / (2 * sqrt(dcsq) * sqrt(dasq)));
    }
    else
    {
        G3D::Vector3 v1 = G3D::Vector3(p1[2] - p2[2], p1[0] - p2[0], p1[1] - p2[1]);
        G3D::Vector3 v2 = G3D::Vector3(p3[2] - p2[2], p3[0] - p2[0], p3[1] - p2[1]);

        float v1mag = sqrt(v1.x * v1.x + v1.y * v1.y + v1.z * v1.z);
        G3D::Vector3 v1norm = G3D::Vector3(v1.x / v1mag, v1.y / v1mag, v1.z / v1mag);

        float v2mag = sqrt(v2.x * v2.x + v2.y * v2.y + v2.z * v2.z);
        G3D::Vector3 v2norm = G3D::Vector3(v2.x / v2mag, v2.y / v2mag, v2.z / v2mag);

        float res = v1norm.x * v2norm.x + v1norm.y * v2norm.y + v1norm.z * v2norm.z;
        ang = acos(res);

    }

    return ang * (180 / M_PI);
}

dtStatus PathGenerator::FindSmartPath(const float* startPos, const float* endPos,
                                     const dtPolyRef* polyPath, uint32 polyPathSize,
                                     float* smartPath, int* pointCount, float maxPathLength)
{
    float pathPoints[MAX_POINT_PATH_POINTS*VERTEX_SIZE];
    dtStatus dtResult = _navMeshQuery->findStraightPath(
            startPos,           // start position
            endPos,             // end position
            polyPath,           // current path
            polyPathSize,       // lenth of current path
            pathPoints,         // [out] path corner points
            NULL,               // [out] flags
            NULL,               // [out] shortened path
            pointCount,         // [out] number of points
            MAX_POINT_PATH_POINTS,
            DT_STRAIGHTPATH_ALL_CROSSINGS);

    if (*pointCount < 2 || dtStatusFailed(dtResult))
        return dtResult;

    if (dtResult & DT_BUFFER_TOO_SMALL)
        return dtResult;

    //float whitepath[MAX_WHITE_PATH_LENGTH*VERTEX_SIZE];
    //if (!GenerateWhitePoints(pathPoints, pointCount, whitepath))
        //return DT_FAILURE;

    NormalizePath(pathPoints, *pointCount);

    if (!GenerateSmartPoints(pathPoints, pointCount, smartPath))
        return DT_BUFFER_TOO_SMALL;

    float pathLength = CalculateLength(smartPath, *pointCount);

    if (maxPathLength)
        return pathLength <= maxPathLength ? dtResult : DT_BUFFER_TOO_SMALL;
    else return dtResult;
}

static float GetDist(const G3D::Vector3 &p1, const G3D::Vector3 &p2)
{
    float res = ((p2.x - p1.x) * (p2.x - p1.x)) + ((p2.y - p1.y) * (p2.y - p1.y));
    return sqrt(res);
}

float PathGenerator::CalculateLength(float* pathPoints, uint32 pathPointsSize)
{
    float dist = 0.0f;
    for (uint32 i = 1; i < pathPointsSize; ++i)
    {
        //TODO: should this be 3d?
        float _dist = GetDist(G3D::Vector3(pathPoints[i*VERTEX_SIZE + 2], pathPoints[i*VERTEX_SIZE + 0], pathPoints[i*VERTEX_SIZE + 1]), G3D::Vector3(pathPoints[(i - 1)*VERTEX_SIZE + 2], pathPoints[(i - 1)*VERTEX_SIZE + 0], pathPoints[(i - 1)*VERTEX_SIZE + 1]));
        dist += _dist;
    }

    return dist;
}

bool PathGenerator::GenerateWhitePoints(float* pathPoints, int* pathPointsSize, float* returnPath)
{
    uint32 npath = 0;
    uint32 nreturnpath = 0;
    uint32 maxpath = (uint32)*pathPointsSize;

    float iterPos[VERTEX_SIZE], nextPos[VERTEX_SIZE];
    while (npath < maxpath - 1)
    {
        if (nreturnpath >= MAX_WHITE_PATH_LENGTH)
            return false;

        returnPath[nreturnpath*VERTEX_SIZE+0] = pathPoints[npath*VERTEX_SIZE+0];
        returnPath[nreturnpath*VERTEX_SIZE+1] = pathPoints[npath*VERTEX_SIZE+1];
        returnPath[nreturnpath*VERTEX_SIZE+2] = pathPoints[npath*VERTEX_SIZE+2];
        ++nreturnpath;

        iterPos[0] = pathPoints[npath*VERTEX_SIZE+0];
        iterPos[1] = pathPoints[npath*VERTEX_SIZE+1];
        iterPos[2] = pathPoints[npath*VERTEX_SIZE+2];

        nextPos[0] = pathPoints[(npath+1)*VERTEX_SIZE+0];
        nextPos[1] = pathPoints[(npath+1)*VERTEX_SIZE+1];
        nextPos[2] = pathPoints[(npath+1)*VERTEX_SIZE+2];
        ++npath;

        float dist = GetDist(G3D::Vector3(iterPos[2], iterPos[0], iterPos[1]), G3D::Vector3(nextPos[2], nextPos[0], nextPos[1]));
        uint32 numpoints = (uint32)ceilf(dist / 2.0f);

        for (uint32 i = 1; i < numpoints; ++i)
        {
            returnPath[nreturnpath*VERTEX_SIZE+0] = iterPos[0] + (((nextPos[0] - iterPos[0]) / numpoints) * i); // Y
            returnPath[nreturnpath*VERTEX_SIZE+1] = iterPos[1] + (((nextPos[1] - iterPos[1]) / numpoints) * i); // Z
            returnPath[nreturnpath*VERTEX_SIZE+2] = iterPos[2] + (((nextPos[2] - iterPos[2]) / numpoints) * i); // X
            ++nreturnpath;

            if (nreturnpath >= MAX_WHITE_PATH_LENGTH)
                return false;
        }
    }


    // last point
    returnPath[nreturnpath*VERTEX_SIZE+0] = pathPoints[(maxpath-1)*VERTEX_SIZE+0];
    returnPath[nreturnpath*VERTEX_SIZE+1] = pathPoints[(maxpath-1)*VERTEX_SIZE+1];
    returnPath[nreturnpath*VERTEX_SIZE+2] = pathPoints[(maxpath-1)*VERTEX_SIZE+2];
    ++nreturnpath;

    *pathPointsSize = nreturnpath;
    return true;
}

bool PathGenerator::GenerateSmartPoints(float* pathPoints, int* pathPointsSize, float* smartPath)
{
    uint32 npoints = (uint32)*pathPointsSize;
    uint32 npath = 0;
    uint32 nSmartPath = 0;
    *pathPointsSize = 0;

    float iterPos[VERTEX_SIZE], middlePos[VERTEX_SIZE], endPos[VERTEX_SIZE];
    const float TURN_ANGLE = 179.75f;
    const float SLOPE_ANGLE = 179.0f;
    const float MAX_CLIMB = 2.0f;

    // copy first point
    smartPath[nSmartPath*VERTEX_SIZE+0] = pathPoints[npath*VERTEX_SIZE+0];
    smartPath[nSmartPath*VERTEX_SIZE+1] = pathPoints[npath*VERTEX_SIZE+1];
    smartPath[nSmartPath*VERTEX_SIZE+2] = pathPoints[npath*VERTEX_SIZE+2];
    ++nSmartPath;

    while (npath < npoints - 2)
    {
        iterPos[0] = pathPoints[(npath+0)*VERTEX_SIZE+0];
        iterPos[1] = pathPoints[(npath+0)*VERTEX_SIZE+1];
        iterPos[2] = pathPoints[(npath+0)*VERTEX_SIZE+2];

        middlePos[0] = pathPoints[(npath+1)*VERTEX_SIZE+0];
        middlePos[1] = pathPoints[(npath+1)*VERTEX_SIZE+1];
        middlePos[2] = pathPoints[(npath+1)*VERTEX_SIZE+2];

        endPos[0] = pathPoints[(npath+2)*VERTEX_SIZE+0];
        endPos[1] = pathPoints[(npath+2)*VERTEX_SIZE+1];
        endPos[2] = pathPoints[(npath+2)*VERTEX_SIZE+2];

        if (npath >= npoints - 3) // last loop
            break;

        float angle2d = GetAngle(iterPos, middlePos, endPos, false);
        float angle3d = GetAngle(iterPos, middlePos, endPos, true);
        if (angle2d < TURN_ANGLE || angle3d < SLOPE_ANGLE)
        {
            // Don't add points that are 'fall points'
            float dist = GetDist(G3D::Vector3(iterPos[2], iterPos[0], iterPos[1]), G3D::Vector3(middlePos[2], middlePos[0], middlePos[1]));
            if ((iterPos[1] - middlePos[1]) <= (dist + MAX_CLIMB))
            {
                smartPath[nSmartPath*VERTEX_SIZE+0] = middlePos[0];
                smartPath[nSmartPath*VERTEX_SIZE+2] = middlePos[2];
                smartPath[nSmartPath*VERTEX_SIZE+1] = middlePos[1];
                ++nSmartPath;

                if (nSmartPath >= MAX_POINT_PATH_POINTS)
                    return false;
            }
            else // attempt to correct their position
            {
                float nz = iterPos[1] + 2.0f;
                _sourceUnit->UpdateAllowedPositionZ(middlePos[2], middlePos[0], nz);
                if ((iterPos[1] - nz) <= (dist + MAX_CLIMB))
                {
                    smartPath[nSmartPath*VERTEX_SIZE+0] = middlePos[0];
                    smartPath[nSmartPath*VERTEX_SIZE+2] = middlePos[2];
                    smartPath[nSmartPath*VERTEX_SIZE+1] = nz;
                    ++nSmartPath;

                    if (nSmartPath >= MAX_POINT_PATH_POINTS)
                        return false;
                }
            }
        }

        ++npath;
    }

    smartPath[nSmartPath*VERTEX_SIZE+0] = pathPoints[(npoints-1)*VERTEX_SIZE+0];
    smartPath[nSmartPath*VERTEX_SIZE+1] = pathPoints[(npoints-1)*VERTEX_SIZE+1];
    smartPath[nSmartPath*VERTEX_SIZE+2] = pathPoints[(npoints-1)*VERTEX_SIZE+2];
    ++nSmartPath;

    *pathPointsSize = nSmartPath;
    return true;
}

bool PathGenerator::InRange(G3D::Vector3 const& p1, G3D::Vector3 const& p2, float r, float h) const
{
    G3D::Vector3 d = p1 - p2;
    return (d.x * d.x + d.y * d.y) < r * r && fabsf(d.z) < h;
}

float PathGenerator::Dist3DSqr(G3D::Vector3 const& p1, G3D::Vector3 const& p2) const
{
    return (p1 - p2).squaredLength();
}

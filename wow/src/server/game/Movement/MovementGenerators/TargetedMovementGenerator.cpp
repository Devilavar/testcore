/*
 * Copyright (C) 2008-2014 TrinityCore 
 * Copyright (C) 2005-2009 MaNGOS <http://getmangos.com/>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include "ByteBuffer.h"
#include "TargetedMovementGenerator.h"
#include "Errors.h"
#include "Creature.h"
#include "CreatureAI.h"
#include "World.h"
#include "MoveSplineInit.h"
#include "MoveSpline.h"
#include "Player.h"
#include "VehicleDefines.h"
#include "VMapFactory.h"

template<class T, typename D>
void TargetedMovementGeneratorMedium<T, D>::_setTargetLocation(T* owner, bool updateDestination)
{
    if (!i_target.isValid() || !i_target->IsInWorld())
        return;

    if (owner->HasUnitState(UNIT_STATE_NOT_MOVE) || owner->HasUnitState(UNIT_STATE_LOST_CONTROL))
        return;

    if (owner->GetTypeId() == TYPEID_UNIT && !i_target->isInAccessiblePlaceFor(owner->ToCreature()))
        return;

    float x = 0;
    float y = 0;
    float z = 0;

    if (updateDestination || !i_path)
    {
        float dist = 0;
        float size = 0;

        if (!i_offset)
        {
            dist = i_target->GetCombatReach();
            size = i_target->GetCombatReach() - i_target->GetObjectSize();
        }
        else
        {
            // Pets need special handling.
            // We need to subtract GetObjectSize() because it gets added back further down the chain
            //  and that makes pets too far away. Subtracting it allows pets to properly
            //  be (GetCombatReach() + i_offset) away.
            // Only applies when i_target is pet's owner otherwise pets and mobs end up
            //   doing a "dance" while fighting
            if (owner->IsPet() && i_target->GetTypeId() == TYPEID_PLAYER)
            {
                dist = i_target->GetCombatReach();
                size = i_target->GetCombatReach() - i_target->GetObjectSize();
            }
            else
            {
                size = owner->GetObjectSize();
                dist = i_offset;
            }
        }

        G3D::Vector3 dest = !owner->movespline->Finalized() ? owner->movespline->FinalDestination() : G3D::Vector3(owner->GetPositionX(), owner->GetPositionY(), owner->GetPositionZ());

        if (i_target->GetExactDist2d(dest.x, dest.y) <= (dist + size) && i_target->IsWithinLOS(dest.x, dest.y, dest.z))
        {
            i_recalculateTravel = false;
            return;
        }

        i_target->GetClosePoint(x, y, z, size, (dist - 0.1f), i_angle);

        if (!i_target->IsWithinLOS(x, y, z))
            i_target->GetPosition(x, y, z);
        else if (fabs(i_target->GetPositionZ() - z) >= 2.0f || fabs(z - i_target->GetPositionZ()) >= 2.0f)
            i_target->GetPosition(x, y, z);
    }
    else
    {
        // the destination has not changed, we just need to refresh the path (usually speed change)
        G3D::Vector3 end = i_path->GetEndPosition();
        x = end.x;
        y = end.y;
        z = end.z;
    }

    float _z = z;
    owner->UpdateAllowedPositionZ(x, y, z);

    VMAP::IVMapManager* vmgr = VMAP::VMapFactory::createOrGetVMapManager();
    GridMap* gmap = const_cast<Map*>(owner->GetMap())->GetGrid(x, y);
    if (vmgr && vmgr->isHeightCalcEnabled() && gmap)
    {
        float targetFloor = vmgr->getHeight(owner->GetMap()->GetId(), x, y, _z + 2.0f, 50.0f);
        float targetground = VMAP_INVALID_HEIGHT_VALUE;
        float targetgridHeight = gmap->getHeight(x, y);

        if (z + 4.0f > targetgridHeight)
            targetground = targetgridHeight;

        targetFloor = targetFloor != VMAP_INVALID_HEIGHT_VALUE ? targetFloor : targetground;

        if (_z - z >= 4.0f || _z - targetFloor >= 4.0f)
        {
            float _ownerZ = owner->GetPositionZ();
            owner->UpdateAllowedPositionZ(owner->GetPositionX(), owner->GetPositionY(), _ownerZ);

            float ownerFloor = vmgr->getHeight(owner->GetMap()->GetId(), owner->GetPositionX(), owner->GetPositionY(), owner->GetPositionZ() + 2.0f, 50.0f);
            float ownerground = VMAP_INVALID_HEIGHT_VALUE;
            float ownergridHeight = gmap->getHeight(x, y);

            if (_ownerZ + 4.0f > ownergridHeight)
                ownerground = ownergridHeight;

            ownerFloor = ownerFloor != VMAP_INVALID_HEIGHT_VALUE ? ownerFloor : ownerground;

            if (owner->GetPositionZ() - _ownerZ >= 4.0f || owner->GetPositionZ() - ownerFloor >= 4.0f)
            {
                Movement::MoveSplineInit init(owner);
                init.MoveTo(x, y, _z, false, true);
                init.Launch();
            }
            return;
        }
    }

    if (owner->GetMap()->IsInWater(x, y, z))
    {
        Movement::MoveSplineInit init(owner);
        init.MoveTo(x, y, z);
        init.Launch();
    }
    else
    {
        if (!i_path)
            i_path = new PathGenerator(owner);
        else i_path->Clear();

        bool result = i_path->CalculatePath(x, y, z);
        if (!result || (i_path->GetPathType() & PATHFIND_NOPATH))
        {
            if (owner->HasUnitState(UNIT_STATE_FOLLOW))
            {
                Movement::MoveSplineInit init(owner);
                init.MoveTo(x, y, z, false, true);
                init.Launch();
            }
            else
                i_recalculateTravel = true;

            return;
        }

        if (!owner->movespline->Finalized())
        {
            owner->UpdateSplinePosition();
            owner->movespline->_Finalize();
        }

        Movement::MoveSplineInit init(owner);
        init.MovebyPath(i_path->GetPath());
        init.Launch();
    }

    D::_addUnitStateMove(owner);
    i_targetReached = false;
    i_recalculateTravel = false;
    owner->AddUnitState(UNIT_STATE_CHASE);
}

template<class T, typename D>
bool TargetedMovementGeneratorMedium<T, D>::DoUpdate(T* owner, uint32 time_diff)
{
    if (!i_target.isValid() || !i_target->IsInWorld())
        return false;

    if (!owner || !owner->IsAlive())
        return false;

    if (owner->HasUnitState(UNIT_STATE_LOST_CONTROL) || owner->HasUnitState(UNIT_STATE_NOT_MOVE))
    {
        D::_clearUnitStateMove(owner);
        return true;
    }

    if (owner->GetTypeId() == TYPEID_UNIT && !i_target->isInAccessiblePlaceFor((const Creature*)owner))
        return false;

    // prevent movement while casting spells with cast time or channel time
    if (owner->HasUnitState(UNIT_STATE_CASTING))
    {
        if (!owner->IsStopped())
            owner->StopMoving();
        return true;
    }

    if (owner->HasUnitState(UNIT_STATE_POSSESSED))
        return false;

    // prevent crash after creature killed pet
    if (static_cast<D*>(this)->_lostTarget(owner))
    {
        D::_clearUnitStateMove(owner);
        return true;
    }

    bool targetMoved = false;
    i_recheckDistance.Update(time_diff);
    if (i_recheckDistance.Passed())
    {
        i_recheckDistance.Reset(250);

        float allowed_dist = owner->GetCombatReach();

        if (owner->HasUnitState(UNIT_STATE_FOLLOW))
            allowed_dist = i_target->GetCombatReach();

        G3D::Vector3 dest = owner->movespline->FinalDestination();

        if (!owner->movespline->Finalized())
            targetMoved = !i_target->IsWithinDist3d(dest.x, dest.y, dest.z, allowed_dist) || !i_target->IsWithinLOS(dest.x, dest.y, dest.z);
        else
            targetMoved = !i_target->IsWithinDist3d(owner->GetPositionX(), owner->GetPositionY(), owner->GetPositionZ(), allowed_dist) || !i_target->IsWithinLOS(owner->GetPositionX(), owner->GetPositionY(), owner->GetPositionZ());
    }

    if (i_recalculateTravel || targetMoved)
        _setTargetLocation(owner, true);

    if (owner->movespline->Finalized())
    {
        static_cast<D*>(this)->MovementInform(owner);
        if (i_angle == 0.0f && !owner->HasInArc(0.01f, i_target.getTarget()))
            owner->SetInFront(i_target.getTarget());

        if (!i_targetReached)
        {
            i_targetReached = true;
            static_cast<D*>(this)->_reachTarget(owner);
            //owner->SetFacingTo(i_target->GetOrientation());
        }

        if (i_path)
            i_path->Clear();
    }

    return true;
}

//-----------------------------------------------//
template<class T>
void ChaseMovementGenerator<T>::_reachTarget(T* owner)
{
    if (owner->IsWithinMeleeRange(this->i_target.getTarget()))
        owner->Attack(this->i_target.getTarget(), true);
}

template<>
void ChaseMovementGenerator<Player>::DoInitialize(Player* owner)
{
    owner->AddUnitState(UNIT_STATE_CHASE | UNIT_STATE_CHASE_MOVE);
    _setTargetLocation(owner, true);
}

template<>
void ChaseMovementGenerator<Creature>::DoInitialize(Creature* owner)
{
    owner->SetWalk(false);
    owner->AddUnitState(UNIT_STATE_CHASE | UNIT_STATE_CHASE_MOVE);
    _setTargetLocation(owner, true);
}

template<class T>
void ChaseMovementGenerator<T>::DoFinalize(T* owner)
{
    owner->ClearUnitState(UNIT_STATE_CHASE | UNIT_STATE_CHASE_MOVE);
}

template<class T>
void ChaseMovementGenerator<T>::DoReset(T* owner)
{
    DoInitialize(owner);
}

template<class T>
void ChaseMovementGenerator<T>::MovementInform(T* /*unit*/) { }

template<>
void ChaseMovementGenerator<Creature>::MovementInform(Creature* unit)
{
    // Pass back the GUIDLow of the target. If it is pet's owner then PetAI will handle
    if (unit->AI())
        unit->AI()->MovementInform(CHASE_MOTION_TYPE, i_target.getTarget()->GetGUIDLow());
}

//-----------------------------------------------//
template<>
bool FollowMovementGenerator<Creature>::EnableWalking() const
{
    return i_target.isValid() && i_target->IsWalking();
}

template<>
bool FollowMovementGenerator<Player>::EnableWalking() const
{
    return false;
}

template<>
void FollowMovementGenerator<Player>::_updateSpeed(Player* /*owner*/)
{
    // nothing to do for Player
}

template<>
void FollowMovementGenerator<Creature>::_updateSpeed(Creature* owner)
{
    // pet only sync speed with owner
    /// Make sure we are not in the process of a map change (IsInWorld)
    if (!owner->IsPet() || !owner->IsInWorld() || !i_target.isValid() || i_target->GetGUID() != owner->GetOwnerGUID())
        return;

    owner->UpdateSpeed(MOVE_RUN, true);
    owner->UpdateSpeed(MOVE_WALK, true);
    owner->UpdateSpeed(MOVE_SWIM, true);
}

template<>
void FollowMovementGenerator<Player>::DoInitialize(Player* owner)
{
    owner->AddUnitState(UNIT_STATE_FOLLOW | UNIT_STATE_FOLLOW_MOVE);
    _updateSpeed(owner);
    _setTargetLocation(owner, true);
}

template<>
void FollowMovementGenerator<Creature>::DoInitialize(Creature* owner)
{
    owner->AddUnitState(UNIT_STATE_FOLLOW | UNIT_STATE_FOLLOW_MOVE);
    _updateSpeed(owner);
    _setTargetLocation(owner, true);
}

template<class T>
void FollowMovementGenerator<T>::DoFinalize(T* owner)
{
    owner->ClearUnitState(UNIT_STATE_FOLLOW | UNIT_STATE_FOLLOW_MOVE);
    _updateSpeed(owner);
}

template<class T>
void FollowMovementGenerator<T>::DoReset(T* owner)
{
    DoInitialize(owner);
}

template<class T>
void FollowMovementGenerator<T>::MovementInform(T* /*unit*/) { }

template<>
void FollowMovementGenerator<Creature>::MovementInform(Creature* unit)
{
    // Pass back the GUIDLow of the target. If it is pet's owner then PetAI will handle
    if (unit->AI())
        unit->AI()->MovementInform(FOLLOW_MOTION_TYPE, i_target.getTarget()->GetGUIDLow());
}

//-----------------------------------------------//
template void TargetedMovementGeneratorMedium<Player, ChaseMovementGenerator<Player> >::_setTargetLocation(Player*, bool);
template void TargetedMovementGeneratorMedium<Player, FollowMovementGenerator<Player> >::_setTargetLocation(Player*, bool);
template void TargetedMovementGeneratorMedium<Creature, ChaseMovementGenerator<Creature> >::_setTargetLocation(Creature*, bool);
template void TargetedMovementGeneratorMedium<Creature, FollowMovementGenerator<Creature> >::_setTargetLocation(Creature*, bool);
template bool TargetedMovementGeneratorMedium<Player, ChaseMovementGenerator<Player> >::DoUpdate(Player*, uint32);
template bool TargetedMovementGeneratorMedium<Player, FollowMovementGenerator<Player> >::DoUpdate(Player*, uint32);
template bool TargetedMovementGeneratorMedium<Creature, ChaseMovementGenerator<Creature> >::DoUpdate(Creature*, uint32);
template bool TargetedMovementGeneratorMedium<Creature, FollowMovementGenerator<Creature> >::DoUpdate(Creature*, uint32);

template void ChaseMovementGenerator<Player>::_reachTarget(Player*);
template void ChaseMovementGenerator<Creature>::_reachTarget(Creature*);
template void ChaseMovementGenerator<Player>::DoFinalize(Player*);
template void ChaseMovementGenerator<Creature>::DoFinalize(Creature*);
template void ChaseMovementGenerator<Player>::DoReset(Player*);
template void ChaseMovementGenerator<Creature>::DoReset(Creature*);
template void ChaseMovementGenerator<Player>::MovementInform(Player*);

template void FollowMovementGenerator<Player>::DoFinalize(Player*);
template void FollowMovementGenerator<Creature>::DoFinalize(Creature*);
template void FollowMovementGenerator<Player>::DoReset(Player*);
template void FollowMovementGenerator<Creature>::DoReset(Creature*);
template void FollowMovementGenerator<Player>::MovementInform(Player*);

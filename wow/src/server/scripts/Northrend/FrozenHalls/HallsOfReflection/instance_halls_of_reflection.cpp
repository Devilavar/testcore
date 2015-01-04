/*
 * Copyright (C) 2008-2012 TrinityCore 
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

#include "ScriptPCH.h"
#include "halls_of_reflection.h"

/*
%DONE: 98
BUGS:
visual for the cannons firing is at the wrong place (related to triggers)

TODO:
timers on texts could be improved
*/

enum eEnum
{
    ENCOUNTER_WAVE_MERCENARY                      = 6,
    ENCOUNTER_WAVE_FOOTMAN                        = 10,
    ENCOUNTER_WAVE_RIFLEMAN                       = 6,
    ENCOUNTER_WAVE_PRIEST                         = 6,
    ENCOUNTER_WAVE_MAGE                           = 6,
};

enum Events
{
    EVENT_NONE                  = 1,
    EVENT_NEXT_WAVE             = 2,
    EVENT_ADVANCE_WAVE          = 3,
    EVENT_WIPE_CHECK            = 4,
};

class instance_halls_of_reflection : public InstanceMapScript
{
public:
    instance_halls_of_reflection() : InstanceMapScript("instance_halls_of_reflection", 668) { }

    InstanceScript* GetInstanceScript(InstanceMap* map) const
    {
        return new instance_halls_of_reflection_InstanceMapScript(map);
    }

    struct instance_halls_of_reflection_InstanceMapScript : public InstanceScript
    {
        instance_halls_of_reflection_InstanceMapScript(Map* map) : InstanceScript(map) {};

        bool m_bIsCall;
        bool WaveAdvanced;

        uint64 uiFalric;
        uint64 uiMarwyn;
        uint64 uiLichKing;
        uint64 uiJainaPart1;
        uint64 uiSylvanasPart1;
        uint64 uiEscapeLeader;
        uint64 uicaptain;

        uint64 uiGunship;

        uint64 uiFrostmourne;
        uint64 uiFrontDoor;
        uint64 uiFrostwornDoor;
        uint64 uiArthasDoor;
        uint64 uiRunDoor;
        uint64 uiWall[4];
        uint64 uiWallID[4];
        uint64 uiCaveDoor;
        uint64 m_uiSummonGUID[34];

        uint32 uiEncounter[MAX_ENCOUNTER];
        uint32 uiTeamInInstance;
        uint32 uiWaveCount;
        uint32 uiWaveState;
        uint32 uiIntroDone;
        uint32 uiSummons;
        uint32 uiDataPhase;
        uint32 m_uiLocNo;
        uint32 m_uiCheckSummon;
        uint32 WaveAlive;
        uint32 randsummon;

        EventMap events;

        void Initialize()
        {
            events.Reset();

            m_bIsCall = false;
            WaveAdvanced = false;

            uiFalric = 0;
            uiMarwyn = 0;
            uiLichKing = 0;
            uiJainaPart1 = 0;
            uiSylvanasPart1 = 0;
            uiEscapeLeader = 0;
            uicaptain = 0;
            uiGunship = 0;

            uiFrostmourne = 0;
            uiArthasDoor = 0;
            uiFrostwornDoor = 0;
            uiFrontDoor = 0;
            uiCaveDoor = 0;
            uiTeamInInstance = 0;
            uiWaveCount = 0;
            uiWaveState = 0;
            uiIntroDone = 0;
            uiSummons = 0;
            uiDataPhase = 0;
            m_uiCheckSummon = 0;
            WaveAlive = 0;

            for (uint8 i = 0; i < 4; ++i)
            {
                uiWall[i] = 0;
                uiWallID[i] = 0;
            }

            for (uint8 i = 0; i < MAX_ENCOUNTER; ++i)
                uiEncounter[i] = NOT_STARTED;
        }

        void OpenDoor(uint64 guid)
        {
            if(!guid) return;
            GameObject* go = instance->GetGameObject(guid);
            if(go) go->SetGoState(GO_STATE_ACTIVE);
        }

        void CloseDoor(uint64 guid)
        {
            if(!guid) return;
            GameObject* go = instance->GetGameObject(guid);
            if(go) go->SetGoState(GO_STATE_READY);
        }

        void OnCreatureCreate(Creature* creature)
        {
            Map::PlayerList const &players = instance->GetPlayers();
            if (!players.isEmpty())
                if (Player* player = players.begin()->GetSource())
                    uiTeamInInstance = player->GetTeam();

            switch (creature->GetEntry())
            {
                case NPC_FALRIC:
                    uiFalric = creature->GetGUID();
                    break;
                case NPC_MARWYN:
                    uiMarwyn = creature->GetGUID();
                    break;
                case NPC_LICH_KING_EVENT:
                    break;
                case NPC_JAINA_PART1:
                    if (uiTeamInInstance == HORDE)
                        creature->UpdateEntry(NPC_SYLVANAS_PART1);
                    uiJainaPart1 = creature->GetGUID();
                    uiSylvanasPart1 = creature->GetGUID();
                    break;
                case NPC_SYLVANAS_PART1:
                    uiSylvanasPart1 = creature->GetGUID();
                    break;
                case NPC_FROSTWORN_GENERAL:
                    break;
                case NPC_JAINA_OUTRO:
                    if (uiTeamInInstance == HORDE)
                        creature->UpdateEntry(NPC_SYLVANA_OUTRO);
                    creature->SetHealth(252000);
                    uiEscapeLeader = creature->GetGUID();
                    break;
                case BOSS_LICH_KING:
                    creature->SetHealth(20917000);
                    uiLichKing = creature->GetGUID();
                    break;
                case NPC_BARTLETT:
                    creature->SetPhaseMask(2, true);
                    if (uiTeamInInstance == ALLIANCE)
                        uicaptain = creature->GetGUID();
                    break;
                case NPC_KORM:
                    creature->SetPhaseMask(2, true);
                    if (uiTeamInInstance == HORDE)
                        uicaptain = creature->GetGUID();
                    break;
                default:
                    break;
            }
        }

        void OnUnitDeath(Unit* unit)
        {
            Creature* creature = unit->ToCreature();
            if (!creature)
                return;

            switch (creature->GetEntry())
            {
                case NPC_WAVE_MERCENARY:
                case NPC_WAVE_FOOTMAN:
                case NPC_WAVE_RIFLEMAN:
                case NPC_WAVE_PRIEST:
                case NPC_WAVE_MAGE:
                    WaveAlive = WaveAlive--;
                    break;
                default:
                    break;
            }
        }

        void OnGameObjectCreate(GameObject* go)
        {
            /// @todo init state depending on encounters
            switch (go->GetEntry())
            {
                case GO_FROSTMOURNE:
                    uiFrostmourne = go->GetGUID();
                    go->SetFlag(GAMEOBJECT_FLAGS, GO_FLAG_INTERACT_COND);
                    HandleGameObject(0, false, go);
                    break;
                case GO_FROSTMOURNE_ALTAR:
                    go->SetFlag(GAMEOBJECT_FLAGS, GO_FLAG_INTERACT_COND);
                    HandleGameObject(0, true, go);
                    break;
                case GO_FRONT_DOOR:
                    uiFrontDoor = go->GetGUID();
                    go->SetFlag(GAMEOBJECT_FLAGS, GO_FLAG_INTERACT_COND);
                    OpenDoor(uiFrontDoor);
                    break;
                case GO_FROSTWORN_DOOR:
                    uiFrostwornDoor = go->GetGUID();
                    go->SetFlag(GAMEOBJECT_FLAGS, GO_FLAG_INTERACT_COND);

                    if (uiEncounter[1] == DONE)
                        OpenDoor(uiFrostwornDoor);
                    else
                        CloseDoor(uiFrostwornDoor);
                    break;
                case GO_RUN_DOOR:
                    uiRunDoor = go->GetGUID();
                    break;
                case GO_ARTHAS_DOOR:
                    uiArthasDoor = go->GetGUID();
                    break;
                case GO_ICE_WALL_1:
                    uiWallID[0] = go->GetGUID();
                    break;
                case GO_ICE_WALL_2:
                    uiWallID[1] = go->GetGUID();
                    break;
                case GO_ICE_WALL_3:
                    uiWallID[2] = go->GetGUID();
                    break;
                case GO_ICE_WALL_4:
                    uiWallID[3] = go->GetGUID();
                    break;
                case GO_CAVE:
                    uiCaveDoor = go->GetGUID();
                    break;
                case GO_SKYBREAKER:
                    if (GetData(DATA_LICHKING_EVENT) != DONE || uiTeamInInstance == HORDE)
                        go->SetPhaseMask(2, true);
                    if (uiTeamInInstance == ALLIANCE)
                    {
                        if (GetData(DATA_LICHKING_EVENT) == DONE) // handle respawn of objects in the event a server crash
                        {
                            go->SummonGameObject(GO_STAIRS_SKYBREAKER, 5223.15f, 1639.64f, 786.649f, 5.5224f, 0, 0, 0.371286f, -0.928518f, 1*DAY);
                            go->SummonGameObject(GO_STAIRS_SKYBREAKER, 5209.64f, 1625.31f, 799.147f, 5.5224f, 0, 0, 0.371286f, -0.928518f, 1*DAY);
                            go->SummonGameObject(GO_PORTAL, 5200.53f, 1601.46f, 808.389f, 1.22126f, 0, 0, 0.573384f, 0.819287f, 1*DAY);
                        }
                        uiGunship = go->GetGUID();
                    }
                    break;
                case GO_ORGRIM_HAMMER:
                    if (GetData(DATA_LICHKING_EVENT) != DONE || uiTeamInInstance == ALLIANCE)
                        go->SetPhaseMask(2, true);
                    if (uiTeamInInstance == HORDE)
                    {
                        if (GetData(DATA_LICHKING_EVENT) == DONE) // handle respawn of objects in the event a server crash
                        {
                            go->SummonCreature(NPC_WARSONG_CANNON, 5211.55f, 1582.87f, 793.857f, 1.35741f, TEMPSUMMON_MANUAL_DESPAWN, 0);
                            go->SummonCreature(NPC_WARSONG_CANNON, 5224.60f, 1579.75f, 794.432f, 1.35741f, TEMPSUMMON_MANUAL_DESPAWN, 0);
                            go->SummonCreature(NPC_WARSONG_CANNON, 5199.05f, 1586.30f, 794.432f, 1.35741f, TEMPSUMMON_MANUAL_DESPAWN, 0);
                            go->SummonCreature(NPC_WARSONG_CANNON, 5197.13f, 1519.70f, 793.855f, 4.48017f, TEMPSUMMON_MANUAL_DESPAWN, 0);
                            go->SummonCreature(NPC_WARSONG_CANNON, 5210.27f, 1517.34f, 794.441f, 4.48017f, TEMPSUMMON_MANUAL_DESPAWN, 0);
                            go->SummonCreature(NPC_WARSONG_CANNON, 5184.53f, 1523.01f, 793.855f, 4.48017f, TEMPSUMMON_MANUAL_DESPAWN, 0);
                            go->SummonGameObject(GO_PORTAL, 5229.2f, 1566.9f, 819.405f, 4.70851f, 0, 0, 0.708479f, -0.705732f, 1*DAY);
                            go->SummonGameObject(GO_STAIRS_ORGRIM_HAMMER, 5245.00f, 1625.00f, 784.00f, 5.8709f, 0, 0, 0.204685f, -0.978828f, 1*DAY);
                            go->SummonGameObject(GO_STAIRS_ORGRIM_HAMMER, 5236.89f, 1606.70f, 796.60f, 5.8709f, 0, 0, 0.192211f, -0.981354f, 1*DAY);
                            go->SummonGameObject(GO_STAIRS_ORGRIM_HAMMER, 5228.78f, 1588.40f, 809.20f, 5.8709f, 0, 0, 0.204685f, -0.978828f, 1*DAY);
                        }
                        uiGunship = go->GetGUID();
                    }
                    break;
            }
        }

        void SetData(uint32 type, uint32 data) override
        {
            switch (type)
            {
                case DATA_INTRO_EVENT:
                    uiIntroDone = data;
                    break;
                case DATA_WAVE_STATE:
                    uiWaveState = data;
                    switch (data)
                    {
                        case SPECIAL:
                            CloseDoor(uiFrontDoor);
                            if (!m_bIsCall)
                            {
                               m_bIsCall = true;
                               Summon();
                            }
                            events.ScheduleEvent(EVENT_NEXT_WAVE, 15000);
                            break;
                        case IN_PROGRESS:
                            CloseDoor(uiFrontDoor);
                            if (!m_bIsCall)
                            {
                               m_bIsCall = true;
                               Summon();
                            }
                            events.ScheduleEvent(EVENT_NEXT_WAVE, 3000);
                            events.ScheduleEvent(EVENT_WIPE_CHECK, 1000);
                            break;
                        default:
                            break;
                    }

                    if (uiWaveCount && data == FAIL)
                        DoWipe();
                    break;
                case DATA_FALRIC_EVENT:
                    uiEncounter[0] = data;
                    if (data == DONE)
                        events.ScheduleEvent(EVENT_NEXT_WAVE, 60000);
                    break;
                case DATA_MARWYN_EVENT:
                    uiEncounter[1] = data;
                    if (data == DONE)
                    {
                        OpenDoor(uiFrostwornDoor);
                        OpenDoor(uiFrontDoor);
                    }
                    break;
                case DATA_FROSWORN_EVENT:
                    uiEncounter[2] = data;
                    if (data == DONE)
                    {
                        OpenDoor(uiArthasDoor);
                        SetData(DATA_PHASE, 3);
                        instance->SummonCreature(BOSS_LICH_KING, OutroSpawns[0]);
                        instance->SummonCreature(NPC_JAINA_OUTRO, OutroSpawns[1]);
                    }
                    break;
                case DATA_LICHKING_EVENT:
                    uiEncounter[3] = data;
                    switch (data)
                    {
                        case IN_PROGRESS:
                            if(instance->IsHeroic())
                                DoStartTimedAchievement(ACHIEVEMENT_TIMED_TYPE_EVENT, ACHIEV_NOT_RETREATING_EVENT);
                            break;
                        case FAIL:
                            for(uint8 i = 0; i<4; i++)
                                OpenDoor(uiWallID[i]);

                            OpenDoor(uiRunDoor);

                            if(Creature* lichking = instance->GetCreature(uiLichKing))
                                lichking->DespawnOrUnsummon(10000);
                            if(Creature* leader = instance->GetCreature(uiEscapeLeader))
                                leader->DespawnOrUnsummon(10000);

                            if (instance->IsHeroic())
                                DoStopTimedAchievement(ACHIEVEMENT_TIMED_TYPE_EVENT, ACHIEV_NOT_RETREATING_EVENT);

                            SetData(DATA_PHASE, 3);
                            instance->SummonCreature(BOSS_LICH_KING, OutroSpawns[0]);
                            instance->SummonCreature(NPC_JAINA_OUTRO, OutroSpawns[1]);
                            break;
                        case DONE:
                            DoCastSpellOnPlayers(SPELL_ACHIEV_CHECK);
                            if(instance->IsHeroic())
                                DoStopTimedAchievement(ACHIEVEMENT_TIMED_TYPE_EVENT, ACHIEV_NOT_RETREATING_EVENT);
                            break;
                        default:
                            break;

                    }
                    break;
                case DATA_SUMMONS:
                    switch (data)
                    {
                        case 0: // summon died, we need to subtract
                            --uiSummons;
                            break;
                        case 1: // summon spawned, we need to add
                            ++uiSummons;
                            break;
                        case 3: // new wall, restart
                            uiSummons = 0;
                            break;
                        default:
                            break;
                    }
                    break;
                case DATA_ICE_WALL_1:
                    uiWall[0] = data;
                    break;
                case DATA_ICE_WALL_2:
                    uiWall[1] = data;
                    break;
                case DATA_ICE_WALL_3:
                    uiWall[2] = data;
                    break;
                case DATA_ICE_WALL_4:
                    uiWall[3] = data;
                    break;
                case DATA_PHASE:
                    uiDataPhase = data;
                    break;
                default:
                    break;
            }

            if (data == DONE)
                SaveToDB();
        }

        uint32 GetData(uint32 type) const override
        {
            switch (type)
            {
                case DATA_INTRO_EVENT:          return uiIntroDone;
                case DATA_TEAM_IN_INSTANCE:     return uiTeamInInstance;

                case DATA_FALRIC_EVENT:         return uiEncounter[0];
                case DATA_MARWYN_EVENT:         return uiEncounter[1];
                case DATA_WAVE_COUNT:           return uiWaveCount;
                case DATA_WAVE_STATE:           return uiWaveState;

                case DATA_FROSWORN_EVENT:       return uiEncounter[2];

                case DATA_LICHKING_EVENT:       return uiEncounter[3];
                case DATA_ICE_WALL_1:           return uiWall[0];
                case DATA_ICE_WALL_2:           return uiWall[1];
                case DATA_ICE_WALL_3:           return uiWall[2];
                case DATA_ICE_WALL_4:           return uiWall[3];
                case DATA_SUMMONS:              return uiSummons;

                case DATA_PHASE:                return uiDataPhase;
            }
            return 0;
        }

        uint64 GetData64(uint32 identifier) const override
        {
            switch (identifier)
            {
                case DATA_FALRIC:
                    return uiFalric;
                case DATA_MARWYN:
                    return uiMarwyn;
                case DATA_LICHKING:
                    return uiLichKing;
                case DATA_ESCAPE_LEADER:
                    return uiEscapeLeader;
                case DATA_FROSTMOURNE:
                    return uiFrostmourne;
                case DATA_FRONT_DOOR:
                    return uiFrontDoor;
                case DATA_FROSTWORN_DOOR:
                    return uiFrostwornDoor;
                case DATA_ARTHAS_DOOR:
                    return uiArthasDoor;
                case DATA_GUNSHIP:
                    return uiGunship;
                case DATA_CAPTAINS:
                    return uicaptain;
                case GO_ICE_WALL_1:
                    return uiWallID[0];
                case GO_ICE_WALL_2:
                    return uiWallID[1];
                case GO_ICE_WALL_3:
                    return uiWallID[2];
                case GO_ICE_WALL_4:
                    return uiWallID[3];
                case GO_CAVE:
                    return uiCaveDoor;
            }
            return 0;
        }

        std::string GetSaveData()
        {
            OUT_SAVE_INST_DATA;

            std::ostringstream saveStream;
            saveStream << "H R 1 " << uiEncounter[0] << " " << uiEncounter[1] << " " << uiEncounter[2] << " " << uiEncounter[3] << " " << uiIntroDone;

            OUT_SAVE_INST_DATA_COMPLETE;
            return saveStream.str();
        }

        void Load(const char* in)
        {
            if (!in)
            {
                OUT_LOAD_INST_DATA_FAIL;
                return;
            }

            OUT_LOAD_INST_DATA(in);

            char dataHead1, dataHead2;
            uint16 version;
            uint16 data0, data1, data2, data3, data4;

            std::istringstream loadStream(in);
            loadStream >> dataHead1 >> dataHead2 >> version >> data0 >> data1 >> data2 >> data3 >> data4;

            if (dataHead1 == 'H' && dataHead2 == 'R')
            {
                uiEncounter[0] = data0;
                uiEncounter[1] = data1;
                uiEncounter[2] = data2;
                uiEncounter[3] = data3;
                uiIntroDone = data4;

                for (uint8 i = 0; i < MAX_ENCOUNTER; ++i)
                    if (uiEncounter[i] == IN_PROGRESS)
                        uiEncounter[i] = NOT_STARTED;

                OpenDoor(uiFrontDoor);
                if (uiEncounter[1] == DONE)
                    OpenDoor(uiFrostwornDoor);
                if (uiEncounter[2] == DONE)
                    OpenDoor(uiArthasDoor);

            } else OUT_LOAD_INST_DATA_FAIL;

            OUT_LOAD_INST_DATA_COMPLETE;
        }

        void AddWave()
        {
            DoUpdateWorldState(WORLD_STATE_HOR, 1);
            DoUpdateWorldState(WORLD_STATE_HOR_WAVE_COUNT, uiWaveCount);

            switch (uiWaveCount)
            {
                case 1:
                    if (Creature* pFalric = instance->GetCreature(uiFalric))
                        SpawnWave(pFalric);
                    break;
                case 2:
                    if (Creature* pFalric = instance->GetCreature(uiFalric))
                        SpawnWave(pFalric);
                    break;
                case 3:
                    if (Creature* pFalric = instance->GetCreature(uiFalric))
                        SpawnWave(pFalric);
                    break;
                case 4:
                    if (Creature* pFalric = instance->GetCreature(uiFalric))
                        SpawnWave(pFalric);
                    break;
                case 5:
                    if (GetData(DATA_FALRIC_EVENT) == DONE)
                        events.ScheduleEvent(EVENT_NEXT_WAVE, 10000);
                    else if (Creature* pFalric = instance->GetCreature(uiFalric))
                        if (pFalric->AI())
                        {
                            pFalric->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_PC|UNIT_FLAG_NON_ATTACKABLE);
                            pFalric->AI()->DoAction(ACTION_ENTER_COMBAT);
                        }
                    break;
                case 6:
                    if (Creature* pMarwyn = instance->GetCreature(uiMarwyn))
                        SpawnWave(pMarwyn);
                    break;
                case 7:
                    if (Creature* pMarwyn = instance->GetCreature(uiMarwyn))
                        SpawnWave(pMarwyn);
                    break;
                case 8:
                    if (Creature* pMarwyn = instance->GetCreature(uiMarwyn))
                        SpawnWave(pMarwyn);
                    break;
                case 9:
                    if (Creature* pMarwyn = instance->GetCreature(uiMarwyn))
                        SpawnWave(pMarwyn);
                    break;
                case 10:
                    if (GetData(DATA_MARWYN_EVENT) != DONE) // wave should not have been started if DONE. Check anyway to avoid bug exploit!
                        if (Creature* pMarwyn = instance->GetCreature(uiMarwyn))
                            if (pMarwyn->AI())
                            {
                                pMarwyn->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_PC|UNIT_FLAG_NON_ATTACKABLE);
                                pMarwyn->AI()->DoAction(ACTION_ENTER_COMBAT);
                            }
                    break;
            }
        }

        void Summon()
        {
             m_uiLocNo = 0;

             for (uint8 i = 0; i < 34; i++)
             {
                 m_uiCheckSummon = 0;
                 switch (i)
                 {
                     case 0:
                         randsummon = NPC_WAVE_PRIEST; break;
                     case 1:
                         randsummon = NPC_WAVE_FOOTMAN; break;
                     case 2:
                         randsummon = NPC_WAVE_RIFLEMAN; break;
                     case 3:
                         randsummon = NPC_WAVE_FOOTMAN; break;
                     case 4:
                         randsummon = NPC_WAVE_FOOTMAN; break;
                     case 5:
                         randsummon = NPC_WAVE_PRIEST; break;
                     case 6:
                         randsummon = NPC_WAVE_RIFLEMAN; break;
                     case 7:
                         randsummon = NPC_WAVE_MERCENARY; break;
                     case 8:
                         randsummon = NPC_WAVE_FOOTMAN; break;
                     case 9:
                         randsummon = NPC_WAVE_RIFLEMAN; break;
                     case 10:
                         randsummon = NPC_WAVE_MAGE; break;
                     case 11:
                         randsummon = NPC_WAVE_MAGE; break;
                     case 12:
                         randsummon = NPC_WAVE_PRIEST; break;
                     case 13:
                         randsummon = NPC_WAVE_MERCENARY; break;
                     case 14:
                         randsummon = NPC_WAVE_FOOTMAN; break;
                     case 15:
                         randsummon = NPC_WAVE_RIFLEMAN; break;
                     case 16:
                         randsummon = NPC_WAVE_PRIEST; break;
                     case 17:
                         randsummon = NPC_WAVE_FOOTMAN; break;
                     case 18:
                         randsummon = NPC_WAVE_MAGE; break;
                     case 19:
                         randsummon = NPC_WAVE_RIFLEMAN; break;
                     case 20:
                         randsummon = NPC_WAVE_FOOTMAN; break;
                     case 21:
                         randsummon = NPC_WAVE_MERCENARY; break;
                     case 22:
                         randsummon = NPC_WAVE_PRIEST; break;
                     case 23:
                         randsummon = NPC_WAVE_MAGE; break;
                     case 24:
                         randsummon = NPC_WAVE_FOOTMAN; break;
                     case 25:
                         randsummon = NPC_WAVE_FOOTMAN; break;
                     case 26:
                         randsummon = NPC_WAVE_MERCENARY; break;
                     case 27:
                         randsummon = NPC_WAVE_PRIEST; break;
                     case 28:
                         randsummon = NPC_WAVE_MAGE; break;
                     case 29:
                         randsummon = NPC_WAVE_PRIEST; break;
                     case 30:
                         randsummon = NPC_WAVE_MAGE; break;
                     case 31:
                         randsummon = NPC_WAVE_FOOTMAN; break;
                     case 32:
                         randsummon = NPC_WAVE_FOOTMAN; break;
                     case 33:
                         randsummon = NPC_WAVE_MERCENARY; break;
                     default:
                         break;
                 }

                 if (Creature* pMarwyn = instance->GetCreature(uiMarwyn))
                 { 
                     if (Creature* trashwave = pMarwyn->SummonCreature(randsummon, SpawnLoc[i], TEMPSUMMON_DEAD_DESPAWN))
                     {
                        m_uiSummonGUID[i] = trashwave->GetGUID();
                        trashwave->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE|UNIT_FLAG_NOT_SELECTABLE|UNIT_FLAG_IMMUNE_TO_PC);
                        trashwave->SetReactState(REACT_PASSIVE);
                        trashwave->CastSpell(trashwave, SPELL_SPIRIT_SPAWN, true);
                     }
                 }
                 m_uiLocNo++;
             }
        }

        // Wipe has been detected. Perform cleanup and reset.
        void DoWipe()
        {
            if (GetData(DATA_MARWYN_EVENT) != DONE)
            {
                uiWaveCount = 0;
                WaveAlive = 0;
                m_bIsCall = false;
                WaveAdvanced = false;
                m_uiCheckSummon = 0;
                events.Reset();
                DoUpdateWorldState(WORLD_STATE_HOR, 1);
                DoUpdateWorldState(WORLD_STATE_HOR_WAVE_COUNT, uiWaveCount);
                OpenDoor(uiFrontDoor);
                for (uint8 i = 0; i < 34; i++)
                {
                    if (Creature* trashwave = instance->GetCreature(m_uiSummonGUID[i]))
                    trashwave->DespawnOrUnsummon(5000);
                }

            if (Creature* pFalric = instance->GetCreature(uiFalric))
                pFalric->SetVisible(false);
            if (Creature* pMarwyn = instance->GetCreature(uiMarwyn))
                pMarwyn->SetVisible(false);
            }
        }

        // Activate a trash wave.
        void SpawnWave(Creature* trashwave)
        {
            switch (uiWaveCount)
            {
                case 1:
                    for (uint8 i = 0; i < 3; i++)
                    {
                        if (Creature* trashwave = instance->GetCreature(m_uiSummonGUID[m_uiCheckSummon]))
                        {
                            trashwave->AI()->DoAction(ACTION_TRASH_ACTIVATE);
                            trashwave->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                            trashwave->CastSpell(trashwave, SPELL_SPIRIT_ACTIVATE_VIS, true);
                        }
                        m_uiCheckSummon++;
                        WaveAlive++;
                    }
                    break;
                case 2:
                case 3:
                    for (uint8 i = 0; i < 4; i++)
                    {
                        if (Creature* trashwave = instance->GetCreature(m_uiSummonGUID[m_uiCheckSummon]))
                        {
                            trashwave->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                            trashwave->CastSpell(trashwave, SPELL_SPIRIT_ACTIVATE_VIS, true);
                            trashwave->AI()->DoAction(ACTION_TRASH_ACTIVATE);
                        }
                        m_uiCheckSummon++;
                        WaveAlive++;
                    }
                    break;
                case 4:
                    for (uint8 i = 0; i < 5; i++)
                    {
                        if (Creature* trashwave = instance->GetCreature(m_uiSummonGUID[m_uiCheckSummon]))
                        {
                            trashwave->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                            trashwave->CastSpell(trashwave, SPELL_SPIRIT_ACTIVATE_VIS, true);
                            trashwave->AI()->DoAction(ACTION_TRASH_ACTIVATE);
                        }
                        m_uiCheckSummon++;
                        WaveAlive++;
                    }
                    break;
                case 6:
                case 7:
                    for (uint8 i = 0; i < 4; i++)
                    {
                        if (Creature* trashwave = instance->GetCreature(m_uiSummonGUID[m_uiCheckSummon]))
                        {
                            trashwave->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                            trashwave->CastSpell(trashwave, SPELL_SPIRIT_ACTIVATE_VIS, true);
                            trashwave->AI()->DoAction(ACTION_TRASH_ACTIVATE);
                        }
                        m_uiCheckSummon++;
                        WaveAlive++;
                    }
                    break;
                case 8:
                case 9:
                    for (uint8 i = 0; i < 5; i++)
                    {
                        if (Creature* trashwave = instance->GetCreature(m_uiSummonGUID[m_uiCheckSummon]))
                        {
                            trashwave->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                            trashwave->CastSpell(trashwave, SPELL_SPIRIT_ACTIVATE_VIS, true);
                            trashwave->AI()->DoAction(ACTION_TRASH_ACTIVATE);
                        }
                        m_uiCheckSummon++;
                        WaveAlive++;
                    }
                    break;
                default:
                    break;
            }
            events.ScheduleEvent(EVENT_NEXT_WAVE, 60000);
        }

        void Update(uint32 diff)
        {
            if (!instance->HavePlayers())
                return;

            if (WaveAlive == 0 && !WaveAdvanced)
            {
                if (uiWaveCount == 1 || uiWaveCount == 2 || uiWaveCount == 3 || uiWaveCount == 4 || uiWaveCount == 6 || uiWaveCount == 7 ||uiWaveCount == 8  || uiWaveCount == 9)
                {
                    WaveAdvanced = true;
                    events.RescheduleEvent(EVENT_NEXT_WAVE, 1000);
                    events.ScheduleEvent(EVENT_ADVANCE_WAVE, 5000);
                }
            }

            events.Update(diff);

            switch (events.ExecuteEvent())
            {
                case EVENT_NEXT_WAVE:
                    uiWaveCount++;
                    AddWave();
                    break;
                case EVENT_ADVANCE_WAVE:
                    WaveAdvanced = false;
                    break;
                case EVENT_WIPE_CHECK:
                {
                    if (GetData(DATA_MARWYN_EVENT) == DONE)
                        break;

                    bool hasWiped = true;
                    Map::PlayerList const &PlayerList = instance->GetPlayers();
                    for (Map::PlayerList::const_iterator i = PlayerList.begin(); i != PlayerList.end(); ++i)
                    {
                        if (i->GetSource()->IsAlive() && !i->GetSource()->IsGameMaster())
                        {
                            hasWiped = false;
                            break;
                        }
                    }

                    if (hasWiped)
                        DoWipe();
                    else events.ScheduleEvent(EVENT_WIPE_CHECK, 2000);
                    break;
                }
                default:
                    break;
            }
        }
    };

};

void AddSC_instance_halls_of_reflection()
{
    new instance_halls_of_reflection();
}

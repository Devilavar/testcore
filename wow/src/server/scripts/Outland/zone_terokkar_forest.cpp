/*
 * Copyright (C) 2008-2014 TrinityCore 
 * Copyright (C) 2006-2009 ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
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

/* ScriptData
SDName: Terokkar_Forest
SD%Complete: 85
SDComment: Quest support: 9889, 10009, 10873, 10896, 10898, 11096, 10052, 10051. Skettis->Ogri'la Flight
SDCategory: Terokkar Forest
EndScriptData */

/* ContentData
npc_unkor_the_ruthless
npc_infested_root_walker
npc_rotting_forest_rager
npc_netherweb_victim
npc_floon
npc_isla_starmane
npc_slim
EndContentData */

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "ScriptedGossip.h"
#include "ScriptedEscortAI.h"
#include "Group.h"
#include "Player.h"
#include "WorldSession.h"

/*######
## npc_unkor_the_ruthless
######*/

enum UnkorTheRuthless
{
    SAY_SUBMIT                      = 0,

    FACTION_HOSTILE                 = 45,
    FACTION_FRIENDLY                = 35,
    QUEST_DONTKILLTHEFATONE         = 9889,

    SPELL_PULVERIZE                 = 2676
};

class npc_unkor_the_ruthless : public CreatureScript
{
public:
    npc_unkor_the_ruthless() : CreatureScript("npc_unkor_the_ruthless") { }

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new npc_unkor_the_ruthlessAI(creature);
    }

    struct npc_unkor_the_ruthlessAI : public ScriptedAI
    {
        npc_unkor_the_ruthlessAI(Creature* creature) : ScriptedAI(creature) { }

        bool CanDoQuest;
        uint32 UnkorUnfriendly_Timer;
        uint32 Pulverize_Timer;

        void Reset() override
        {
            CanDoQuest = false;
            UnkorUnfriendly_Timer = 0;
            Pulverize_Timer = 3000;
            me->SetStandState(UNIT_STAND_STATE_STAND);
            me->setFaction(FACTION_HOSTILE);
        }

        void EnterCombat(Unit* /*who*/) override { }

        void DoNice()
        {
            Talk(SAY_SUBMIT);
            me->setFaction(FACTION_FRIENDLY);
            me->SetStandState(UNIT_STAND_STATE_SIT);
            me->RemoveAllAuras();
            me->DeleteThreatList();
            me->CombatStop(true);
            UnkorUnfriendly_Timer = 60000;
        }

        void DamageTaken(Unit* done_by, uint32 &damage) override
        {
            Player* player = done_by->ToPlayer();

            if (player && me->HealthBelowPctDamaged(30, damage))
            {
                if (Group* group = player->GetGroup())
                {
                    for (GroupReference* itr = group->GetFirstMember(); itr != NULL; itr = itr->next())
                    {
                        Player* groupie = itr->GetSource();
                        if (groupie &&
                            groupie->GetQuestStatus(QUEST_DONTKILLTHEFATONE) == QUEST_STATUS_INCOMPLETE &&
                            groupie->GetReqKillOrCastCurrentCount(QUEST_DONTKILLTHEFATONE, 18260) == 10)
                        {
                            groupie->AreaExploredOrEventHappens(QUEST_DONTKILLTHEFATONE);
                            if (!CanDoQuest)
                                CanDoQuest = true;
                        }
                    }
                }
                else if (player->GetQuestStatus(QUEST_DONTKILLTHEFATONE) == QUEST_STATUS_INCOMPLETE &&
                    player->GetReqKillOrCastCurrentCount(QUEST_DONTKILLTHEFATONE, 18260) == 10)
                {
                    player->AreaExploredOrEventHappens(QUEST_DONTKILLTHEFATONE);
                    CanDoQuest = true;
                }
            }
        }

        void UpdateAI(uint32 diff) override
        {
            if (CanDoQuest)
            {
                if (!UnkorUnfriendly_Timer)
                {
                    //DoCast(me, SPELL_QUID9889);        //not using spell for now
                    DoNice();
                }
                else
                {
                    if (UnkorUnfriendly_Timer <= diff)
                    {
                        EnterEvadeMode();
                        return;
                    } else UnkorUnfriendly_Timer -= diff;
                }
            }

            if (!UpdateVictim())
                return;

            if (Pulverize_Timer <= diff)
            {
                DoCast(me, SPELL_PULVERIZE);
                Pulverize_Timer = 9000;
            } else Pulverize_Timer -= diff;

            DoMeleeAttackIfReady();
        }
    };
};

/*######
## npc_infested_root_walker
######*/

class npc_infested_root_walker : public CreatureScript
{
public:
    npc_infested_root_walker() : CreatureScript("npc_infested_root_walker") { }

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new npc_infested_root_walkerAI(creature);
    }

    struct npc_infested_root_walkerAI : public ScriptedAI
    {
        npc_infested_root_walkerAI(Creature* creature) : ScriptedAI(creature) { }

        void Reset() override { }
        void EnterCombat(Unit* /*who*/) override { }

        void DamageTaken(Unit* done_by, uint32 &damage) override
        {
            if (done_by && done_by->GetTypeId() == TYPEID_PLAYER)
                if (me->GetHealth() <= damage)
                    if (rand()%100 < 75)
                        //Summon Wood Mites
                        DoCast(me, 39130, true);
        }
    };
};

/*######
## npc_skywing
######*/
class npc_skywing : public CreatureScript
{
public:
    npc_skywing() : CreatureScript("npc_skywing") { }

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new npc_skywingAI(creature);
    }

    struct npc_skywingAI : public npc_escortAI
    {
    public:
        npc_skywingAI(Creature* creature) : npc_escortAI(creature) { }

        void WaypointReached(uint32 waypointId) override
        {
            Player* player = GetPlayerForEscort();
            if (!player)
                return;

            switch (waypointId)
            {
                case 8:
                    player->AreaExploredOrEventHappens(10898);
                    break;
            }
        }

        void EnterCombat(Unit* /*who*/) override { }

        void MoveInLineOfSight(Unit* who) override

        {
            if (HasEscortState(STATE_ESCORT_ESCORTING))
                return;

            Player* player = who->ToPlayer();
            if (player && player->GetQuestStatus(10898) == QUEST_STATUS_INCOMPLETE)
                if (me->IsWithinDistInMap(who, 10.0f))
                    Start(false, false, who->GetGUID());
        }

        void Reset() override { }

        void UpdateAI(uint32 diff) override
        {
            npc_escortAI::UpdateAI(diff);
        }
    };
};

/*######
## npc_rotting_forest_rager
######*/

class npc_rotting_forest_rager : public CreatureScript
{
public:
    npc_rotting_forest_rager() : CreatureScript("npc_rotting_forest_rager") { }

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new npc_rotting_forest_ragerAI(creature);
    }

    struct npc_rotting_forest_ragerAI : public ScriptedAI
    {
        npc_rotting_forest_ragerAI(Creature* creature) : ScriptedAI(creature) { }

        void Reset() override { }
        void EnterCombat(Unit* /*who*/) override { }

        void DamageTaken(Unit* done_by, uint32 &damage) override
        {
            if (done_by->GetTypeId() == TYPEID_PLAYER)
                if (me->GetHealth() <= damage)
                    if (rand()%100 < 75)
                        //Summon Lots of Wood Mights
                        DoCast(me, 39134, true);
        }
    };
};

/*######
## npc_netherweb_victim
######*/

enum NetherwebVictim
{
    QUEST_TARGET            = 22459
    //SPELL_FREE_WEBBED       = 38950
};

const uint32 netherwebVictims[6] =
{
    18470, 16805, 21242, 18452, 22482, 21285
};

class npc_netherweb_victim : public CreatureScript
{
public:
    npc_netherweb_victim() : CreatureScript("npc_netherweb_victim") { }

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new npc_netherweb_victimAI(creature);
    }

    struct npc_netherweb_victimAI : public ScriptedAI
    {
        npc_netherweb_victimAI(Creature* creature) : ScriptedAI(creature) { }

        void Reset() override { }
        void EnterCombat(Unit* /*who*/) override { }
        void MoveInLineOfSight(Unit* /*who*/) override { }


        void JustDied(Unit* killer) override
        {
            Player* player = killer->ToPlayer();
            if (!player)
                return;

            if (player->GetQuestStatus(10873) == QUEST_STATUS_INCOMPLETE)
            {
                if (rand()%100 < 25)
                {
                    me->SummonCreature(QUEST_TARGET, 0.0f, 0.0f, 0.0f, 0.0f, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 60000);
                    player->KilledMonsterCredit(QUEST_TARGET, 0);
                }
                else
                    me->SummonCreature(netherwebVictims[rand()%6], 0.0f, 0.0f, 0.0f, 0.0f, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 60000);

                if (rand()%100 < 75)
                    me->SummonCreature(netherwebVictims[rand()%6], 0.0f, 0.0f, 0.0f, 0.0f, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 60000);

                me->SummonCreature(netherwebVictims[rand()%6], 0.0f, 0.0f, 0.0f, 0.0f, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 60000);
            }
        }
    };
};

/*######
## npc_floon
######*/

#define GOSSIP_FLOON1           "You owe Sim'salabim money. Hand them over or die!"
#define GOSSIP_FLOON2           "Hand over the money or die...again!"

enum Floon
{
    SAY_FLOON_ATTACK        = 0,

    SPELL_SILENCE           = 6726,
    SPELL_FROSTBOLT         = 9672,
    SPELL_FROST_NOVA        = 11831,

    FACTION_HOSTILE_FL      = 1738,
    QUEST_CRACK_SKULLS      = 10009
};

class npc_floon : public CreatureScript
{
public:
    npc_floon() : CreatureScript("npc_floon") { }

    bool OnGossipSelect(Player* player, Creature* creature, uint32 /*sender*/, uint32 action) override
    {
        player->PlayerTalkClass->ClearMenus();
        if (action == GOSSIP_ACTION_INFO_DEF)
        {
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_FLOON2, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+1);
            player->SEND_GOSSIP_MENU(9443, creature->GetGUID());
        }
        if (action == GOSSIP_ACTION_INFO_DEF+1)
        {
            player->CLOSE_GOSSIP_MENU();
            creature->setFaction(FACTION_HOSTILE_FL);
            creature->AI()->Talk(SAY_FLOON_ATTACK, player);
            creature->AI()->AttackStart(player);
        }
        return true;
    }

    bool OnGossipHello(Player* player, Creature* creature) override
    {
        if (player->GetQuestStatus(QUEST_CRACK_SKULLS) == QUEST_STATUS_INCOMPLETE)
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_FLOON1, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF);

        player->SEND_GOSSIP_MENU(9442, creature->GetGUID());
        return true;
    }

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new npc_floonAI(creature);
    }

    struct npc_floonAI : public ScriptedAI
    {
        npc_floonAI(Creature* creature) : ScriptedAI(creature)
        {
            m_uiNormFaction = creature->getFaction();
        }

        uint32 m_uiNormFaction;
        uint32 Silence_Timer;
        uint32 Frostbolt_Timer;
        uint32 FrostNova_Timer;

        void Reset() override
        {
            Silence_Timer = 2000;
            Frostbolt_Timer = 4000;
            FrostNova_Timer = 9000;
            if (me->getFaction() != m_uiNormFaction)
                me->setFaction(m_uiNormFaction);
        }

        void EnterCombat(Unit* /*who*/) override { }

        void UpdateAI(uint32 diff) override
        {
            if (!UpdateVictim())
                return;

            if (Silence_Timer <= diff)
            {
                DoCastVictim(SPELL_SILENCE);
                Silence_Timer = 30000;
            } else Silence_Timer -= diff;

            if (FrostNova_Timer <= diff)
            {
                DoCast(me, SPELL_FROST_NOVA);
                FrostNova_Timer = 20000;
            } else FrostNova_Timer -= diff;

            if (Frostbolt_Timer <= diff)
            {
                DoCastVictim(SPELL_FROSTBOLT);
                Frostbolt_Timer = 5000;
            } else Frostbolt_Timer -= diff;

            DoMeleeAttackIfReady();
        }
    };
};

/*######
## npc_isla_starmane
######*/
enum IslaStarmaneData
{
    SAY_PROGRESS_1  = 0,
    SAY_PROGRESS_2  = 1,
    SAY_PROGRESS_3  = 2,
    SAY_PROGRESS_4  = 3,

    QUEST_EFTW_H    = 10052,
    QUEST_EFTW_A    = 10051,
    GO_CAGE         = 182794,
    SPELL_CAT       = 32447,
};

class npc_isla_starmane : public CreatureScript
{
public:
    npc_isla_starmane() : CreatureScript("npc_isla_starmane") { }

    struct npc_isla_starmaneAI : public npc_escortAI
    {
        npc_isla_starmaneAI(Creature* creature) : npc_escortAI(creature) { }

        void WaypointReached(uint32 waypointId) override
        {
            Player* player = GetPlayerForEscort();
            if (!player)
                return;

            switch (waypointId)
            {
                case 0:
                    if (GameObject* Cage = me->FindNearestGameObject(GO_CAGE, 10))
                        Cage->SetGoState(GO_STATE_ACTIVE);
                    break;
                case 2:
                    Talk(SAY_PROGRESS_1, player);
                    break;
                case 5:
                    Talk(SAY_PROGRESS_2, player);
                    break;
                case 6:
                    Talk(SAY_PROGRESS_3, player);
                    break;
                case 29:
                    Talk(SAY_PROGRESS_4, player);
                    if (player->GetTeam() == ALLIANCE)
                        player->GroupEventHappens(QUEST_EFTW_A, me);
                    else if (player->GetTeam() == HORDE)
                        player->GroupEventHappens(QUEST_EFTW_H, me);
                    me->SetInFront(player);
                    break;
                case 30:
                    me->HandleEmoteCommand(EMOTE_ONESHOT_WAVE);
                    break;
                case 31:
                    DoCast(me, SPELL_CAT);
                    me->SetWalk(false);
                    break;
            }
        }

        void Reset() override
        {
            me->RestoreFaction();
        }

        void JustDied(Unit* /*killer*/) override
        {
            if (Player* player = GetPlayerForEscort())
            {
                if (player->GetTeam() == ALLIANCE)
                    player->FailQuest(QUEST_EFTW_A);
                else if (player->GetTeam() == HORDE)
                    player->FailQuest(QUEST_EFTW_H);
            }
        }
    };

    bool OnQuestAccept(Player* player, Creature* creature, Quest const* quest) override
    {
        if (quest->GetQuestId() == QUEST_EFTW_H || quest->GetQuestId() == QUEST_EFTW_A)
        {
            CAST_AI(npc_escortAI, (creature->AI()))->Start(true, false, player->GetGUID());
            creature->setFaction(113);
        }
        return true;
    }

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new npc_isla_starmaneAI(creature);
    }
};

/*######
## go_skull_pile
######*/
#define GOSSIP_S_DARKSCREECHER_AKKARAI         "Summon Darkscreecher Akkarai"
#define GOSSIP_S_KARROG         "Summon Karrog"
#define GOSSIP_S_GEZZARAK_THE_HUNTRESS         "Summon Gezzarak the Huntress"
#define GOSSIP_S_VAKKIZ_THE_WINDRAGER         "Summon Vakkiz the Windrager"

class go_skull_pile : public GameObjectScript
{
public:
    go_skull_pile() : GameObjectScript("go_skull_pile") { }

    bool OnGossipSelect(Player* player, GameObject* go, uint32 sender, uint32 action) override
    {
        player->PlayerTalkClass->ClearMenus();
        switch (sender)
        {
            case GOSSIP_SENDER_MAIN:    SendActionMenu(player, go, action); break;
        }
        return true;
    }

    bool OnGossipHello(Player* player, GameObject* go) override
    {
        if ((player->GetQuestStatus(11885) == QUEST_STATUS_INCOMPLETE) || player->GetQuestRewardStatus(11885))
        {
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_S_DARKSCREECHER_AKKARAI, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_S_KARROG, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 2);
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_S_GEZZARAK_THE_HUNTRESS, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 3);
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_S_VAKKIZ_THE_WINDRAGER, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 4);
        }

        player->SEND_GOSSIP_MENU(go->GetGOInfo()->questgiver.gossipID, go->GetGUID());
        return true;
    }

    void SendActionMenu(Player* player, GameObject* /*go*/, uint32 action)
    {
        switch (action)
        {
            case GOSSIP_ACTION_INFO_DEF + 1:
                  player->CastSpell(player, 40642, false);
                break;
            case GOSSIP_ACTION_INFO_DEF + 2:
                  player->CastSpell(player, 40640, false);
                break;
            case GOSSIP_ACTION_INFO_DEF + 3:
                  player->CastSpell(player, 40632, false);
                break;
            case GOSSIP_ACTION_INFO_DEF + 4:
                  player->CastSpell(player, 40644, false);
                break;
        }
    }
};

/*######
## npc_slim
######*/

enum Slim
{
    FACTION_CONSORTIUM  = 933
};

class npc_slim : public CreatureScript
{
public:
    npc_slim() : CreatureScript("npc_slim") { }

    bool OnGossipSelect(Player* player, Creature* creature, uint32 /*sender*/, uint32 action) override
    {
        player->PlayerTalkClass->ClearMenus();
        if (action == GOSSIP_ACTION_TRADE)
            player->GetSession()->SendListInventory(creature->GetGUID());

        return true;
    }

    bool OnGossipHello(Player* player, Creature* creature) override
    {
        if (creature->IsVendor() && player->GetReputationRank(FACTION_CONSORTIUM) >= REP_FRIENDLY)
        {
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_VENDOR, GOSSIP_TEXT_BROWSE_GOODS, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_TRADE);
            player->SEND_GOSSIP_MENU(9896, creature->GetGUID());
        }
        else
            player->SEND_GOSSIP_MENU(9895, creature->GetGUID());

        return true;
    }
};

/*########
####npc_akuno
#####*/

enum Akuno
{
    QUEST_ESCAPING_THE_TOMB = 10887,
    NPC_CABAL_SKRIMISHER    = 21661
};

class npc_akuno : public CreatureScript
{
public:
    npc_akuno() : CreatureScript("npc_akuno") { }

    bool OnQuestAccept(Player* player, Creature* creature, Quest const* quest) override
    {
        if (quest->GetQuestId() == QUEST_ESCAPING_THE_TOMB)
        {
            if (npc_akunoAI* pEscortAI = CAST_AI(npc_akuno::npc_akunoAI, creature->AI()))
                pEscortAI->Start(false, false, player->GetGUID());

            if (player->GetTeamId() == TEAM_ALLIANCE)
                creature->setFaction(FACTION_ESCORT_A_NEUTRAL_PASSIVE);
            else
                creature->setFaction(FACTION_ESCORT_H_NEUTRAL_PASSIVE);
        }
        return true;
    }

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new npc_akunoAI(creature);
    }

    struct npc_akunoAI : public npc_escortAI
    {
        npc_akunoAI(Creature* creature) : npc_escortAI(creature) { }

        void WaypointReached(uint32 waypointId) override
        {
            Player* player = GetPlayerForEscort();
            if (!player)
                return;

            switch (waypointId)
            {
                case 3:
                    me->SummonCreature(NPC_CABAL_SKRIMISHER, -2795.99f, 5420.33f, -34.53f, 0.0f, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 25000);
                    me->SummonCreature(NPC_CABAL_SKRIMISHER, -2793.55f, 5412.79f, -34.53f, 0.0f, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 25000);
                    break;
                case 11:
                    if (player->GetTypeId() == TYPEID_PLAYER)
                        player->GroupEventHappens(QUEST_ESCAPING_THE_TOMB, me);
                    break;
            }
        }

        void JustSummoned(Creature* summon) override
        {
            summon->AI()->AttackStart(me);
        }
    };
};

/*######
## npc_letoll
## http://www.wowhead.com/quest=10922 "Digging Through Bones"
######*/

enum NpcLetoll {
    // Letoll texts
    SAY_LE_START                = 0,
    SAY_LE_KEEP_SAFE            = 1,
    SAY_LE_NORTH                = 2,
    SAY_LE_ARRIVE               = 3,
    SAY_LE_BURIED               = 4,
    SAY_LE_ALMOST               = 5,
    SAY_LE_DRUM                 = 6,
    SAY_LE_DISCOVERY            = 7,
    SAY_LE_NO_LEAVE             = 8,
    SAY_LE_SHUT                 = 9,
    SAY_LE_IN_YOUR_FACE         = 10,
    SAY_LE_HELP_HIM             = 11,
    SAY_LE_HELP_HER             = 12,
    EMOTE_LE_PICK_UP            = 13,
    SAY_LE_THANKS               = 14,

    // Researcher's texts
    SAY_RE_DRUM_REPLY           = 0,
    SAY_RE_DISCOVERY_REPLY      = 1,
    SAY_RE_NO_LEAVE_REPLY1      = 2,
    SAY_RE_NO_LEAVE_REPLY2      = 3,
    SAY_RE_NO_LEAVE_REPLY3      = 4,
    SAY_RE_NO_LEAVE_REPLY4      = 5,
    SAY_RE_REPLY_HEAR           = 6,

    QUEST_DIGGING_BONES         = 10922,

    NPC_RESEARCHER              = 22464,
    NPC_BONE_SIFTER             = 22466,

    GO_FUMPER                   = 185304,

    MAX_RESEARCHER              = 4,
};

class npc_letoll : public CreatureScript
{
public:
    npc_letoll() : CreatureScript("npc_letoll") { }

    bool OnQuestAccept(Player* player, Creature* creature, Quest const* quest) override
    {
        if (quest->GetQuestId() == QUEST_DIGGING_BONES)
        {
            if (npc_letollAI* escortAI = dynamic_cast<npc_letollAI*>(creature->AI()))
            {
                creature->setFaction(FACTION_ESCORT_N_NEUTRAL_PASSIVE);
                escortAI->Talk(SAY_LE_START);
                escortAI->Start(false, false, player->GetGUID(), quest, true);
            }
        }

        return true;
    }

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new npc_letollAI(creature);
    }

    struct npc_letollAI : public npc_escortAI
    {
        npc_letollAI(Creature* creature) : npc_escortAI(creature)
        {
            eventTimer = 5000;
            eventCount = 0;
        }

        std::list<Creature*> researchersList;
        uint32 eventTimer;
        uint32 eventCount;
        GameObject* fumper;


        void JustDied(Unit* /*killer*/) override
        {
            Player* player = GetPlayerForEscort();
            if (player)
                player->FailQuest(QUEST_DIGGING_BONES);
        }

        // gather a maximum of 4 researchers
        void SetFormation() 
        {
            researchersList.clear();

            uint32 count = 0;
            GetCreatureListWithEntryInGrid(researchersList, me, NPC_RESEARCHER, 30.0f);

            for (std::list<Creature*>::iterator itr = researchersList.begin(); itr != researchersList.end(); ++itr) 
            {
                float angle = count < MAX_RESEARCHER ? M_PI / MAX_RESEARCHER - (count * 2 * M_PI / MAX_RESEARCHER) : 0.0f;

                if (Creature * researcher = *itr) 
                {
                    if (researcher->IsAlive() && !researcher->IsInCombat())
                        researcher->GetMotionMaster()->MoveFollow(me, 2.5f, angle);
                }
                
                ++count;
            }
        }

        // gets an available researcher at the given position in the list
        Creature * GetAvailableResearcher(uint8 position) 
        {
            if (!researchersList.empty()) 
            {
                uint8 number = 1;

                for (std::list<Creature*>::iterator itr = researchersList.begin(); itr != researchersList.end(); ++itr) 
                {
                    if (position && position != number) 
                    {
                        ++number;
                        continue;
                    }

                    // given researcher must be alive and close by
                    if ((*itr)->IsAlive() && (*itr)->IsWithinDistInMap(me, 20.0f))
                        return (*itr);
                }
            }

            return NULL;
        }

        // Set Researchers Digging animation
        void SetResearchersAnimation(uint32 animId) 
        {
            if (!researchersList.empty()) 
            {
                for (std::list<Creature*>::iterator itr = researchersList.begin(); itr != researchersList.end(); ++itr) 
                {
                    // given researcher must be alive and close by
                    if ((*itr)->IsAlive() && (*itr)->IsWithinDistInMap(me, 20.0f))
                        (*itr)->SetUInt32Value(UNIT_NPC_EMOTESTATE, animId);
                }
            }
        }

        void WaypointReached(uint32 pointId) override
        {
            switch (pointId) 
            {
                case 0:
                    if (Player * player = GetPlayerForEscort())
                        Talk(SAY_LE_KEEP_SAFE, player);
                    // gather a maximum of 4 researchers
                    SetFormation();
                    break;
                case 1:
                    Talk(SAY_LE_NORTH);
                    break;
                case 10:
                    Talk(SAY_LE_ARRIVE);
                    break;
                case 11:
                    me->SetUInt32Value(UNIT_NPC_EMOTESTATE, EMOTE_STATE_WORK_MINING);
                    SetResearchersAnimation(EMOTE_STATE_WORK_MINING);
                    break;
                case 12:
                    Talk(SAY_LE_BURIED);
                    SetEscortPaused(true);
                    me->SetUInt32Value(UNIT_NPC_EMOTESTATE, EMOTE_STATE_WORK_MINING);
                    break;
                case 13:
                    me->SetUInt32Value(UNIT_NPC_EMOTESTATE, EMOTE_STATE_NONE);
                    SetResearchersAnimation(EMOTE_STATE_NONE);
                    SetRun();
                    break;
            }
        }

        void JustSummoned(Creature* summoned) override
        {
            Player* player = GetPlayerForEscort();
            if (player && player->IsAlive()) 
            {
                summoned->AI()->AttackStart(player);
                if (player->getGender() == GENDER_MALE)
                    Talk(SAY_LE_HELP_HIM);
                else
                    Talk(SAY_LE_HELP_HER); 
            } 
            else
                summoned->AI()->AttackStart(me);
        }

        void UpdateEscortAI(const uint32 diff) override
        {
            if (!UpdateVictim()) 
            {
                if (HasEscortState(STATE_ESCORT_PAUSED)) 
                {
                    if (eventTimer < diff) 
                    {
                        eventTimer = 7000;

                        switch (eventCount) 
                        {
                            case 0:
                                Talk(SAY_LE_ALMOST);
                                break;
                            case 1:
                                me->SetUInt32Value(UNIT_NPC_EMOTESTATE, EMOTE_STATE_NONE);
                                SetResearchersAnimation(EMOTE_STATE_NONE);
                                if (fumper = me->SummonGameObject(GO_FUMPER, -3547.019f, 5451.390f, -12.1882f, 0, 0, 0, 0, 0, HOUR))
                                    fumper->SetFlag(GAMEOBJECT_FLAGS, GO_FLAG_NOT_SELECTABLE);
                                Talk(SAY_LE_DRUM);
                                break;
                            case 2:
                                if (Creature * researcher = GetAvailableResearcher(0))
                                    researcher->AI()->Talk(SAY_RE_DRUM_REPLY);
                                break;
                            case 3:
                                Talk(SAY_LE_DISCOVERY);
                                break;
                            case 4:
                                if (Creature * researcher = GetAvailableResearcher(0))
                                    researcher->AI()->Talk(SAY_RE_DISCOVERY_REPLY);
                                break;
                            case 5:
                                Talk(SAY_LE_NO_LEAVE);
                                break;
                            case 6:
                                if (Creature * researcher = GetAvailableResearcher(1))
                                    researcher->AI()->Talk(SAY_RE_NO_LEAVE_REPLY1);
                                break;
                            case 7:
                                if (Creature * researcher = GetAvailableResearcher(2))
                                    researcher->AI()->Talk(SAY_RE_NO_LEAVE_REPLY2);
                                break;
                            case 8:
                                if (Creature * researcher = GetAvailableResearcher(3))
                                    researcher->AI()->Talk(SAY_RE_NO_LEAVE_REPLY3);
                                break;
                            case 9:
                                if (Creature * researcher = GetAvailableResearcher(4))
                                    researcher->AI()->Talk(SAY_RE_NO_LEAVE_REPLY4);
                                break;
                            case 10:
                                Talk(SAY_LE_SHUT);
                                break;
                            case 11:
                                if (Creature * researcher = GetAvailableResearcher(0))
                                    researcher->AI()->Talk(SAY_RE_REPLY_HEAR);
                                break;
                            case 12:
                                Talk(SAY_LE_IN_YOUR_FACE);
                                me->SummonCreature(NPC_BONE_SIFTER, 0.0f, 0.0f, 0.0f, 0.0f, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 30000);
                                break;
                            case 13:
                                Talk(EMOTE_LE_PICK_UP);

                                if (fumper)
                                    fumper->RemoveFromWorld();

                                if (Player * player = GetPlayerForEscort()) 
                                {
                                    Talk(SAY_LE_THANKS, player);
                                    player->GroupEventHappens(QUEST_DIGGING_BONES, me);
                                }

                               SetEscortPaused(false);
                                break;
                        }

                        ++eventCount;
                    } 
                    else
                        eventTimer -= diff;
                }

                return;
            }

            DoMeleeAttackIfReady();
        }
    };
};

/*######
## npc_mana_bomb_trigger
## http://www.wowhead.com/quest=10446 The Final Code (Alliance)
## http://www.wowhead.com/quest=10447 The Final Code (Horde)
######*/
enum FinalCode
{
    SAY_COUNT_5                 = 4,
    SAY_COUNT_4                 = 3,
    SAY_COUNT_3                 = 2,
    SAY_COUNT_2                 = 1,
    SAY_COUNT_1                 = 0,

    GO_MANA_BOMB                = 184725,

    SPELL_MANA_BOMB_LIGHTNING   = 37843,
    SPELL_MANA_BOMB_EXPL        = 35513,

    NPC_MANA_BOMB_EXPL_TRIGGER  = 20767,
    
    COUNTER_STEP_TIME           = 3 * IN_MILLISECONDS,

    EVENT_COUNTER_SEC_5         = 1,
    EVENT_COUNTER_SEC_4         = 2,
    EVENT_COUNTER_SEC_3         = 3,
    EVENT_COUNTER_SEC_2         = 4,
    EVENT_COUNTER_SEC_1         = 5,
    EVENT_BOMB_EXPLODE          = 6,
    EVENT_RESET                 = 7
};

class npc_mana_bomb_trigger : public CreatureScript
{
public:
    npc_mana_bomb_trigger() : CreatureScript("npc_mana_bomb_trigger") { }

    struct npc_mana_bomb_triggerAI : public ScriptedAI
    {
        EventMap events;
        bool isActive;
        GameObject* bomb;
        Player* activator;

        npc_mana_bomb_triggerAI(Creature* creature) : ScriptedAI(creature)
        {
            Reset();
        }

        void Reset() override
        {
            isActive = false;
            events.Reset();
            bomb = 0;
            activator = 0;
        }

        void AttackStart(Unit* target) override
        {
            // Regularly, this should not happen, but if it does, we have to stop it until reset.
            if (isActive)
                return;

            if (Player* player = target->ToPlayer()) 
            {
                if (bomb = GetClosestGameObjectWithEntry(me, GO_MANA_BOMB, INTERACTION_DISTANCE)) 
                {
                    isActive = true;
                    activator = player;
                    events.ScheduleEvent(EVENT_COUNTER_SEC_5, COUNTER_STEP_TIME);
                    player->KilledMonsterCredit(me->GetEntry(), me->GetGUID());
                }
            }
        }

        void DamageTaken(Unit* /*caster*/, uint32& damage) override
        {
            damage = 0;
        }

        void UpdateAI(uint32 diff) override
        {
            if (!isActive)
                return;

            events.Update(diff);

            while (uint32 event = events.ExecuteEvent()) 
            {
                me->CastSpell(me, SPELL_MANA_BOMB_LIGHTNING, false);

                switch (event) {
                    case EVENT_COUNTER_SEC_5:
                        if (bomb)
                            bomb->SetGoState(GO_STATE_ACTIVE);
                        Talk(SAY_COUNT_5);
                        events.ScheduleEvent(EVENT_COUNTER_SEC_4, COUNTER_STEP_TIME);
                        break;
                    case EVENT_COUNTER_SEC_4:
                        Talk(SAY_COUNT_4);
                        events.ScheduleEvent(EVENT_COUNTER_SEC_3, COUNTER_STEP_TIME);
                        break;
                    case EVENT_COUNTER_SEC_3:
                        Talk(SAY_COUNT_3);
                        events.ScheduleEvent(EVENT_COUNTER_SEC_2, COUNTER_STEP_TIME);
                        break;
                    case EVENT_COUNTER_SEC_2:
                        Talk(SAY_COUNT_2);
                        events.ScheduleEvent(EVENT_COUNTER_SEC_1, COUNTER_STEP_TIME);
                        break;
                    case EVENT_COUNTER_SEC_1:
                        Talk(SAY_COUNT_1);
                        events.ScheduleEvent(EVENT_BOMB_EXPLODE, COUNTER_STEP_TIME);
                        break;
                    case EVENT_BOMB_EXPLODE:
                        // BEWARE: If players are in the reach of the bomb, they will be killed by the explosion! (blizz-like)                        
                        me->CastSpell(me, SPELL_MANA_BOMB_EXPL, false);
                        events.ScheduleEvent(EVENT_RESET, 30 * IN_MILLISECONDS);
                        break;
                    case EVENT_RESET:
                        if (bomb)
                            bomb->SetGoState(GO_STATE_READY);
                        Reset();
                        break;
                    default:
                        break;
                }
            }
        }
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new npc_mana_bomb_triggerAI(creature);
    }
};

void AddSC_terokkar_forest()
{
    new npc_unkor_the_ruthless();
    new npc_infested_root_walker();
    new npc_rotting_forest_rager();
    new npc_netherweb_victim();
    new npc_floon();
    new npc_isla_starmane();
    new go_skull_pile();
    new npc_skywing();
    new npc_slim();
    new npc_akuno();
    new npc_letoll();
    new npc_mana_bomb_trigger();
}

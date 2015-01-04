#include "ScriptPCH.h"
#include "Language.h"
#include "Pet.h"
#include "World.h"
#include "BattlegroundMgr.h"
#include "ArenaTeam.h"
#include "ArenaTeamMgr.h"
#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "ScriptedGossip.h"
#include "Player.h"

enum Creatures
{
    NPC_AHUNE                   = 25740,
    NPC_FROZEN_CORE             = 25865,
    NPC_AHUNITE_COLDWEAVE       = 25756,
    NPC_AHUNITE_FROSTWIND       = 25757,
    NPC_AHUNITE_HAILSTONE       = 25755,
};

enum Spells
{
    // Ahune
    SPELL_AHUNES_SHIELD         = 45954,
    SPELL_COLD_SLAP             = 46145,

    SPELL_MAKE_BONFIRE          = 45930,
    SPELL_SUMMONING_VISUAL1     = 45937,
    SPELL_SUMMONING_VISUAL2     = 45938,
    SPELL_SUMMON_MINION_VISUAL  = 46103,
    SPELL_GHOST_VISUAL          = 46786,
    SPELL_RESURFACE             = 46402,

    SPELL_LOOT_CHEST            = 45939,
    SPELL_LOOT_CHEST_HC         = 46622,

    SPELL_AHUNE_ACHIEVEMENT     = 62043,

    // Coldweave
    SPELL_BITTER_BLAST          = 46406,

    // Frostwind
    SPELL_LIGHTNING_SHIELD      = 12550,
    SPELL_WIND_BUFFET           = 46568,

    // Hailstone
    SPELL_CHILLING_AURA         = 46885,
    SPELL_PULVERIZE             = 2676,
};

enum Events
{
    // Ahune
    EVENT_SWITCH_PHASE          = 0,
    EVENT_SUMMON_HAILSTONE      = 1,
    EVENT_SUMMON_COLDWEAVE      = 2,
    EVENT_SUMMON_FROSTWIND      = 3,
    EVENT_ICE_SPEAR             = 4,
    EVENT_COLD_SLAP             = 5,

    // Frozen Core
    EVENT_GHOST_VISUAL          = 6,
    EVENT_RESURFACE_SOON        = 7,

    // Coldweave
    EVENT_BITTER_BLAST          = 8,

    // Frostwind
    EVENT_WIND_BUFFET           = 9,

    // Hailstone
    EVENT_CHILLING_AURA         = 10,
    EVENT_PULVERIZE             = 11,
};

enum Phases
{
    PHASE_ONE    = 0,
    PHASE_TWO    = 1,
};

#define GOSSIP_SUMMON "Disturb the Stone and summon Lord Ahune.";

Position const SummonPositions[4] =
{
        {-88.495071f, -254.462997f, -1.077302f}, // Ahune / Frozen-Core
        {-90.891891f, -243.488068f, -1.116222f}, // Hailstone
        {-97.389175f, -239.780701f, -1.264044f}, // Coldweave #1
        {-85.160637f, -236.127808f, -1.572418f}, // Coldweave #2
};

class boss_ahune : public CreatureScript
{
    public:
        boss_ahune() : CreatureScript("boss_ahune") { }

        CreatureAI* GetAI(Creature* creature) const override
        {
            return new boss_ahuneAI(creature);
        }

        struct boss_ahuneAI : public ScriptedAI
        {
            boss_ahuneAI(Creature* creature) : ScriptedAI(creature), summons(me)
            {
                SetCombatMovement(false);
            }

            EventMap events;
            SummonList summons;

            uint64 frozenCoreGUID;

            void Reset() override
            {
                summons.DespawnAll();

                events.Reset();
                events.SetPhase(PHASE_ONE);

                frozenCoreGUID = 0;

                me->ApplySpellImmune(0, IMMUNITY_EFFECT, SPELL_EFFECT_KNOCK_BACK, true);
                me->ApplySpellImmune(0, IMMUNITY_EFFECT, SPELL_EFFECT_KNOCK_BACK_DEST, true);

                me->SetUInt32Value(UNIT_NPC_EMOTESTATE, 0);
                me->HandleEmoteCommand(EMOTE_ONESHOT_EMERGE);
            }

            void EnterCombat(Unit* who) override
            {
                DoZoneInCombat();

                events.Reset();
                events.SetPhase(PHASE_ONE);
                events.ScheduleEvent(EVENT_SWITCH_PHASE, 90000); // phase 2 after 90 seconds
                events.ScheduleEvent(EVENT_COLD_SLAP, 500, 0, PHASE_ONE); // every 500ms in melee range in phase 1
                events.ScheduleEvent(EVENT_SUMMON_HAILSTONE, 1000, 0, PHASE_ONE); // once in every phase 1
                events.ScheduleEvent(EVENT_SUMMON_COLDWEAVE, 8000, 0, PHASE_ONE); // every 7 seconds in phase 1
                events.ScheduleEvent(EVENT_ICE_SPEAR, 9500, 0, PHASE_ONE); // every 7 seconds in phase 1, first after 9.5 seconds

                me->CastSpell(me->GetPositionX(), me->GetPositionY(), me->GetPositionZ(), SPELL_SUMMONING_VISUAL1, true);
                me->AddAura(SPELL_AHUNES_SHIELD, me);
            }

            void JustDied(Unit* killer) override
            {
                Map::PlayerList const& playerList = me->GetMap()->GetPlayers();
                if (!playerList.isEmpty())
                    for (Map::PlayerList::const_iterator i = playerList.begin(); i != playerList.end(); ++i)
                        if (i->GetSource())
                            DoCast(i->GetSource(), SPELL_AHUNE_ACHIEVEMENT);

                DoCast(me, DUNGEON_MODE(SPELL_LOOT_CHEST, SPELL_LOOT_CHEST_HC));
            }

            void JustSummoned(Creature* summoned) override
            {
                DoZoneInCombat(summoned);

                summons.Summon(summoned);
            }

            void SummonedCreatureDespawn(Creature* summoned) override
            {
                summons.Despawn(summoned);
            }

            void UpdateAI(uint32 diff) override
            {
                if (!UpdateVictim())
                    return;

                events.Update(diff);

                while (uint32 eventId = events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_SWITCH_PHASE:
                            if (events.GetPhaseMask() & PHASE_ONE)
                            {
                                events.SetPhase(PHASE_TWO);
                                events.ScheduleEvent(EVENT_SWITCH_PHASE, 30000);

                                DoCast(me, SPELL_MAKE_BONFIRE);

                                me->SetReactState(REACT_PASSIVE);
                                me->AttackStop();
                                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_NON_ATTACKABLE);
                                me->SetUInt32Value(UNIT_NPC_EMOTESTATE, EMOTE_STATE_SUBMERGED);
                                me->HandleEmoteCommand(EMOTE_ONESHOT_SUBMERGE);

                                // Emote: Ahune retreats. His defenses diminish.

                                // spawn core
                                if (Unit* frozenCore = me->SummonCreature(NPC_FROZEN_CORE, SummonPositions[0], TEMPSUMMON_CORPSE_DESPAWN))
                                {
                                    frozenCoreGUID = frozenCore->GetGUID();
                                    frozenCore->SetHealth(me->GetHealth()); // sync health on phase change
                                }
                            }
                            else
                            {
                                events.SetPhase(PHASE_ONE);
                                events.ScheduleEvent(EVENT_SWITCH_PHASE, 90000);
                                events.ScheduleEvent(EVENT_SUMMON_HAILSTONE, 1000, 0, PHASE_ONE);
                                events.ScheduleEvent(EVENT_SUMMON_FROSTWIND, 9000, 0, PHASE_ONE);

                                me->SetReactState(REACT_AGGRESSIVE);
                                me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_NON_ATTACKABLE);
                                me->SetUInt32Value(UNIT_NPC_EMOTESTATE, 0);
                                me->HandleEmoteCommand(EMOTE_ONESHOT_EMERGE);

                                // despawn core
                                if (Creature* frozenCore = me->GetCreature(*me, frozenCoreGUID))
                                    frozenCore->DespawnOrUnsummon(0);
                            }
                            break;
                        case EVENT_COLD_SLAP:
                            if (Unit* target = SelectTarget(SELECT_TARGET_NEAREST, 0, 8.0f, true))
                                DoCast(target, SPELL_COLD_SLAP);
                            events.ScheduleEvent(EVENT_COLD_SLAP, 500, 0, PHASE_ONE);
                            break;
                        case EVENT_ICE_SPEAR:
                            // TODO: lots of spells involved, no idea of their order yet
                            break;
                        case EVENT_SUMMON_HAILSTONE:
                            me->SummonCreature(NPC_AHUNITE_HAILSTONE, SummonPositions[1], TEMPSUMMON_CORPSE_TIMED_DESPAWN, 3000);
                            break;
                        case EVENT_SUMMON_COLDWEAVE: // they always come in pairs of two
                            me->CastSpell(SummonPositions[2].GetPositionX(), SummonPositions[2].GetPositionY(), SummonPositions[2].GetPositionZ(), SPELL_SUMMON_MINION_VISUAL, false);
                            me->SummonCreature(NPC_AHUNITE_COLDWEAVE, SummonPositions[2], TEMPSUMMON_CORPSE_TIMED_DESPAWN, 3000);
                            me->CastSpell(SummonPositions[3].GetPositionX(), SummonPositions[3].GetPositionY(), SummonPositions[3].GetPositionZ(), SPELL_SUMMON_MINION_VISUAL, false);
                            me->SummonCreature(NPC_AHUNITE_COLDWEAVE, SummonPositions[3], TEMPSUMMON_CORPSE_TIMED_DESPAWN, 3000);
                            events.ScheduleEvent(EVENT_SUMMON_COLDWEAVE, 5000, 0, PHASE_ONE);
                            break;
                        case EVENT_SUMMON_FROSTWIND: // not in first phase 1
                            me->SummonCreature(NPC_AHUNITE_FROSTWIND, SummonPositions[urand(2,3)], TEMPSUMMON_CORPSE_TIMED_DESPAWN, 3000);
                            events.ScheduleEvent(EVENT_SUMMON_FROSTWIND, 7000, 0, PHASE_ONE);
                            break;
                    }
                }

                DoMeleeAttackIfReady();
            }
        };
};

class npc_frozen_core : public CreatureScript
{
    public:
        npc_frozen_core() : CreatureScript("npc_frozen_core") { }

        CreatureAI* GetAI(Creature* creature) const override
        {
            return new npc_frozen_coreAI(creature);
        }

        struct npc_frozen_coreAI : public ScriptedAI
        {
            npc_frozen_coreAI(Creature* creature) : ScriptedAI(creature)
            {
                SetCombatMovement(false);
            }

            EventMap events;

            void Reset() override
            {
                events.Reset();

                events.ScheduleEvent(EVENT_GHOST_VISUAL, 6000); // every 6 seconds Ghost Visual
                events.ScheduleEvent(EVENT_RESURFACE_SOON, 15000); // after 15 seconds Emote: Ahune will soon resurface.

                me->ApplySpellImmune(0, IMMUNITY_EFFECT, SPELL_EFFECT_KNOCK_BACK, true);
                me->ApplySpellImmune(0, IMMUNITY_EFFECT, SPELL_EFFECT_KNOCK_BACK_DEST, true);
            }

            void DamageTaken(Unit* who, uint32& damage) override
            {
                if (me->IsSummon())
                {
                    if (Unit* owner = me->GetOwner())
                    {
                        if (owner->GetHealth() > damage)
                            owner->SetHealth(owner->GetHealth() - damage);
                        else
                            who->Kill(owner);
                    }
                }
            }

            void UpdateAI(uint32 diff) override
            {
                if (!UpdateVictim())
                    return;

                events.Update(diff);

                while (uint32 eventId = events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_GHOST_VISUAL:
                            DoCast(me, SPELL_GHOST_VISUAL);
                            events.ScheduleEvent(EVENT_GHOST_VISUAL, 6000);
                            break;
                        case EVENT_RESURFACE_SOON:
                            // Emote: Ahune will soon resurface.
                            break;
                    }
                }

                DoMeleeAttackIfReady();
            }
        };
};

class npc_ahunite_hailstone : public CreatureScript
{
    public:
        npc_ahunite_hailstone() : CreatureScript("npc_ahunite_hailstone") { }

        CreatureAI* GetAI(Creature* creature) const override
        {
            return new npc_ahunite_hailstoneAI(creature);
        }

        struct npc_ahunite_hailstoneAI : public ScriptedAI
        {
            npc_ahunite_hailstoneAI(Creature* creature) : ScriptedAI(creature) { }

            EventMap events;

            void Reset() override
            {
                events.Reset();
                events.ScheduleEvent(EVENT_PULVERIZE, urand(6000, 8000));

                DoCast(me, SPELL_CHILLING_AURA);
            }

            void UpdateAI(uint32 diff) override
            {
                if (!UpdateVictim())
                    return;

                events.Update(diff);

                while (uint32 eventId = events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_PULVERIZE:
                            DoCastVictim(SPELL_PULVERIZE);
                            events.ScheduleEvent(EVENT_PULVERIZE, urand(6000, 8000));
                            break;
                    }
                }

                DoMeleeAttackIfReady();
            }
        };
};

class npc_ahunite_coldwave : public CreatureScript
{
    public:
        npc_ahunite_coldwave() : CreatureScript("npc_ahunite_coldwave") { }

        CreatureAI* GetAI(Creature* creature) const override
        {
            return new npc_ahunite_coldwaveAI(creature);
        }

        struct npc_ahunite_coldwaveAI : public ScriptedAI
        {
            npc_ahunite_coldwaveAI(Creature* creature) : ScriptedAI(creature) { }

            EventMap events;

            void Reset() override
            {
                events.Reset();
                events.ScheduleEvent(EVENT_BITTER_BLAST, 500);
            }

            void UpdateAI(uint32 diff) override
            {
                if (!UpdateVictim())
                    return;

                events.Update(diff);

                while (uint32 eventId = events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_BITTER_BLAST:
                            DoCastVictim(SPELL_BITTER_BLAST);
                            events.ScheduleEvent(EVENT_BITTER_BLAST, urand(5000, 7000));
                            break;
                    }
                }

                DoMeleeAttackIfReady();
            }
        };
};

class npc_ahunite_frostwind : public CreatureScript
{
    public:
        npc_ahunite_frostwind() : CreatureScript("npc_ahunite_frostwind") { }

        CreatureAI* GetAI(Creature* creature) const override
        {
            return new npc_ahunite_frostwindAI(creature);
        }

        struct npc_ahunite_frostwindAI : public ScriptedAI
        {
            npc_ahunite_frostwindAI(Creature* creature) : ScriptedAI(creature) { }

            EventMap events;

            void Reset() override
            {
                events.Reset();
                events.ScheduleEvent(EVENT_WIND_BUFFET, 2000); // TODO: get correct timing for wind buffet

                DoCast(me, SPELL_LIGHTNING_SHIELD);
            }

            void UpdateAI(uint32 diff) override
            {
                if (!UpdateVictim())
                    return;

                events.Update(diff);

                while (uint32 eventId = events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_WIND_BUFFET:
                            if (Unit* target = SelectTarget(SELECT_TARGET_FARTHEST, 0, 120.0f))
                                DoCast(target, SPELL_WIND_BUFFET);
                            events.ScheduleEvent(EVENT_WIND_BUFFET, urand(5000, 7000));
                            break;
                    }
                }

                DoMeleeAttackIfReady();
            }
        };
};

enum ArenaOptions
{
    ARENA_SPECTATE_MENU_CREATE_TEAM = 2,
    ARENA_SPECTATE_MENU_QUEUE,
    ARENA_SPECTATE_MENU_2V2_MATCHES,
    ARENA_SPECTATE_MENU_3V3_MATCHES,
    ARENA_SPECTATE_MENU_5V5_MATCHES,
    ARENA_SPECTATE_MENU_SPECTATE_PLAYER,
    ARENA_SPECTATE_MENU_PAGE_NEXT,
    ARENA_SPECTATE_MENU_PAGE_PREVIOUS,
};

class npc_arenamaster : public CreatureScript
{
public:
    npc_arenamaster() : CreatureScript("npc_arenamaster") { } // to horde from alliance

    struct npc_arenamasterAI : public ScriptedAI
    {
        npc_arenamasterAI(Creature* creature) : ScriptedAI(creature) {}

        void Reset() override
        {
            // you can add custom morphs here
            if (urand(0, 1))
            {
                me->SetDisplayId(19245);
                me->SetObjectScale(1.3f);
            }
            else
            {
                me->SetDisplayId(20236);
                me->SetObjectScale(0.5f);
            }
        }
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new  npc_arenamasterAI(creature);
    }

    const int MAX_RESULTS_PER_PAGE = 15;

    bool OnGossipHello(Player* player, Creature* creature) override
    {
        page = 1;
        CreateArenasMap();

        player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Spectate 2v2 Matches", GOSSIP_SENDER_MAIN, ARENA_SPECTATE_MENU_2V2_MATCHES);
        player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Spectate 3v3 Matches", GOSSIP_SENDER_MAIN, ARENA_SPECTATE_MENU_3V3_MATCHES);
        player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Spectate 5v5 Matches", GOSSIP_SENDER_MAIN, ARENA_SPECTATE_MENU_5V5_MATCHES);
        player->ADD_GOSSIP_ITEM_EXTENDED(GOSSIP_ICON_CHAT, "Spectate Player", GOSSIP_SENDER_MAIN, ARENA_SPECTATE_MENU_SPECTATE_PLAYER, "", 0, true);
        player->SEND_GOSSIP_MENU(1, creature->GetGUID());
        return true;
    }

    bool OnGossipSelect(Player* player, Creature* creature, uint32 sender, uint32 action) override
    {
        player->PlayerTalkClass->ClearMenus();

        switch (sender)
        {
            case GOSSIP_SENDER_MAIN:
            {
                switch (action)
                {
                    case GOSSIP_SENDER_MAIN:
                    {
                        OnGossipHello(player, creature);
                        break;
                    }
                    case ARENA_SPECTATE_MENU_CREATE_TEAM:
                    {
                        HandleCreateTeam(player, creature);
                        break;
                    }
                    case ARENA_SPECTATE_MENU_QUEUE:
                    {
                        HandleQueue(player, creature);
                        break;
                    }
                    case ARENA_SPECTATE_MENU_2V2_MATCHES:
                    case ARENA_SPECTATE_MENU_3V3_MATCHES:
                    case ARENA_SPECTATE_MENU_5V5_MATCHES:
                    {
                        HandleShowMatches(player, creature, sender, action);
                        break;
                    }
                }
                break;
            }
            case ARENA_SPECTATE_MENU_2V2_MATCHES:
            case ARENA_SPECTATE_MENU_3V3_MATCHES:
            case ARENA_SPECTATE_MENU_5V5_MATCHES:
            {
                switch (action)
                {
                    case ARENA_SPECTATE_MENU_PAGE_NEXT:
                    case ARENA_SPECTATE_MENU_PAGE_PREVIOUS:
                    {
                        HandleShowMatches(player, creature, sender, action);
                        break;
                    }
                    default:
                    {
                        AddPlayerToArena(player, action);
                        break;
                    }
                }
                break;
            }
        }
        return true;
    }

    bool OnGossipSelectCode(Player* player, Creature* creature, uint32 sender, uint32 action, const char* code)
    {
        switch (action) { case ARENA_SPECTATE_MENU_SPECTATE_PLAYER: HandleSpectatePlayer(player, code); break; }
        return true;
    }
    
    void HandleCreateTeam(Player* player, Creature* creature)
    {
        player->CLOSE_GOSSIP_MENU();
    
        WorldPacket data(SMSG_PETITION_SHOWLIST, 81);
        data << creature->GetGUID();
        data << uint8(3);                                                               // count
        // 2v2
        data << uint32(1);                                                              // index
        data << uint32(23560);                                                          // charter entry
        data << uint32(16161);                                                          // charter display id
        data << uint32(0);                                                              // charter cost
        data << uint32(2);                                                              // unknown
        data << uint32(2);   // required signs?
        // 3v3
        data << uint32(2);                                                              // index
        data << uint32(23561);                                                          // charter entry
        data << uint32(16161);                                                          // charter display id
        data << uint32(0);                                                              // charter cost
        data << uint32(3);                                                              // unknown
        data << uint32(3);   // required signs?
        // 5v5
        data << uint32(3);                                                              // index
        data << uint32(23562);                                                          // charter entry
        data << uint32(16161);                                                          // charter display id
        data << uint32(0);                                                              // charter cost
        data << uint32(5);                                                              // unknown
        data << uint32(5);   // required signs?
        player->GetSession()->SendPacket(&data);
    }
    
    void HandleQueue(Player* player, Creature* creature)
    {
        player->CLOSE_GOSSIP_MENU();
        player->GetSession()->SendBattleGroundList(creature->GetGUID(), BATTLEGROUND_AA);
    }
    
    void HandleShowMatches(Player* player, Creature* creature, uint32 sender, uint32 action)
    {
        ChatHandler handler = ChatHandler(player->GetSession());
        CreateArenasMap();
        uint8 arenaType = 0;
        uint32 gossipAction = 0;
    
        switch (sender)
        {
            case GOSSIP_SENDER_MAIN: arenaType = GetArenaTypeByAction(action); gossipAction = action; break;
            default:                 arenaType = GetArenaTypeByAction(sender); gossipAction = sender; break;
        }
    
        uint8 roomLeft = MAX_RESULTS_PER_PAGE;
    
        if (arenasMap.empty())
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "No Arena Matches In Progress", GOSSIP_SENDER_MAIN, gossipAction);
        else
        {
            BattlegroundContainer::iterator itr = arenasMap.begin();

            switch (action)
            {
                case ARENA_SPECTATE_MENU_PAGE_NEXT:
                    ++page;
                    break;
                case ARENA_SPECTATE_MENU_PAGE_PREVIOUS:
                    --page;
                    break;
                default:
                    break;
            }
            uint16 increment = page * roomLeft;

            if (page > 1)
            {
                for (uint16 i = 0; i < increment; ++i)
                {
                    ++itr;
                    if (itr == arenasMap.end())
                        break;
                }
            }

            for (; itr != arenasMap.end(); ++itr)
            {
                if (!roomLeft)
                    break;

                Battleground* arena = itr->second;
                if (!arena)
                    continue;
    
                if (!CheckBattleground(arena))
                    continue;
    
                if (!arena->GetBgMap())
                    continue;
    
                if (arena->GetArenaType() != arenaType)
                    continue;
    
                if (!arena->isRated())
                    continue;
    
                ArenaTeam* goldTeam = sArenaTeamMgr->GetArenaTeamById(arena->GetArenaTeamIdByIndex(BG_TEAM_ALLIANCE));
                if (!goldTeam)
                    continue;
    
                ArenaTeam* greenTeam = sArenaTeamMgr->GetArenaTeamById(arena->GetArenaTeamIdByIndex(BG_TEAM_HORDE));
                if (!greenTeam)
                    continue;
    
                if (goldTeam->GetStats().Rating < sWorld->getIntConfig(CONFIG_ARENA_SPECTATOR_MIN_RATING) && greenTeam->GetStats().Rating < sWorld->getIntConfig(CONFIG_ARENA_SPECTATOR_MIN_RATING))
                    continue;
    
                std::stringstream gossipText;
                // GoldTeamName[GoldTeamRating] vs GreenTeamName[GreenTeamRating]
                gossipText << goldTeam->GetName() << "[" << goldTeam->GetRating() << "] vs " << greenTeam->GetName() << "[" << greenTeam->GetRating() << "]";
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, gossipText.str(), gossipAction, itr->first);
                --roomLeft;
            }
    
            if (roomLeft == MAX_RESULTS_PER_PAGE)
            {
                std::stringstream gossipText;
                if (sWorld->getIntConfig(CONFIG_ARENA_SPECTATOR_MIN_RATING))
                    gossipText << "No Arena Matches Above " << sWorld->getIntConfig(CONFIG_ARENA_SPECTATOR_MIN_RATING) << " Rating In Progress";
                else
                    gossipText << "No Arena Matches In Progress";

                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, gossipText.str(), GOSSIP_SENDER_MAIN, gossipAction);
            }
    
            if (itr != arenasMap.end())
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Next Page", gossipAction, ARENA_SPECTATE_MENU_PAGE_NEXT);

            if (page > 1)
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Previous Page", gossipAction, ARENA_SPECTATE_MENU_PAGE_PREVIOUS);
        }
    
        player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TALK, "Back", GOSSIP_SENDER_MAIN, GOSSIP_SENDER_MAIN);
        player->SEND_GOSSIP_MENU(1, creature->GetGUID());
    }
    
    void HandleSpectatePlayer(Player* player, const char* cPlayerName)
    {
        ChatHandler handler = ChatHandler(player->GetSession());
        CreateArenasMap();
        uint32 arenaId = 0;
        Player* target = NULL;
        uint64 tmpGuid = sObjectMgr->GetPlayerGUIDByName(cPlayerName);
        uint32 guidLow = GUID_LOPART(tmpGuid);
    
        player->CLOSE_GOSSIP_MENU();
    
        if (!guidLow)
        {
            handler.PSendSysMessage("Unable to find a player with that name.");
            return;
        }
    
        target = sObjectMgr->GetPlayerByLowGUID(guidLow);
        if (!target)
        {
            handler.PSendSysMessage("The player you're trying to spectate is offline.");
            return;
        }
    
        arenaId = target->GetBattlegroundId();
        if (!arenaId)
        {
            handler.PSendSysMessage("The player you're trying to spectate isn't in a arena.");
            return;
        }
    
        if (!arenasMap[arenaId])
        {
            handler.PSendSysMessage("The arena match of the player you're trying to spectate either hasn't started yet or doesn't exist.");
            return;
        }
    
        AddPlayerToArena(player, arenaId);
    }
    
    void AddPlayerToArena(Player* player, uint32 action)
    {
        ChatHandler handler = ChatHandler(player->GetSession());
        CreateArenasMap();
        player->CLOSE_GOSSIP_MENU();
    
        if (!arenasMap[action])
        {
            handler.PSendSysMessage("The arena match you're trying to spectate either hasn't started yet or doesn't exist.");
            return;
        }
    
        if (!CheckBattleground(arenasMap[action]))
        {
            handler.PSendSysMessage("The arena match you're trying to spectate hasn't started yet.");
            return;
        }
    
        if (!arenasMap[action]->GetBgMap())
        {
            handler.PSendSysMessage("The map for the arena match you're trying to spectate is being deconstructed.");
            return;
        }
    
        Battleground* arena = sBattlegroundMgr->GetBattleground(action, arenasMap[action]->GetTypeID(false));
        if (!arena)
        {
            handler.PSendSysMessage("The arena match of the player you're trying to spectate no longer exists.");
            return;
        }
    
        player->RemoveFromAllBattlegroundQueues();
        player->SetArenaSpectatorState(true);
        player->SetBattlegroundId(action, arena->GetTypeID(false));
        player->SetBattlegroundEntryPoint();
        arena->HandlePlayerUnderMap(player);
    }
    
    bool CheckBattleground(Battleground* bg)
    {
        if (bg->GetStatus() != STATUS_IN_PROGRESS)
            return false;
    
        if (!bg->GetPlayersCountByTeam(ALLIANCE) || !bg->GetPlayersCountByTeam(HORDE))
            return false;
    
        return true;
    }

    void CreateArenasMap()
    {
        arenasMap.clear();
        BattlegroundMgr::BattlegroundDataContainer const* store = sBattlegroundMgr->GetBattlegroundDataStore();

        for (BattlegroundMgr::BattlegroundDataContainer::const_iterator itr1 = store->begin(); itr1 != store->end(); ++itr1)
        {
            const BattlegroundContainer& bgs = itr1->second.m_Battlegrounds;
            for (BattlegroundContainer::const_iterator itr = bgs.begin(); itr != bgs.end();++itr)
            {
                if (itr->second->isArena())
                    arenasMap[itr->first] = itr->second;
            }
        }
    }
    private:
        BattlegroundContainer arenasMap;
        uint32 page;

        uint8 GetArenaTypeByAction(uint32 action)
        {
            switch (action)
            {
                case ARENA_SPECTATE_MENU_2V2_MATCHES: return ARENA_TYPE_2v2;
                case ARENA_SPECTATE_MENU_3V3_MATCHES: return ARENA_TYPE_3v3;
                case ARENA_SPECTATE_MENU_5V5_MATCHES: return ARENA_TYPE_5v5;
                default: return 0;
            }
        }
};

enum Chata
{
	SAY_EBI_MU_MAIKATA = 0,
};

class queldelar_giver : public CreatureScript
{
public:
    queldelar_giver() : CreatureScript("queldelar_giver") { }

    bool OnGossipHello(Player* player, Creature* creature) override
    {
		/* ALLIANCE */
		if (player->GetTeamId() == TEAM_ALLIANCE) // ako e aliqns
			if (player->IsQuestRewarded(24535))
				if (player->getClass() == CLASS_PRIEST || CLASS_SHAMAN || CLASS_DRUID) // shaman priest i druid ne mogat sword
				{
					Quest const* questa = sObjectMgr->GetQuestTemplate(24795);
					player->AddQuest(questa, NULL);
					player->CompleteQuest(24796);
				}
				else // ako ne sa priest/sham/druid da imat questa za mechovete
				{
					Quest const* questa = sObjectMgr->GetQuestTemplate(24796);
					player->AddQuest(questa, NULL);
					player->CompleteQuest(24796);
				}
		
		if (player->GetTeamId() == TEAM_HORDE) // ako e hord
			if (player->IsQuestRewarded(24563))
				if (player->getClass() == CLASS_PRIEST || CLASS_SHAMAN || CLASS_DRUID) // shaman priest i druid ne mogat sword
				{
					Quest const* questa = sObjectMgr->GetQuestTemplate(24795);
					player->AddQuest(questa, NULL);
					player->CompleteQuest(24798);
				}
				else // ako ne sa priest/sham/druid da imat questa za mechovete
				{
					Quest const* questa = sObjectMgr->GetQuestTemplate(24796);
					player->AddQuest(questa, NULL);
					player->CompleteQuest(24800);
				}


		TC_LOG_INFO("server.worldserver", "nito 1 ot 2te"); // debug log
		
	return true;
    }
};

void AddSC_npcs_custom()
{
    new boss_ahune();
    new npc_frozen_core();
    new npc_ahunite_hailstone();
    new npc_ahunite_coldwave();
    new npc_ahunite_frostwind();
    new npc_arenamaster();
    new queldelar_giver();
}

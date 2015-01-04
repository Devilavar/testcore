/*
 * Copyright (C) 2008-2014 TrinityCore 
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

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "ScriptedGossip.h"
#include "SpellScript.h"
#include "SpellAuraEffects.h"
#include "CombatAI.h"
#include "Player.h"
#include "Vehicle.h"
#include "oculus.h"

enum GossipNPCs
{
	GOSSIP_TEXTID_DRAKES = 13267,
	GOSSIP_TEXTID_BELGARISTRASZ1 = 12916,
	GOSSIP_TEXTID_BELGARISTRASZ2 = 13466,
	GOSSIP_TEXTID_BELGARISTRASZ3 = 13254,
	GOSSIP_TEXTID_VERDISA1 = 1,
	GOSSIP_TEXTID_VERDISA2 = 1,
	GOSSIP_TEXTID_VERDISA3 = 13258,
	GOSSIP_TEXTID_ETERNOS1 = 1,
	GOSSIP_TEXTID_ETERNOS2 = 1,
	GOSSIP_TEXTID_ETERNOS3 = 13256,

	ITEM_EMERALD_ESSENCE = 37815,
	ITEM_AMBER_ESSENCE = 37859,
	ITEM_RUBY_ESSENCE = 37860,
};

enum Drakes
{
/*
 * Ruby Drake (27756)
 * (summoned by spell Ruby Essence (37860) --> Call Amber Drake (49462) --> Summon 27756)
 */
    SPELL_RIDE_RUBY_DRAKE_QUE           = 49463,          // Apply Aura: Periodic Trigger, Interval: 3 seconds --> 49464
    SPELL_RUBY_DRAKE_SADDLE             = 49464,          // Allows you to ride on the back of an Amber Drake. --> Dummy
    SPELL_RUBY_SEARING_WRATH            = 50232,          // (60 yds) - Instant - Breathes a stream of fire at an enemy dragon, dealing 6800 to 9200 Fire damage and then jumping to additional dragons within 30 yards. Each jump increases the damage by 50%. Affects up to 5 total targets
    SPELL_RUBY_EVASIVE_AURA             = 50248,          // Instant - Allows the Ruby Drake to generate Evasive Charges when hit by hostile attacks and spells.
    SPELL_RUBY_EVASIVE_CHARGES          = 50241,
    SPELL_RUBY_EVASIVE_MANEUVERS        = 50240,          // Instant - 5 sec. cooldown - Allows your drake to dodge all incoming attacks and spells. Requires Evasive Charges to use. Each attack or spell dodged while this ability is active burns one Evasive Charge. Lasts 30 sec. or until all charges are exhausted.
    // you do not have acces to until you kill Mage-Lord Urom
    SPELL_RUBY_MARTYR                   = 50253,          // Instant - 10 sec. cooldown - Redirect all harmful spells cast at friendly drakes to yourself for 10 sec.

/*
 * Amber Drake (27755)
 * (summoned by spell Amber Essence (37859) --> Call Amber Drake (49461) --> Summon 27755)
 */
    SPELL_RIDE_AMBER_DRAKE_QUE          = 49459,          // Apply Aura: Periodic Trigger, Interval: 3 seconds --> 49460
    SPELL_AMBER_DRAKE_SADDLE            = 49460,          // Allows you to ride on the back of an Amber Drake. --> Dummy
    SPELL_AMBER_SHOCK_CHARGE            = 49836,
    SPELL_AMBER_SHOCK_LANCE             = 49840,          // (60 yds) - Instant - Deals 4822 to 5602 Arcane damage and detonates all Shock Charges on an enemy dragon. Damage is increased by 6525 for each detonated.
    SPELL_AMBER_STOP_TIME               = 49838,          // Instant - 1 min cooldown - Halts the passage of time, freezing all enemy dragons in place for 10 sec. This attack applies 5 Shock Charges to each affected target.
    // you do not have access to until you kill the Mage-Lord Urom.
    SPELL_AMBER_TEMPORAL_RIFT           = 49592,          // (60 yds) - Channeled - Channels a temporal rift on an enemy dragon for 10 sec. While trapped in the rift, all damage done to the target is increased by 100%. In addition, for every 15, 000 damage done to a target affected by Temporal Rift, 1 Shock Charge is generated.

/*
 * Emerald Drake (27692)
 * (summoned by spell Emerald Essence (37815) --> Call Emerald Drake (49345) --> Summon 27692)
 */
    SPELL_RIDE_EMERALD_DRAKE_QUE        = 49427,         // Apply Aura: Periodic Trigger, Interval: 3 seconds --> 49346
    SPELL_EMERALD_DRAKE_SADDLE          = 49346,         // Allows you to ride on the back of an Amber Drake. --> Dummy
    SPELL_EMERALD_LEECHING_POISON       = 50328,         // (60 yds) - Instant - Poisons the enemy dragon, leeching 1300 to the caster every 2 sec. for 12 sec. Stacks up to 3 times.
    SPELL_EMERALD_TOUCH_THE_NIGHTMARE   = 50341,         // (60 yds) - Instant - Consumes 30% of the caster's max health to inflict 25, 000 nature damage to an enemy dragon and reduce the damage it deals by 25% for 30 sec.
    // you do not have access to until you kill the Mage-Lord Urom
    SPELL_EMERALD_DREAM_FUNNEL          = 50344,         // (60 yds) - Channeled - Transfers 5% of the caster's max health to a friendly drake every second for 10 seconds as long as the caster channels.

    // Misc
    POINT_LAND                          = 2,
    POINT_TAKE_OFF                      = 3
};

enum DrakeEvents
{
    EVENT_WELCOME = 1,
    EVENT_ABILITIES,
    EVENT_SPECIAL_ATTACK,
    EVENT_LOW_HEALTH,
    EVENT_RESET_LOW_HEALTH,
    EVENT_TAKE_OFF
};

enum Says
{
    SAY_VAROS                         = 0,
    SAY_UROM                          = 1,
    SAY_BELGARISTRASZ                 = 0,
    SAY_DRAKES_TAKEOFF                = 0,
    WHISPER_DRAKES_WELCOME            = 1,
    WHISPER_DRAKES_ABILITIES          = 2,
    WHISPER_DRAKES_SPECIAL            = 3,
    WHISPER_DRAKES_LOWHEALTH          = 4
};

#define HAS_ESSENCE(a) ((a)->HasItemCount(ITEM_EMERALD_ESSENCE, 1) || (a)->HasItemCount(ITEM_AMBER_ESSENCE, 1) || (a)->HasItemCount(ITEM_RUBY_ESSENCE, 1))
#define GOSSIP_ITEM_DRAKES         "So where do we go from here?"
#define GOSSIP_ITEM_BELGARISTRASZ1 "I want to fly on the wings of the Red Flight"
#define GOSSIP_ITEM_BELGARISTRASZ2 "What abilities do Ruby Drakes have?"
#define GOSSIP_ITEM_VERDISA1       "I want to fly on the wings of the Green Flight"
#define GOSSIP_ITEM_VERDISA2       "What abilities do Emerald Drakes have?"
#define GOSSIP_ITEM_ETERNOS1       "I want to fly on the wings of the Bronze Flight"
#define GOSSIP_ITEM_ETERNOS2       "What abilities do Amber Drakes have?"

class npc_oculus_drake_givers : public CreatureScript
{
    public:
        npc_oculus_drake_givers() : CreatureScript("npc_oculus_drake_givers") { }

        bool OnGossipSelect(Player* player, Creature* creature, uint32 /*sender*/, uint32 action)
        {
            player->PlayerTalkClass->ClearMenus();
            switch (creature->GetEntry())
            {
                case NPC_VERDISA:
                    switch (action)
                    {
                        case GOSSIP_ACTION_INFO_DEF + 1:
                            if (!HAS_ESSENCE(player))
                            {
                                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ITEM_VERDISA1, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 2);
                                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ITEM_VERDISA2, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 3);
                                player->SEND_GOSSIP_MENU(GOSSIP_TEXTID_VERDISA1, creature->GetGUID());
                            }
                            else
                            {
                                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ITEM_VERDISA2, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 3);
                                player->SEND_GOSSIP_MENU(GOSSIP_TEXTID_VERDISA2, creature->GetGUID());
                            }
                            break;
                        case GOSSIP_ACTION_INFO_DEF + 2:
                        {
                            player->AddItem(ITEM_EMERALD_ESSENCE, 1);
                            player->CLOSE_GOSSIP_MENU();
                            break;
                        }
                        case GOSSIP_ACTION_INFO_DEF + 3:
                            player->SEND_GOSSIP_MENU(GOSSIP_TEXTID_VERDISA3, creature->GetGUID());
                            break;
                    }
                    break;
                case NPC_BELGARISTRASZ:
                    switch (action)
                    {
                        case GOSSIP_ACTION_INFO_DEF + 1:
                            if (!HAS_ESSENCE(player))
                            {
                                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ITEM_BELGARISTRASZ1, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 2);
                                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ITEM_BELGARISTRASZ2, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 3);
                                player->SEND_GOSSIP_MENU(GOSSIP_TEXTID_BELGARISTRASZ1, creature->GetGUID());
                            }
                            else
                            {
                                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ITEM_BELGARISTRASZ2, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 3);
                                player->SEND_GOSSIP_MENU(GOSSIP_TEXTID_BELGARISTRASZ2, creature->GetGUID());
                            }
                            break;
                        case GOSSIP_ACTION_INFO_DEF + 2:
                        {
                            player->AddItem(ITEM_RUBY_ESSENCE, 1);
                            player->CLOSE_GOSSIP_MENU();
                            break;
                        }
                        case GOSSIP_ACTION_INFO_DEF + 3:
                            player->SEND_GOSSIP_MENU(GOSSIP_TEXTID_BELGARISTRASZ3, creature->GetGUID());
                            break;
                    }
                    break;
                case NPC_ETERNOS:
                    switch (action)
                    {
                        case GOSSIP_ACTION_INFO_DEF + 1:
                            if (!HAS_ESSENCE(player))
                            {
                                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ITEM_ETERNOS1, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 2);
                                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ITEM_ETERNOS2, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 3);
                                player->SEND_GOSSIP_MENU(GOSSIP_TEXTID_ETERNOS1, creature->GetGUID());
                            }
                            else
                            {
                                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ITEM_ETERNOS2, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 3);
                                player->SEND_GOSSIP_MENU(GOSSIP_TEXTID_ETERNOS2, creature->GetGUID());
                            }
                            break;
                        case GOSSIP_ACTION_INFO_DEF + 2:
                        {
                            player->AddItem(ITEM_AMBER_ESSENCE, 1);
                            player->CLOSE_GOSSIP_MENU();
                            break;
                        }
                        case GOSSIP_ACTION_INFO_DEF + 3:
                            player->SEND_GOSSIP_MENU(GOSSIP_TEXTID_ETERNOS3, creature->GetGUID());
                            break;
                    }
                    break;
            }

            return true;
        }

        bool OnGossipHello(Player* player, Creature* creature)
        {
            if (creature->IsQuestGiver())
                player->PrepareQuestMenu(creature->GetGUID());

            if (InstanceScript* instance = creature->GetInstanceScript())
            {
                if (instance->GetBossState(DATA_DRAKOS) == DONE)
                {
                    player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ITEM_DRAKES, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);
                    player->SEND_GOSSIP_MENU(GOSSIP_TEXTID_DRAKES, creature->GetGUID());
                }
            }

            return true;
        }
};

class npc_oculus_mounts : public CreatureScript
{
    public:
        npc_oculus_mounts() : CreatureScript("npc_oculus_mounts") { }

        struct npc_oculus_mountsAI : public NullCreatureAI
        {
            npc_oculus_mountsAI(Creature* c) : NullCreatureAI(c)
            {
                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                _enterTimer = 1500;
                _entered = false;
            }

            void PassengerBoarded(Unit* /*unit*/, int8 /*seat*/, bool apply)
            {
                if (!apply)
                    me->DespawnOrUnsummon(1500);
            }

            void UpdateAI(uint32 diff) override
            {
                if (!_entered)
                {
                    if (_enterTimer <= diff)
                    {
                        uint32 spellId = 0;
                        _entered = true;

                        switch (me->GetEntry())
                        {
                            case NPC_EMERALD_DRAKE_VEHICLE:
                                spellId = 49346;
                                break;
							case NPC_AMBER_DRAKE_VEHICLE:
                                spellId = 49460;
                                break;
							case NPC_RUBY_DRAKE_VEHICLE:
                                spellId = 49464;
                                break;
                        }

                        if (!me->ToTempSummon())
                            return;

                        Unit* summoner = me->ToTempSummon()->GetSummoner();

                        if (summoner && summoner->IsAlive() && summoner->GetDistance(me) < 30.0f && !summoner->IsInCombat())
                        {
                            me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                            summoner->CastSpell(me, spellId, true);
                        }
                        else
                            me->DespawnOrUnsummon();
                    }
                    else
                        _enterTimer -= diff;
                }
            }

        private:
            bool _entered;
            uint32 _enterTimer;
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_oculus_mountsAI(creature);
        }
};


class npc_image_belgaristrasz : public CreatureScript
{
    public:
        npc_image_belgaristrasz() : CreatureScript("npc_image_belgaristrasz") { }

        struct npc_image_belgaristraszAI : public ScriptedAI
        {
            npc_image_belgaristraszAI(Creature* creature) : ScriptedAI(creature) { }

            void IsSummonedBy(Unit* summoner) override
            {
                if (summoner->GetEntry() == NPC_VAROS)
                {
                   Talk(SAY_VAROS);
                   me->DespawnOrUnsummon(60000);
                }

                if (summoner->GetEntry() == NPC_UROM)
                {
                   Talk(SAY_UROM);
                   me->DespawnOrUnsummon(60000);
                }
            }
        };

        CreatureAI* GetAI(Creature* creature) const override
        {
            return GetOculusAI<npc_image_belgaristraszAI>(creature);
        }
};

class npc_ruby_emerald_amber_drake : public CreatureScript
{
    public:
        npc_ruby_emerald_amber_drake() : CreatureScript("npc_ruby_emerald_amber_drake") { }

        struct npc_ruby_emerald_amber_drakeAI : public VehicleAI
        {
            npc_ruby_emerald_amber_drakeAI(Creature* creature) : VehicleAI(creature)
            {
                _instance = creature->GetInstanceScript();
            }

            void Reset() override
            {
                _events.Reset();
                _healthWarning = true;
            }

            void IsSummonedBy(Unit* summoner) override
            {
                if (_instance->GetBossState(DATA_EREGOS) == IN_PROGRESS)
                    if (Creature* eregos = me->FindNearestCreature(NPC_EREGOS, 450.0f, true))
                        eregos->DespawnOrUnsummon(); // On retail this kills abusive call of drake during engaged Eregos

                me->SetFacingToObject(summoner);

                switch (me->GetEntry())
                {
                    case NPC_RUBY_DRAKE_VEHICLE:
                        me->CastSpell(summoner, SPELL_RIDE_RUBY_DRAKE_QUE);
                        break;
                    case NPC_EMERALD_DRAKE_VEHICLE:
                        me->CastSpell(summoner, SPELL_RIDE_EMERALD_DRAKE_QUE);
                        break;
                    case NPC_AMBER_DRAKE_VEHICLE:
                        me->CastSpell(summoner, SPELL_RIDE_AMBER_DRAKE_QUE);
                        break;
                    default:
                        return;
                }

                Position pos = summoner->GetPosition();
                me->GetMotionMaster()->MovePoint(POINT_LAND, pos);
            }

            void MovementInform(uint32 type, uint32 id) override
            {
                if (type == POINT_MOTION_TYPE && id == POINT_LAND)
                    me->SetDisableGravity(false); // Needed this for proper animation after spawn, the summon in air fall to ground bug leave no other option for now, if this isn't used the drake will only walk on move.
            }

            void PassengerBoarded(Unit* passenger, int8 /*seatId*/, bool apply) override
            {
                if (passenger->GetTypeId() != TYPEID_PLAYER)
                    return;

                if (apply)
                {
                    if (_instance->GetBossState(DATA_VAROS) != DONE)
                        _events.ScheduleEvent(EVENT_WELCOME, 10 * IN_MILLISECONDS);

                    else if (_instance->GetBossState(DATA_UROM) == DONE)
                        _events.ScheduleEvent(EVENT_SPECIAL_ATTACK, 10 * IN_MILLISECONDS);
                }
                else
                {
                    _events.Reset();
                    _events.ScheduleEvent(EVENT_TAKE_OFF, 2 * IN_MILLISECONDS);
                }
            }

            void UpdateAI(uint32 diff) override
            {
                if (_healthWarning)
                {
                    if (me->GetHealthPct() <= 40.0f)
                        _events.ScheduleEvent(EVENT_LOW_HEALTH, 0);
                }

                _events.Update(diff);

                while (uint32 eventId = _events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_WELCOME:
                            if (Unit* creator = ObjectAccessor::GetUnit(*me, me->GetCreatorGUID()))
                                Talk(WHISPER_DRAKES_WELCOME, creator);
                            _events.ScheduleEvent(EVENT_ABILITIES, 5 * IN_MILLISECONDS);
                            break;
                        case EVENT_ABILITIES:
                            if (Unit* creator = ObjectAccessor::GetUnit(*me, me->GetCreatorGUID()))
                                Talk(WHISPER_DRAKES_ABILITIES, creator);
                            break;
                        case EVENT_SPECIAL_ATTACK:
                            if (Unit* creator = ObjectAccessor::GetUnit(*me, me->GetCreatorGUID()))
                                Talk(WHISPER_DRAKES_SPECIAL, creator);
                            break;
                        case EVENT_LOW_HEALTH:
                            if (Unit* creator = ObjectAccessor::GetUnit(*me, me->GetCreatorGUID()))
                                Talk(WHISPER_DRAKES_LOWHEALTH, creator);
                            _healthWarning = false;
                            _events.ScheduleEvent(EVENT_RESET_LOW_HEALTH, 25000);
                            break;
                        case EVENT_RESET_LOW_HEALTH:
                            _healthWarning = true;
                            break;
                        case EVENT_TAKE_OFF:
                        {
                            me->DespawnOrUnsummon(2050);
                            me->SetOrientation(2.5f);
                            me->SetSpeed(MOVE_FLIGHT, 1.0f, true);
                            Talk(SAY_DRAKES_TAKEOFF);
                            Position pos = me->GetPosition();
                            Position offset = { 10.0f, 10.0f, 12.0f, 0.0f };
                            pos.RelocateOffset(offset);
                            me->SetDisableGravity(true);
                            me->GetMotionMaster()->MovePoint(POINT_TAKE_OFF, pos);
                            break;
                        }
                        default:
                            break;
                    }
                }
            };

        private:
            InstanceScript* _instance;
            EventMap _events;
            bool _healthWarning;
        };

        CreatureAI* GetAI(Creature* creature) const override
        {
            return GetOculusAI<npc_ruby_emerald_amber_drakeAI>(creature);
        }
};

// 49345 - Call Emerald Drake
// 49461 - Call Amber Drake
// 49462 - Call Ruby Drake
class spell_oculus_call_ruby_emerald_amber_drake : public SpellScriptLoader
{
    public:
        spell_oculus_call_ruby_emerald_amber_drake() : SpellScriptLoader("spell_oculus_call_ruby_emerald_amber_drake") { }

        class spell_oculus_call_ruby_emerald_amber_drake_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_oculus_call_ruby_emerald_amber_drake_SpellScript);

            void SetDest(SpellDestination& dest)
            {
                // Adjust effect summon position
                Position const offset = { 0.0f, 0.0f, 12.0f, 0.0f };
                dest.RelocateOffset(offset);
            }

            void Register() override
            {
                OnDestinationTargetSelect += SpellDestinationTargetSelectFn(spell_oculus_call_ruby_emerald_amber_drake_SpellScript::SetDest, EFFECT_0, TARGET_DEST_CASTER_FRONT);
            }
        };

        SpellScript* GetSpellScript() const override
        {
            return new spell_oculus_call_ruby_emerald_amber_drake_SpellScript();
        }
};

// 49427 - Ride Emerald Drake Que
// 49459 - Ride Amber Drake Que
// 49463 - Ride Ruby Drake Que
class spell_oculus_ride_ruby_emerald_amber_drake_que : public SpellScriptLoader
{
    public:
        spell_oculus_ride_ruby_emerald_amber_drake_que() : SpellScriptLoader("spell_oculus_ride_ruby_emerald_amber_drake_que") { }

        class spell_oculus_ride_ruby_emerald_amber_drake_que_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_oculus_ride_ruby_emerald_amber_drake_que_AuraScript);

            void HandlePeriodic(AuraEffect const* aurEff)
            {
                // caster of the triggered spell is wrong for an unknown reason, handle it here correctly
                PreventDefaultAction();
                if (Unit* caster = GetCaster())
                    GetTarget()->CastSpell(caster, GetSpellInfo()->Effects[aurEff->GetEffIndex()].TriggerSpell, true);
            }

            void Register() override
            {
                OnEffectPeriodic += AuraEffectPeriodicFn(spell_oculus_ride_ruby_emerald_amber_drake_que_AuraScript::HandlePeriodic, EFFECT_0, SPELL_AURA_PERIODIC_TRIGGER_SPELL);
            }
        };

        AuraScript* GetAuraScript() const override
        {
            return new spell_oculus_ride_ruby_emerald_amber_drake_que_AuraScript();
        }
};

// 50240 - Evasive Maneuvers
class spell_oculus_evasive_maneuvers : public SpellScriptLoader
{
    public:
        spell_oculus_evasive_maneuvers() : SpellScriptLoader("spell_oculus_evasive_maneuvers") { }

        class spell_oculus_evasive_maneuvers_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_oculus_evasive_maneuvers_AuraScript);

            bool Validate(SpellInfo const* /*spellInfo*/) override
            {
                if (!sSpellMgr->GetSpellInfo(SPELL_RUBY_EVASIVE_CHARGES))
                    return false;
                return true;
            }

            void HandleProc(AuraEffect const* /*aurEff*/, ProcEventInfo& /*eventInfo*/)
            {
                PreventDefaultAction();
                GetTarget()->RemoveAuraFromStack(SPELL_RUBY_EVASIVE_CHARGES);
                if (!GetTarget()->HasAura(SPELL_RUBY_EVASIVE_CHARGES))
                    Remove();
            }

            void Register() override
            {
                OnEffectProc += AuraEffectProcFn(spell_oculus_evasive_maneuvers_AuraScript::HandleProc, EFFECT_2, SPELL_AURA_PROC_TRIGGER_SPELL);
            }
        };

        AuraScript* GetAuraScript() const override
        {
            return new spell_oculus_evasive_maneuvers_AuraScript();
        }
};

// 49840 - Shock Lance
class spell_oculus_shock_lance : public SpellScriptLoader
{
    public:
        spell_oculus_shock_lance() : SpellScriptLoader("spell_oculus_shock_lance") { }

        class spell_oculus_shock_lance_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_oculus_shock_lance_SpellScript);

            bool Validate(SpellInfo const* /*spellInfo*/) override
            {
                if (!sSpellMgr->GetSpellInfo(SPELL_AMBER_SHOCK_CHARGE))
                    return false;
                return true;
            }

            void CalcDamage()
            {
                int32 damage = GetHitDamage();
                if (Unit* target = GetHitUnit())
                    if (AuraEffect const* shockCharges = target->GetAuraEffect(SPELL_AMBER_SHOCK_CHARGE, EFFECT_0, GetCaster()->GetGUID()))
                    {
                        damage += shockCharges->GetAmount();
                        shockCharges->GetBase()->Remove();
                    }

                SetHitDamage(damage);
            }

            void Register() override
            {
                OnHit += SpellHitFn(spell_oculus_shock_lance_SpellScript::CalcDamage);
            }
        };

        SpellScript* GetSpellScript() const override
        {
            return new spell_oculus_shock_lance_SpellScript();
        }
};

// 49838 - Stop Time
class spell_oculus_stop_time : public SpellScriptLoader
{
    public:
        spell_oculus_stop_time() : SpellScriptLoader("spell_oculus_stop_time") { }

        class spell_oculus_stop_time_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_oculus_stop_time_AuraScript);

            bool Validate(SpellInfo const* /*spellInfo*/) override
            {
                if (!sSpellMgr->GetSpellInfo(SPELL_AMBER_SHOCK_CHARGE))
                    return false;
                return true;
            }

            void Apply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                Unit* caster = GetCaster();
                if (!caster)
                    return;

                Unit* target = GetTarget();
                for (uint32 i = 0; i < 5; ++i)
                    caster->CastSpell(target, SPELL_AMBER_SHOCK_CHARGE, true);
            }

            void Register() override
            {
                AfterEffectApply += AuraEffectApplyFn(spell_oculus_stop_time_AuraScript::Apply, EFFECT_0, SPELL_AURA_MOD_STUN, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const override
        {
            return new spell_oculus_stop_time_AuraScript();
        }
};

// 49592 - Temporal Rift
class spell_oculus_temporal_rift : public SpellScriptLoader
{
    public:
        spell_oculus_temporal_rift() : SpellScriptLoader("spell_oculus_temporal_rift") { }

        class spell_oculus_temporal_rift_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_oculus_temporal_rift_AuraScript);

            bool Validate(SpellInfo const* /*spellInfo*/) override
            {
                if (!sSpellMgr->GetSpellInfo(SPELL_AMBER_SHOCK_CHARGE))
                    return false;
                return true;
            }

            void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
            {
                PreventDefaultAction();
                int32 amount = aurEff->GetAmount() + eventInfo.GetDamageInfo()->GetDamage();

                if (amount >= 15000)
                {
                    if (Unit* caster = GetCaster())
                        caster->CastSpell(GetTarget(), SPELL_AMBER_SHOCK_CHARGE, true);
                    amount -= 15000;
                }

                const_cast<AuraEffect*>(aurEff)->SetAmount(amount);
            }

            void Register() override
            {
                OnEffectProc += AuraEffectProcFn(spell_oculus_temporal_rift_AuraScript::HandleProc, EFFECT_2, SPELL_AURA_DUMMY);
            }
        };

        AuraScript* GetAuraScript() const override
        {
            return new spell_oculus_temporal_rift_AuraScript();
        }
};

// 50341 - Touch the Nightmare
class spell_oculus_touch_the_nightmare : public SpellScriptLoader
{
    public:
        spell_oculus_touch_the_nightmare() : SpellScriptLoader("spell_oculus_touch_the_nightmare") { }

        class spell_oculus_touch_the_nightmare_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_oculus_touch_the_nightmare_SpellScript);

            void HandleDamageCalc(SpellEffIndex /*effIndex*/)
            {
                SetHitDamage(int32(GetCaster()->CountPctFromMaxHealth(30)));
            }

            void Register() override
            {
                OnEffectHitTarget += SpellEffectFn(spell_oculus_touch_the_nightmare_SpellScript::HandleDamageCalc, EFFECT_2, SPELL_EFFECT_SCHOOL_DAMAGE);
            }
        };

        SpellScript* GetSpellScript() const override
        {
            return new spell_oculus_touch_the_nightmare_SpellScript();
        }
};

// 50344 - Dream Funnel
class spell_oculus_dream_funnel : public SpellScriptLoader
{
    public:
        spell_oculus_dream_funnel() : SpellScriptLoader("spell_oculus_dream_funnel") { }

        class spell_oculus_dream_funnel_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_oculus_dream_funnel_AuraScript);

            void HandleEffectCalcAmount(AuraEffect const* /*aurEff*/, int32& amount, bool& canBeRecalculated)
            {
                if (Unit* caster = GetCaster())
                    amount = int32(caster->CountPctFromMaxHealth(5));

                canBeRecalculated = false;
            }

            void Register() override
            {
                DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_oculus_dream_funnel_AuraScript::HandleEffectCalcAmount, EFFECT_0, SPELL_AURA_PERIODIC_HEAL);
                DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_oculus_dream_funnel_AuraScript::HandleEffectCalcAmount, EFFECT_2, SPELL_AURA_PERIODIC_DAMAGE);
            }
        };

        AuraScript* GetAuraScript() const override
        {
            return new spell_oculus_dream_funnel_AuraScript();
        }
};

void AddSC_oculus()
{
	new npc_oculus_drake_givers();
	new npc_oculus_mounts();
    new npc_image_belgaristrasz();
    new npc_ruby_emerald_amber_drake();
    new spell_oculus_call_ruby_emerald_amber_drake();
    new spell_oculus_ride_ruby_emerald_amber_drake_que();
    new spell_oculus_evasive_maneuvers();
    new spell_oculus_shock_lance();
    new spell_oculus_stop_time();
    new spell_oculus_temporal_rift();
    new spell_oculus_touch_the_nightmare();
    new spell_oculus_dream_funnel();
}

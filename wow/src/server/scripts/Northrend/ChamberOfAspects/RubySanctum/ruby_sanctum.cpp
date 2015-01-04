#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "ScriptedGossip.h"
#include "ruby_sanctum.h"
 
enum Texts
{
    SAY_XERESTRASZA_EVENT       = 0,
    SAY_XERESTRASZA_EVENT_1     = 1,
    SAY_XERESTRASZA_EVENT_2     = 2,
    SAY_XERESTRASZA_EVENT_3     = 3,
    SAY_XERESTRASZA_EVENT_4     = 4,
    SAY_XERESTRASZA_EVENT_5     = 5,
    SAY_XERESTRASZA_EVENT_6     = 6,
    SAY_XERESTRASZA_EVENT_7     = 7,
    SAY_XERESTRASZA_INTRO       = 8,
    
    COMMANDER_RALLY_EMOTE       = 0,
};
 
enum Events
{
    EVENT_XERESTRASZA_EVENT_1   = 1,
    EVENT_XERESTRASZA_EVENT_2   = 2,
    EVENT_XERESTRASZA_EVENT_3   = 3,
    EVENT_XERESTRASZA_EVENT_4   = 4,
    EVENT_XERESTRASZA_EVENT_5   = 5,
    EVENT_XERESTRASZA_EVENT_6   = 6,
    EVENT_XERESTRASZA_EVENT_7   = 7,
};
 
enum RubySanctumTrash
{
    CharscaleInvoker            = 40417,
    CharscaleAssaulter          = 40419,
    CharscaleElite              = 40421,
    CharScaleCommander          = 40423,
};
 
enum RubySanctumTrashSpells
{
    // charscale Assualter    
    SPELL_CLEAVE                = 15284,
    SPELL_SHOCKWAVE_10          = 75417,
    SPELL_SHOCKWAVE_25          = 75418,
    SPELL_RALLYING_SHOUT        = 75416,
 
    // Charscale Elite
    SPELL_SKULL_CRACK           = 15621,
 
    // Charscale Invoker
    SPELL_SCORCH_10             = 75412,
    SPELL_SCORCH_25             = 75419,
    SPELL_FLAME_WAVE            = 75413,
 
    // Charscale Commander
    SPELL_RALLYING_SHOUT_C      = 75414,
    SPELL_MORTAL_STRIKE         = 13737,
};
 
enum RubySanctumTrashEvents
{
    EVENT_CLEAVE                = 1,
    EVENT_SHOCKWAVE             = 2,
    EVENT_MORTAL_STRIKE         = 3,
    EVENT_SKULL_CRACK           = 4,
    EVENT_FLAME_WAVE            = 5,
    EVENT_SCORCH                = 6,
    EVENT_RALLY_COMMANDER       = 7,
    EVENT_RALLY                 = 8,
};
 
Position const xerestraszaMovePos = 
{
    3151.236f, 379.8733f, 86.31996f, 0.0f
};
 
class npc_xerestrasza : public CreatureScript
{
public:
 
    npc_xerestrasza() : CreatureScript("npc_xerestrasza")
    {
    }
 
    struct npc_xerestraszaAI : public ScriptedAI
    {
 
        npc_xerestraszaAI(Creature * creature) : ScriptedAI(creature)
        {
            isIntro = true;
            introDone = false;
        }
 
        void Reset() override
        {
            events.Reset();
            me->RemoveFlag(UNIT_NPC_FLAGS, GOSSIP_OPTION_QUESTGIVER);
        }
 
        void DoAction(int32 action) override
        {
            if (action == ACTION_BALTHARUS_DEATH)
            {
                me->setActive(true);
                isIntro = false;
 
                Talk(SAY_XERESTRASZA_EVENT);
                me->SetWalk(true);
                me->GetMotionMaster()->MovePoint(0, xerestraszaMovePos);
 
                events.ScheduleEvent(EVENT_XERESTRASZA_EVENT_1, 16000);
                events.ScheduleEvent(EVENT_XERESTRASZA_EVENT_2, 25000);
                events.ScheduleEvent(EVENT_XERESTRASZA_EVENT_3, 32000);
                events.ScheduleEvent(EVENT_XERESTRASZA_EVENT_4, 42000);
                events.ScheduleEvent(EVENT_XERESTRASZA_EVENT_5, 51000);
                events.ScheduleEvent(EVENT_XERESTRASZA_EVENT_6, 61000);
                events.ScheduleEvent(EVENT_XERESTRASZA_EVENT_7, 69000);
            } else if (action == ACTION_INTRO_BALTHARUS && !introDone)
            {
                introDone = true;
                Talk(SAY_XERESTRASZA_INTRO);
            }
        }
 
        void UpdateAI(uint32 diff) override
        {
            if (isIntro)
                return;
 
            events.Update(diff);
 
            while (uint32 eventId = events.ExecuteEvent())
            {
                switch (eventId)
                {
                    case EVENT_XERESTRASZA_EVENT_1:
                        Talk(SAY_XERESTRASZA_EVENT_1);
                        break;
 
                    case EVENT_XERESTRASZA_EVENT_2:
                        Talk(SAY_XERESTRASZA_EVENT_2);
                        break;
 
                    case EVENT_XERESTRASZA_EVENT_3:
                        Talk(SAY_XERESTRASZA_EVENT_3);
                        break;
 
                    case EVENT_XERESTRASZA_EVENT_4:
                        Talk(SAY_XERESTRASZA_EVENT_4);
                        break;
 
                    case EVENT_XERESTRASZA_EVENT_5:
                        Talk(SAY_XERESTRASZA_EVENT_5);
                        break;
 
                    case EVENT_XERESTRASZA_EVENT_6:
                        Talk(SAY_XERESTRASZA_EVENT_6);
                        break;
 
                    case EVENT_XERESTRASZA_EVENT_7:
                        me->SetFlag(UNIT_NPC_FLAGS, GOSSIP_OPTION_QUESTGIVER);
                        Talk(SAY_XERESTRASZA_EVENT_7);
                        me->setActive(false);
                        break;
 
                    default:
                        break;
                }
            }
        }
 
    private:
        EventMap events;
        bool isIntro;
        bool introDone;
    };
 
    CreatureAI* GetAI(Creature* creature) const override
    {
        return GetRubySanctumAI<npc_xerestraszaAI> (creature);
    }
};
 
// Trash
class npc_charscale_invoker : public CreatureScript
{
public:
 
    npc_charscale_invoker() : CreatureScript("npc_charscale_invoker")
    {
    }
 
    struct npc_charscale_invokerAI : public ScriptedAI
    {
        npc_charscale_invokerAI(Creature * creature) : ScriptedAI(creature)
        {
            me->ApplySpellImmune(0, IMMUNITY_EFFECT, SPELL_EFFECT_KNOCK_BACK, true);
            me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_GRIP, true);
        }
 
        void Reset() override
        {
            events.Reset();
            events.ScheduleEvent(EVENT_FLAME_WAVE, urand(10000, 20000));
            events.ScheduleEvent(EVENT_SCORCH, urand(5000, 10000));
            events.ScheduleEvent(EVENT_RALLY, 30000);
            me->InterruptNonMeleeSpells(true);
        }
 
        void UpdateAI(uint32 diff) override
        {
            if (!UpdateVictim())
                return;
 
            events.Update(diff);
 
            if (me->HasUnitState(UNIT_STATE_CASTING))
                return;
 
            while (uint32 eventId = events.ExecuteEvent())
            {
                switch (eventId)
                {
                    case EVENT_FLAME_WAVE:
                        DoCastAOE(SPELL_FLAME_WAVE);
                        events.RescheduleEvent(EVENT_FLAME_WAVE, urand(15000, 30000));
                        break;
 
                    case EVENT_SCORCH:
                        if (Unit * target = SelectTarget(SELECT_TARGET_RANDOM, 0, 30.0f, true))
                            DoCast(target, RAID_MODE(SPELL_SCORCH_10, SPELL_SCORCH_25, SPELL_SCORCH_10, SPELL_SCORCH_25));
                        events.RescheduleEvent(EVENT_SCORCH, urand(5000, 10000));
                        break;
 
                    case EVENT_RALLY:
                        DoCast(SPELL_RALLYING_SHOUT);
                        events.RescheduleEvent(EVENT_RALLY, 30000);
                        break;
                }
            }
 
            DoMeleeAttackIfReady();
        }
 
        void JustDied(Unit* /*killer*/) override
        {
            events.Reset();
        }
 
    private:
        EventMap events;
    };
 
    CreatureAI * GetAI(Creature* creature) const override
    {
        return GetRubySanctumAI<npc_charscale_invokerAI> (creature);
    }
};
 
class npc_charscale_assaulter : public CreatureScript
{
public:
 
    npc_charscale_assaulter() : CreatureScript("npc_charscale_assaulter")
    {
    }
 
    struct npc_charscale_assaulterAI : public ScriptedAI
    {
        npc_charscale_assaulterAI(Creature * creature) : ScriptedAI(creature)
        {
            me->ApplySpellImmune(0, IMMUNITY_EFFECT, SPELL_EFFECT_KNOCK_BACK, true);
            me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_GRIP, true);
        }
 
        void Reset() override
        {
            events.Reset();
            events.ScheduleEvent(EVENT_CLEAVE, urand(10000, 20000));
            events.ScheduleEvent(EVENT_SHOCKWAVE, urand(5000, 10000));
            events.ScheduleEvent(EVENT_RALLY, 30000);
            me->InterruptNonMeleeSpells(true);
        }
 
        void UpdateAI(uint32 diff) override
        {
            if (!UpdateVictim())
                return;
 
            events.Update(diff);
 
            if (me->HasUnitState(UNIT_STATE_CASTING))
                return;
 
            while (uint32 eventId = events.ExecuteEvent())
            {
                switch (eventId)
                {
                    case EVENT_CLEAVE:
                        DoCastVictim(SPELL_CLEAVE);
                        events.ScheduleEvent(EVENT_CLEAVE, urand(5000, 10000));
                        break;
 
                    case EVENT_SHOCKWAVE:
                        DoCastAOE(RAID_MODE(SPELL_SHOCKWAVE_10, SPELL_SHOCKWAVE_25, SPELL_SHOCKWAVE_10, SPELL_SHOCKWAVE_25));
                        events.RescheduleEvent(EVENT_SHOCKWAVE, urand(10000, 20000));
                        break;
 
                    case EVENT_RALLY:
                        DoCast(SPELL_RALLYING_SHOUT);
                        events.RescheduleEvent(EVENT_RALLY, 30000);
                        break;
                }
            }
 
            DoMeleeAttackIfReady();
        }
 
        void JustDied(Unit* /*killer*/) override
        {
            events.Reset();
        }
 
    private:
        EventMap events;
    };
 
    CreatureAI* GetAI(Creature* creature) const override
    {
        return GetRubySanctumAI<npc_charscale_assaulterAI> (creature);
    }
};
 
class npc_charscale_elite : public CreatureScript
{
public:
 
    npc_charscale_elite() : CreatureScript("npc_charscale_elite")
    {
    }
 
    struct npc_charscale_eliteAI : public ScriptedAI
    {
        npc_charscale_eliteAI(Creature * creature) : ScriptedAI(creature)
        {
            me->ApplySpellImmune(0, IMMUNITY_EFFECT, SPELL_EFFECT_KNOCK_BACK, true);
            me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_GRIP, true);
        }
 
        void Reset() override
        {
            events.Reset();
            events.ScheduleEvent(EVENT_SKULL_CRACK, urand(5000, 10000));
            me->InterruptNonMeleeSpells(true);
        }
 
        void UpdateAI(uint32 diff) override
        {
            if (!UpdateVictim())
                return;
 
            events.Update(diff);
 
            if (me->HasUnitState(UNIT_STATE_CASTING))
                return;
 
            while (uint32 eventId = events.ExecuteEvent())
            {
                if (eventId == EVENT_SKULL_CRACK)
                {
                    DoCastVictim(SPELL_SKULL_CRACK);
                    events.ScheduleEvent(EVENT_SKULL_CRACK, urand(10000, 20000));
                }
            }
 
            DoMeleeAttackIfReady();
        }
 
        void JustDied(Unit* /*killer*/) override
        {
            events.Reset();
        }
 
    private:
        EventMap events;
    };
 
    CreatureAI* GetAI(Creature* creature) const override
    {
        return GetRubySanctumAI<npc_charscale_eliteAI> (creature);
    }
};
 
class npc_charscale_commander : public CreatureScript
{
public:
 
    npc_charscale_commander() : CreatureScript("npc_charscale_commander")
    {
    }
 
    struct npc_charscale_commanderAI : public ScriptedAI
    {
        npc_charscale_commanderAI(Creature * creature) : ScriptedAI(creature)
        {
            instance = me->GetInstanceScript();
 
            me->ApplySpellImmune(0, IMMUNITY_EFFECT, SPELL_EFFECT_KNOCK_BACK, true);
            me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_GRIP, true);
        }
 
        void Reset() override
        {
            events.Reset();
            events.ScheduleEvent(EVENT_MORTAL_STRIKE, urand(9000, 13000));
            events.ScheduleEvent(EVENT_RALLY_COMMANDER, 20000);
            me->InterruptNonMeleeSpells(true);
        }
 
        void UpdateAI(uint32 diff) override
        {
            if (!UpdateVictim())
                return;
 
            events.Update(diff);
 
            if (me->HasUnitState(UNIT_STATE_CASTING))
                return;
 
            while (uint32 eventId = events.ExecuteEvent())
            {
                switch (eventId)
                {
                    case EVENT_RALLY_COMMANDER:
                        me->AddAura(SPELL_RALLYING_SHOUT_C, me);
                        me->CallForHelp(30.0f); // setting this to high *will* pull the whole instance :p
                        Talk(COMMANDER_RALLY_EMOTE);
                        events.ScheduleEvent(EVENT_RALLY_COMMANDER, urand(60000, 90000));
                        break;
 
                    case EVENT_MORTAL_STRIKE:
                        DoCastVictim(SPELL_MORTAL_STRIKE);
                        events.ScheduleEvent(EVENT_MORTAL_STRIKE, urand(10000, 20000));
                        break;
                }
            }
 
            DoMeleeAttackIfReady();
        }
 
        void JustDied(Unit* /*killer*/) override
        {
            events.Reset();
        }
 
    private:
        EventMap events;
        InstanceScript* instance;
    };
 
    CreatureAI* GetAI(Creature* creature) const override
    {
        return GetRubySanctumAI<npc_charscale_commanderAI> (creature);
    }
};
 
class at_baltharus_plateau : public AreaTriggerScript
{
public:
 
    at_baltharus_plateau() : AreaTriggerScript("at_baltharus_plateau")
    {
    }
 
    bool OnTrigger(Player* player, AreaTriggerEntry const* /*areaTrigger*/)
    {
        // Only trigger once
        if (InstanceScript * instance = player->GetInstanceScript())
        {
            if (Creature * xerestrasza = ObjectAccessor::GetCreature(*player, instance->GetData64(DATA_XERESTRASZA)))
                xerestrasza->AI()->DoAction(ACTION_INTRO_BALTHARUS);
 
            if (Creature * baltharus = ObjectAccessor::GetCreature(*player, instance->GetData64(DATA_BALTHARUS_THE_WARBORN)))
                baltharus->AI()->DoAction(ACTION_INTRO_BALTHARUS);
        }
 
        return true;
    }
};
 
void AddSC_ruby_sanctum()
{
    new npc_xerestrasza();
    new npc_charscale_assaulter();
    new npc_charscale_commander();
    new npc_charscale_elite();
    new npc_charscale_invoker();
    new at_baltharus_plateau();
}

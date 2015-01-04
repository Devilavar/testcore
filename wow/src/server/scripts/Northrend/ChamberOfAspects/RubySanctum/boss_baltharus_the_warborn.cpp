#include "ObjectMgr.h"
#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "SpellAuraEffects.h"
#include "ruby_sanctum.h"
 
enum Texts
{
    SAY_BALTHARUS_INTRO         = 0, // Your power wanes, ancient one.... Soon you will join your friends.
    SAY_AGGRO                   = 1, // Ah, the entertainment has arrived.
    SAY_KILL                    = 2, // Baltharus leaves no survivors! / This world has enough heroes.
    SAY_CLONE                   = 3, // Twice the pain and half the fun.
    SAY_DEATH                   = 4, // I... didn't see that coming....
};
 
enum Spells
{
    SPELL_BARRIER_CHANNEL       = 76221, // Channeling dummy spell
    SPELL_ENERVATING_BRAND      = 74502, // friendlys in 12yards = 74505
    SPELL_SIPHONED_MIGHT        = 74507, // Increases the caster's damage by 2% per applied brands.
    SPELL_CLEAVE                = 40504, // every 10-15 secs
    SPELL_BLADE_TEMPEST         = 75125, // every 22 seconds
    SPELL_CLONE                 = 74511, // Summons a npc 39899 (Clone) with the same health. Used at 50%. (in 10) or else 66% & 30 %
    SPELL_REPELLING_WAVE        = 74509, // once if call clone
    SPELL_CLEAR_DEBUFFS         = 34098, // once if call clone
    SPELL_SPAWN_EFFECT          = 64195, // once if call clone
};
 
enum Events 
{
    EVENT_BLADE_TEMPEST         = 1,
    EVENT_CLEAVE                = 2,
    EVENT_ENERVATING_BRAND      = 3,
    EVENT_INTRO_TALK            = 4,
    EVENT_OOC_CHANNEL           = 5,
};
 
enum Actions
{
    ACTION_CLONE                = 1,
};
 
enum Phases
{
    PHASE_ALL                   = 0,
    PHASE_INTRO                 = 1,
    PHASE_COMBAT                = 2,
 
    PHASE_INTRO_MASK            = 1 << PHASE_INTRO,
};
 
class boss_baltharus_the_warborn : public CreatureScript
{
public:
 
    boss_baltharus_the_warborn() : CreatureScript("boss_baltharus_the_warborn")
    {
    }
 
    struct boss_baltharus_the_warbornAI : public BossAI
    {
 
        boss_baltharus_the_warbornAI(Creature* creature) : BossAI(creature, DATA_BALTHARUS_THE_WARBORN)
        {
            introDone = false;
        }
 
        void Reset() override
        {
            _Reset();
            events.SetPhase(PHASE_INTRO);
            events.ScheduleEvent(EVENT_OOC_CHANNEL, 0, 0, PHASE_INTRO);
            cloneCount = RAID_MODE<uint8 > (1, 2, 2, 2);
            instance->SetData(DATA_BALTHARUS_SHARED_HEALTH, me->GetMaxHealth());
            instance->SetBossState(DATA_BALTHARUS_THE_WARBORN, NOT_STARTED);
        }
 
        void DoAction(int32 action) override
        {
            switch (action)
            {
                // intro talk
                case ACTION_INTRO_BALTHARUS:
                    if (introDone)
                        return;
                    introDone = true;
                    me->setActive(true);
                    events.ScheduleEvent(EVENT_INTRO_TALK, 7 * IN_MILLISECONDS, 0, PHASE_INTRO);
                    break;
 
                // clone himself
                case ACTION_CLONE:
                    me->CastStop();
                    DoCast(me, SPELL_CLEAR_DEBUFFS);
                    DoCast(me, SPELL_CLONE);
                    DoCast(me, SPELL_REPELLING_WAVE);
                    Talk(SAY_CLONE);
                    --cloneCount;
                    break;
 
                default:
                    break;
            }
        }
 
        void EnterCombat(Unit* /*who*/) override
        {
            me->InterruptNonMeleeSpells(false);
            _EnterCombat();
 
            events.Reset();
            events.SetPhase(PHASE_COMBAT);
            events.ScheduleEvent(EVENT_CLEAVE, 11 * IN_MILLISECONDS, 0, PHASE_COMBAT);
            events.ScheduleEvent(EVENT_ENERVATING_BRAND, 11 * IN_MILLISECONDS, 0, PHASE_COMBAT);
            events.ScheduleEvent(EVENT_BLADE_TEMPEST, 15 * IN_MILLISECONDS, 0, PHASE_COMBAT);
 
            Talk(SAY_AGGRO);
            instance->SetBossState(DATA_BALTHARUS_THE_WARBORN, IN_PROGRESS);
        }
 
        void JustDied(Unit* /*killer*/) override
        {
            _JustDied();
            Talk(SAY_DEATH);
            if (Creature* xerestrasza = ObjectAccessor::GetCreature(*me, instance->GetData64(DATA_XERESTRASZA)))
                xerestrasza->AI()->DoAction(ACTION_BALTHARUS_DEATH);

            instance->SetBossState(DATA_BALTHARUS_THE_WARBORN, DONE);
        }
 
        void KilledUnit(Unit* victim) override
        {
            if (victim->GetTypeId() == TYPEID_PLAYER)
                Talk(SAY_KILL);
        }
 
        // called when a clone has just been summoned
        void JustSummoned(Creature* summon) override
        {
            summons.Summon(summon);
            // set the health of the clone equal to baltharus' health
            summon->SetHealth(me->GetHealth());
            summon->CastSpell(summon, SPELL_SPAWN_EFFECT, true);
        }
 
        // called before damage is applied
        void DamageTaken(Unit* /*attacker*/, uint32 & damage) override
        {
            if (GetDifficulty() == RAID_DIFFICULTY_10MAN_NORMAL)
            {
                // 10 men only one clone at 50%
                if (me->HealthBelowPctDamaged(50, damage) && cloneCount == 1)
                    DoAction(ACTION_CLONE);
            }
            else
            {
                // any raid difficulty other then 10 men normal gets two clones, one at 66% and 33%
                if ((me->HealthBelowPctDamaged(66, damage) && cloneCount == 2) || (me->HealthBelowPctDamaged(33, damage) && cloneCount == 1))
                    DoAction(ACTION_CLONE);
            }
 
            // apply damage
            if (me->GetHealth() > damage)
                instance->SetData(DATA_BALTHARUS_SHARED_HEALTH, me->GetHealth() - damage);
        }
 
        void UpdateAI(uint32 diff) override
        {
            if (!UpdateVictim() && !(events.GetPhaseMask() & PHASE_INTRO_MASK))
                return;
 
            if (!(events.GetPhaseMask() & PHASE_INTRO_MASK))
                me->SetHealth(instance->GetData(DATA_BALTHARUS_SHARED_HEALTH));
 
            events.Update(diff);
 
            if (me->HasUnitState(UNIT_STATE_CASTING) && !(events.GetPhaseMask() & PHASE_INTRO_MASK))
                return;
 
            while (uint32 eventId = events.ExecuteEvent())
            {
                switch (eventId)
                {
                    case EVENT_INTRO_TALK:
                        Talk(SAY_BALTHARUS_INTRO);
                        break;
 
                    case EVENT_OOC_CHANNEL:
                        if (Creature* channelTarget = ObjectAccessor::GetCreature(*me, instance->GetData64(DATA_CRYSTAL_CHANNEL_TARGET)))
                            DoCast(channelTarget, SPELL_BARRIER_CHANNEL);
                        events.ScheduleEvent(EVENT_OOC_CHANNEL, 7 * IN_MILLISECONDS, 0, PHASE_INTRO);
                        break;
 
                    case EVENT_CLEAVE:
                        DoCastVictim(SPELL_CLEAVE);
                        events.ScheduleEvent(EVENT_CLEAVE, 24 * IN_MILLISECONDS, 0, PHASE_COMBAT);
                        break;
 
                    case EVENT_BLADE_TEMPEST:
                        DoCast(me, SPELL_BLADE_TEMPEST);
                        events.ScheduleEvent(EVENT_BLADE_TEMPEST, 24 * IN_MILLISECONDS, 0, PHASE_COMBAT);
                        break;
 
                    case EVENT_ENERVATING_BRAND:
                        for (uint8 i = 0; i < RAID_MODE<uint8 > (4, 8, 8, 10); i++)
                            if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 45.0f, true))
                                DoCast(target, SPELL_ENERVATING_BRAND);
 
                        events.ScheduleEvent(EVENT_ENERVATING_BRAND, 26 * IN_MILLISECONDS, 0, PHASE_COMBAT);
                        break;
 
                    default:
                        break;
                }
            }
 
            DoMeleeAttackIfReady();
        }
 
    private:
        uint8 cloneCount;
        bool introDone;
    };
 
    CreatureAI* GetAI(Creature* creature) const override
    {
        return GetRubySanctumAI<boss_baltharus_the_warbornAI > (creature);
    }
};
 
class npc_baltharus_the_warborn_clone : public CreatureScript
{
public:

    npc_baltharus_the_warborn_clone() : CreatureScript("npc_baltharus_the_warborn_clone") { }

    struct npc_baltharus_the_warborn_cloneAI : public ScriptedAI
    {
 
        npc_baltharus_the_warborn_cloneAI(Creature* creature) : ScriptedAI(creature), instance(creature->GetInstanceScript())
        {
            me->ApplySpellImmune(0, IMMUNITY_EFFECT, SPELL_EFFECT_KNOCK_BACK, true);
            me->ApplySpellImmune(0, IMMUNITY_EFFECT, SPELL_EFFECT_KNOCK_BACK_DEST, true);
        }

        void EnterCombat(Unit* /*who*/) override
        {
            DoZoneInCombat();
            events.Reset();
            events.ScheduleEvent(EVENT_CLEAVE, urand(5 * IN_MILLISECONDS, 10 * IN_MILLISECONDS));
            events.ScheduleEvent(EVENT_BLADE_TEMPEST, urand(18 * IN_MILLISECONDS, 25 * IN_MILLISECONDS));
            events.ScheduleEvent(EVENT_ENERVATING_BRAND, urand(10 * IN_MILLISECONDS, 15 * IN_MILLISECONDS));
        }
 
        void DamageTaken(Unit* /*attacker*/, uint32 & damage) override
        {
            // Setting DATA_BALTHARUS_SHARED_HEALTH to 0 when killed would bug the boss.
            if (instance && me->GetHealth() > damage)
                instance->SetData(DATA_BALTHARUS_SHARED_HEALTH, me->GetHealth() - damage);
        }
 
        void JustDied(Unit* killer) override
        {
            // This is here because DamageTaken won't trigger if the damage is deadly.
            if (instance)
                if (Creature* baltharus = ObjectAccessor::GetCreature(*me, instance->GetData64(DATA_BALTHARUS_THE_WARBORN)))
                    killer->Kill(baltharus);
        }
 
        void UpdateAI(uint32 diff) override
        {
            if (!me->IsInCombat())
                if (Unit* target = me->SelectNearestPlayer(100.0f))
                    me->AI()->AttackStart(target);

            if (!UpdateVictim())
                return;
 
            if (instance)
                me->SetHealth(instance->GetData(DATA_BALTHARUS_SHARED_HEALTH));
 
            events.Update(diff);
 
            if (me->HasUnitState(UNIT_STATE_CASTING))
                return;
 
            while (uint32 eventId = events.ExecuteEvent())
            {
                switch (eventId)
                {
                    case EVENT_CLEAVE:
                        DoCastVictim(SPELL_CLEAVE);
                        events.ScheduleEvent(EVENT_CLEAVE, 24 * IN_MILLISECONDS);
                        break;
 
                    case EVENT_BLADE_TEMPEST:
                        DoCastVictim(SPELL_BLADE_TEMPEST);
                        events.ScheduleEvent(EVENT_BLADE_TEMPEST, 24 * IN_MILLISECONDS);
                        break;
 
                    case EVENT_ENERVATING_BRAND:
                        for (uint8 i = 0; i < RAID_MODE<uint8 > (4, 8, 8, 10); i++)
                            if (Unit * target = SelectTarget(SELECT_TARGET_RANDOM, 0, 45.0f, true))
                                DoCast(target, SPELL_ENERVATING_BRAND);
                        events.ScheduleEvent(EVENT_ENERVATING_BRAND, 26 * IN_MILLISECONDS);
                        break;
 
                    default:
                        break;
                }
            }
 
            DoMeleeAttackIfReady();
        }
 
    private:
        EventMap events;
        InstanceScript* instance;
    };
 
    CreatureAI* GetAI(Creature* creature) const override
    {
        return GetRubySanctumAI<npc_baltharus_the_warborn_cloneAI > (creature);
    }
};
 
class spell_baltharus_enervating_brand_trigger : public SpellScriptLoader
{
public:
 
    spell_baltharus_enervating_brand_trigger() : SpellScriptLoader("spell_baltharus_enervating_brand_trigger")
    {
    }
 
    class spell_baltharus_enervating_brand_trigger_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_baltharus_enervating_brand_trigger_SpellScript);
 
        void CheckDistance()
        {
            if (Unit* caster = GetOriginalCaster())
            {
                if (Unit* target = GetHitUnit())
                    target->CastSpell(caster, SPELL_SIPHONED_MIGHT, true);
            }
        }
 
        void Register() override
        {
            OnHit += SpellHitFn(spell_baltharus_enervating_brand_trigger_SpellScript::CheckDistance);
        }
    };
 
    SpellScript* GetSpellScript() const override
    {
        return new spell_baltharus_enervating_brand_trigger_SpellScript();
    }
};
 
void AddSC_boss_baltharus_the_warborn()
{
    new boss_baltharus_the_warborn();
    new npc_baltharus_the_warborn_clone();
    new spell_baltharus_enervating_brand_trigger();
}

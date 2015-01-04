#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "ruby_sanctum.h"
 
enum Texts
{
    SAY_AGGRO                   = 0, // You will sssuffer for this intrusion!
    SAY_CONFLAGRATION           = 1, // Burn in the master's flame!
    EMOTE_ENRAGED               = 2, // %s becomes enraged!
    SAY_KILL                    = 3, // Halion will be pleased. / As it should be....
};
 
enum Spells
{
    SPELL_CONFLAGRATION         = 74452, // after fly up
    SPELL_FLAME_BEACON          = 74453, // mark for conflag, in enter to fly phase, 3 in 10, 6 in 25
    SPELL_CONFLAGRATION_2       = 74454, // triggered?
    SPELL_CONFLAGRATION_AURA    = 74456, // aura
    SPELL_ENRAGE                = 78722, // soft enrage + fire nova
 
    SPELL_FLAME_BREATH_10       = 74403,
    SPELL_FLAME_BREATH_25       = 74404,
};
 
enum Events
{
    EVENT_ENRAGE                = 1,
    EVENT_FLIGHT                = 2,
    EVENT_FLAME_BREATH          = 3,
    EVENT_CONFLAGRATION            = 4,
 
    // Event group
    EVENT_GROUP_LAND_PHASE      = 1,
};
 
enum MovementPoints
{
    POINT_FLIGHT                = 1,
    POINT_LAND                  = 2,
};
 
enum Misc
{
    SOUND_ID_DEATH              = 17531,
};
 
Position const SavianaRagefireFlyPos =  {3155.51f, 683.844f, 95.20f, 4.69f}; // 0 Saviana start point
Position const SavianaRagefireLandPos = {3151.07f, 636.443f, 79.54f, 4.69f}; // 1 Saviana fly phase
 
class boss_saviana_ragefire : public CreatureScript
{
public:
 
    boss_saviana_ragefire() : CreatureScript("boss_saviana_ragefire")
    {
    }
 
    struct boss_saviana_ragefireAI : public BossAI
    {
 
        boss_saviana_ragefireAI(Creature* creature) : BossAI(creature, DATA_SAVIANA_RAGEFIRE)
        {
        }
 
        void Reset() override
        {
            _Reset();
            me->SetReactState(REACT_AGGRESSIVE);
            instance->SetBossState(DATA_SAVIANA_RAGEFIRE, NOT_STARTED);
        }
 
        void EnterCombat(Unit* /*who*/) override
        {
            _EnterCombat();
            Talk(SAY_AGGRO);
            events.Reset();
            events.ScheduleEvent(EVENT_ENRAGE, 20 * IN_MILLISECONDS, EVENT_GROUP_LAND_PHASE);
            events.ScheduleEvent(EVENT_FLAME_BREATH, 14 * IN_MILLISECONDS, EVENT_GROUP_LAND_PHASE);
            events.ScheduleEvent(EVENT_FLIGHT, 60 * IN_MILLISECONDS);
            instance->SetBossState(DATA_SAVIANA_RAGEFIRE, IN_PROGRESS);
        }
 
        void JustDied(Unit* /*killer*/) override
        {
            _JustDied();
            me->PlayDirectSound(SOUND_ID_DEATH);
            instance->SetBossState(DATA_SAVIANA_RAGEFIRE, DONE);
        }
 
        void MovementInform(uint32 type, uint32 point) override
        {
            if (type != POINT_MOTION_TYPE)
                return;
 
            switch (point)
            {
                case POINT_FLIGHT:
                    events.ScheduleEvent(EVENT_CONFLAGRATION, 1 * IN_MILLISECONDS);
                    Talk(SAY_CONFLAGRATION);
                    break;
 
                case POINT_LAND:
                {
                    me->SetCanFly(false);
                    me->RemoveUnitMovementFlag(MOVEMENTFLAG_DISABLE_GRAVITY);
                    me->SetReactState(REACT_AGGRESSIVE);
                    if (me->GetMotionMaster()->GetCurrentMovementGeneratorType() == POINT_MOTION_TYPE)
                        me->GetMotionMaster()->MovementExpired();
                    DoStartMovement(me->GetVictim());
                    break;
                }
 
                default:
                    break;
            }
        }
 
        void JustReachedHome() override
        {
            _JustReachedHome();
            me->SetCanFly(false);
            me->RemoveUnitMovementFlag(MOVEMENTFLAG_DISABLE_GRAVITY);
        }
 
        void KilledUnit(Unit* victim) override
        {
            if (victim->GetTypeId() == TYPEID_PLAYER)
                Talk(SAY_KILL);
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
                    case EVENT_FLIGHT:
                    {
                        me->SetCanFly(true);
                        me->AddUnitMovementFlag(MOVEMENTFLAG_DISABLE_GRAVITY);
                        me->SetReactState(REACT_PASSIVE);
                        me->GetMotionMaster()->MovePoint(POINT_FLIGHT, SavianaRagefireFlyPos, false);
                        events.ScheduleEvent(EVENT_FLIGHT, 50 * IN_MILLISECONDS);
                        events.DelayEvents(12500, EVENT_GROUP_LAND_PHASE);
                        break;
                    }
                    // marks random players for conflagration
                    case EVENT_CONFLAGRATION:
                        DoCast(me, SPELL_CONFLAGRATION, true);
                        break;
 
                    case EVENT_ENRAGE:
                        DoCast(me, SPELL_ENRAGE);
                        Talk(EMOTE_ENRAGED);
                        events.ScheduleEvent(EVENT_ENRAGE, urand(15 * IN_MILLISECONDS, 20 * IN_MILLISECONDS), EVENT_GROUP_LAND_PHASE);
                        break;
 
                    case EVENT_FLAME_BREATH:
                        DoCastVictim(RAID_MODE(SPELL_FLAME_BREATH_10, SPELL_FLAME_BREATH_25, SPELL_FLAME_BREATH_10, SPELL_FLAME_BREATH_25));
                        events.ScheduleEvent(EVENT_FLAME_BREATH, urand(20 * IN_MILLISECONDS, 30 * IN_MILLISECONDS), EVENT_GROUP_LAND_PHASE);
                        break;
                    default:
                        break;
                }
            }
 
            DoMeleeAttackIfReady();
        }
    };
 
    CreatureAI* GetAI(Creature* creature) const override
    {
        return GetRubySanctumAI<boss_saviana_ragefireAI> (creature);
    }
};
 
class ConflagrationTargetSelector
{
public:
 
    ConflagrationTargetSelector()
    {
    }
 
    bool operator()(WorldObject* unit) const
    {
        return unit->GetTypeId() != TYPEID_PLAYER;
    }
};
 
class spell_saviana_conflagration_init : public SpellScriptLoader
{
public:
 
    spell_saviana_conflagration_init() : SpellScriptLoader("spell_saviana_conflagration_init")
    {
    }
 
    class spell_saviana_conflagration_init_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_saviana_conflagration_init_SpellScript);
 
        void FilterTargets(std::list<WorldObject*>& targets)
        {
            if (targets.empty())
                return;
 
            targets.remove_if(ConflagrationTargetSelector());
            // determine the amount of people getting beacon
            uint8 maxSize = uint8(GetCaster()->GetMap()->GetSpawnMode() & 1 ? 6 : 3);
            if (targets.size() > maxSize)
                Trinity::Containers::RandomResizeList(targets, maxSize);
        }
 
        void HandleDummy(SpellEffIndex effIndex)
        {
            PreventHitDefaultEffect(effIndex);
            GetCaster()->CastSpell(GetHitUnit(), SPELL_FLAME_BEACON, true);
            GetCaster()->CastSpell(GetHitUnit(), SPELL_CONFLAGRATION_2, false);
        }
 
        void Register() override
        {
            OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_saviana_conflagration_init_SpellScript::FilterTargets, EFFECT_0, TARGET_UNIT_SRC_AREA_ENEMY);
            OnEffectHitTarget += SpellEffectFn(spell_saviana_conflagration_init_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
        }
    };
 
    SpellScript* GetSpellScript() const override
    {
        return new spell_saviana_conflagration_init_SpellScript();
    }
};
 
class spell_saviana_conflagration_throwback : public SpellScriptLoader
{
public:
 
    spell_saviana_conflagration_throwback() : SpellScriptLoader("spell_saviana_conflagration_throwback")
    {
    }
 
    class spell_saviana_conflagration_throwback_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_saviana_conflagration_throwback_SpellScript);
 
        void HandleScript(SpellEffIndex effIndex)
        {
            PreventHitDefaultEffect(effIndex);
            GetHitUnit()->CastSpell(GetCaster(), uint32(GetEffectValue()), true);
 
            // make Saviana land
            Unit* caster = GetCaster();
            InstanceScript* instance = caster->GetInstanceScript();
            if (Creature* saviana = Unit::GetCreature(*caster, instance->GetData64(DATA_SAVIANA_RAGEFIRE)))
                saviana->GetMotionMaster()->MovePoint(POINT_LAND, SavianaRagefireLandPos, false);
        }
 
        void Register() override
        {
            OnEffectHitTarget += SpellEffectFn(spell_saviana_conflagration_throwback_SpellScript::HandleScript, EFFECT_0, SPELL_EFFECT_SCRIPT_EFFECT);
        }
    };
 
    SpellScript* GetSpellScript() const override
    {
        return new spell_saviana_conflagration_throwback_SpellScript();
    }
};
 
void AddSC_boss_saviana_ragefire()
{
    new boss_saviana_ragefire();
    new spell_saviana_conflagration_init();
    new spell_saviana_conflagration_throwback();
}

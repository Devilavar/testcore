#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "ScriptedEscortAI.h"
#include "ruby_sanctum.h"
 
enum Texts 
{
    SAY_AGGRO    = 0, // Alexstrasza has chosen capable allies.... A pity that I must END YOU!
    SAY_KILL    = 1, // You thought you stood a chance? / It's for the best.
    SAY_ADDS    = 2, // Turn them to ash, minions!
    SAY_DEATH    = 3, // HALION! I...
};
 
enum Spells
{
    // General Zarithrian
    SPELL_INTIMIDATING_ROAR             = 74384,
    SPELL_CLEAVE_ARMOR                  = 74367,
 
    // Zarithrian Spawn Stalker
    SPELL_SUMMON_FLAMECALLER            = 74398,
 
    // Onyx Flamecaller
    SPELL_BLAST_NOVA_10                 = 74392,
    SPELL_BLAST_NOVA_25                 = 74393,
    SPELL_LAVA_GOUT_10                  = 74394,
    SPELL_LAVA_GOUT_25                  = 74395,
};
 
enum Events
{
    // General Zarithrian
    EVENT_CLEAVE_ARMOR                  = 1,
    EVENT_INTIMIDATING_ROAR             = 2,
    EVENT_SUMMON_FLAMECALLER            = 3,
    EVENT_ADDITIONAL_FLAMECALLER        = 4,
 
    // Onyx Flamecaller
    EVENT_BLAST_NOVA                    = 10,
    EVENT_LAVA_GOUT                     = 11,
};
 
uint32 const MAX_PATH_FLAMECALLER_WAYPOINTS = 12;
 
Position const FlamecallerWaypoints[MAX_PATH_FLAMECALLER_WAYPOINTS * 2] =
{
    // East
    {3042.971f, 419.8809f, 86.94320f, 0.0f},
    {3043.971f, 419.8809f, 86.94320f, 0.0f},
    {3044.885f, 428.8281f, 86.19320f, 0.0f},
    {3045.494f, 434.7930f, 85.56398f, 0.0f},
    {3045.900f, 438.7695f, 84.81398f, 0.0f},
    {3045.657f, 456.8290f, 85.95601f, 0.0f},
    {3043.657f, 459.0790f, 87.20601f, 0.0f},
    {3042.157f, 460.5790f, 87.70601f, 0.0f},
    {3040.907f, 462.0790f, 88.45601f, 0.0f},
    {3038.907f, 464.0790f, 89.20601f, 0.0f},
    {3025.907f, 478.0790f, 89.70601f, 0.0f},
    {3003.832f, 501.2510f, 89.47303f, 0.0f},
    // West
    {3062.596f, 636.9980f, 82.50338f, 0.0f},
    {3062.514f, 624.9980f, 83.70634f, 0.0f},
    {3062.486f, 620.9980f, 84.33134f, 0.0f},
    {3062.445f, 613.9930f, 84.45634f, 0.0f},
    {3062.445f, 613.9930f, 84.45634f, 0.0f},
    {3059.208f, 610.6501f, 85.39581f, 0.0f},
    {3055.958f, 606.9001f, 86.14581f, 0.0f},
    {3046.458f, 596.4001f, 86.89581f, 0.0f},
    {3043.958f, 593.4001f, 87.64581f, 0.0f},
    {3040.458f, 589.9001f, 88.39581f, 0.0f},
    {3034.458f, 583.1501f, 88.89581f, 0.0f},
    {3014.970f, 561.8073f, 88.83527f, 0.0f},
};
 
class boss_general_zarithrian : public CreatureScript
{
public:
 
    boss_general_zarithrian() : CreatureScript("boss_general_zarithrian")
    {
    }
 
    struct boss_general_zarithrianAI : public BossAI
    {
 
        boss_general_zarithrianAI(Creature* creature) : BossAI(creature, DATA_GENERAL_ZARITHRIAN)
        {
        }

        std::list<uint64> summons;

        void Reset() override
        {
            _Reset();
 
            summons.clear();

            if (instance->GetBossState(DATA_SAVIANA_RAGEFIRE) == DONE && instance->GetBossState(DATA_BALTHARUS_THE_WARBORN) == DONE)
                me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_PC|UNIT_FLAG_NOT_SELECTABLE);
        }
 
        void EnterCombat(Unit* /*who*/) override
        {
            _EnterCombat();
 
            Talk(SAY_AGGRO);
 
            events.Reset();
            events.ScheduleEvent(EVENT_CLEAVE_ARMOR, 15 * IN_MILLISECONDS);
            events.ScheduleEvent(EVENT_INTIMIDATING_ROAR, 40 * IN_MILLISECONDS);
        }
 
        void JustReachedHome() override
        {
            _JustReachedHome();
            for (std::list<uint64>::iterator itr = summons.begin(); itr != summons.end(); ++itr)
                if (Unit* summon = ObjectAccessor::GetUnit(*me, (*itr)))
                    summon->ToCreature()->DespawnOrUnsummon();

            summons.clear();
            instance->SetBossState(DATA_GENERAL_ZARITHRIAN, FAIL);
        }
        
        void JustSummoned(Creature* summoned) override
        {
            summons.push_back(summoned->GetGUID());
        }
 
        void JustDied(Unit* /*killer*/) override
        {
            _JustDied();
            Talk(SAY_DEATH);
            instance->SetBossState(DATA_GENERAL_ZARITHRIAN, DONE);
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
 
            // Can't use room boundary here, the gameobject is spawned at the same position as the boss. This is just as good anyway.
            if (me->GetPositionX() > 3060.0f)
            {
                EnterEvadeMode();
                return;
            }
 
            events.Update(diff);
 
            if (me->HasUnitState(UNIT_STATE_CASTING))
                return;
 
            while (uint32 eventId = events.ExecuteEvent())
            {
                switch (eventId)
                {
                    case EVENT_SUMMON_FLAMECALLER:
                    {
                        if (Creature* stalker1 = ObjectAccessor::GetCreature(*me, instance->GetData64(DATA_ZARITHRIAN_SPAWN_STALKER_1)))
                            stalker1->AI()->DoCast(stalker1, SPELL_SUMMON_FLAMECALLER);
                        if (Creature* stalker2 = ObjectAccessor::GetCreature(*me, instance->GetData64(DATA_ZARITHRIAN_SPAWN_STALKER_2)))
                            stalker2->AI()->DoCast(stalker2, SPELL_SUMMON_FLAMECALLER);
 
                        // two additional flame callers are spawned in 25 man 
                        if (instance->instance->GetSpawnMode() == RAID_DIFFICULTY_25MAN_NORMAL || instance->instance->GetSpawnMode() == RAID_DIFFICULTY_25MAN_HEROIC)
                        {
                            // wait 2 secs before summon another pair of flame callers
                            events.ScheduleEvent(EVENT_ADDITIONAL_FLAMECALLER, 2 * IN_MILLISECONDS);
                        }
 
                        Talk(SAY_ADDS);
 
                        break;
                    }
 
                    // summon two additional flame callers (25 man only)
                    case EVENT_ADDITIONAL_FLAMECALLER:
                    {
                        if (Creature* stalker1 = ObjectAccessor::GetCreature(*me, instance->GetData64(DATA_ZARITHRIAN_SPAWN_STALKER_1)))
                        {
                            stalker1->AI()->DoCast(stalker1, SPELL_SUMMON_FLAMECALLER);
                        }
                        if (Creature* stalker2 = ObjectAccessor::GetCreature(*me, instance->GetData64(DATA_ZARITHRIAN_SPAWN_STALKER_2)))
                        {
                            stalker2->AI()->DoCast(stalker2, SPELL_SUMMON_FLAMECALLER);
                        }
                    }
                        break;
 
                    case EVENT_INTIMIDATING_ROAR:
                        DoCastAOE(SPELL_INTIMIDATING_ROAR, true); // range should be 40
 
                        // should reset aggro after intimidiating roar (http://www.wowwiki.com/General_Zarithrian)
                        me->getThreatManager().resetAllAggro();
 
                        // summon the adds 2 secs after intimidating roar
                        events.ScheduleEvent(EVENT_SUMMON_FLAMECALLER, 2 * IN_MILLISECONDS);
 
                        // next intimidating roar in 40 secs
                        events.ScheduleEvent(EVENT_INTIMIDATING_ROAR, 40 * IN_MILLISECONDS);
                        break;
 
                    case EVENT_CLEAVE_ARMOR:
                        DoCastVictim(SPELL_CLEAVE_ARMOR);
                        events.ScheduleEvent(EVENT_CLEAVE_ARMOR, 15 * IN_MILLISECONDS);
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
        return GetRubySanctumAI<boss_general_zarithrianAI> (creature);
    }
};
 
class npc_onyx_flamecaller : public CreatureScript
{
public:
 
    npc_onyx_flamecaller() : CreatureScript("npc_onyx_flamecaller")
    {
    }
 
    struct npc_onyx_flamecallerAI : public npc_escortAI
    {
 
        npc_onyx_flamecallerAI(Creature* creature) : npc_escortAI(creature)
        {
            instance = creature->GetInstanceScript();
            npc_escortAI::SetDespawnAtEnd(false);
 
            me->ApplySpellImmune(0, IMMUNITY_EFFECT, SPELL_EFFECT_KNOCK_BACK, true);
            me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_GRIP, true);
        }
 
        void Reset() override
        {
            lavaGoutCount = 0;
            me->setActive(true);
            AddWaypoints();
            Start(true, true);
        }
 
        void EnterCombat(Unit* /*who*/) override
        {
            events.Reset();
            events.ScheduleEvent(EVENT_BLAST_NOVA, urand(20000, 30000));
            events.ScheduleEvent(EVENT_LAVA_GOUT, 5000);
        }
 
        void EnterEvadeMode() override
        {
            // Prevent EvadeMode
        }
 
        void IsSummonedBy(Unit* /*summoner*/) override
        {
            // Let Zarithrian count as summoner. instance cant be null since we got GetRubySanctumAI
            if (Creature* zarithrian = ObjectAccessor::GetCreature(*me, instance->GetData64(DATA_GENERAL_ZARITHRIAN)))
                zarithrian->AI()->JustSummoned(me);
        }
 
        void WaypointReached(uint32 waypointId) override
        {
            if (waypointId == MAX_PATH_FLAMECALLER_WAYPOINTS - 1)
            {
                DoZoneInCombat();
                SetEscortPaused(true);
 
                Unit* target = me->GetVictim();
                if (!target)
                    target = SelectTarget(SELECT_TARGET_RANDOM, 0, 100.0, true);
                if (target)
                {
                    me->AddThreat(target, 100.0f);
                    me->GetMotionMaster()->MoveChase(target);
                }
            }
        }
 
        void AddWaypoints()
        {
            if (me->GetPositionY() < 500.0f)
            {
                for (uint8 i = 0; i < MAX_PATH_FLAMECALLER_WAYPOINTS; i++)
                    AddWaypoint(i, FlamecallerWaypoints[i].GetPositionX(), FlamecallerWaypoints[i].GetPositionY(), FlamecallerWaypoints[i].GetPositionZ());
            }
            else
            {
                for (uint8 i = 0, j = MAX_PATH_FLAMECALLER_WAYPOINTS; j < MAX_PATH_FLAMECALLER_WAYPOINTS * 2; j++, i++)
                    AddWaypoint(i, FlamecallerWaypoints[j].GetPositionX(), FlamecallerWaypoints[j].GetPositionY(), FlamecallerWaypoints[j].GetPositionZ());
            }
        }
 
        void UpdateEscortAI(uint32 const diff) override
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
                    case EVENT_BLAST_NOVA:
                        DoCastAOE(RAID_MODE(SPELL_BLAST_NOVA_10, SPELL_BLAST_NOVA_25, SPELL_BLAST_NOVA_10, SPELL_BLAST_NOVA_25));
                        events.ScheduleEvent(EVENT_BLAST_NOVA, urand(15000, 25000));
                        break;
 
                    case EVENT_LAVA_GOUT:
                        if (lavaGoutCount >= 3)
                        {
                            lavaGoutCount = 0;
                            events.ScheduleEvent(EVENT_LAVA_GOUT, urand(30000, 35000));
                            break;
                        }
 
                        DoCastVictim(RAID_MODE(SPELL_LAVA_GOUT_10, SPELL_LAVA_GOUT_25, SPELL_LAVA_GOUT_10, SPELL_LAVA_GOUT_25));
                        lavaGoutCount++;
                        events.ScheduleEvent(EVENT_LAVA_GOUT, 1500);
                        break;
 
                    default:
                        break;
                }
            }
 
            DoMeleeAttackIfReady();
        }
 
    private:
        EventMap events;
        bool movementComplete;
        InstanceScript* instance;
        uint8 lavaGoutCount;
    };
 
    CreatureAI* GetAI(Creature* creature) const override
    {
        return GetRubySanctumAI<npc_onyx_flamecallerAI> (creature);
    }
};
 
class spell_zarithrian_summon_flamecaller : SpellScriptLoader
{
public:
 
    spell_zarithrian_summon_flamecaller() : SpellScriptLoader("spell_zarithrian_summon_flamecaller")
    {
    }
 
    class spell_zarithrian_summon_flamecaller_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_zarithrian_summon_flamecaller_SpellScript);
 
        void HandleSummon(SpellEffIndex effIndex)
        {
            PreventHitDefaultEffect(effIndex);
            Unit* caster = GetCaster();
 
            uint32 entry = uint32(GetSpellInfo()->Effects[effIndex].MiscValue);
            SummonPropertiesEntry const* properties = sSummonPropertiesStore.LookupEntry(uint32(GetSpellInfo()->Effects[effIndex].MiscValueB));
            uint32 duration = uint32(GetSpellInfo()->GetDuration());
 
            Position pos = caster->GetPosition();
 
            TempSummon* summon = caster->GetMap()->SummonCreature(entry, pos, properties, duration, caster, GetSpellInfo()->Id);
 
            InstanceScript* instance = caster->GetInstanceScript();
            Creature* generalZarithrian = Unit::GetCreature(*caster, instance->GetData64(DATA_GENERAL_ZARITHRIAN));
            summon->SetCreatorGUID(generalZarithrian->GetGUID());
            generalZarithrian->AI()->JustSummoned(summon);
        }
 
        void Register() override
        {
            OnEffectHit += SpellEffectFn(spell_zarithrian_summon_flamecaller_SpellScript::HandleSummon, EFFECT_0, SPELL_EFFECT_SUMMON);
        }
    };
 
    SpellScript* GetSpellScript() const override
    {
        return new spell_zarithrian_summon_flamecaller_SpellScript();
    }
};
 
void AddSC_boss_general_zarithrian()
{
    new boss_general_zarithrian();
    new npc_onyx_flamecaller();
    new spell_zarithrian_summon_flamecaller();
}

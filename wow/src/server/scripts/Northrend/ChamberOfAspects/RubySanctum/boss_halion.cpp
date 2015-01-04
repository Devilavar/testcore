#include "ScriptPCH.h"
#include "SpellScript.h"
#include "SpellAuraEffects.h"
#include "Spell.h"
#include "Vehicle.h"
#include "MapManager.h"
#include "ruby_sanctum.h"
#include "GameObjectAI.h"

enum Texts
{
    SAY_INTRO                           = 0, // Meddlesome insects! You are too late. The Ruby Sanctum is lost!
    SAY_AGGRO                           = 1, // Your world teeters on the brink of annihilation. You will ALL bear witness to the coming of a new age of DESTRUCTION!
    SAY_METEOR_STRIKE                   = 2, // The heavens burn!
    SAY_PHASE_TWO                       = 3, // You will find only suffering within the realm of twilight! Enter if you dare!
    SAY_DEATH                           = 4, // Relish this victory, mortals, for it will be your last! This world will burn with the master's return!
    SAY_KILL                            = 5, // Another "hero" falls. / Ha Ha Ha!
    SAY_BERSERK                         = 6, // Not good enough.

    SAY_SPHERE_PULSE                    = 0, // Beware the shadow!
    SAY_PHASE_THREE                     = 1, // I am the light and the darkness! Cower, mortals, before the herald of Deathwing!

    EMOTE_PHYSICAL_OUT_PHYSICAL         = 0, // Your companion's efforts have forced Halion further out of the Physical realm!
    EMOTE_PHYSICAL_IN_PHYSICAL          = 1, // Your efforts have forced Halion further into the Physical realm!
    EMOTE_TWILIGHT_OUT_TWILIGHT         = 2, // Your companion's efforts have forced Halion further out of the Twilight realm!
    EMOTE_TWILIGHT_IN_TWILIGHT          = 3, // Your efforts have forced Halion further into the Twilight realm!
    EMOTE_REGENERATE                    = 4, // Without pressure in both realms, Halion begins to regenerate.

    EMOTE_WARN_LASER                    = 0, // The orbiting spheres pulse with dark energy!
};

enum Spells
{
    // Halion
    SPELL_FLAME_BREATH_10_N             = 74525, // Inflicts 17,500 to 22,500 Fire damage to players in front of Halion
    SPELL_FLAME_BREATH_25_N             = 74526, // Inflicts 26,250 to 33,750 Fire damage to enemies in a cone in front of the caster.
    SPELL_FLAME_BREATH_10_H             = 74527, // Inflicts 26,250 to 33,750 Fire damage to enemies in a cone in front of the caster.
    SPELL_FLAME_BREATH_25_H             = 74528, // Inflicts 39,375 to 50,625 Fire damage to enemies in a cone in front of the caster.
    SPELL_CLEAVE                        = 74524, // Inflicts 75% weapon damage to an enemy and its nearest allies, affecting up to 10 targets.
    SPELL_METEOR_STRIKE                 = 74637, // Script Start (summon NPC_METEOR_STRIKE)
    SPELL_TAIL_LASH                     = 74531, // A sweeping tail strike hits all enemies behind the caster, inflicting 3063 to 3937 damage and stunning them for 2 sec.
    SPELL_TWILIGHT_PRECISION            = 78243, // Halion's insight into movement between phases allows him to deal unexpected strikes, increasing his chance to hit by 5% and reducing the enemies' chance to dodge his attacks by 20%.

    SPELL_FIERY_COMBUSTION              = 74562, // Inflicts 4, 000 Fire damage every 2 seconds for 30 seconds to a random raider. Every time Fiery Combustion does damage, it applies a stackable Mark of Combustion.
    SPELL_MARK_OF_COMBUSTION            = 74567, // Dummy effect only
    SPELL_FIERY_COMBUSTION_EXPLOSION    = 74607, // cast on self by player -> spell link (see sql)
    SPELL_FIERY_COMBUSTION_SUMMON       = 74610, // casted on player when Fiery Combustion is removed => spell link (see sql)

    // Combustion & Consumption
    SPELL_SCALE_AURA                    = 70507, // Custom spell created in spell_dbc. (see sql)
    SPELL_CONSUMPTION_DAMAGE_AURA       = 74803,
    SPELL_COMBUSTION_DAMAGE_AURA        = 74629,

    // Twilight Halion
    SPELL_DARK_BREATH_10_N              = 74806, // Inflicts 17,500 to 22,500 Shadow damage to players in front of Halion
    SPELL_DARK_BREATH_25_N              = 75954, // Inflicts 26,250 to 33,750 Shadow damage to enemies in a cone in front of the caster.
    SPELL_DARK_BREATH_10_H              = 75955, // Inflicts 26,250 to 33,750 Shadow damage to enemies in a cone in front of the caster.
    SPELL_DARK_BREATH_25_H              = 75956, // Inflicts 39,375 to 50,625 Shadow damage to enemies in a cone in front of the caster.

    SPELL_MARK_OF_CONSUMPTION           = 74795,
    SPELL_SOUL_CONSUMPTION              = 74792,
    SPELL_SOUL_CONSUMPTION_EXPLOSION    = 74799, // cast on self by player -> spell link (see sql)
    SPELL_SOUL_CONSUMPTION_SUMMON       = 74800, // casted on player when Soul Consumption is removed => spell link (see sql)
    SPELL_DUSK_SHROUD                   = 75476, // The caster radiates twilight energy, inflicting 2500 Shadow damage every 2 sec. to nearby enemies

    // Living Inferno
    SPELL_BLAZING_AURA                  = 75885,

    // Living Ember
    SPELL_AWAKEN_FLAMES                 = 75889,

    // Halion Controller
    SPELL_COSMETIC_FIRE_PILLAR          = 76006, // Visual Intro
    SPELL_FIERY_EXPLOSION               = 76010, // Visual Intro
    SPELL_CLEAR_DEBUFFS                 = 75396,

    // Meteor Strike
    SPELL_METEOR_STRIKE_COUNTDOWN       = 74641, // Impact zone for meteor
    SPELL_METEOR_STRIKE_AOE_DAMAGE_10_N = 74648, // Inflicts 18,750 to 21,250 Fire damage to enemies within 12 yards of the targeted area. Takes about 5 seconds to land.
    SPELL_METEOR_STRIKE_AOE_DAMAGE_25_N = 75877, // Deals 18750 to 21250 Fire damage to enemies within 9 yards.
    SPELL_METEOR_STRIKE_AOE_DAMAGE_10_H = 75878, // Deals 75000 to 85000 Fire damage to enemies within 9 yards.
    SPELL_METEOR_STRIKE_AOE_DAMAGE_25_H = 75879, // Deals 75000 to 85000 Fire damage to enemies within 9 yards and spawns a Living Inferno.
    SPELL_METEOR_STRIKE_FIRE_AURA_1     = 74713,
    SPELL_METEOR_STRIKE_FIRE_AURA_2     = 74718, // flame from meteor
    SPELL_BIRTH_NO_VISUAL               = 40031,

    // Shadow Orb
    SPELL_TWILIGHT_CUTTER               = 74768, // Channeling shadow cutter visual, triggers 74769 
    SPELL_TWILIGHT_CUTTER_TRIGGERED     = 74769, // Fires a beam of concentrated twilight energy, dealing massive Shadow damage to any enemies that make contact with it. 13875 - 16125
    SPELL_TWILIGHT_PULSE_PERIODIC       = 78861, // Triggers 78862 (Twilight Pulse) - Deals 27000 to 33000 Shadow damage to enemies within 5 yards.
    SPELL_TRACK_ROTATION                = 74758,

    // Misc
    SPELL_TWILIGHT_DIVISION             = 75063, // Phase spell from phase 2 to phase 3; Used by Halion to be in both Twilight & Physical Realms
    SPELL_LEAVE_TWILIGHT_REALM          = 74812, // Used on players to leave Twilight Realm
    SPELL_TWILIGHT_PHASING              = 74808, // Phase spell from phase 1 to phase 2; Used by Halion at 75% to enter Twilight Realm (just animation)
    SPELL_SUMMON_TWILIGHT_PORTAL        = 74809, // Summons go 202794, Twilight Portal
    SPELL_SUMMON_EXIT_PORTALS           = 74805, // Custom spell created in spell_dbc. (see sql)
    SPELL_TWILIGHT_MENDING              = 75509,
    SPELL_TWILIGHT_REALM                = 74807, // Halion aura to be in Twilight Realm and may used for players to be in twilight realm

    SPELL_COPY_DAMAGE                   = 74810, // Custom spell created in spell_dbc. (see sql) 
};

enum Events
{
    // Halion
    EVENT_ACTIVATE_FIREWALL = 1,
    EVENT_CLEAVE, // Used by Twilight Halion too
    EVENT_FLAME_BREATH,
    EVENT_METEOR_STRIKE,
    EVENT_FIERY_COMBUSTION,
    EVENT_TAIL_LASH,
    EVENT_TWILIGHT_MENDING,
    EVENT_PHASE_THREE,
    EVENT_PH2_WIPE_CHECK,

    // Twilight Halion
    EVENT_DARK_BREATH,
    EVENT_SOUL_CONSUMPTION,

    // Meteor Strike
    EVENT_SPAWN_METEOR_FLAME,
    EVENT_SUMMON_INFERNO,
    EVENT_SUMMON_EMBER,

    // Halion Controller
    EVENT_START_INTRO,
    EVENT_INTRO_PROGRESS_1,
    EVENT_INTRO_PROGRESS_2,
    EVENT_INTRO_PROGRESS_3,
    EVENT_CHECK_CORPOREALITY,
    EVENT_SHADOW_PULSARS_SHOOT,
    EVENT_TRIGGER_BERSERK,
    EVENT_ACTIVATE_EMBERS,

    // Living Ember
    EVENT_EMBER_ENRAGE,
    EVENT_EMBER_CHECK_AURA,

    // Orb Carrier
    EVENT_TRACK_ROTATION,

    // Misc
    EVENT_EVADE_CHECK, // checks for Halion (being) pulled outside of the Flame Ring
};

enum Actions
{
    // Meteor Strike
    ACTION_METEOR_STRIKE_BURN = 1,
    ACTION_METEOR_STRIKE_AOE,

    // Halion
    ACTION_BERSERK,
    ACTION_FIERY_COMBUSTION,
    ACTION_SOUL_CONSUMPTION,
    ACTION_DARK_BREATH,
    ACTION_METEOR_STRIKE,

    // Halion Controller
    ACTION_PHASE_TWO,
    ACTION_PHASE_THREE,
    ACTION_PH2_WIPE_CHECK,

    // Orb Carrier
    ACTION_SHOOT,
    ACTION_ROTATE,
    ACTION_STOP_ROTATE,
};

enum Phases
{
    PHASE_ONE   = 1,
    PHASE_TWO   = 2,
    PHASE_THREE = 3,
};

enum BossPhaseMasks
{
    PHASE_MASK_ONE   = 1,
    PHASE_MASK_TWO   = 2,
    PHASE_MASK_THREE = 4,
};

enum Misc
{
    DATA_TWILIGHT_DAMAGE_TAKEN  = 1,
    DATA_MATERIAL_DAMAGE_TAKEN  = 2,
    DATA_STACKS_DISPELLED       = 3,
    DATA_FIGHT_PHASE            = 4,
    DATA_MENDING                = 5,
    DATA_LIVING_EMBER_SUMMONS   = 6,
    DATA_PHASE                  = 7,
    RING_RADIUS                 = 45,
};

enum OrbCarrierSeats
{
    SEAT_NORTH  = 0,
    SEAT_SOUTH  = 1,
    SEAT_EAST   = 2,
    SEAT_WEST   = 3,
};

Position const HalionSpawnPos = {3156.67f, 533.8108f, 72.98822f, 3.159046f};

struct CorporealityData
{
    uint8 materialPercentage;           // Boss Health diff in percent  
    uint32 materialRealmSpellId;        // Buff pair (real world)
    uint32 twilightRealmSpellId;        // Buff pair (twilight world)
};

uint8 const MAX_CORPOREALITY_STATE = 11;

CorporealityData const corporealityReference[MAX_CORPOREALITY_STATE] = 
{
    { 0, 74836, 74831},
    { 10, 74835, 74830},
    { 20, 74834, 74829},
    { 30, 74833, 74828},
    { 40, 74832, 74827},
    { 50, 74826, 74826},
    { 60, 74827, 74832},
    { 70, 74828, 74833},
    { 80, 74829, 74834},
    { 90, 74830, 74835},
    {100, 74831, 74836},
};

Unit* SelectRandomTarget(Creature* ref, float range = 0.0f, int32 aura = 0)
{
    if (!ref)
        return NULL;

    std::list<Unit*> exclusions;
    if (ref->GetVictim())
        exclusions.push_back(ref->GetVictim());

    return ref->AI()->SelectTarget(SELECT_TARGET_RANDOM, 0, range, true, aura, exclusions);
}

int round(int numToRound, int multiple)  
{
    if(multiple == 0)  
        return numToRound;

    if (numToRound < multiple)
    {
        if (numToRound >= (multiple / 2))
            return multiple;
        else return 0;
    }

    int remainder = numToRound % multiple; 

    if (remainder == 0)
        return numToRound; 
    else if (remainder >= 5)
        return numToRound + (multiple - remainder);
    else return numToRound - remainder;

}

// Halion
class boss_halion : public CreatureScript
{
public:

    boss_halion() : CreatureScript("boss_halion")
    {
    }

    struct boss_halionAI : public BossAI
    {

        boss_halionAI(Creature* creature) : BossAI(creature, DATA_HALION)
        {
        }

        void Reset() override
        {
            _Reset();
            me->SetReactState(REACT_DEFENSIVE);
            me->SetHealth(me->GetMaxHealth());
            me->RemoveAurasDueToSpell(SPELL_TWILIGHT_PHASING);
        }

        void EnterCombat(Unit* who) override
        {
            _EnterCombat();
            Talk(SAY_AGGRO);
            instance->SendEncounterUnit(ENCOUNTER_FRAME_ENGAGE, me, 1);
            me->AddAura(SPELL_TWILIGHT_PRECISION, me);

            events.Reset();
            events.SetPhase(PHASE_ONE);

            // Schedule events without taking care of phases, since EventMap will not be updated under phase 2.
            events.ScheduleEvent(EVENT_ACTIVATE_FIREWALL, 10 * IN_MILLISECONDS);
            events.ScheduleEvent(EVENT_CLEAVE, urand(8 * IN_MILLISECONDS, 10 * IN_MILLISECONDS));
            events.ScheduleEvent(EVENT_FLAME_BREATH, urand(10 * IN_MILLISECONDS, 12 * IN_MILLISECONDS));
            events.ScheduleEvent(EVENT_METEOR_STRIKE, 20 * IN_MILLISECONDS);
            events.ScheduleEvent(EVENT_TAIL_LASH, 10 * IN_MILLISECONDS);

            // evade check every 3 secs
            events.ScheduleEvent(EVENT_EVADE_CHECK, 3 * IN_MILLISECONDS);

            // Due to Halion's EventMap not being updated under phase two, Berserk will be triggered by the Controller
            // so that the timer still ticks in phase two.
            if (Creature* controller = ObjectAccessor::GetCreature(*me, instance->GetData64(DATA_HALION_CONTROLLER)))
            {
                if (controller->AI()->GetData(DATA_PHASE) == PHASE_ONE)
                    events.ScheduleEvent(EVENT_FIERY_COMBUSTION, 15 * IN_MILLISECONDS);

                controller->AI()->EnterCombat(who);
            }
        }

        void JustDied(Unit* killer) override
        {
            _JustDied();
            instance->SendEncounterUnit(ENCOUNTER_FRAME_DISENGAGE, me);
            me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE);

 
            if (Creature* controller = ObjectAccessor::GetCreature(*me, instance->GetData64(DATA_HALION_CONTROLLER)))
                controller->AI()->Reset();

            if (GameObject* go = me->FindNearestGameObject(GO_HALION_PORTAL_1, 200.0f))
                go->RemoveFromWorld();
        }

        void KilledUnit(Unit* victim) override
        {
            if (victim->GetTypeId() == TYPEID_PLAYER)
                Talk(SAY_KILL);
        }

        void JustReachedHome() override
        {
            instance->SendEncounterUnit(ENCOUNTER_FRAME_DISENGAGE, me);

            me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
            me->RemoveAurasDueToSpell(SPELL_TWILIGHT_PHASING);
            me->SetReactState(REACT_AGGRESSIVE);

            if (GameObject* go = me->FindNearestGameObject(GO_HALION_PORTAL_1, 200.0f))
                go->RemoveFromWorld();
            if (Creature* controller = ObjectAccessor::GetCreature(*me, instance->GetData64(DATA_HALION_CONTROLLER)))
                controller->AI()->Reset();

            _JustReachedHome();
        }

        void DamageTaken(Unit* attacker, uint32& damage) override
        {
            if ((events.GetPhaseMask() & PHASE_MASK_THREE) && attacker->GetEntry() != NPC_TWILIGHT_HALION)
            {
                if (Creature* twilightHalion = ObjectAccessor::GetCreature(*me, instance->GetData64(DATA_TWILIGHT_HALION)))
                {
                    int32 _damage = damage + (twilightHalion->GetHealth() - me->GetHealth());

                    if (_damage < 0)
                        _damage = 0;

                    SpellNonMeleeDamage damageInfo(me, twilightHalion, SPELL_COPY_DAMAGE, SPELL_SCHOOL_MASK_NONE);
                    damageInfo.damage = (uint32)_damage;
                    me->SendSpellNonMeleeDamageLog(&damageInfo);
                    me->DealSpellDamage(&damageInfo, false);
                }
            }

            // Check phase under 40% health, hackish way to prevent exploit
            if (HealthBelowPct(40) && !(events.GetPhaseMask() & PHASE_MASK_THREE))
            {
                events.SetPhase(PHASE_THREE);
                if (Creature* controller = ObjectAccessor::GetCreature(*me, instance->GetData64(DATA_HALION_CONTROLLER)))
                    controller->AI()->DoAction(ACTION_PHASE_THREE);
            }

            if (HealthBelowPct(75) && (events.GetPhaseMask() & PHASE_MASK_ONE))
            {
                events.SetPhase(PHASE_TWO);
                events.DelayEvents(2600); // 2.5 sec + 0.1 sec lag

                me->CastStop();
                me->AttackStop();

                Talk(SAY_PHASE_TWO);

                // any player(s) outside of the Flame Ring are being teleported back inside of it, near the portal
                TeleportPlayersInsideFlameRing();

                if (Creature* controller = ObjectAccessor::GetCreature(*me, instance->GetData64(DATA_HALION_CONTROLLER)))
                    controller->AI()->DoAction(ACTION_PHASE_TWO);

                DoCast(me, SPELL_TWILIGHT_PHASING);
                if (Creature* controller = ObjectAccessor::GetCreature(*me, instance->GetData64(DATA_HALION_CONTROLLER)))
                    controller->AI()->DoAction(ACTION_PH2_WIPE_CHECK);
            }

            if ((events.GetPhaseMask() & PHASE_MASK_THREE) && attacker->GetEntry() != NPC_TWILIGHT_HALION)
            {
                me->m_PlayerDamageReq = 0;
                if (Creature* controller = ObjectAccessor::GetCreature(*me, instance->GetData64(DATA_HALION_CONTROLLER)))
                    controller->AI()->SetData(DATA_MATERIAL_DAMAGE_TAKEN, damage);
            }
        }

        Position const* GetMeteorStrikePosition() const
        {
            return &meteorStrikePos;
        }

        bool CanAIAttack(Unit const* victim) const override
        {
            return !victim->HasAura(SPELL_TWILIGHT_REALM);
        }

        // checks if Halion is outside of the flame ring
        bool IsHalionOutsideFlameRing()
        {
            if (Creature* controller = ObjectAccessor::GetCreature(*me, instance->GetData64(DATA_HALION_CONTROLLER)))
            {
            float distance = sqrt((HalionSpawnPos.GetPositionX() - me->GetPositionX()) * (HalionSpawnPos.GetPositionX() - me->GetPositionX())
                    + (HalionSpawnPos.GetPositionY() - me->GetPositionY()) * (HalionSpawnPos.GetPositionY() - me->GetPositionY()));
                // flame ring radius is 45
                return (distance > 46.0f);
            }
            return false;
        }

        // executed during phase 1 -> 2 transition; any player(s) outside of the flame ring are
        // teleported back inside the flame ring.
        void TeleportPlayersInsideFlameRing()
        {
            if (Map* map = me->GetMap())
                if (map->IsDungeon())
                {
                    Map::PlayerList const &PlayerList = map->GetPlayers();
                    if (!PlayerList.isEmpty())
                        for (Map::PlayerList::const_iterator i = PlayerList.begin(); i != PlayerList.end(); ++i)
                            if (i->GetSource() && i->GetSource()->IsAlive())
                            {
                                float distance = sqrt((HalionSpawnPos.GetPositionX() - i->GetSource()->GetPositionX()) * (HalionSpawnPos.GetPositionX() - i->GetSource()->GetPositionX())
                                        + (HalionSpawnPos.GetPositionY() - i->GetSource()->GetPositionY()) * (HalionSpawnPos.GetPositionY() - i->GetSource()->GetPositionY()));
                                if (distance > 45.0f)
                                    // teleport player
                                    i->GetSource()->NearTeleportTo(HalionSpawnPos.GetPositionX() + 30,
                                        HalionSpawnPos.GetPositionY(), HalionSpawnPos.GetPositionZ(), 0.0f);
                            }
                }
        }

        void UpdateAI(uint32 diff) override
        {
            if (!(events.GetPhaseMask() & PHASE_MASK_TWO))
                if (!UpdateVictim())
                    return;

            // Events won't be updated under phase two.
            if (!(events.GetPhaseMask() & PHASE_MASK_TWO))
                events.Update(diff);

            while (uint32 eventId = events.ExecuteEvent())
            {
                switch (eventId)
                {
                    case EVENT_ACTIVATE_FIREWALL:
                    {
                        // Firewall is activated 10 seconds after starting encounter, DOOR_TYPE_ROOM is only instant.
                        if (GameObject* firewall = ObjectAccessor::GetGameObject(*me, instance->GetData64(DATA_FLAME_RING)))
                            instance->HandleGameObject(instance->GetData64(DATA_FLAME_RING), false, firewall);
                        if (GameObject* firewall = ObjectAccessor::GetGameObject(*me, instance->GetData64(DATA_TWILIGHT_FLAME_RING)))
                            instance->HandleGameObject(instance->GetData64(DATA_TWILIGHT_FLAME_RING), false, firewall);
                        break;
                    }

                    case EVENT_FLAME_BREATH:
                        orientation = me->GetOrientation();
                        DoCastVictim(RAID_MODE(SPELL_FLAME_BREATH_10_N, SPELL_FLAME_BREATH_25_N, SPELL_FLAME_BREATH_10_H, SPELL_FLAME_BREATH_25_H));
                        me->SetFacingTo(orientation); // hack
                        events.ScheduleEvent(EVENT_FLAME_BREATH, 19 * IN_MILLISECONDS);
                        break;

                    case EVENT_CLEAVE:
                        DoCastVictim(SPELL_CLEAVE);
                        events.ScheduleEvent(EVENT_CLEAVE, urand(8 * IN_MILLISECONDS, 10 * IN_MILLISECONDS));
                        break;

                    case EVENT_TAIL_LASH:
                        DoCastAOE(SPELL_TAIL_LASH);
                        events.ScheduleEvent(EVENT_TAIL_LASH, 10 * IN_MILLISECONDS);
                        break;

                    case EVENT_METEOR_STRIKE:
                    {
                        if (Unit* target = SelectRandomTarget(me, 0.0f, -SPELL_TWILIGHT_REALM))
                        {
                            meteorStrikePos = target->GetPosition();
                            me->CastStop();
                            me->CastSpell(meteorStrikePos.GetPositionX(), meteorStrikePos.GetPositionY(), meteorStrikePos.GetPositionZ(),
                                    SPELL_METEOR_STRIKE, true, NULL, NULL, me->GetGUID());
                            Talk(SAY_METEOR_STRIKE);
                        }

                        events.ScheduleEvent(EVENT_METEOR_STRIKE, 40 * IN_MILLISECONDS);
                        break;
                    }

                    case EVENT_FIERY_COMBUSTION:
                    {
                        if (Unit* target = SelectRandomTarget(me, 0.0f, -SPELL_TWILIGHT_REALM))
                        {
                            me->CastStop();
                            DoCast(target, SPELL_FIERY_COMBUSTION);
                        }

                        events.ScheduleEvent(EVENT_FIERY_COMBUSTION, 25 * IN_MILLISECONDS);
                        break;
                    }

                    case EVENT_TWILIGHT_MENDING:
                        DoCast(me, SPELL_TWILIGHT_MENDING, true);
                        events.ScheduleEvent(EVENT_TWILIGHT_MENDING, 2 * IN_MILLISECONDS);
                        break;

                    case EVENT_EVADE_CHECK:
                    {
                        bool evade = IsHalionOutsideFlameRing();
                        // enter Evade if Halion is (being) pulled outside of Flame Ring
                        if (evade)
                        {
                            EnterEvadeMode();
                            return;
                        }

                        // next check in 3 seconds
                        events.ScheduleEvent(EVENT_EVADE_CHECK, 3 * IN_MILLISECONDS);
                        break;
                    }

                    default:
                        break;
                }
            }

            DoMeleeAttackIfReady();
        }

        void SetData(uint32 index, uint32 value) override
        {
            if (index == DATA_FIGHT_PHASE)
                events.SetPhase(value);
            else if (index == DATA_MENDING)
            {
                if (value)
                    events.ScheduleEvent(EVENT_TWILIGHT_MENDING, 1000);
                else
                    events.CancelEvent(EVENT_TWILIGHT_MENDING);
            }
        }

        uint32 GetData(uint32 index) const override
        {
            if (index == DATA_FIGHT_PHASE)
                return events.GetPhaseMask();

            return 0;
        }

        void DoAction(int32 action) override
        {
            switch (action)
            {
                case ACTION_BERSERK:
                    Talk(SAY_BERSERK);
                    DoCast(me, SPELL_BERSERK);
                    break;
                case ACTION_FIERY_COMBUSTION:
                    events.ScheduleEvent(EVENT_FIERY_COMBUSTION, 0);
                    break;
                case ACTION_METEOR_STRIKE:
                    events.ScheduleEvent(EVENT_METEOR_STRIKE, 0);
                    break;
                case ACTION_PHASE_THREE:
                    events.CancelEvent(EVENT_FIERY_COMBUSTION);
                    events.CancelEvent(EVENT_METEOR_STRIKE);
                    break;
                default:
                    break;
            }
        }

    private:
        Position meteorStrikePos;
        float orientation;
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return GetRubySanctumAI<boss_halionAI> (creature);
    }
};

typedef boss_halion::boss_halionAI HalionAI;

class boss_twilight_halion : public CreatureScript
{
public:

    boss_twilight_halion() : CreatureScript("boss_twilight_halion")
    {
    }

    struct boss_twilight_halionAI : public ScriptedAI
    {

        boss_twilight_halionAI(Creature* creature) : ScriptedAI(creature), instance(creature->GetInstanceScript())
        {
            me->ApplySpellImmune(0, IMMUNITY_EFFECT, SPELL_EFFECT_KNOCK_BACK, true);
            me->ApplySpellImmune(0, IMMUNITY_EFFECT, SPELL_EFFECT_KNOCK_BACK_DEST, true);

            me->SetPhaseMask(0x20, true); // Should not be visible with phasemask 0x21, so only 0x20
            events.SetPhase(PHASE_ONE);

            if (GameObject* flameRing = ObjectAccessor::GetGameObject(*me, instance->GetData64(DATA_TWILIGHT_FLAME_RING)))
                instance->HandleGameObject(instance->GetData64(DATA_TWILIGHT_FLAME_RING), false, flameRing);
        }

        void Reset() override
        {
            if (Creature* halion = ObjectAccessor::GetCreature(*me, instance->GetData64(DATA_HALION)))
            {
                halion->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                halion->SetHealth(halion->GetMaxHealth());
                halion->AI()->EnterEvadeMode();
            }
            if (GameObject* go = ObjectAccessor::GetGameObject(*me, portal1))
                go->RemoveFromWorld();
            if (GameObject* go = ObjectAccessor::GetGameObject(*me, portal2))
                go->RemoveFromWorld();
            if (GameObject* go = ObjectAccessor::GetGameObject(*me, portal3))
                go->RemoveFromWorld();
            if (GameObject* go = ObjectAccessor::GetGameObject(*me, portal4))
                go->RemoveFromWorld();
            if (Creature* orbCarrier = ObjectAccessor::GetCreature(*me, instance->GetData64(DATA_ORB_CARRIER)))
                orbCarrier->AI()->DoAction(ACTION_STOP_ROTATE);
        }

        void EnterCombat(Unit* who) override
        {
            if (Creature* whoCreature = who->ToCreature())
                if (whoCreature->GetEntry() == NPC_COMBAT_STALKER)
                    return;

            instance->SendEncounterUnit(ENCOUNTER_FRAME_ENGAGE, me, 2);
            me->AddAura(SPELL_TWILIGHT_PRECISION, me);
            me->AddAura(SPELL_DUSK_SHROUD, me);

            events.Reset();
            events.SetPhase(PHASE_TWO);

            // set health of twilight halion to the health of the real halion, should be 75%
            if (Creature* halion = ObjectAccessor::GetCreature(*me, instance->GetData64(DATA_HALION)))
                me->SetHealth(halion->GetHealth());

            if (Creature* orbCarrier = ObjectAccessor::GetCreature(*me, instance->GetData64(DATA_ORB_CARRIER)))
                orbCarrier->AI()->DoAction(ACTION_ROTATE);

            // All of Twilight Halion's abilities are not phase dependant as he is never on Phase One.
            // However, phasemasks are "needed" so that we know on which phase we are when Halion takes
            // damage, causing corporeality not to tick in phase two.
            events.ScheduleEvent(EVENT_CLEAVE, urand(8 * IN_MILLISECONDS, 10 * IN_MILLISECONDS));
            events.ScheduleEvent(EVENT_TAIL_LASH, 10 * IN_MILLISECONDS);
        }

        void KilledUnit(Unit* victim) override
        {

            if (victim->GetTypeId() == TYPEID_PLAYER)
                if (Creature* halion = ObjectAccessor::GetCreature(*me, instance->GetData64(DATA_HALION)))
                    halion->AI()->Talk(SAY_KILL);

            // Victims should not be in the Twilight Realm
            me->CastSpell(victim, SPELL_LEAVE_TWILIGHT_REALM, true);
        }

        void JustDied(Unit* killer) override
        {
            if (Creature* controller = ObjectAccessor::GetCreature(*me, instance->GetData64(DATA_HALION_CONTROLLER)))
                controller->CastSpell(controller, SPELL_CLEAR_DEBUFFS);

            if (Creature* orbCarrier = ObjectAccessor::GetCreature(*me, instance->GetData64(DATA_ORB_CARRIER)))
                orbCarrier->AI()->DoAction(ACTION_STOP_ROTATE);

            instance->SendEncounterUnit(ENCOUNTER_FRAME_DISENGAGE, me);

            if (Creature* halion = ObjectAccessor::GetCreature(*me, instance->GetData64(DATA_HALION)))
                halion->AI()->Talk(SAY_DEATH);
        }

        void JustReachedHome() override
        {
            instance->SendEncounterUnit(ENCOUNTER_FRAME_DISENGAGE, me);
            if (Creature* controller = ObjectAccessor::GetCreature(*me, instance->GetData64(DATA_HALION_CONTROLLER)))
                controller->CastSpell(controller, SPELL_CLEAR_DEBUFFS);

            ScriptedAI::JustReachedHome();
        }

        void DamageTaken(Unit* attacker, uint32& damage) override
        {
            if (!(events.GetPhaseMask() & PHASE_MASK_ONE) && attacker->GetEntry() != NPC_HALION)
            {
                if (Creature* halion = ObjectAccessor::GetCreature(*me, instance->GetData64(DATA_HALION)))
                {
                    int32 _damage = damage + (halion->GetHealth() - me->GetHealth());

                    if (_damage < 0)
                        _damage = 0;

                    SpellNonMeleeDamage damageInfo(me, halion, SPELL_COPY_DAMAGE, SPELL_SCHOOL_MASK_NONE);
                    damageInfo.damage = (uint32)_damage;
                    me->SendSpellNonMeleeDamageLog(&damageInfo);
                    me->DealSpellDamage(&damageInfo, false);
                }
            }

            if (me->HealthBelowPctDamaged(50, damage) && (events.GetPhaseMask() & PHASE_MASK_TWO))
            {
                events.SetPhase(PHASE_THREE);
                events.DelayEvents(2600); // 2.5 sec + 0.1sec lag

                me->CastStop();
                DoCast(me, SPELL_TWILIGHT_DIVISION);
                Talk(SAY_PHASE_THREE);

                // Stop here, else damage that triggered the phase change will be taken
                // into consideration in the next lines.
                //return;
            }

            if ((events.GetPhaseMask() & PHASE_MASK_THREE) && attacker->GetEntry() != NPC_HALION)
                if (Creature* controller = ObjectAccessor::GetCreature(*me, instance->GetData64(DATA_HALION_CONTROLLER)))
                    controller->AI()->SetData(DATA_TWILIGHT_DAMAGE_TAKEN, damage);
        }

        void SpellHitTarget(Unit* /*who*/, const SpellInfo* spell) override
        {
            if (spell->Id != SPELL_TWILIGHT_DIVISION)
                return;

            DoCast(me, corporealityReference[5].twilightRealmSpellId); // 50%
            if (Creature* halion = ObjectAccessor::GetCreature(*me, instance->GetData64(DATA_HALION)))
            {
                halion->CastSpell(halion, corporealityReference[5].materialRealmSpellId, false);
                halion->RemoveAurasDueToSpell(SPELL_TWILIGHT_PHASING);
                halion->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);

                if (GameObject* go = halion->FindNearestGameObject(GO_HALION_PORTAL_1, 200.0f))
                    go->RemoveFromWorld();

                if (GameObject* go = halion->SummonGameObject(GO_HALION_PORTAL_EXIT, HalionSpawnPos.GetPositionX() + 30,
                        HalionSpawnPos.GetPositionY(), HalionSpawnPos.GetPositionZ(), 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0))
                {
                    go->SetPhaseMask(0x20, true);
                    portal1 = go->GetGUID();
                }

                if (GameObject* go = halion->SummonGameObject(GO_HALION_PORTAL_EXIT, HalionSpawnPos.GetPositionX() - 30,
                        HalionSpawnPos.GetPositionY(), HalionSpawnPos.GetPositionZ(), 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0))
                {
                    go->SetPhaseMask(0x20, true);
                    portal2 = go->GetGUID();
                }

                if (GameObject* go = me->SummonGameObject(GO_HALION_PORTAL_2, HalionSpawnPos.GetPositionX(), HalionSpawnPos.GetPositionY() + 30,
                        HalionSpawnPos.GetPositionZ(), 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0))
                {
                    go->SetPhaseMask(0x01, true);
                    portal3 = go->GetGUID();
                }

                if (GameObject* go = me->SummonGameObject(GO_HALION_PORTAL_2, HalionSpawnPos.GetPositionX(), HalionSpawnPos.GetPositionY() - 30,
                        HalionSpawnPos.GetPositionZ(), 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0))
                {
                    go->SetPhaseMask(0x01, true);
                    portal4 = go->GetGUID();
                }
            }

            events.ScheduleEvent(EVENT_PHASE_THREE, 10 * IN_MILLISECONDS);
            if (Creature* controller = ObjectAccessor::GetCreature(*me, instance->GetData64(DATA_HALION_CONTROLLER)))
                controller->AI()->DoAction(ACTION_PHASE_THREE);
        }

        bool CanAIAttack(Unit const* victim) const override
        {
            return victim->HasAura(SPELL_TWILIGHT_REALM);
        }

        void DoAction(int32 action) override
        {
            switch (action)
            {
                case ACTION_BERSERK:
                    DoCast(me, SPELL_BERSERK);
                    break;
                case ACTION_SOUL_CONSUMPTION:
                    events.ScheduleEvent(EVENT_SOUL_CONSUMPTION, 0);
                    break;
                case ACTION_DARK_BREATH:
                    events.ScheduleEvent(EVENT_DARK_BREATH, 0);
                    break;
                default:
                    break;
            }
        }

        void SetData(uint32 index, uint32 value) override
        {
            if (index == DATA_MENDING)
            {
                if (value)
                    events.ScheduleEvent(EVENT_TWILIGHT_MENDING, 1 * IN_MILLISECONDS);
                else
                    events.CancelEvent(EVENT_TWILIGHT_MENDING);
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
                    case EVENT_DARK_BREATH:
                        orientation = me->GetOrientation();
                        DoCastVictim(RAID_MODE(SPELL_DARK_BREATH_10_N, SPELL_DARK_BREATH_25_N, SPELL_DARK_BREATH_10_H, SPELL_DARK_BREATH_25_H));
                        me->SetFacingTo(orientation); // hack
                        events.ScheduleEvent(EVENT_DARK_BREATH, 19 * IN_MILLISECONDS);
                        break;

                    case EVENT_SOUL_CONSUMPTION:
                    {
                        if (Unit* target = SelectRandomTarget(me, 0.0f, SPELL_TWILIGHT_REALM))
                        {
                            me->CastStop();
                            DoCast(target, SPELL_SOUL_CONSUMPTION);
                        }

                        events.ScheduleEvent(EVENT_SOUL_CONSUMPTION, 25 * IN_MILLISECONDS);
                        break;
                    }

                    case EVENT_CLEAVE:
                        DoCastVictim(SPELL_CLEAVE);
                        events.ScheduleEvent(EVENT_CLEAVE, urand(8 * IN_MILLISECONDS, 10 * IN_MILLISECONDS));
                        break;

                    case EVENT_TAIL_LASH:
                        DoCastAOE(SPELL_TAIL_LASH);
                        events.ScheduleEvent(EVENT_TAIL_LASH, 10 * IN_MILLISECONDS);
                        break;

                    case EVENT_PHASE_THREE:
                        if (Creature* halion = ObjectAccessor::GetCreature(*me, instance->GetData64(DATA_HALION)))
                            halion->AI()->SetData(DATA_FIGHT_PHASE, PHASE_THREE);
                        break;

                    case EVENT_TWILIGHT_MENDING:
                        DoCast(me, SPELL_TWILIGHT_MENDING, true);
                        events.ScheduleEvent(EVENT_TWILIGHT_MENDING, 2 * IN_MILLISECONDS);
                        break;

                    default:
                        break;
                }
            }

                DoMeleeAttackIfReady();
        }

    private:
        InstanceScript* instance;
        EventMap events;
        uint64 portal1;
        uint64 portal2;
        uint64 portal3;
        uint64 portal4;
        float orientation;
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return GetRubySanctumAI<boss_twilight_halionAI> (creature);
    }
};

typedef boss_twilight_halion::boss_twilight_halionAI twilightHalionAI;

class npc_halion_controller : public CreatureScript
{
public:

    npc_halion_controller() : CreatureScript("npc_halion_controller")
    {
    }

    struct npc_halion_controllerAI : public ScriptedAI
    {
        npc_halion_controllerAI(Creature* creature) : ScriptedAI(creature),
                instance(creature->GetInstanceScript()), summons(me)
        {
            me->SetPhaseMask(me->GetPhaseMask() | 0x20, true);
            livingembersummons = 0;
            phase = PHASE_ONE;
        }

        void Reset() override
        {
            me->SetReactState(REACT_PASSIVE);
            summons.DespawnAll();
            events.Reset();

            phase = PHASE_ONE;

            DoCast(me, SPELL_CLEAR_DEBUFFS);
        }

        // Halion controller counts as summoner for various NPCs, making them dissapear at encounter failure or success
        void JustSummoned(Creature* who) override
        {
            summons.Summon(who);

            if (who->GetEntry() == NPC_LIVING_INFERNO)
            {
                who->CastSpell(who, SPELL_BLAZING_AURA, true);
                //who->AI()->DoZoneInCombat();
            }
        }

        void EnterCombat(Unit* who) override
        {
            if (Creature* combatStalker = ObjectAccessor::GetCreature(*me, instance->GetData64(DATA_COMBAT_STALKER)))
                combatStalker->AI()->EnterCombat(who);

            events.ScheduleEvent(EVENT_TRIGGER_BERSERK, 8 * MINUTE * IN_MILLISECONDS);
        }

        void DoAction(int32 action) override
        {
            switch (action)
            {
                case ACTION_PH2_WIPE_CHECK:
                    events.ScheduleEvent(EVENT_PH2_WIPE_CHECK, 20000);
                    break;
                case ACTION_INTRO_HALION:
                    events.Reset();
                    events.ScheduleEvent(EVENT_START_INTRO, 2 * IN_MILLISECONDS);
                    break;

                case ACTION_PHASE_TWO:
                {
                    // shadow pulsars after 34 seconds
                    events.ScheduleEvent(EVENT_SHADOW_PULSARS_SHOOT, 34 * IN_MILLISECONDS);
                    events.ScheduleEvent(EVENT_SOUL_CONSUMPTION, 20 * IN_MILLISECONDS);
                    events.ScheduleEvent(EVENT_DARK_BREATH, 25 * IN_MILLISECONDS);
                    phase = PHASE_TWO;
                    break;
                }

                case ACTION_PHASE_THREE:
                {
                    events.ScheduleEvent(EVENT_CHECK_CORPOREALITY, 20 * IN_MILLISECONDS);
                    events.ScheduleEvent(EVENT_FIERY_COMBUSTION, 17 * IN_MILLISECONDS);
                    events.ScheduleEvent(EVENT_METEOR_STRIKE, 27 * IN_MILLISECONDS);
                    twilightDamageTaken = 0;
                    materialDamageTaken = 0;
                    materialCorporealityValue = 50;
                    phase = PHASE_THREE;
                    if (Creature* halion = ObjectAccessor::GetCreature(*me, instance->GetData64(DATA_HALION)))
                        halion->AI()->DoAction(ACTION_PHASE_THREE);

                    instance->DoUpdateWorldState(WORLDSTATE_CORPOREALITY_TOGGLE, 1);
                    instance->DoUpdateWorldState(WORLDSTATE_CORPOREALITY_MATERIAL, 50);
                    instance->DoUpdateWorldState(WORLDSTATE_CORPOREALITY_TWILIGHT, 50);
                    break;
                }

                default:
                    break;
            }
        }

        void ActivateEmbers()
        {
            std::list<Creature*> livingemberlist;
            me->GetCreatureListWithEntryInGrid(livingemberlist, NPC_LIVING_EMBER, 46.0f);
            if (Creature* halion = ObjectAccessor::GetCreature(*me, instance->GetData64(DATA_HALION)))
            {
                if (Unit* target = SelectRandomTarget(halion, 46.0f, -SPELL_TWILIGHT_REALM))
                {
                    for (std::list<Creature*>::iterator itr = livingemberlist.begin(); itr != livingemberlist.end(); ++itr)
                    {
                        if ((*itr) && !(*itr)->IsInCombat())
                        {
                            (*itr)->SetReactState(REACT_AGGRESSIVE);
                            // we all shouldn't attack the same target, but we should definitely move there (source:youtube)
                            Position pos = target->GetPosition();
                            Position rpos = me->GetRandomPoint(pos, 5.0f);
                            (*itr)->GetMotionMaster()->MoveCharge(rpos.m_positionX, rpos.m_positionY, rpos.m_positionZ, (*itr)->GetSpeed(MOVE_RUN), 1);
                            //(*itr)->AI()->DoZoneInCombat();
                            (*itr)->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_PC|UNIT_FLAG_NOT_SELECTABLE);
                        }
                    }
                }
                else if (Unit* target = me->SelectNearestPlayer(46.0f))
                {
                    for (std::list<Creature*>::iterator itr = livingemberlist.begin(); itr != livingemberlist.end(); ++itr)
                    {
                        if ((*itr) && !(*itr)->IsInCombat())
                        {
                            (*itr)->SetReactState(REACT_AGGRESSIVE);
                            // we all shouldn't attack the same target, but we should definitely move there (source:youtube)
                            Position pos;
                            Position rpos;
                            pos = target->GetPosition();
                            rpos = me->GetRandomPoint(pos, 5.0f);
                            (*itr)->GetMotionMaster()->MoveCharge(rpos.m_positionX, rpos.m_positionY, rpos.m_positionZ, (*itr)->GetSpeed(MOVE_RUN), 1);
                            (*itr)->AI()->DoZoneInCombat();
                            (*itr)->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_PC|UNIT_FLAG_NOT_SELECTABLE);
                        }
                    }
                }
            }
        }

        void UpdateAI(uint32 diff) override
        {
            events.Update(diff);
            if (livingembersummons >= 8)
                events.ScheduleEvent(EVENT_ACTIVATE_EMBERS, 3000);

            while (uint32 eventId = events.ExecuteEvent())
            {
                switch (eventId)
                {
                    case EVENT_FIERY_COMBUSTION:
                        if (Creature* halion = ObjectAccessor::GetCreature(*me, instance->GetData64(DATA_HALION)))
                            halion->AI()->DoAction(ACTION_FIERY_COMBUSTION);
                        break;
                    case EVENT_METEOR_STRIKE:
                        if (Creature* halion = ObjectAccessor::GetCreature(*me, instance->GetData64(DATA_HALION)))
                            halion->AI()->DoAction(ACTION_METEOR_STRIKE);
                        break;
                    case EVENT_DARK_BREATH:
                        if (Creature* twilighthalion = ObjectAccessor::GetCreature(*me, instance->GetData64(DATA_TWILIGHT_HALION)))
                            twilighthalion->AI()->DoAction(ACTION_DARK_BREATH);
                        break;
                    case EVENT_ACTIVATE_EMBERS:
                        ActivateEmbers();
                        break;
                    case EVENT_START_INTRO:
                        DoCast(me, SPELL_COSMETIC_FIRE_PILLAR, true);

                        events.ScheduleEvent(EVENT_INTRO_PROGRESS_1, 4 * IN_MILLISECONDS);
                        break;

                    case EVENT_INTRO_PROGRESS_1:
                        for (uint8 i = DATA_BURNING_TREE_3; i <= DATA_BURNING_TREE_4; ++i)
                            if (GameObject* tree = ObjectAccessor::GetGameObject(*me, instance->GetData64(i)))
                                instance->HandleGameObject(instance->GetData64(i), true, tree);

                        events.ScheduleEvent(EVENT_INTRO_PROGRESS_2, 4 * IN_MILLISECONDS);
                        break;

                    case EVENT_INTRO_PROGRESS_2:
                        for (uint8 i = DATA_BURNING_TREE_1; i <= DATA_BURNING_TREE_2; ++i)
                            if (GameObject* tree = ObjectAccessor::GetGameObject(*me, instance->GetData64(i)))
                                instance->HandleGameObject(instance->GetData64(i), true, tree);

                        events.ScheduleEvent(EVENT_INTRO_PROGRESS_3, 4 * IN_MILLISECONDS);
                        break;

                    case EVENT_INTRO_PROGRESS_3:
                    {
                        DoCast(me, SPELL_FIERY_EXPLOSION);
                        if (Creature* halion = me->GetMap()->SummonCreature(NPC_HALION, HalionSpawnPos))
                            halion->AI()->Talk(SAY_INTRO);

                        me->GetMap()->SummonCreature(NPC_TWILIGHT_HALION, HalionSpawnPos);
                        break;
                    }

                    // This event can't be in the Shadow Pulsars, else Halion would yell at every world update tick.
                    case EVENT_SHADOW_PULSARS_SHOOT:
                    {
                        if (Creature* twilightHalion = ObjectAccessor::GetCreature(*me, instance->GetData64(DATA_TWILIGHT_HALION)))
                            twilightHalion->AI()->Talk(SAY_SPHERE_PULSE); // beware the shadow

                        if (Creature* orbCarrier = ObjectAccessor::GetCreature(*me, instance->GetData64(DATA_ORB_CARRIER)))
                            orbCarrier->AI()->DoAction(ACTION_SHOOT);

                        events.ScheduleEvent(EVENT_SHADOW_PULSARS_SHOOT, 30 * IN_MILLISECONDS);
                        break;
                    }

                    case EVENT_SOUL_CONSUMPTION:
                        if (Creature* twilightHalion = ObjectAccessor::GetCreature(*me, instance->GetData64(DATA_TWILIGHT_HALION)))
                            twilightHalion->AI()->DoAction(ACTION_SOUL_CONSUMPTION);
                        break;

                    case EVENT_TRIGGER_BERSERK:
                        for (uint8 i = DATA_HALION; i <= DATA_TWILIGHT_HALION; i++)
                            if (Creature* halion = ObjectAccessor::GetCreature(*me, instance->GetData64(i)))
                                halion->AI()->DoAction(ACTION_BERSERK);
                        break;

                    case EVENT_CHECK_CORPOREALITY:
                    {
                        if (instance->GetBossState(DATA_HALION) == DONE)
                            break;

                        bool canUpdate = false;
                        uint8 mode = 0;
                        if (materialDamageTaken != 0 && twilightDamageTaken != 0)
                        {
                            float val = float(float(twilightDamageTaken) / float(materialDamageTaken)) * 100.0f;
                            float pct = float(float(val) - 100.0f) * 0.5f;
                            uint8 _materialCorporealityValue = (uint8)pct + 50;

                            if (_materialCorporealityValue > 100)
                                _materialCorporealityValue = 100;

                            _materialCorporealityValue = round(_materialCorporealityValue, 10);

                            twilightDamageTaken = 0;
                            materialDamageTaken = 0;

                            canUpdate = _materialCorporealityValue != materialCorporealityValue;

                            if (canUpdate)
                            {
                                mode = _materialCorporealityValue > materialCorporealityValue;
                                materialCorporealityValue += _materialCorporealityValue > materialCorporealityValue ? 10 : -10;
                            }

                            if (Creature* halion = ObjectAccessor::GetCreature(*me, instance->GetData64(DATA_HALION)))
                            {
                                if (Creature* twilightHalion = ObjectAccessor::GetCreature(*me, instance->GetData64(DATA_TWILIGHT_HALION)))
                                {
                                    halion->AI()->SetData(DATA_MENDING, 0);
                                    twilightHalion->AI()->SetData(DATA_MENDING, 0);
                                }
                            }
                        } else
                        {
                            if (Creature* halion = ObjectAccessor::GetCreature(*me, instance->GetData64(DATA_HALION)))
                            {
                                if (Creature* twilightHalion = ObjectAccessor::GetCreature(*me, instance->GetData64(DATA_TWILIGHT_HALION)))
                                {
                                    halion->AI()->SetData(DATA_MENDING, 1);
                                    twilightHalion->AI()->SetData(DATA_MENDING, 1);

                                    Map::PlayerList const &PlList = me->GetMap()->GetPlayers();
                                    for (Map::PlayerList::const_iterator i = PlList.begin(); i != PlList.end(); ++i)
                                        if (Player* player = i->GetSource())
                                            Talk(EMOTE_REGENERATE, player);
                                }
                            }

                            events.ScheduleEvent(EVENT_CHECK_CORPOREALITY, 15 * IN_MILLISECONDS);
                            break;
                        }

                        if (canUpdate)
                        {
                            for (uint8 i = 0; i < MAX_CORPOREALITY_STATE; i++)
                            {
                                if (corporealityReference[i].materialPercentage == materialCorporealityValue)
                                {
                                    for (uint8 j = DATA_HALION; j <= DATA_TWILIGHT_HALION; j++)
                                    {
                                        if (Creature* halion = ObjectAccessor::GetCreature(*me, instance->GetData64(j)))
                                        {
                                            RemoveCorporeality(halion);
                                            halion->CastSpell(halion, (j == DATA_HALION ? corporealityReference[i].materialRealmSpellId : corporealityReference[i].twilightRealmSpellId), true);
                                        }
                                    }

                                    break; // No need to iterate more
                                }
                            }

                            instance->DoUpdateWorldState(WORLDSTATE_CORPOREALITY_MATERIAL, materialCorporealityValue);
                            instance->DoUpdateWorldState(WORLDSTATE_CORPOREALITY_TWILIGHT, (100 - materialCorporealityValue));

                            Map::PlayerList const &playersList = me->GetMap()->GetPlayers();
                            for (Map::PlayerList::const_iterator i = playersList.begin(); i != playersList.end(); ++i)
                            {
                                if (Player* player = i->GetSource())
                                {
                                    if (mode) // Range is 0 ... 100
                                    {
                                        if (player->HasAura(SPELL_TWILIGHT_REALM))
                                            Talk(EMOTE_TWILIGHT_OUT_TWILIGHT, player);
                                        else
                                            Talk(EMOTE_PHYSICAL_IN_PHYSICAL, player);
                                    } else
                                    {
                                        if (player->HasAura(SPELL_TWILIGHT_REALM))
                                            Talk(EMOTE_TWILIGHT_IN_TWILIGHT, player);
                                        else
                                            Talk(EMOTE_PHYSICAL_OUT_PHYSICAL, player);
                                    }
                                }
                            }
                        }

                        events.ScheduleEvent(EVENT_CHECK_CORPOREALITY, 15 * IN_MILLISECONDS);
                        break;
                    }

                    default:
                        break;
                }
            }
        }

        void SetData(uint32 id, uint32 value) override
        {
            switch (id)
            {
                case DATA_MATERIAL_DAMAGE_TAKEN:
                    materialDamageTaken += value;
                    break;
                case DATA_TWILIGHT_DAMAGE_TAKEN:
                    twilightDamageTaken += value;
                    break;
                case DATA_LIVING_EMBER_SUMMONS:
                    livingembersummons = value;
                    break;
                default:
                    break;
            }
        }

        uint32 GetData(uint32 index) const override
        {
            switch (index)
            {
                case DATA_LIVING_EMBER_SUMMONS:
                    return livingembersummons;
                    break;
                case DATA_PHASE:
                    return phase;
                default:
                    return 0;
            }
        }

    private:

        void RemoveCorporeality(Creature* who)
        {
            for (uint8 i = 0; i < MAX_CORPOREALITY_STATE; i++)
            {
                if (who->HasAura(corporealityReference[i].materialRealmSpellId))
                {
                    who->RemoveAurasDueToSpell(corporealityReference[i].materialRealmSpellId);
                    break;
                }
                if (who->HasAura(corporealityReference[i].twilightRealmSpellId))
                {
                    who->RemoveAurasDueToSpell(corporealityReference[i].twilightRealmSpellId);
                    break;
                }
            }
        }

        EventMap events;
        InstanceScript* instance;
        SummonList summons;

        uint32 phase;

        uint32 twilightDamageTaken;
        uint32 materialDamageTaken;
        uint8 materialCorporealityValue;
        uint32 livingembersummons;
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return GetRubySanctumAI<npc_halion_controllerAI> (creature);
    }
};

typedef npc_halion_controller::npc_halion_controllerAI controllerAI;

class npc_meteor_strike_initial : public CreatureScript
{
public:

    npc_meteor_strike_initial() : CreatureScript("npc_meteor_strike_initial")
    {
    }

    struct npc_meteor_strike_initialAI : public ScriptedAI
    {

        npc_meteor_strike_initialAI(Creature* creature) : ScriptedAI(creature),
                instance(creature->GetInstanceScript())
        {
            SetCombatMovement(false);
        }

        void DoAction(int32 action) override
        {
            switch (action)
            {
                case ACTION_METEOR_STRIKE_AOE:
                    DoCast(me, RAID_MODE(SPELL_METEOR_STRIKE_AOE_DAMAGE_10_N, SPELL_METEOR_STRIKE_AOE_DAMAGE_25_N,
                            SPELL_METEOR_STRIKE_AOE_DAMAGE_10_H, SPELL_METEOR_STRIKE_AOE_DAMAGE_25_H), true);
                    DoCast(me, SPELL_METEOR_STRIKE_FIRE_AURA_1, true);

                    for (std::list<Creature*>::iterator itr = meteorList.begin(); itr != meteorList.end(); ++itr)
                        (*itr)->AI()->DoAction(ACTION_METEOR_STRIKE_BURN);

                    break;
            }
        }

        void IsSummonedBy(Unit* summoner) override
        {
            Creature* owner = summoner->ToCreature();
            if (!owner)
                return;

            // Let Halion Controller count as summoner
            if (Creature* controller = ObjectAccessor::GetCreature(*me, instance->GetData64(DATA_HALION_CONTROLLER)))
                controller->AI()->JustSummoned(me);

            DoCast(me, SPELL_METEOR_STRIKE_COUNTDOWN);
            DoCast(me, SPELL_BIRTH_NO_VISUAL); // Unknown purpose

            if (HalionAI* halionAI = CAST_AI(HalionAI, owner->AI()))
            {
                Position const* ownerPos = halionAI->GetMeteorStrikePosition();
                Position newPos;
                float angle[4];
                angle[0] = me->GetAngle(ownerPos);
                angle[1] = me->GetAngle(ownerPos) - static_cast<float> (M_PI / 2);
                angle[2] = me->GetAngle(ownerPos) - static_cast<float> (-M_PI / 2);
                angle[3] = me->GetAngle(ownerPos) - static_cast<float> (M_PI);

                meteorList.clear();
                for (uint8 i = 0; i < 4; i++)
                {
                    Position::NormalizeOrientation(angle[i]);
                    me->SetOrientation(angle[i]);
                    newPos = me->GetNearPosition(10.0f, 0.0f); // Exact distance
                    if (Creature* meteor = me->SummonCreature(NPC_METEOR_STRIKE_NORTH + i, newPos, TEMPSUMMON_TIMED_DESPAWN, 30000))
                        meteorList.push_back(meteor);
                }
            }
        }

        void UpdateAI(uint32 /*diff*/) override
        {
        }

        void EnterEvadeMode() override
        {
        }

    private:
        InstanceScript* instance;
        std::list<Creature*> meteorList;
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return GetRubySanctumAI<npc_meteor_strike_initialAI> (creature);
    }
};

class npc_meteor_strike : public CreatureScript
{
public:

    npc_meteor_strike() : CreatureScript("npc_meteor_strike")
    {
    }

    struct npc_meteor_strikeAI : public ScriptedAI
    {
        npc_meteor_strikeAI(Creature* creature) : ScriptedAI(creature),
                instance(creature->GetInstanceScript())
        {
            SetCombatMovement(false);
            lastsummon = me;
            spawnCount = 0;
        }

        void DoAction(int32 action) override
        {
            if (action == ACTION_METEOR_STRIKE_BURN)
            {
                DoCast(me, SPELL_METEOR_STRIKE_FIRE_AURA_2, true);
                me->setActive(true);
                events.ScheduleEvent(EVENT_SPAWN_METEOR_FLAME, 500);
            }
        }

        void IsSummonedBy(Unit* /*summoner*/) override
        {
            // Let Halion Controller count as summoner.
            if (Creature* controller = ObjectAccessor::GetCreature(*me, instance->GetData64(DATA_HALION_CONTROLLER)))
                controller->AI()->JustSummoned(me);
        }

        void UpdateAI(uint32 diff) override
        {
            if (Creature* controller = ObjectAccessor::GetCreature(*me, instance->GetData64(DATA_HALION_CONTROLLER)))
            {
                if (lastsummon->GetDistance(controller) >= 45.0f || spawnCount > 10)
                    return;
            }
            else return;

            events.Update(diff);

            if (events.ExecuteEvent() == EVENT_SPAWN_METEOR_FLAME)
            {
                Position pos;
                float angle = frand(0.0f, 2.5f);
                float range = 5.0f /*- (randfac / 2)*/;
                pos = lastsummon->GetNearPosition(range, angle);

                if (Creature* flame = me->SummonCreature(NPC_METEOR_STRIKE_FLAME, pos, TEMPSUMMON_TIMED_DESPAWN, 25000))
                {
                    if (Creature* controller = ObjectAccessor::GetCreature(*me, instance->GetData64(DATA_HALION_CONTROLLER)))
                        controller->AI()->JustSummoned(flame);

                    flame->CastSpell(flame, SPELL_METEOR_STRIKE_FIRE_AURA_2, true);
                    ++spawnCount;
                    lastsummon = flame;
                }

                events.ScheduleEvent(EVENT_SPAWN_METEOR_FLAME, 500);
            }
        }

    private:
        InstanceScript* instance;
        EventMap events;
        float range;
        uint8 spawnCount;
        Creature* lastsummon;
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return GetRubySanctumAI<npc_meteor_strikeAI> (creature);
    }
};

class npc_meteor_strike_flame : public CreatureScript
{
public:

    npc_meteor_strike_flame() : CreatureScript("npc_meteor_strike_flame")
    {
    }

    struct npc_meteor_strike_flameAI : public ScriptedAI
    {
        npc_meteor_strike_flameAI(Creature* creature) : ScriptedAI(creature), instance(creature->GetInstanceScript())
        {
            SetCombatMovement(false);
            if (Creature* _controller = ObjectAccessor::GetCreature(*me, instance->GetData64(DATA_HALION_CONTROLLER)))
            {
                controller = _controller;
                controller->AI()->JustSummoned(me);
            }
        }

        void Reset() override
        {
            if (controller && Is25ManRaid() && IsHeroic()) // adds only spawn in 25hc
            {
                uint32 spawnCount = controller->AI()->GetData(DATA_LIVING_EMBER_SUMMONS);
                if (spawnCount <= 8)
                {
                    events.ScheduleEvent(EVENT_SUMMON_EMBER, 2000);
                    controller->AI()->SetData(DATA_LIVING_EMBER_SUMMONS, spawnCount + 1);
                }
            }
        }

        void DoAction(int32 action) override
        {
        }

        void IsSummonedBy(Unit* /*summoner*/) override
        {
            // Let Halion Controller count as summoner.
            if (Creature* controller = ObjectAccessor::GetCreature(*me, instance->GetData64(DATA_HALION_CONTROLLER)))
                controller->AI()->JustSummoned(me);
        }

        void UpdateAI(uint32 diff) override
        {
            events.Update(diff);
            while (uint32 eventId = events.ExecuteEvent())
            {
                switch (eventId)
                {
                    case EVENT_SUMMON_EMBER:
                        if (Creature* ember = me->SummonCreature(NPC_LIVING_EMBER, me->GetPositionX(), me->GetPositionY(), me->GetPositionZ(), me->GetOrientation(), TEMPSUMMON_CORPSE_DESPAWN))
                        {
                            ember->SetPhaseMask(0x01, true);

                            // Let Halion Controller count as summoner.
                            if (controller)
                                controller->AI()->JustSummoned(ember);
                        }
                        break;
                    default:
                        break;
                }
            }
        }

    private:
        InstanceScript* instance;
        EventMap events;
        Creature* controller;
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return GetRubySanctumAI<npc_meteor_strike_flameAI> (creature);
    }
};

class npc_combustion_consumption : public CreatureScript
{
public:

    npc_combustion_consumption() : CreatureScript("npc_combustion_consumption")
    {
    }

    struct npc_combustion_consumptionAI : public ScriptedAI
    {

        npc_combustion_consumptionAI(Creature* creature) : ScriptedAI(creature),
                summonerGuid(0), instance(creature->GetInstanceScript())
        {
            SetCombatMovement(false);
            switch (me->GetEntry())
            {
                case NPC_COMBUSTION:
                    explosionSpell = SPELL_FIERY_COMBUSTION_EXPLOSION;
                    damageSpell = SPELL_COMBUSTION_DAMAGE_AURA;
                    break;

                case NPC_CONSUMPTION:
                    explosionSpell = SPELL_SOUL_CONSUMPTION_EXPLOSION;
                    damageSpell = SPELL_CONSUMPTION_DAMAGE_AURA;
                    me->SetPhaseMask(0x20, true);
                    break;

                default: // Should never happen
                    explosionSpell = 0;
                    damageSpell = 0;
                    break;
            }

            if (IsHeroic())
                me->SetPhaseMask(0x01 | 0x20, true);
        }

        void IsSummonedBy(Unit* summoner) override
        {
            // Let Halion Controller count as summoner
            if (Creature* controller = ObjectAccessor::GetCreature(*me, instance->GetData64(DATA_HALION_CONTROLLER)))
                controller->AI()->JustSummoned(me);

            summonerGuid = summoner->GetGUID();
        }

        void SetData(uint32 type, uint32 value) override
        {
            Unit* summoner = ObjectAccessor::GetUnit(*me, summonerGuid);

            if (type != DATA_STACKS_DISPELLED || !damageSpell || !explosionSpell || !summoner)
                return;

            uint32 _value = value * (IsHeroic() ? 6 : 4) <= 255 ? value * (IsHeroic() ? 6 : 4) : 255; // max stack is 255
            me->CastCustomSpell(SPELL_SCALE_AURA, SPELLVALUE_AURA_STACK, _value, me);
            DoCast(me, damageSpell);

            int32 damage = 1200 + (value * 1290); // Needs more research.
            // Target is TARGET_UNIT_AREA_ALLY_SRC (TARGET_SRC_CASTER)
            summoner->CastCustomSpell(explosionSpell, SPELLVALUE_BASE_POINT0, damage, summoner);
        }

        void UpdateAI(uint32 /*diff*/) override { }

    private:
        InstanceScript* instance;
        uint32 explosionSpell;
        uint32 damageSpell;
        uint64 summonerGuid;
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return GetRubySanctumAI<npc_combustion_consumptionAI> (creature);
    }
};

class npc_orb_carrier : public CreatureScript
{
public:

    npc_orb_carrier() : CreatureScript("npc_orb_carrier")
    {
    }

    struct npc_orb_carrierAI : public ScriptedAI
    {

        npc_orb_carrierAI(Creature* creature) : ScriptedAI(creature), instance(creature->GetInstanceScript())
        {
            ASSERT(creature->GetVehicleKit());
            SetCombatMovement(false);
            me->setActive(true);
            me->SetPhaseMask(32, true);
            me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE);
            me->RemoveUnitMovementFlag(MOVEMENTFLAG_WALKING); 
            me->SetSpeed(MOVE_RUN, 6.0f, true);
        }

        void AttackStart(Unit* pWho) override
        {
            // ignore any attacks
            return;
        }

        void UpdateAI(uint32 diff) override
        {
            if (Creature* rotationFocus = ObjectAccessor::GetCreature(*me, instance->GetData64(DATA_ORB_ROTATION_FOCUS)))
                me->SetFacingToObject(rotationFocus); // setInFront

            events.Update(diff);
            while (uint32 eventId = events.ExecuteEvent())
            {
                switch (eventId)
                {
                    case EVENT_TRACK_ROTATION:
                    {
                        if (!me->HasUnitState(UNIT_STATE_CASTING))
                            me->CastSpell((Unit*) NULL, SPELL_TRACK_ROTATION, false);

                        events.ScheduleEvent(EVENT_TRACK_ROTATION, 500);
                        break;
                    }

                    case EVENT_SHADOW_PULSARS_SHOOT:
                    {
                        Vehicle* vehicle = me->GetVehicleKit();
                        Unit* southOrb = vehicle->GetPassenger(SEAT_SOUTH);
                        Unit* northOrb = vehicle->GetPassenger(SEAT_NORTH);
                        if (southOrb && northOrb)
                        {
                            if (northOrb->GetTypeId() != TYPEID_UNIT || southOrb->GetTypeId() != TYPEID_UNIT)
                                return;

                            northOrb->CastSpell(northOrb, SPELL_TWILIGHT_PULSE_PERIODIC, true);
                            southOrb->CastSpell(southOrb, SPELL_TWILIGHT_PULSE_PERIODIC, true);
                            northOrb->CastSpell(southOrb, SPELL_TWILIGHT_CUTTER, false);

                            northOrb->SetTarget(southOrb->GetGUID());
                            southOrb->SetTarget(northOrb->GetGUID());
                        }

                        if (!IsHeroic())
                            return;

                        Unit* eastOrb = vehicle->GetPassenger(SEAT_EAST);
                        Unit* westOrb = vehicle->GetPassenger(SEAT_WEST);
                        if (eastOrb && westOrb)
                        {
                            if (eastOrb->GetTypeId() != TYPEID_UNIT || westOrb->GetTypeId() != TYPEID_UNIT)
                                return;

                            eastOrb->CastSpell(eastOrb, SPELL_TWILIGHT_PULSE_PERIODIC, true);
                            westOrb->CastSpell(westOrb, SPELL_TWILIGHT_PULSE_PERIODIC, true);
                            eastOrb->CastSpell(westOrb, SPELL_TWILIGHT_CUTTER, false);

                            eastOrb->SetTarget(westOrb->GetGUID());
                            westOrb->SetTarget(eastOrb->GetGUID());
                        }
                        break;
                    }

                    default:
                        break;
                }
            }
        }

        void DoAction(int32 action) override
        {
            switch (action)
            {
                case ACTION_SHOOT:
                    // warn 5 seconds before shooting the twilight cutter
                    Talk(EMOTE_WARN_LASER);
                    events.ScheduleEvent(EVENT_SHADOW_PULSARS_SHOOT, 5 * IN_MILLISECONDS);
                    break;

                case ACTION_ROTATE:
                    events.ScheduleEvent(EVENT_TRACK_ROTATION, 500);

                    // Hide east and west orb if not in heroic mode
                    if (!IsHeroic())
                    {
                        Vehicle* vehicle = me->GetVehicleKit();
                        Unit* eastOrb = vehicle->GetPassenger(SEAT_EAST);
                        Unit* westOrb = vehicle->GetPassenger(SEAT_WEST);
                        if (eastOrb && westOrb)
                        {
                            if (eastOrb->GetTypeId() != TYPEID_UNIT || westOrb->GetTypeId() != TYPEID_UNIT)
                                return;

                            eastOrb->SetVisible(false);
                            westOrb->SetVisible(false);
                        }
                    }
                    break;

                case ACTION_STOP_ROTATE:
                    events.CancelEvent(EVENT_TRACK_ROTATION);
                    break;

                default:
                    break;
            }
        }

    private:
        InstanceScript* instance;
        EventMap events;
        float x, y;
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return GetRubySanctumAI<npc_orb_carrierAI> (creature);
    }
};

class npc_combat_stalker : public CreatureScript
{
public:

    npc_combat_stalker() : CreatureScript("npc_combat_stalker")
    {
    }

    struct npc_combat_stalkerAI : public ScriptedAI
    {

        npc_combat_stalkerAI(Creature* creature) : ScriptedAI(creature), instance(creature->GetInstanceScript())
        {
            SetCombatMovement(false);
            creature->SetPhaseMask(0x20 | 0x1, true);
            me->setActive(true);
        }

        void Reset() override
        {
            ScriptedAI::Reset();
        }

        void EnterCombat(Unit* who) override
        {
            DoZoneInCombat();
            if (who->GetTypeId() == TYPEID_UNIT)
                if (who->ToCreature()->GetEntry() == NPC_HALION || who->ToCreature()->GetEntry() == NPC_TWILIGHT_HALION)
                    me->AddThreat(who, frand(1.0f, 3.0f) * 100.0f);

            wipeCheck = true;
        }

        void EnterEvadeMode() override
        {
            if (Creature* halion = ObjectAccessor::GetCreature(*me, instance->GetData64(DATA_HALION)))
                halion->AI()->EnterEvadeMode();

            if (Creature* twilightHalion = ObjectAccessor::GetCreature(*me, instance->GetData64(DATA_TWILIGHT_HALION)))
                twilightHalion->AI()->EnterEvadeMode();

            wipeCheck = false;

            ScriptedAI::EnterEvadeMode();
        }

        void UpdateAI(uint32 diff) override
        {
            if (!me->IsInCombat())
                return;

            std::list<HostileReference*> const& threatList = me->getThreatManager().getThreatList();
            if (threatList.empty())
            {
                EnterEvadeMode();
                return;
            }

            if (!wipeCheck)
                return;

            wipeCheck = !wipeCheck;
            for (std::list<HostileReference*>::const_iterator itr = threatList.begin(); itr != threatList.end(); ++itr)
                if (Unit* refTarget = (*itr)->getTarget())
                    if (refTarget->GetTypeId() == TYPEID_PLAYER)
                        return;

            EnterEvadeMode();
        }

    private:
        InstanceScript* instance;
        bool wipeCheck;
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return GetRubySanctumAI<npc_combat_stalkerAI> (creature);
    }
};

class npc_living_inferno : public CreatureScript
{
public:

    npc_living_inferno() : CreatureScript("npc_living_inferno")
    {
    }

    struct npc_living_infernoAI : public ScriptedAI
    {

        npc_living_infernoAI(Creature* creature) : ScriptedAI(creature), instance(creature->GetInstanceScript())
        {
        }

        void IsSummonedBy(Unit* /*summoner*/) override
        {
            // Let Halion Controller count as summoner.
            if (Creature* controller = ObjectAccessor::GetCreature(*me, instance->GetData64(DATA_HALION_CONTROLLER)))
            {
                controller->AI()->JustSummoned(me);
                controller->AI()->SetData(DATA_LIVING_EMBER_SUMMONS, 0);
            }
        }

        void Reset() override { }

        // only attack people in 'real' realm
        bool CanAIAttack(Unit const* victim) const override
        {
            return !victim->HasAura(SPELL_TWILIGHT_REALM);
        }

    private:
        InstanceScript* instance;
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return GetRubySanctumAI<npc_living_infernoAI> (creature);
    }
};

class npc_living_ember : public CreatureScript
{
public:

    npc_living_ember() : CreatureScript("npc_living_ember")
    {
    }

    struct npc_living_emberAI : public ScriptedAI
    {

        npc_living_emberAI(Creature* creature) : ScriptedAI(creature), instance(creature->GetInstanceScript())
        {
            gaveaura = false;
        }

        void Reset() override
        {
            me->SetReactState(REACT_PASSIVE);
            me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_PC|UNIT_FLAG_NOT_SELECTABLE);
        }

        void JustDied(Unit* /*killer*/) override
        {
            if (Creature* controller = ObjectAccessor::GetCreature(*me, instance->GetData64(DATA_HALION_CONTROLLER)))
                controller->AI()->SetData(DATA_LIVING_EMBER_SUMMONS, controller->AI()->GetData(DATA_LIVING_EMBER_SUMMONS) - 1);
        }

        void EnterCombat(Unit* /*who*/) override
        {
            events.Reset();
            events.ScheduleEvent(EVENT_EMBER_ENRAGE, 120000);
            events.ScheduleEvent(EVENT_EMBER_CHECK_AURA, 3000);
        }

        // only attack people in 'real' realm

        bool CanAIAttack(Unit const* victim) const override
        {
            return !victim->HasAura(SPELL_TWILIGHT_REALM);
        }

        void MovementInform(uint32 type, uint32 id) override
        {
            switch (id)
            {
                case 1:
                    if (Player* nearestplayer = me->SelectNearestPlayer(90.0f))
                        me->AI()->AttackStart(nearestplayer);
                    break;
                default:
                    break;
            }
        }

        void UpdateAI(uint32 diff) override
        {
            UpdateVictim();

            events.Update(diff);

            while (uint32 eventId = events.ExecuteEvent())
            {
                switch (eventId)
                {
                    case EVENT_EMBER_ENRAGE:
                        DoCast(me, SPELL_BERSERK);
                        break;
                    case EVENT_EMBER_CHECK_AURA: // TODO: fix this properly
                        if (gaveaura)
                            break;

                        creaturesindist.clear();
                        me->GetCreatureListWithEntryInGrid(creaturesindist, NPC_LIVING_INFERNO, 15.0f);
                        for (std::list<Creature*>::iterator itr = creaturesindist.begin(); itr != creaturesindist.end(); ++itr)
                        {
                            if (*itr)
                            {
                                me->AddAura(SPELL_AWAKEN_FLAMES, (*itr));
                                gaveaura = true;
                            }
                        }

                        events.ScheduleEvent(EVENT_EMBER_CHECK_AURA, 3000);
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
        std::list<Creature*> creaturesindist;
        bool gaveaura;

    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return GetRubySanctumAI<npc_living_emberAI> (creature);
    }
};

class spell_halion_meteor_strike_marker : public SpellScriptLoader
{
public:

    spell_halion_meteor_strike_marker() : SpellScriptLoader("spell_halion_meteor_strike_marker")
    {
    }

    class spell_halion_meteor_strike_marker_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_halion_meteor_strike_marker_AuraScript);

        void OnRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            if (!GetCaster())
                return;

            if (GetTargetApplication()->GetRemoveMode() == AURA_REMOVE_BY_EXPIRE)
                if (Creature* creCaster = GetCaster()->ToCreature())
                    creCaster->AI()->DoAction(ACTION_METEOR_STRIKE_AOE);
        }

        void Register() override
        {
            AfterEffectRemove += AuraEffectRemoveFn(spell_halion_meteor_strike_marker_AuraScript::OnRemove, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new spell_halion_meteor_strike_marker_AuraScript();
    }
};

class spell_halion_fiery_combustion : public SpellScriptLoader
{
public:

    spell_halion_fiery_combustion() : SpellScriptLoader("spell_halion_fiery_combustion")
    {
    }

    class spell_halion_fiery_combustion_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_halion_fiery_combustion_AuraScript);

        bool Validate(SpellInfo const* /*spell*/) override
        {
            if (!sSpellMgr->GetSpellInfo(SPELL_MARK_OF_COMBUSTION))
                return false;
            return true;
        }

        void OnRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            if (GetTarget()->HasAura(SPELL_MARK_OF_COMBUSTION))
                GetTarget()->RemoveAurasDueToSpell(SPELL_MARK_OF_COMBUSTION, 0, 0, AURA_REMOVE_BY_EXPIRE);
        }

        void OnApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            GetTarget()->CastSpell(GetTarget(), SPELL_MARK_OF_COMBUSTION, true);
        }

        void AddMarkStack(AuraEffect const* /*aurEff*/)
        {
            GetTarget()->CastSpell(GetTarget(), SPELL_MARK_OF_COMBUSTION, true);
        }

        void Register() override
        {
            OnEffectPeriodic += AuraEffectPeriodicFn(spell_halion_fiery_combustion_AuraScript::AddMarkStack, EFFECT_0, SPELL_AURA_PERIODIC_DAMAGE);
            AfterEffectApply += AuraEffectApplyFn(spell_halion_fiery_combustion_AuraScript::OnApply, EFFECT_0, SPELL_AURA_PERIODIC_DAMAGE, AURA_EFFECT_HANDLE_REAL);
            AfterEffectRemove += AuraEffectRemoveFn(spell_halion_fiery_combustion_AuraScript::OnRemove, EFFECT_0, SPELL_AURA_PERIODIC_DAMAGE, AURA_EFFECT_HANDLE_REAL);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new spell_halion_fiery_combustion_AuraScript();
    }
};

class spell_halion_soul_consumption : public SpellScriptLoader
{
public:

    spell_halion_soul_consumption() : SpellScriptLoader("spell_halion_soul_consumption")
    {
    }

    class spell_halion_soul_consumption_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_halion_soul_consumption_AuraScript);

        bool Validate(SpellInfo const* /*spell*/) override
        {
            if (!sSpellMgr->GetSpellInfo(SPELL_MARK_OF_CONSUMPTION))
                return false;
            return true;
        }

        void OnRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            if (GetTarget()->HasAura(SPELL_MARK_OF_CONSUMPTION))
                GetTarget()->RemoveAurasDueToSpell(SPELL_MARK_OF_CONSUMPTION, 0, 0, AURA_REMOVE_BY_EXPIRE);
        }

        void OnApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            GetTarget()->CastSpell(GetTarget(), SPELL_MARK_OF_CONSUMPTION, true);
        }

        void AddMarkStack(AuraEffect const* /*aurEff*/)
        {
            GetTarget()->CastSpell(GetTarget(), SPELL_MARK_OF_CONSUMPTION, true);
        }

        void Register() override
        {
            OnEffectPeriodic += AuraEffectPeriodicFn(spell_halion_soul_consumption_AuraScript::AddMarkStack, EFFECT_0, SPELL_AURA_PERIODIC_DAMAGE);
            AfterEffectApply += AuraEffectApplyFn(spell_halion_soul_consumption_AuraScript::OnApply, EFFECT_0, SPELL_AURA_PERIODIC_DAMAGE, AURA_EFFECT_HANDLE_REAL);
            AfterEffectRemove += AuraEffectRemoveFn(spell_halion_soul_consumption_AuraScript::OnRemove, EFFECT_0, SPELL_AURA_PERIODIC_DAMAGE, AURA_EFFECT_HANDLE_REAL);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new spell_halion_soul_consumption_AuraScript();
    }
};

class spell_halion_mark_of_combustion : public SpellScriptLoader
{
public:

    spell_halion_mark_of_combustion() : SpellScriptLoader("spell_halion_mark_of_combustion")
    {
    }

    class spell_halion_mark_of_combustion_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_halion_mark_of_combustion_AuraScript);

        bool Validate(SpellInfo const* /*spell*/) override
        {
            if (!sSpellMgr->GetSpellInfo(SPELL_FIERY_COMBUSTION_SUMMON))
                return false;
            if (!sSpellMgr->GetSpellInfo(SPELL_FIERY_COMBUSTION_EXPLOSION))
                return false;
            return true;
        }

        void OnRemove(AuraEffect const* aurEff, AuraEffectHandleModes /*mode*/)
        {
            Unit* target = GetTarget();

            if (GetTargetApplication()->GetRemoveMode() != AURA_REMOVE_BY_EXPIRE)
                return;

            InstanceScript* instance = target->GetInstanceScript();
            if (!instance)
                return;

            uint8 stacks = aurEff->GetBase()->GetStackAmount();

            // Keep track of stacks when dispelling, there's only one effect in the spell.
            CustomSpellValues values;
            values.AddSpellMod(SPELLVALUE_BASE_POINT1, stacks);

            target->CastCustomSpell(SPELL_FIERY_COMBUSTION_SUMMON, values, target, TRIGGERED_FULL_MASK, NULL, NULL, GetCasterGUID());
        }

        void Register() override
        {
            AfterEffectRemove += AuraEffectRemoveFn(spell_halion_mark_of_combustion_AuraScript::OnRemove, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new spell_halion_mark_of_combustion_AuraScript();
    }
};


class spell_halion_mark_of_consumption : public SpellScriptLoader
{
public:

    spell_halion_mark_of_consumption() : SpellScriptLoader("spell_halion_mark_of_consumption")
    {
    }

    class spell_halion_mark_of_consumption_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_halion_mark_of_consumption_AuraScript);

        bool Validate(SpellInfo const* /*spell*/) override
        {
            if (!sSpellMgr->GetSpellInfo(SPELL_SOUL_CONSUMPTION_SUMMON))
                return false;
            if (!sSpellMgr->GetSpellInfo(SPELL_SOUL_CONSUMPTION_EXPLOSION))
                return false;
            return true;
        }

        void OnRemove(AuraEffect const* aurEff, AuraEffectHandleModes /*mode*/)
        {
            Unit* target = GetTarget();

            if (GetTargetApplication()->GetRemoveMode() != AURA_REMOVE_BY_EXPIRE)
                return;

            InstanceScript* instance = target->GetInstanceScript();
            if (!instance)
                return;

            uint8 stacks = aurEff->GetBase()->GetStackAmount();

            // Keep track of stacks when dispelling, there's only one effect in the spell.
            CustomSpellValues values;
            values.AddSpellMod(SPELLVALUE_BASE_POINT1, stacks);

            target->CastCustomSpell(SPELL_SOUL_CONSUMPTION_SUMMON, values, target, TRIGGERED_FULL_MASK, NULL, NULL, GetCasterGUID());
        }

        void Register() override
        {
            AfterEffectRemove += AuraEffectRemoveFn(spell_halion_mark_of_consumption_AuraScript::OnRemove, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new spell_halion_mark_of_consumption_AuraScript();
    }
};

class spell_halion_combustion_consumption_summon : public SpellScriptLoader
{
public:

    spell_halion_combustion_consumption_summon() : SpellScriptLoader("spell_halion_combustion_consumption_summon")
    {
    }

    class spell_halion_combustion_consumption_summon_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_halion_combustion_consumption_summon_SpellScript);

        void HandleSummon(SpellEffIndex effIndex)
        {
            PreventHitDefaultEffect(effIndex);
            Unit* caster = GetCaster();
            uint32 entry = uint32(GetSpellInfo()->Effects[effIndex].MiscValue);
            SummonPropertiesEntry const* properties = sSummonPropertiesStore.LookupEntry(uint32(GetSpellInfo()->Effects[effIndex].MiscValueB));
            uint32 duration = uint32(GetSpellInfo()->GetDuration());

            InstanceScript* instance = caster->GetInstanceScript();
            if (!instance)
                return;

            Position pos = caster->GetPosition();
            if (Creature* summon = caster->GetMap()->SummonCreature(entry, pos, properties, duration, caster, GetSpellInfo()->Id))
                if (summon->IsAIEnabled)
                    summon->AI()->SetData(DATA_STACKS_DISPELLED, GetSpellValue()->EffectBasePoints[EFFECT_1]);
        }

        void Register() override
        {
            OnEffectHit += SpellEffectFn(spell_halion_combustion_consumption_summon_SpellScript::HandleSummon, EFFECT_0, SPELL_EFFECT_SUMMON);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_halion_combustion_consumption_summon_SpellScript();
    }
};

class spell_halion_leave_twilight_realm : public SpellScriptLoader
{
public:

    spell_halion_leave_twilight_realm() : SpellScriptLoader("spell_halion_leave_twilight_realm")
    {
    }

    class spell_halion_leave_twilight_realm_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_halion_leave_twilight_realm_AuraScript);

        void OnRemove(AuraEffect const* aurEff, AuraEffectHandleModes /*handle*/)
        {
            GetTarget()->RemoveAurasDueToSpell(SPELL_TWILIGHT_REALM);
        }

        void Register() override
        {
            AfterEffectRemove += AuraEffectRemoveFn(spell_halion_leave_twilight_realm_AuraScript::OnRemove, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
        }
    };

    class spell_halion_leave_twilight_realm_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_halion_leave_twilight_realm_SpellScript);

        void HandleBeforeHit()
        {
            if (Player* player = GetHitPlayer())
                player->RemoveAurasDueToSpell(SPELL_SOUL_CONSUMPTION, 0, 0, AURA_REMOVE_BY_ENEMY_SPELL);
        }

        void FilterTargets(std::list<WorldObject*>& targets)
        {
            if (!targets.empty())
                targets.remove_if(Trinity::UnitAuraCheck(false, SPELL_TWILIGHT_REALM));
        }

        void Register() override
        {
            BeforeHit += SpellHitFn(spell_halion_leave_twilight_realm_SpellScript::HandleBeforeHit);
            OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_halion_leave_twilight_realm_SpellScript::FilterTargets, EFFECT_0, TARGET_UNIT_TARGET_ANY);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_halion_leave_twilight_realm_SpellScript();
    }

    AuraScript* GetAuraScript() const override
    {
        return new spell_halion_leave_twilight_realm_AuraScript();
    }
};

class spell_halion_enter_twilight_realm : public SpellScriptLoader
{
public:

    spell_halion_enter_twilight_realm() : SpellScriptLoader("spell_halion_enter_twilight_realm")
    {
    }

    class spell_halion_enter_twilight_realm_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_halion_enter_twilight_realm_SpellScript);

        void HandleBeforeHit()
        {
            if (Player* player = GetHitPlayer())
                player->RemoveAurasDueToSpell(SPELL_FIERY_COMBUSTION, 0, 0, AURA_REMOVE_BY_ENEMY_SPELL);
        }

        void Register() override
        {
            BeforeHit += SpellHitFn(spell_halion_enter_twilight_realm_SpellScript::HandleBeforeHit);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_halion_enter_twilight_realm_SpellScript();
    }
};

class spell_halion_twilight_phasing : public SpellScriptLoader
{
public:

    spell_halion_twilight_phasing() : SpellScriptLoader("spell_halion_twilight_phasing") { }

    class spell_halion_twilight_phasing_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_halion_twilight_phasing_SpellScript);

        void Phase()
        {
            Unit* caster = GetCaster();
            caster->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
            caster->CastSpell(HalionSpawnPos.GetPositionX(), HalionSpawnPos.GetPositionY(), HalionSpawnPos.GetPositionZ(), SPELL_SUMMON_TWILIGHT_PORTAL, true);
        }

        void Register() override
        {
            OnHit += SpellHitFn(spell_halion_twilight_phasing_SpellScript::Phase);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_halion_twilight_phasing_SpellScript();
    }
};

class spell_halion_clear_debuffs : public SpellScriptLoader
{
public:

    spell_halion_clear_debuffs() : SpellScriptLoader("spell_halion_clear_debuffs")
    {
    }

    class spell_halion_clear_debuffs_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_halion_clear_debuffs_SpellScript);

        bool Validate(SpellInfo const* /*spell*/) override
        {
            if (!sSpellMgr->GetSpellInfo(SPELL_CLEAR_DEBUFFS))
                return false;
            if (!sSpellMgr->GetSpellInfo(SPELL_TWILIGHT_REALM))
                return false;

            return true;
        }

        void HandleScript(SpellEffIndex effIndex)
        {
            if (GetHitUnit()->HasAura(GetSpellInfo()->Effects[effIndex].CalcValue()))
                GetHitUnit()->RemoveAurasDueToSpell(GetSpellInfo()->Effects[effIndex].CalcValue());
        }

        void Register() override
        {
            OnEffectHitTarget += SpellEffectFn(spell_halion_clear_debuffs_SpellScript::HandleScript, EFFECT_0, SPELL_EFFECT_SCRIPT_EFFECT);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_halion_clear_debuffs_SpellScript();
    }
};

class TwilightCutterSelector
{
public:

    TwilightCutterSelector(Unit* caster, Unit* cutterCaster) : _caster(caster), _cutterCaster(cutterCaster)
    {
    }

    bool operator()(WorldObject* unit)
    {
        return !unit->IsInBetween(_caster, _cutterCaster, 2.0f);
    }

private:
    Unit* _caster;
    Unit* _cutterCaster;
};

class spell_halion_twilight_cutter : public SpellScriptLoader
{
public:

    spell_halion_twilight_cutter() : SpellScriptLoader("spell_halion_twilight_cutter")
    {
    }

    class spell_halion_twilight_cutter_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_halion_twilight_cutter_SpellScript);

        void RemoveNotBetween(std::list<WorldObject*>& unitList)
        {
            if (unitList.empty())
                return;

            Unit* target = NULL;
            Unit* caster = GetCaster();
            if (Unit* _target = ObjectAccessor::GetCreature(*caster, caster->GetTarget()))
            {
                target = _target;
                uint32 TargetEntry = target->GetEntry();
                uint32 MyEntry = caster->GetEntry();
            }

            if (caster->GetTypeId() == TYPEID_UNIT && !target)
            {
                if (InstanceScript* instance = caster->GetInstanceScript())
                {
                    if (Creature* orbCarrier = ObjectAccessor::GetCreature(*caster, instance->GetData64(DATA_ORB_CARRIER)))
                    {
                        Vehicle* vehicle = orbCarrier->GetVehicleKit(); // cant be null

                        switch (caster->GetEntry())
                        {
                            case NPC_SHADOW_ORB_N:
                                target = vehicle->GetPassenger(SEAT_SOUTH);
                                break;
                            case NPC_SHADOW_ORB_S:
                                target = vehicle->GetPassenger(SEAT_NORTH);
                                break;
                            case NPC_SHADOW_ORB_E:
                                target = vehicle->GetPassenger(SEAT_WEST);
                                break;
                            case NPC_SHADOW_ORB_W:
                                target = vehicle->GetPassenger(SEAT_EAST);
                                break;
                        }
                    }
                }
            }

            if (target)
            {
                unitList.remove_if(TwilightCutterSelector(caster, target));
                return;
            }

            // In case cutter caster werent found for some reason
            unitList.clear();
        }

        void Register() override
        {
            OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_halion_twilight_cutter_SpellScript::RemoveNotBetween, EFFECT_0, TARGET_UNIT_SRC_AREA_ENEMY);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_halion_twilight_cutter_SpellScript();
    }
};

class spell_halion_track_rotation : public SpellScriptLoader
{
public:

    spell_halion_track_rotation() : SpellScriptLoader("spell_halion_track_rotation")
    {
    }

    class spell_halion_track_rotation_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_halion_track_rotation_AuraScript);

        void HandleEffectApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            Unit* caster = GetCaster();
            if (Unit* target = GetTarget())
            {
                caster->SetOrientation(caster->GetAngle(target));
                caster->SendMovementFlagUpdate();
            }
        }

        void Register() override
        {
            OnEffectApply += AuraEffectApplyFn(spell_halion_track_rotation_AuraScript::HandleEffectApply, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL_OR_REAPPLY_MASK);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new spell_halion_track_rotation_AuraScript();
    }
};

class spell_halion_twilight_mending : public SpellScriptLoader
{
    public:
        spell_halion_twilight_mending() : SpellScriptLoader("spell_halion_twilight_mending") { }

        class spell_halion_twilight_mending_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_halion_twilight_mending_SpellScript);

            void HandleDummy()
            {
                GetCaster()->ModifyHealth(int32(GetCaster()->GetMaxHealth() * 0.02));
            }

            void Register() override
            {
                OnCast += SpellCastFn(spell_halion_twilight_mending_SpellScript::HandleDummy);
            }
        };

        SpellScript* GetSpellScript() const override
        {
            return new spell_halion_twilight_mending_SpellScript();
        }
};

class go_exit_twilight_realm : public GameObjectScript
{
public:

    go_exit_twilight_realm() : GameObjectScript("go_exit_twilight_realm")
    {
    }

    bool OnGossipHello(Player* player, GameObject* /*go*/) override
    {
        player->CastSpell(player, SPELL_LEAVE_TWILIGHT_REALM, true);
        return true;
    }
};

class go_enter_twilight_realm : public GameObjectScript
{
public:

    go_enter_twilight_realm() : GameObjectScript("go_enter_twilight_realm")
    {
    }

    bool OnGossipHello(Player* player, GameObject* /*go*/) override
    {
        player->CastSpell(player, SPELL_TWILIGHT_REALM, true);
        return true;
    }
};

void AddSC_boss_halion()
{
    new boss_halion();
    new boss_twilight_halion();

    new npc_halion_controller();
    new npc_meteor_strike_initial();
    new npc_meteor_strike();
    new npc_meteor_strike_flame();
    new npc_combustion_consumption();
    new npc_orb_carrier();
    new npc_living_inferno();
    new npc_living_ember();
    new npc_combat_stalker();

    new spell_halion_meteor_strike_marker();
    new spell_halion_combustion_consumption_summon();
    new spell_halion_mark_of_combustion();
    new spell_halion_mark_of_consumption();
    new spell_halion_fiery_combustion();
    new spell_halion_soul_consumption();
    new spell_halion_leave_twilight_realm();
    new spell_halion_enter_twilight_realm();
    new spell_halion_twilight_phasing();
    new spell_halion_twilight_cutter();
    new spell_halion_clear_debuffs();
    new spell_halion_track_rotation();
    new spell_halion_twilight_mending();

    new go_exit_twilight_realm();
    new go_enter_twilight_realm();
}

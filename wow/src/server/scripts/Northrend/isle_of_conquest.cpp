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
#include "PassiveAI.h"
#include "BattlegroundIC.h"
#include "Player.h"

// TO-DO: This should be done with SmartAI, but yet it does not correctly support vehicles's AIs.
//        Even adding ReactState Passive we still have issues using SmartAI.

class npc_four_car_garage : public CreatureScript
{
    public:
        npc_four_car_garage() : CreatureScript("npc_four_car_garage") { }

        struct npc_four_car_garageAI : public NullCreatureAI
        {
            npc_four_car_garageAI(Creature* creature) : NullCreatureAI(creature) { }

            void PassengerBoarded(Unit* who, int8 /*seatId*/, bool apply) override
            {
                if (apply)
                {
                    uint32 spellId = 0;

                    switch (me->GetEntry())
                    {
                        case NPC_DEMOLISHER:
                            spellId = SPELL_DRIVING_CREDIT_DEMOLISHER;
                            break;
                        case NPC_GLAIVE_THROWER_A:
                        case NPC_GLAIVE_THROWER_H:
                            spellId = SPELL_DRIVING_CREDIT_GLAIVE;
                            break;
                        case NPC_SIEGE_ENGINE_H:
                        case NPC_SIEGE_ENGINE_A:
                            spellId = SPELL_DRIVING_CREDIT_SIEGE;
                            break;
                        case NPC_CATAPULT:
                            spellId = SPELL_DRIVING_CREDIT_CATAPULT;
                            break;
                        default:
                            return;
                    }

                    me->CastSpell(who, spellId, true);
                }

                me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_PC | UNIT_FLAG_NON_ATTACKABLE);
            }
        };

        CreatureAI* GetAI(Creature* creature) const override
        {
            return new npc_four_car_garageAI(creature);
        }
};

/*#############################
## Bosses Isle Of Conquest (A/H)
##############################*/

enum SpellsBosses
{
  SPELL_DAGGER_THROW_1    = 67280,
  SPELL_DAGGER_THROW_2    = 67881,
  SPELL_CRUSHING_LEAP     = 60895,
  SPELL_MORTAL_STRIKE     = 39171,
  SPELL_RAGE              = 66776 
};

enum events
{
    EVENT_MORTAL_STRIKE = 1,
    EVENT_DAGGER_THROW,
    EVENT_CRUSHING_LEAP,
    EVENT_ENRAGE,
};

class bosses_isle_of_conquest : public CreatureScript
{
    public:
        bosses_isle_of_conquest() : CreatureScript("bosses_isle_of_conquest") { }

        struct bosses_isle_of_conquestAI : public ScriptedAI
        {
            bosses_isle_of_conquestAI(Creature* creature) : ScriptedAI(creature) { }

            void Reset() override
            {
                events.Reset();
                events.ScheduleEvent(EVENT_MORTAL_STRIKE, 8000);
                events.ScheduleEvent(EVENT_DAGGER_THROW, 2000);
                events.ScheduleEvent(EVENT_CRUSHING_LEAP, 6000);
                events.ScheduleEvent(EVENT_ENRAGE, 1000);
            }

            void EnterCombat(Unit * who) override
            {
                if (!me->IsWithinLOSInMap(who))
                {
                    EnterEvadeMode();
                }
            }

            void JustRespawned() override
            {
               Reset();
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
                        case EVENT_MORTAL_STRIKE:
                            DoCastVictim(SPELL_MORTAL_STRIKE);
                            events.ScheduleEvent(EVENT_MORTAL_STRIKE, urand(10*IN_MILLISECONDS,20*IN_MILLISECONDS));
                            break;
                        case EVENT_DAGGER_THROW:
                            if (Unit* pTarget = SelectTarget(SELECT_TARGET_RANDOM))
                                DoCast(pTarget, RAID_MODE(SPELL_DAGGER_THROW_1, SPELL_DAGGER_THROW_2));

                            events.ScheduleEvent(EVENT_DAGGER_THROW, urand(7*IN_MILLISECONDS,12*IN_MILLISECONDS));
                            break;
                        case EVENT_CRUSHING_LEAP:
                            if (Unit* pTarget = SelectTarget(SELECT_TARGET_RANDOM))
                                DoCast(pTarget, SPELL_CRUSHING_LEAP);

                            events.ScheduleEvent(EVENT_CRUSHING_LEAP, urand(12*IN_MILLISECONDS,16*IN_MILLISECONDS));
                            break;
                        case EVENT_ENRAGE:
                            if (me->GetDistance2d(me->GetHomePosition().GetPositionX(), me->GetHomePosition().GetPositionY()) > 50)
                                DoCastVictim(SPELL_RAGE);
                            events.ScheduleEvent(EVENT_ENRAGE, 1000);
                            break;
                    }
                }

                DoMeleeAttackIfReady();
            }

        private:
            EventMap events;
        };

        CreatureAI* GetAI(Creature* creature) const override
        {
            return new bosses_isle_of_conquestAI(creature);
        }
};

void AddSC_isle_of_conquest()
{
    new npc_four_car_garage();
    new bosses_isle_of_conquest();
}

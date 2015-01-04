/*
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
#include "AchievementMgr.h"

enum TheTurkinator
{
    SPELL_KILL_COUNTER_VISUAL       = 62015,
    SPELL_KILL_COUNTER_VISUAL_MAX   = 62021,
};

class spell_gen_turkey_tracker : public SpellScriptLoader
{
    public:
        spell_gen_turkey_tracker() : SpellScriptLoader("spell_gen_turkey_tracker") { }

        class spell_gen_turkey_tracker_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_gen_turkey_tracker_SpellScript);

            bool Validate(SpellInfo const* /*spellEntry*/) override
            {
                if(!sSpellStore.LookupEntry(SPELL_KILL_COUNTER_VISUAL))
                    return false;
                if(!sSpellStore.LookupEntry(SPELL_KILL_COUNTER_VISUAL_MAX))
                    return false;
                return true;
            }

            void HandleScript(SpellEffIndex /*effIndex*/)
            {
                if(GetCaster()->GetAura(SPELL_KILL_COUNTER_VISUAL_MAX))
                    return;

                Player* target = GetHitPlayer();
                if(!target)
                    return;

                if(Aura const* aura = GetCaster()->ToPlayer()->GetAura(GetSpellInfo()->Id))
                {
                    switch(aura->GetStackAmount())
                    {
                        case 10:
                            target->MonsterTextEmote("Turkey Hunter!", 0, true);
                            GetCaster()->CastSpell(target, SPELL_KILL_COUNTER_VISUAL, true, NULL);
                            break;
                        case 20:
                            target->MonsterTextEmote("Turkey Domination!", 0, true);
                            GetCaster()->CastSpell(target, SPELL_KILL_COUNTER_VISUAL, true, NULL);
                            break;
                        case 30:
                            target->MonsterTextEmote("Turkey Slaughter!", 0, true);
                            GetCaster()->CastSpell(target, SPELL_KILL_COUNTER_VISUAL, true, NULL);
                            break;
                        case 40:
                            target->MonsterTextEmote("TURKEY TRIUMPH!", 0, true);
                            GetCaster()->CastSpell(target, SPELL_KILL_COUNTER_VISUAL, true, NULL);
                            GetCaster()->CastSpell(target, SPELL_KILL_COUNTER_VISUAL_MAX, true, NULL); // Achievement Credit
                            break;
                        default:
                            break;
                    }
                }
            }

            void Register()
            {
				OnEffectHitTarget += SpellEffectFn(spell_gen_turkey_tracker_SpellScript::HandleScript, EFFECT_1, SPELL_EFFECT_SCRIPT_EFFECT);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_gen_turkey_tracker_SpellScript();
        }
};

class spell_gen_feast_on : public SpellScriptLoader
{
public:
	spell_gen_feast_on() : SpellScriptLoader("spell_gen_feast_on") { }

	class spell_gen_feast_on_SpellScript : public SpellScript
	{
		PrepareSpellScript(spell_gen_feast_on_SpellScript);

		void HandleDummy(SpellEffIndex /*effIndex*/)
		{
			int32 bp0 = GetSpellInfo()->Effects[EFFECT_0].CalcValue();

			Unit* caster = GetCaster();
			if (caster->GetVehicleKit())
			if (Unit* player = caster->GetVehicleKit()->GetPassenger(0))
				caster->CastSpell(player, bp0, true, NULL, NULL, player->ToPlayer()->GetGUID());
		}

		void Register()
		{
			OnEffectHitTarget += SpellEffectFn(spell_gen_feast_on_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
		}
	};

	SpellScript* GetSpellScript() const
	{
		return new spell_gen_feast_on_SpellScript();
	}
};

enum WellFedPilgrimsBount
{
    // Feast On
    SPELL_A_SERVING_OF_TURKEY           = 61807,
    SPELL_A_SERVING_OF_CRANBERRIES      = 61804,
    SPELL_A_SERVING_OF_STUFFING         = 61806,
    SPELL_A_SERVING_OF_SWEET_POTATOES   = 61808,
    SPELL_A_SERVING_OF_PIE              = 61805,

    // Well Fed
    SPELL_WELL_FED_AP                   = 65414,
    SPELL_WELL_FED_ZM                   = 65412,
    SPELL_WELL_FED_HIT                  = 65416,
    SPELL_WELL_FED_HASTE                = 65410,
    SPELL_WELL_FED_SPIRIT               = 65415,

    // Pilgrim's Paunch
    SPELL_THE_SPIRIT_OF_SHARING         = 61849,
};

class spell_gen_well_fed_pilgrims_bount_ap : public SpellScriptLoader
{
    public:
        spell_gen_well_fed_pilgrims_bount_ap() : SpellScriptLoader("spell_gen_well_fed_pilgrims_bount_ap") { }

        class spell_gen_well_fed_pilgrims_bount_ap_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_gen_well_fed_pilgrims_bount_ap_SpellScript);

            bool Validate(SpellInfo const* /*spellEntry*/) override
            {
                if(!sSpellStore.LookupEntry(SPELL_WELL_FED_AP))
                    return false;
                return true;
            }

            void HandleScript(SpellEffIndex effIndex)
            {
                PreventHitDefaultEffect(effIndex);
                Player* target = GetHitPlayer();
                if(!target)
                    return;

                Aura const* Turkey          = target->GetAura(SPELL_A_SERVING_OF_TURKEY);
                Aura const* Cranberies      = target->GetAura(SPELL_A_SERVING_OF_CRANBERRIES);
                Aura const* Stuffing        = target->GetAura(SPELL_A_SERVING_OF_STUFFING);
                Aura const* SweetPotatoes   = target->GetAura(SPELL_A_SERVING_OF_SWEET_POTATOES);
                Aura const* Pie             = target->GetAura(SPELL_A_SERVING_OF_PIE);

                // A Serving of Turkey - Attack power and Stamina
                if(Aura const* aura = target->GetAura(SPELL_A_SERVING_OF_TURKEY))
                {
                    if(aura->GetStackAmount() == 5)
                        target->CastSpell(target, SPELL_WELL_FED_AP, true);
                }

                // The Spirit of Sharing - Achievement Credit
                if(!target->GetAura(SPELL_THE_SPIRIT_OF_SHARING))
                {
                    if((Turkey && Turkey->GetStackAmount() == 5) && (Cranberies && Cranberies->GetStackAmount() == 5) && (Stuffing && Stuffing->GetStackAmount() == 5) &&
                        (SweetPotatoes && SweetPotatoes->GetStackAmount() == 5) && (Pie && Pie->GetStackAmount() == 5))
                        target->CastSpell(target, SPELL_THE_SPIRIT_OF_SHARING, true);
                }
            }

            void Register()
            {
				OnEffectHitTarget += SpellEffectFn(spell_gen_well_fed_pilgrims_bount_ap_SpellScript::HandleScript, EFFECT_0, SPELL_EFFECT_APPLY_AURA);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_gen_well_fed_pilgrims_bount_ap_SpellScript();
        }
};

class spell_gen_well_fed_pilgrims_bount_zm : public SpellScriptLoader
{
    public:
        spell_gen_well_fed_pilgrims_bount_zm() : SpellScriptLoader("spell_gen_well_fed_pilgrims_bount_zm") { }

        class spell_gen_well_fed_pilgrims_bount_zm_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_gen_well_fed_pilgrims_bount_zm_SpellScript);

            bool Validate(SpellInfo const* /*spellEntry*/) override
            {
                if(!sSpellStore.LookupEntry(SPELL_WELL_FED_ZM))
                    return false;
                return true;
            }

            void HandleScript(SpellEffIndex effIndex)
            {
                PreventHitDefaultEffect(effIndex);
                Player* target = GetHitPlayer();
                if(!target)
                    return;

                Aura const* Turkey          = target->GetAura(SPELL_A_SERVING_OF_TURKEY);
                Aura const* Cranberies      = target->GetAura(SPELL_A_SERVING_OF_CRANBERRIES);
                Aura const* Stuffing        = target->GetAura(SPELL_A_SERVING_OF_STUFFING);
                Aura const* SweetPotatoes   = target->GetAura(SPELL_A_SERVING_OF_SWEET_POTATOES);
                Aura const* Pie             = target->GetAura(SPELL_A_SERVING_OF_PIE);

                // A Serving of Cranberries - Spell power and Stamina
                if(Aura const* aura = target->GetAura(SPELL_A_SERVING_OF_CRANBERRIES))
                {
                    if(aura->GetStackAmount() == 5)
                        target->CastSpell(target, SPELL_WELL_FED_ZM, true);
                }

                // The Spirit of Sharing - Achievement Credit
                if(!target->GetAura(SPELL_THE_SPIRIT_OF_SHARING))
                {
                    if((Turkey && Turkey->GetStackAmount() == 5) && (Cranberies && Cranberies->GetStackAmount() == 5) && (Stuffing && Stuffing->GetStackAmount() == 5) &&
                        (SweetPotatoes && SweetPotatoes->GetStackAmount() == 5) && (Pie && Pie->GetStackAmount() == 5))
                        target->CastSpell(target, SPELL_THE_SPIRIT_OF_SHARING, true);
                }
            }

            void Register()
            {
				OnEffectHitTarget += SpellEffectFn(spell_gen_well_fed_pilgrims_bount_zm_SpellScript::HandleScript, EFFECT_0, SPELL_EFFECT_APPLY_AURA);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_gen_well_fed_pilgrims_bount_zm_SpellScript();
        }
};

class spell_gen_well_fed_pilgrims_bount_hit : public SpellScriptLoader
{
    public:
        spell_gen_well_fed_pilgrims_bount_hit() : SpellScriptLoader("spell_gen_well_fed_pilgrims_bount_hit") { }

        class spell_gen_well_fed_pilgrims_bount_hit_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_gen_well_fed_pilgrims_bount_hit_SpellScript);

            bool Validate(SpellInfo const* /*spellEntry*/) override
            {
                if(!sSpellStore.LookupEntry(SPELL_WELL_FED_HIT))
                    return false;
                return true;
            }

            void HandleScript(SpellEffIndex effIndex)
            {
                PreventHitDefaultEffect(effIndex);
                Player* target = GetHitPlayer();
                if(!target)
                    return;

                Aura const* Turkey          = target->GetAura(SPELL_A_SERVING_OF_TURKEY);
                Aura const* Cranberies      = target->GetAura(SPELL_A_SERVING_OF_CRANBERRIES);
                Aura const* Stuffing        = target->GetAura(SPELL_A_SERVING_OF_STUFFING);
                Aura const* SweetPotatoes   = target->GetAura(SPELL_A_SERVING_OF_SWEET_POTATOES);
                Aura const* Pie             = target->GetAura(SPELL_A_SERVING_OF_PIE);

                // A Serving of Stuffing - Hit rating and Stamina
                if(Aura const* aura = target->GetAura(SPELL_A_SERVING_OF_STUFFING))
                {
                    if(aura->GetStackAmount() == 5)
                        target->CastSpell(target, SPELL_WELL_FED_HIT, true);
                }

                // The Spirit of Sharing - Achievement Credit
                if(!target->GetAura(SPELL_THE_SPIRIT_OF_SHARING))
                {
                    if((Turkey && Turkey->GetStackAmount() == 5) && (Cranberies && Cranberies->GetStackAmount() == 5) && (Stuffing && Stuffing->GetStackAmount() == 5) &&
                        (SweetPotatoes && SweetPotatoes->GetStackAmount() == 5) && (Pie && Pie->GetStackAmount() == 5))
                        target->CastSpell(target, SPELL_THE_SPIRIT_OF_SHARING, true);
                }
            }

            void Register()
            {
				OnEffectHitTarget += SpellEffectFn(spell_gen_well_fed_pilgrims_bount_hit_SpellScript::HandleScript, EFFECT_0, SPELL_EFFECT_APPLY_AURA);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_gen_well_fed_pilgrims_bount_hit_SpellScript();
        }
};

class spell_gen_well_fed_pilgrims_bount_haste : public SpellScriptLoader
{
    public:
        spell_gen_well_fed_pilgrims_bount_haste() : SpellScriptLoader("spell_gen_well_fed_pilgrims_bount_haste") { }

        class spell_gen_well_fed_pilgrims_bount_haste_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_gen_well_fed_pilgrims_bount_haste_SpellScript);

            bool Validate(SpellInfo const* /*spellEntry*/) override
            {
                if(!sSpellStore.LookupEntry(SPELL_WELL_FED_HASTE))
                    return false;
                return true;
            }

            void HandleScript(SpellEffIndex effIndex)
            {
                PreventHitDefaultEffect(effIndex);
                Player* target = GetHitPlayer();
                if(!target)
                    return;

                Aura const* Turkey          = target->GetAura(SPELL_A_SERVING_OF_TURKEY);
                Aura const* Cranberies      = target->GetAura(SPELL_A_SERVING_OF_CRANBERRIES);
                Aura const* Stuffing        = target->GetAura(SPELL_A_SERVING_OF_STUFFING);
                Aura const* SweetPotatoes   = target->GetAura(SPELL_A_SERVING_OF_SWEET_POTATOES);
                Aura const* Pie             = target->GetAura(SPELL_A_SERVING_OF_PIE);

                // A Serving of Sweet Potatoes - Haste rating and Stamina
                if(Aura const* aura = target->GetAura(SPELL_A_SERVING_OF_SWEET_POTATOES))
                {
                    if(aura->GetStackAmount() == 5)
                        target->CastSpell(target, SPELL_WELL_FED_HASTE, true);
                }

                // The Spirit of Sharing - Achievement Credit
                if(!target->GetAura(SPELL_THE_SPIRIT_OF_SHARING))
                {
                    if((Turkey && Turkey->GetStackAmount() == 5) && (Cranberies && Cranberies->GetStackAmount() == 5) && (Stuffing && Stuffing->GetStackAmount() == 5) &&
                        (SweetPotatoes && SweetPotatoes->GetStackAmount() == 5) && (Pie && Pie->GetStackAmount() == 5))
                        target->CastSpell(target, SPELL_THE_SPIRIT_OF_SHARING, true);
                }
            }

            void Register()
            {
				OnEffectHitTarget += SpellEffectFn(spell_gen_well_fed_pilgrims_bount_haste_SpellScript::HandleScript, EFFECT_0, SPELL_EFFECT_APPLY_AURA);
            }

        };

        SpellScript* GetSpellScript() const
        {
            return new spell_gen_well_fed_pilgrims_bount_haste_SpellScript();
        }
};

class spell_gen_well_fed_pilgrims_bount_spirit : public SpellScriptLoader
{
    public:
        spell_gen_well_fed_pilgrims_bount_spirit() : SpellScriptLoader("spell_gen_well_fed_pilgrims_bount_spirit") { }

        class spell_gen_well_fed_pilgrims_bount_spirit_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_gen_well_fed_pilgrims_bount_spirit_SpellScript);

            bool Validate(SpellInfo const* /*spellEntry*/) override
            {
                if(!sSpellStore.LookupEntry(SPELL_WELL_FED_SPIRIT))
                    return false;
                return true;
            }

            void HandleScript(SpellEffIndex effIndex)
            {
                PreventHitDefaultEffect(effIndex);
                Player* target = GetHitPlayer();
                if(!target)
                    return;

                Aura const* Turkey          = target->GetAura(SPELL_A_SERVING_OF_TURKEY);
                Aura const* Cranberies      = target->GetAura(SPELL_A_SERVING_OF_CRANBERRIES);
                Aura const* Stuffing        = target->GetAura(SPELL_A_SERVING_OF_STUFFING);
                Aura const* SweetPotatoes   = target->GetAura(SPELL_A_SERVING_OF_SWEET_POTATOES);
                Aura const* Pie             = target->GetAura(SPELL_A_SERVING_OF_PIE);

                // Feast On Pie - Spirit and Stamina
                if(Aura const* aura = target->GetAura(SPELL_A_SERVING_OF_PIE))
                {
                    if(aura->GetStackAmount() == 5)
                        target->CastSpell(target, SPELL_WELL_FED_SPIRIT, true);
                }

                // The Spirit of Sharing - Achievement Credit
                if(!target->GetAura(SPELL_THE_SPIRIT_OF_SHARING))
                {
                    if((Turkey && Turkey->GetStackAmount() == 5) && (Cranberies && Cranberies->GetStackAmount() == 5) && (Stuffing && Stuffing->GetStackAmount() == 5) &&
                        (SweetPotatoes && SweetPotatoes->GetStackAmount() == 5) && (Pie && Pie->GetStackAmount() == 5))
                        target->CastSpell(target, SPELL_THE_SPIRIT_OF_SHARING, true);
                }
            }

            void Register()
            {
				OnEffectHitTarget += SpellEffectFn(spell_gen_well_fed_pilgrims_bount_spirit_SpellScript::HandleScript, EFFECT_0, SPELL_EFFECT_APPLY_AURA);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_gen_well_fed_pilgrims_bount_spirit_SpellScript();
        }
};

enum OnPlatePilgrimsBount
{
    // "FOOD FIGHT!" - Achivement Credit
    SPELL_ON_PLATE_TURKEY           = 61928,
    SPELL_ON_PLATE_CRANBERRIES      = 61925,
    SPELL_ON_PLATE_STUFFING         = 61927,
    SPELL_ON_PLATE_SWEET_POTATOES   = 61929,
    SPELL_ON_PLATE_PIE              = 61926,

    // Sharing is Caring - Achivement Credit
    SPELL_PASS_THE_TURKEY           = 66373,
    SPELL_PASS_THE_CRANBERRIES      = 66372,
    SPELL_PASS_THE_STUFFING         = 66375,
    SPELL_PASS_THE_SWEET_POTATOES   = 66376,
    SPELL_PASS_THE_PIE              = 66374,
};

class spell_gen_on_plate_pilgrims_bount_turkey : public SpellScriptLoader
{
    public:
        spell_gen_on_plate_pilgrims_bount_turkey() : SpellScriptLoader("spell_gen_on_plate_pilgrims_bount_turkey") { }

        class spell_gen_on_plate_pilgrims_bount_turkey_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_gen_on_plate_pilgrims_bount_turkey_SpellScript);

            bool Validate(SpellInfo const* /*spellEntry*/) override
            {
                if(!sSpellStore.LookupEntry(SPELL_ON_PLATE_TURKEY))
                    return false;
                return true;
            }

            void HandleDummy(SpellEffIndex /*effIndex*/)
            {
                Unit* pCaster = GetCaster();
                if(pCaster && pCaster->IsVehicle())
                {
                    Unit* pPlayer = pCaster->GetVehicleKit()->GetPassenger(0);
                    if(!pPlayer)
                        return;

                    pPlayer->CastSpell(GetHitUnit(), SPELL_ON_PLATE_TURKEY, true, NULL, NULL, pPlayer->ToPlayer()->GetGUID());
                    pPlayer->CastSpell(pPlayer, SPELL_PASS_THE_TURKEY, true);
                }
            }

            void Register()
            {
				OnEffectHitTarget += SpellEffectFn(spell_gen_on_plate_pilgrims_bount_turkey_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_gen_on_plate_pilgrims_bount_turkey_SpellScript();
        }
};

class spell_gen_on_plate_pilgrims_bount_cranberries : public SpellScriptLoader
{
    public:
        spell_gen_on_plate_pilgrims_bount_cranberries() : SpellScriptLoader("spell_gen_on_plate_pilgrims_bount_cranberries") { }

        class spell_gen_on_plate_pilgrims_bount_cranberries_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_gen_on_plate_pilgrims_bount_cranberries_SpellScript);

            bool Validate(SpellInfo const* /*spellEntry*/) override
            {
                if(!sSpellStore.LookupEntry(SPELL_ON_PLATE_CRANBERRIES))
                    return false;
                return true;
            }

            void HandleDummy(SpellEffIndex /*effIndex*/)
            {
                Unit* pCaster = GetCaster();
                if(pCaster && pCaster->IsVehicle())
                {
                    Unit* pPlayer = pCaster->GetVehicleKit()->GetPassenger(0);
                    if(!pPlayer)
                        return;

                    pPlayer->CastSpell(GetHitUnit(), SPELL_ON_PLATE_CRANBERRIES, true, NULL, NULL, pPlayer->ToPlayer()->GetGUID());
                    pPlayer->CastSpell(pPlayer, SPELL_PASS_THE_CRANBERRIES, true);
                }
            }

            void Register()
            {
				OnEffectHitTarget += SpellEffectFn(spell_gen_on_plate_pilgrims_bount_cranberries_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_gen_on_plate_pilgrims_bount_cranberries_SpellScript();
        }
};

class spell_gen_on_plate_pilgrims_bount_stuffing : public SpellScriptLoader
{
    public:
        spell_gen_on_plate_pilgrims_bount_stuffing() : SpellScriptLoader("spell_gen_on_plate_pilgrims_bount_stuffing") { }

        class spell_gen_on_plate_pilgrims_bount_stuffing_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_gen_on_plate_pilgrims_bount_stuffing_SpellScript);

            bool Validate(SpellInfo const* /*spellEntry*/) override
            {
                if(!sSpellStore.LookupEntry(SPELL_ON_PLATE_STUFFING))
                    return false;
                return true;
            }

            void HandleDummy(SpellEffIndex /*effIndex*/)
            {
                Unit* pCaster = GetCaster();
                if(pCaster && pCaster->IsVehicle())
                {
                    Unit* pPlayer = pCaster->GetVehicleKit()->GetPassenger(0);
                    if(!pPlayer)
                        return;

                    pPlayer->CastSpell(GetHitUnit(), SPELL_ON_PLATE_STUFFING, true, NULL, NULL, pPlayer->ToPlayer()->GetGUID());
                    pPlayer->CastSpell(pPlayer, SPELL_PASS_THE_STUFFING, true);
                }
            }

            void Register()
            {
				OnEffectHitTarget += SpellEffectFn(spell_gen_on_plate_pilgrims_bount_stuffing_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_gen_on_plate_pilgrims_bount_stuffing_SpellScript();
        }
};

class spell_gen_on_plate_pilgrims_bount_sweet_potatoes : public SpellScriptLoader
{
    public:
        spell_gen_on_plate_pilgrims_bount_sweet_potatoes() : SpellScriptLoader("spell_gen_on_plate_pilgrims_bount_sweet_potatoes") { }

        class spell_gen_on_plate_pilgrims_bount_sweet_potatoes_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_gen_on_plate_pilgrims_bount_sweet_potatoes_SpellScript);

            bool Validate(SpellInfo const* /*spellEntry*/) override
            {
                if(!sSpellStore.LookupEntry(SPELL_ON_PLATE_SWEET_POTATOES))
                    return false;
                return true;
            }

            void HandleDummy(SpellEffIndex /*effIndex*/)
            {
                Unit* pCaster = GetCaster();
                if(pCaster && pCaster->IsVehicle())
                {
                    Unit* pPlayer = pCaster->GetVehicleKit()->GetPassenger(0);
                    if(!pPlayer)
                        return;

                    pPlayer->CastSpell(GetHitUnit(), SPELL_ON_PLATE_SWEET_POTATOES, true, NULL, NULL, pPlayer->ToPlayer()->GetGUID());
                    pPlayer->CastSpell(pPlayer, SPELL_PASS_THE_SWEET_POTATOES, true);
                }
            }

            void Register()
            {
				OnEffectHitTarget += SpellEffectFn(spell_gen_on_plate_pilgrims_bount_sweet_potatoes_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_gen_on_plate_pilgrims_bount_sweet_potatoes_SpellScript();
        }
};

class spell_gen_on_plate_pilgrims_bount_pie : public SpellScriptLoader
{
    public:
        spell_gen_on_plate_pilgrims_bount_pie() : SpellScriptLoader("spell_gen_on_plate_pilgrims_bount_pie") { }

        class spell_gen_on_plate_pilgrims_bount_pie_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_gen_on_plate_pilgrims_bount_pie_SpellScript);

            bool Validate(SpellInfo const* /*spellEntry*/) override
            {
                if(!sSpellStore.LookupEntry(SPELL_ON_PLATE_PIE))
                    return false;
                return true;
            }

            void HandleDummy(SpellEffIndex /*effIndex*/)
            {
                Unit* pCaster = GetCaster();
                if(pCaster && pCaster->IsVehicle())
                {
                    Unit* pPlayer = pCaster->GetVehicleKit()->GetPassenger(0);
                    if(!pPlayer)
                        return;

                    pPlayer->CastSpell(GetHitUnit(), SPELL_ON_PLATE_PIE, true, NULL, NULL, pPlayer->ToPlayer()->GetGUID());
                    pPlayer->CastSpell(pPlayer, SPELL_PASS_THE_PIE, true);
                }
            }

            void Register()
            {
				OnEffectHitTarget += SpellEffectFn(spell_gen_on_plate_pilgrims_bount_pie_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_gen_on_plate_pilgrims_bount_pie_SpellScript();
        }
};

enum WildTurkey
{
    SPELL_TURKEY_TRACKER = 62014,
};

class npc_wild_turkey : public CreatureScript
{
public:
    npc_wild_turkey() : CreatureScript("npc_wild_turkey") { }

    struct npc_wild_turkeyAI : public ScriptedAI
    {
        npc_wild_turkeyAI(Creature* creature) : ScriptedAI(creature) { }

        void JustDied(Unit* killer)
        {
            if(!killer)
                return;

            if(Player* pPlayer = killer->ToPlayer())
                pPlayer->CastSpell(pPlayer, SPELL_TURKEY_TRACKER, false);
        }
    };

    CreatureAI* GetAI(Creature* creature) const
    {
        return new npc_wild_turkeyAI(creature);
    }
};

// HACKFIX FOR "FOOD FIGHT!"
class hackfix_the_food_fight : public PlayerScript
{
public:
	hackfix_the_food_fight() : PlayerScript("hackfix_the_food_fight") { }

	// i'm a noob so i did it this way, dunno if ok..
	void OnSpellCast(Player* player, Spell* spell, bool skipCheck)
	{
		int32 achievementId = 3579;
		if (player->HasAura(65403))
		if (IsHolidayActive(HOLIDAY_PILGRIMS_BOUNTY))
		{
			AchievementEntry const* achievementEntry = sAchievementMgr->GetAchievement(achievementId);
			if (AchievementEntry const* achievementEntry = sAchievementMgr->GetAchievement(achievementId))
				player->CompletedAchievement(achievementEntry);
		}
	}
};


void AddSC_emo_pilgrims_event()
{
//    new achievement_food_fight();
    new spell_gen_turkey_tracker();
    new spell_gen_feast_on();
    new spell_gen_well_fed_pilgrims_bount_ap();
    new spell_gen_well_fed_pilgrims_bount_zm();
    new spell_gen_well_fed_pilgrims_bount_hit();
    new spell_gen_well_fed_pilgrims_bount_haste();
    new spell_gen_well_fed_pilgrims_bount_spirit();
    new spell_gen_on_plate_pilgrims_bount_turkey();
    new spell_gen_on_plate_pilgrims_bount_cranberries();
    new spell_gen_on_plate_pilgrims_bount_stuffing();
    new spell_gen_on_plate_pilgrims_bount_sweet_potatoes();
    new spell_gen_on_plate_pilgrims_bount_pie();
    new npc_wild_turkey();
	new hackfix_the_food_fight();
}
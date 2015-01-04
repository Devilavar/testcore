#include "Battleground.h"
#include "Chat.h"
#include "Player.h"
#include "Pet.h"

enum professionmenu
{
    OPTION_TAILORING,
    OPTION_JEWELCRAFTING,
    OPTION_ENGINEERING,
    OPTION_BLACKSMITHING,
    OPTION_ALCHEMY,
    OPTION_INSCRIPTION,
    OPTION_ENCHANTING,
    OPTION_LEATHERWORKING,
    OPTION_COOKING,
    OPTION_FIRST_AID,
    OPTION_MAIN_MENU,

    SORT_TAILORING = 0,
    SORT_JEWELCRAFTING,
    SORT_ENGINEERING,
    SORT_BLACKSMITHING,
    SORT_ALCHEMY,
    SORT_INSCRIPTION,
    SORT_ENCHANTING,
    SORT_LEATHERWORKING,
    SORT_COOKING,
    SORT_FIRST_AID,

    TEXT_PROFESSION_MAIN = 50020,
    ACTION_LEARN_PROFESSION,
    ACTION_PURCHASE_MATERIALS,
};

class npc_professions : public CreatureScript
{
public:
    npc_professions() : CreatureScript("npc_professions") { } // to alliance from horde

    bool OnGossipHello(Player* player, Creature* creature) override
    {
        player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TABARD,"Tailoring", GOSSIP_SENDER_MAIN, OPTION_TAILORING);
        player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TABARD,"Jewelcrafting", GOSSIP_SENDER_MAIN, OPTION_JEWELCRAFTING);
        player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TABARD,"Engineering", GOSSIP_SENDER_MAIN, OPTION_ENGINEERING);
        player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TABARD,"Blacksmithing", GOSSIP_SENDER_MAIN, OPTION_BLACKSMITHING);
        player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TABARD,"Alchemy", GOSSIP_SENDER_MAIN, OPTION_ALCHEMY);
        player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TABARD,"Inscription", GOSSIP_SENDER_MAIN, OPTION_INSCRIPTION);
        player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TABARD,"Enchanting", GOSSIP_SENDER_MAIN, OPTION_ENCHANTING);
        player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TABARD,"Leatherworking", GOSSIP_SENDER_MAIN, OPTION_LEATHERWORKING);
        player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TABARD,"Cooking", GOSSIP_SENDER_MAIN, OPTION_COOKING);
        player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TABARD,"First Aid", GOSSIP_SENDER_MAIN, OPTION_FIRST_AID);
        player->SEND_GOSSIP_MENU(TEXT_PROFESSION_MAIN, creature->GetGUID());
        return true;
    }

    bool OnGossipSelect(Player* player, Creature* creature, uint32 sender, uint32 action) override
    {
        player->PlayerTalkClass->ClearMenus();
        switch (sender)
        {
            case GOSSIP_SENDER_MAIN:
                switch (action)
                {
                    case OPTION_TAILORING:
                        player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TABARD,"Learn the skill Tailoring", ACTION_LEARN_PROFESSION, SKILL_TAILORING);
                        //player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TABARD,"|TInterface/ICONS/inv_fabric_linen_01:40:40:-18:0|tTailoring Materials", ACTION_PURCHASE_MATERIALS, SORT_TAILORING);
                        break;
                    case OPTION_JEWELCRAFTING:
                        player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TABARD,"Learn the skill Jewelcrafting", ACTION_LEARN_PROFESSION, SKILL_JEWELCRAFTING);
                        //player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TABARD,"|TInterface/ICONS/inv_jewelcrafting_nightseye_02:40:40:-18:0|tJewelcrafting Materials", ACTION_PURCHASE_MATERIALS, SORT_JEWELCRAFTING);
                        break;
                    case OPTION_ENGINEERING:
                        player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TABARD,"Learn the skill Engineering", ACTION_LEARN_PROFESSION, SKILL_ENGINEERING);
                        //player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TABARD,"|TInterface/ICONS/inv_misc_gear_06:40:40:-18:0|tEngineering Materials", ACTION_PURCHASE_MATERIALS, SORT_ENGINEERING);
                        break;
                    case OPTION_BLACKSMITHING:
                        player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TABARD,"Learn the skill Blacksmithing", ACTION_LEARN_PROFESSION, SKILL_BLACKSMITHING);
                        //player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TABARD,"|TInterface/ICONS/inv_ingot_02:40:40:-18:0|tBlacksmithing Materials", ACTION_PURCHASE_MATERIALS, SORT_BLACKSMITHING);
                        break;
                    case OPTION_ALCHEMY:
                        player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TABARD,"Learn the skill Alchemy", ACTION_LEARN_PROFESSION, SKILL_ALCHEMY);
                        //player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TABARD,"|TInterface/ICONS/inv_misc_flower_02:40:40:-18:0|tAlchemy Materials", ACTION_PURCHASE_MATERIALS, SORT_ALCHEMY);
                        break;
                    case OPTION_INSCRIPTION:
                        player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TABARD,"Learn the skill Inscription", ACTION_LEARN_PROFESSION, SKILL_INSCRIPTION);
                        //player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TABARD,"|TInterface/ICONS/inv_inscription_papyrus:40:40:-18:0|tInscription Materials", ACTION_PURCHASE_MATERIALS, SORT_INSCRIPTION);
                        break;
                    case OPTION_ENCHANTING:
                        player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TABARD,"Learn the skill Enchanting", ACTION_LEARN_PROFESSION, SKILL_ENCHANTING);
                        //player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TABARD,"|TInterface/ICONS/inv_misc_dust_infinite:40:40:-18:0|tEnchanting Materials", ACTION_PURCHASE_MATERIALS, SORT_ENCHANTING);
                        break;
                    case OPTION_LEATHERWORKING:
                        player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TABARD,"Learn the skill Leatherworking", ACTION_LEARN_PROFESSION, SKILL_LEATHERWORKING);
                        //player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TABARD,"|TInterface/ICONS/inv_misc_leatherscrap_03:40:40:-18:0|tLeatherworking Materials", ACTION_PURCHASE_MATERIALS, SORT_LEATHERWORKING);
                        break;
                    case OPTION_COOKING:
                        player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TABARD,"Learn the skill Cooking", ACTION_LEARN_PROFESSION, SKILL_COOKING);
                        //player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TABARD,"|TInterface/ICONS/inv_misc_food_16:40:40:-18:0|tCooking Materials", ACTION_PURCHASE_MATERIALS, SORT_COOKING);
                        break;
                    case OPTION_FIRST_AID:
                        player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TABARD,"Learn the skill First Aid", ACTION_LEARN_PROFESSION, SKILL_FIRST_AID);
                        //player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TABARD,"|TInterface/ICONS/inv_misc_bandage_15:40:40:-18:0|tFirst Aid Materials", ACTION_PURCHASE_MATERIALS, SORT_FIRST_AID);
                        break;
                    case OPTION_MAIN_MENU:
                        OnGossipHello(player, creature);
                        player->SEND_GOSSIP_MENU(TEXT_PROFESSION_MAIN, creature->GetGUID());
                        return true;
                }
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TABARD,"Back", GOSSIP_SENDER_MAIN, OPTION_MAIN_MENU);
                player->SEND_GOSSIP_MENU(TEXT_PROFESSION_MAIN, creature->GetGUID());
                break;
            case ACTION_LEARN_PROFESSION:
                if (!CheckSkill(player, action))
                {
                    player->GetSession()->SendNotification("You already have two professions or already know this skill");
                    OnGossipHello(player, creature);
                    break;
                }
                HandleLearnSkillRecipesHelper(player, action);
                player->SetSkill(action, player->GetSkillStep(action), 450, 450);
                break;
            case ACTION_PURCHASE_MATERIALS:
                player->GetSession()->SendListInventory(creature->GetGUID(), action);
                break;

        }
        return true;
    }

    bool CheckSkill(Player* player, uint16 skill)
    {
        uint8 skillCount = 0;

        if (IsSecondary(skill))
            return true;

        if (player->HasSkill(SKILL_ALCHEMY))
            skillCount++;

        if (player->HasSkill(SKILL_BLACKSMITHING))
            skillCount++;

        if (player->HasSkill(SKILL_ENCHANTING))
            skillCount++;

        if (player->HasSkill(SKILL_ENGINEERING))
            skillCount++;

        if (player->HasSkill(SKILL_HERBALISM))
            skillCount++;

        if (player->HasSkill(SKILL_INSCRIPTION))
            skillCount++;

        if (player->HasSkill(SKILL_JEWELCRAFTING))
            skillCount++;

        if (player->HasSkill(SKILL_LEATHERWORKING))
            skillCount++;

        if (player->HasSkill(SKILL_TAILORING))
            skillCount++;

        if (skillCount > 1)
            return false;

        return true;
    }

    bool IsSecondary(uint16 skill)
    {
        switch (skill)
        {
            case SKILL_COOKING:
            case SKILL_FIRST_AID:
            case SKILL_FISHING:
                return true;
            default:
                return false;
        }
    }

    void HandleLearnSkillRecipesHelper(Player* player, uint32 skillId)
    {
        uint32 classmask = player->getClassMask();

        for (uint32 j = 0; j < sSkillLineAbilityStore.GetNumRows(); ++j)
        {
            SkillLineAbilityEntry const* skillLine = sSkillLineAbilityStore.LookupEntry(j);
            if (!skillLine)
                continue;

            // wrong skill
            if (skillLine->skillId != skillId)
                continue;

            // not high rank
            if (skillLine->forward_spellid)
                continue;

            // skip racial skills
            if (skillLine->racemask != 0)
                continue;

            // skip wrong class skills
            if (skillLine->classmask && (skillLine->classmask & classmask) == 0)
                continue;

            SpellInfo const* spellInfo = sSpellMgr->GetSpellInfo(skillLine->spellId);
            if (!spellInfo || !SpellMgr::IsSpellValid(spellInfo, player, false))
                continue;

            player->learnSpell(skillLine->spellId, false);
        }
    }
};


enum Enchant
{
    ENCHANT_OPTION_HEAD,
    ENCHANT_OPTION_SHOULDERS,
    ENCHANT_OPTION_CHEST,
    ENCHANT_OPTION_GLOVES,
    ENCHANT_OPTION_LEGS,
    ENCHANT_OPTION_BRACERS,
    ENCHANT_OPTION_BELT,
    ENCHANT_OPTION_BOOTS,
    ENCHANT_OPTION_CLOAK,
    ENCHANT_OPTION_RING1,
    ENCHANT_OPTION_RING2,
    ENCHANT_OPTION_MAIN_HAND,
    ENCHANT_OPTION_OFF_HAND,
    ENCHANT_OPTION_RANGED,

    MENU_ENCHANTS
};

enum EnchantId
{
    ENCHANT_HEAD_ARCANUM_OF_TRIUMPH = 3795,
    ENCHANT_HEAD_ARCANUM_OF_DOMINANCE = 3796,
    ENCHANT_HEAD_ARCANUM_OF_TORMENT = 3817,
    ENCHANT_HEAD_ARCANUM_OF_BURNING_MYSTERIES = 3820,
    ENCHANT_HEAD_ARCANUM_OF_BLISSFUL_MENDING = 3819,
    ENCHANT_HEAD_ARCANUM_OF_THE_STALWART_PROTECTOR = 3818,
    ENCHANT_HEAD_ARCANUM_OF_THE_SAVAGE_GLADIATOR = 3842,

    ENCHANT_SHOULDER_INSCRIPTION_OF_TRIUMPH = 3793,
    ENCHANT_SHOULDER_INSCRIPTION_OF_DOMINANCE = 3794,
    ENCHANT_SHOULDER_GREATER_INSCRIPTION_OF_THE_AXE = 3808,
    ENCHANT_SHOULDER_GREATER_INSCRIPTION_OF_THE_STORM = 3810,
    ENCHANT_SHOULDER_GREATER_INSCRIPTION_OF_THE_CRAG = 3809,
    ENCHANT_SHOULDER_GREATER_INSCRIPTION_OF_THE_PINNACLE = 3811,
    ENCHANT_SHOULDER_GREATER_INSCRIPTION_OF_THE_GLADIATOR = 3852,

    ENCHANT_CHEST_EXCEPTIONAL_RESILIENCE = 3245,
    ENCHANT_CHEST_POWERFUL_STATS = 3832,
    ENCHANT_CHEST_SUPER_HEALTH = 3297,

    ENCHANT_GLOVES_CRUSHER = 1603,
    ENCHANT_GLOVES_EXCEPTIONAL_SPELLPOWER = 3246,
    ENCHANT_GLOVES_MAJOR_AGILITY = 3222,
    ENCHANT_GLOVES_PERCISION = 3234,

    ENCHANT_LEGS_ICESCALE_LEG_ARMOR = 3823,
    ENCHANT_LEGS_SAPPHIRE_SPELLTHREAD = 3721,
    ENCHANT_LEGS_BRILLIANT_SPELLTHREAD = 3719,
    ENCHANT_LEGS_FROSTHIDE_LEG_ARMOR = 3822,
    ENCHANT_LEGS_EARTHEN_LEG_ARMOR = 3853,

    ENCHANT_BRACERS_GREATER_ASSAULT = 3845,
    ENCHANT_BRACERS_SUPERIOR_SPELLPOWER = 2332,
    ENCHANT_BRACERS_EXCEPTIONAL_INTELLECT = 1119,
    ENCHANT_BRACERS_MAJOR_SPIRIT = 1147,
    ENCHANT_BRACERS_MAJOR_STAMINA = 3850,

    ENCHANT_BELT_ETERNAL_BELT_BUCKLE = 3729,

    ENCHANT_BOOTS_GREATER_ASSULT = 1597,
    ENCHANT_BOOTS_SUPERIOR_AGILITY = 983,
    ENCHANT_BOOTS_ICEWALKER = 3826,
    ENCHANT_BOOTS_TUSKARR_VITALITY = 3232,
    ENCHANT_BOOTS_GREATER_VITALITY = 3244,
    ENCHANT_BOOTS_GREATER_FORTITUDE = 1075,

    ENCHANT_CLOAK_SPELL_PIERCING = 3243,
    ENCHANT_CLOAK_MAJOR_AGILITY = 1099,
    ENCHANT_CLOAK_GREATER_SPEED = 3831,
    ENCHANT_CLOAK_SHADOW_ARMOR = 3256,

    ENCHANT_2H_WEAPON_MASSACRE = 3827,
    ENCHANT_STAFF_GREATER_SPELLPOWER = 3854,

    ENCHANT_WEAPON_BERSERKING = 3789,
    ENCHANT_WEAPON_BLACK_MAGIC = 3790,
    ENCHANT_WEAPON_BLOOD_DRAINING = 3870,
    ENCHANT_WEAPON_BLADE_WARDING = 3869,
    ENCHANT_WEAPON_TITANIUM_WEAPON_CHAIN = 3731,
    ENCHANT_WEAPON_SUPERIOR_POTENCY = 3833,
    ENCHANT_WEAPON_MIGHTY_SPELLPOWER = 3834,
    ENCHANT_WEAPON_EXCEPTIONAL_AGILITY = 1103,
    ENCHANT_WEAPON_EXCEPTIONAL_SPIRIT = 3844,
    ENCHANT_WEAPON_TITANGUARD = 3851,
    ENCHANT_WEAPON_ACCURACY = 3788,
    ENCHANT_WEAPON_MONGOOSE = 2673,
    ENCHANT_WEAPON_EXECUTIONER = 3225,
    ENCHANT_WEAPON_DEATHFROST = 3273,

    ENCHANT_RANGED_HEARTSEEKER_SCOPE = 3608,
    ENCHANT_RANGED_SUN_SCOPE = 3607,
    ENCHANT_RANGED_DIAMOND_CUT_REFRACTOR_SCOPE = 3843,

    ENCHANT_SHIELD_TITANIUM_PLATING = 3849,
    ENCHANT_SHIELD_GREATER_INTELLECT = 1128,
    ENCHANT_SHIELD_RESILIENCE = 3229,

    // Death Knight Enchants
    ENCHANT_WEAPON_FALLEN_CRUSADER = 3368,
    ENCHANT_WEAPON_CINDERGLACIER = 3369,
    ENCHANT_WEAPON_RAZORICE = 3370,
    ENCHANT_WEAPON_LICHBANE = 3366,
    ENCHANT_WEAPON_SPELLBREAKING = 3595,
    ENCHANT_WEAPON_SPELLSHATTERING = 3367,
    ENCHANT_WEAPON_SWORDBREAKING = 3594,
    ENCHANT_WEAPON_SWORDSHATTERING = 3365,
    ENCHANT_WEAPON_NERUBIAN_CARAPACE = 3883,
    ENCHANT_WEAPON_STONESKIN_GARGOYLE = 3847,

    // Blacksmithing
    ENCHANT_GLOVES_SOCKET = 3723,
    ENCHANT_BRACER_SOCKET = 3717,

    // Enchanting
    ENCHANT_RING_ASSAULT = 3839,
    ENCHANT_RING_GREATER_SPELLPOWER = 3840,
    ENCHANT_RING_STAMINA = 3791,

    // Engineering
    ENCHANT_HEAD_MIND_AMPLIFICATION_DISH = 3878,
    ENCHANT_GLOVES_HAND_MOUNTED_PRYO_ROCKET = 3603,
    ENCHANT_GLOVES_HYPERSPEED_ACCELERATORS = 3604,
    ENCHANT_BELT_FRAG_BELT = 3601,
    ENCHANT_BOOTS_NITRO_BOOSTS = 3606,
    ENCHANT_CLOAK_FLEXWEAVE_UNDERLAY = 3605,
    ENCHANT_CLOAK_SPRINGY_ARACHNOWEAVE = 3859,

    // Inscription
    ENCHANT_SHOULDERS_MASTERS_INSCRIPTION_OF_THE_AXE = 3835,
    ENCHANT_SHOULDERS_MASTERS_INSCRIPTION_OF_THE_STORM = 3838,
    ENCHANT_SHOULDERS_MASTERS_INSCRIPTION_OF_THE_CRAG = 3836,
    ENCHANT_SHOULDERS_MASTERS_INSCRIPTION_OF_THE_PINNACLE = 3837,

    // Leatherworking
    ENCHANT_BRACER_FUR_LINING_ATTACKPOWER = 3756,
    ENCHANT_BRACER_FUR_LINING_SPELLPOWER = 3758,
    ENCHANT_BRACER_FUR_LINING_AGILITY = 4873,
    ENCHANT_BRACER_FUR_LINING_STAMINA = 3757,

    // Tailoring
    ENCHANT_CLOAK_SWORDGUARD_EMBROIDERY = 3730,
    ENCHANT_CLOAK_LIGHTWEAVE_EMBROIDERY = 3722,
    ENCHANT_CLOAK_DARKGLOW_EMBROIDERY = 3728,
};

class npc_enchantment : public CreatureScript
{
public:
    npc_enchantment() : CreatureScript("npc_enchantment") { }

        bool OnGossipHello(Player* player, Creature* creature) override
        {
            return HandleEnchants(player, creature, MENU_ENCHANTS);
        }

        bool OnGossipSelect(Player* player, Creature* creature, uint32 sender, uint32 action) override
        {
            player->PlayerTalkClass->ClearMenus();

            switch (sender)
            {
                case MENU_ENCHANTS:
                    HandleEnchants(player, creature, action);
                    break;
                case ENCHANT_OPTION_HEAD:
                case ENCHANT_OPTION_SHOULDERS:
                case ENCHANT_OPTION_CHEST:
                case ENCHANT_OPTION_GLOVES:
                case ENCHANT_OPTION_LEGS:
                case ENCHANT_OPTION_BRACERS:
                case ENCHANT_OPTION_BELT:
                case ENCHANT_OPTION_BOOTS:
                case ENCHANT_OPTION_CLOAK:
                case ENCHANT_OPTION_RING1:
                case ENCHANT_OPTION_RING2:
                case ENCHANT_OPTION_MAIN_HAND:
                case ENCHANT_OPTION_OFF_HAND:
                case ENCHANT_OPTION_RANGED:
                {
                    EnchantItem(player, creature, action, sender);
                    break;
                }
            }
            return true;
        }

        bool EnchantItem(Player* player, Creature* creature, uint16 enchantId, uint8 enchantOption)
        {
            uint8 slot = GetItemSlotByEnchantOption(enchantOption);
            if (slot > EQUIPMENT_SLOT_RANGED)
            {
                player->GetSession()->SendNotification("Unable to find item slot based on enchant option: %u", enchantOption);
                player->CLOSE_GOSSIP_MENU();
                return false;
            }

            Item* pItem = player->GetItemByPos(INVENTORY_SLOT_BAG_0, slot);
            if (!pItem)
            {
                player->GetSession()->SendNotification("You have no item equipped in your %s slot", SlotToSlotString(slot).c_str());
                player->CLOSE_GOSSIP_MENU();
                return false;
            }

            const ItemTemplate* pItemTemplate = pItem->GetTemplate();
            if (!pItemTemplate)
            {
                player->GetSession()->SendNotification("Unable to get template data for the item in your %s slot", SlotToSlotString(slot).c_str());
                player->CLOSE_GOSSIP_MENU();
                return false;
            }

            const SpellItemEnchantmentEntry* enchantEntry = sSpellItemEnchantmentStore.LookupEntry(enchantId);
            if (!enchantEntry)
            {
                player->GetSession()->SendNotification("Unable to get data for enchant: %u", enchantId);
                player->CLOSE_GOSSIP_MENU();
                return false;
            }

            if (!CheckEnchantItem(player, pItemTemplate, enchantId, slot))
            {
                player->CLOSE_GOSSIP_MENU();
                return false;
            }

            switch (enchantId)
            {
                case ENCHANT_BELT_ETERNAL_BELT_BUCKLE:
                case ENCHANT_GLOVES_SOCKET:
                case ENCHANT_BRACER_SOCKET:
                {
                    bool addSocket = false;

                    for (uint8 i = 0; i < MAX_ITEM_ENCHANTMENT_EFFECTS; ++i)
                        if (enchantEntry->type[i] == ITEM_ENCHANTMENT_TYPE_PRISMATIC_SOCKET)
                        {
                            addSocket = true;
                            break;
                        }

                    if (!addSocket)
                    {
                        player->GetSession()->SendNotification("Unable to add socket to %s because enchant: %u doesn't have enchant entry type: ITEM_ENCHANTMENT_TYPE_PRISMATIC_SOCKET",
                        pItemTemplate->Name1.c_str(), enchantId);
                        player->CLOSE_GOSSIP_MENU();
                        return false;
                    }

                    player->ApplyEnchantment(pItem, PRISMATIC_ENCHANTMENT_SLOT, false);
                    pItem->SetEnchantment(PRISMATIC_ENCHANTMENT_SLOT, enchantId, 0, 0);
                    player->ApplyEnchantment(pItem, PRISMATIC_ENCHANTMENT_SLOT, true);
                    pItem->ClearSoulboundTradeable(player);
                    break;
                }
                default:
                {
                    player->ApplyEnchantment(pItem, PERM_ENCHANTMENT_SLOT, false);
                    pItem->SetEnchantment(PERM_ENCHANTMENT_SLOT, enchantId, 0, 0);
                    player->ApplyEnchantment(pItem, PERM_ENCHANTMENT_SLOT, true);
                    pItem->ClearSoulboundTradeable(player);
                    break;
                }
            }

            player->CastSpell(player, 61335); // Disenchant Visual
            player->PlayerTalkClass->ClearMenus();
            HandleEnchants(player, creature, MENU_ENCHANTS);
            return true;
        }

        uint8 GetItemSlotByEnchantOption(uint8 invType)
        {
            switch (invType)
            {
                case ENCHANT_OPTION_HEAD:      return EQUIPMENT_SLOT_HEAD;
                case ENCHANT_OPTION_SHOULDERS: return EQUIPMENT_SLOT_SHOULDERS;
                case ENCHANT_OPTION_CHEST:     return EQUIPMENT_SLOT_CHEST;
                case ENCHANT_OPTION_GLOVES:    return EQUIPMENT_SLOT_HANDS;
                case ENCHANT_OPTION_LEGS:      return EQUIPMENT_SLOT_LEGS;
                case ENCHANT_OPTION_BRACERS:   return EQUIPMENT_SLOT_WRISTS;
                case ENCHANT_OPTION_BELT:      return EQUIPMENT_SLOT_WAIST;
                case ENCHANT_OPTION_BOOTS:     return EQUIPMENT_SLOT_FEET;
                case ENCHANT_OPTION_CLOAK:     return EQUIPMENT_SLOT_BACK;
                case ENCHANT_OPTION_RING1:     return EQUIPMENT_SLOT_FINGER1;
                case ENCHANT_OPTION_RING2:     return EQUIPMENT_SLOT_FINGER2;
                case ENCHANT_OPTION_MAIN_HAND: return EQUIPMENT_SLOT_MAINHAND;
                case ENCHANT_OPTION_OFF_HAND:  return EQUIPMENT_SLOT_OFFHAND;
                case ENCHANT_OPTION_RANGED:    return EQUIPMENT_SLOT_RANGED;
                default:                       return EQUIPMENT_SLOT_END;
            }
        }

        std::string SlotToSlotString(uint8 slot)
        {
            switch (slot)
            {
                case EQUIPMENT_SLOT_HEAD:      return "head";
                case EQUIPMENT_SLOT_SHOULDERS: return "shoulders";
                case EQUIPMENT_SLOT_CHEST:     return "chest";
                case EQUIPMENT_SLOT_HANDS:     return "hands";
                case EQUIPMENT_SLOT_LEGS:      return "legs";
                case EQUIPMENT_SLOT_WRISTS:    return "wrists";
                case EQUIPMENT_SLOT_WAIST:     return "waist";
                case EQUIPMENT_SLOT_FEET:      return "feet";
                case EQUIPMENT_SLOT_BACK:      return "Back";
                case EQUIPMENT_SLOT_FINGER1:   return "finger1";
                case EQUIPMENT_SLOT_FINGER2:   return "finger2";
                case EQUIPMENT_SLOT_MAINHAND:  return "main hand";
                case EQUIPMENT_SLOT_OFFHAND:   return "off hand";
                case EQUIPMENT_SLOT_RANGED:    return "ranged";
                default:                       return "error";
            }
        }

        bool CheckEnchantItem(Player* player, const ItemTemplate* pItemTemplate, uint16 enchantId, uint8 slot)
        {
            switch (enchantId)
            {
                case ENCHANT_HEAD_ARCANUM_OF_TRIUMPH:
                case ENCHANT_HEAD_ARCANUM_OF_DOMINANCE:
                case ENCHANT_HEAD_ARCANUM_OF_TORMENT:
                case ENCHANT_HEAD_ARCANUM_OF_BURNING_MYSTERIES:
                case ENCHANT_HEAD_ARCANUM_OF_BLISSFUL_MENDING:
                case ENCHANT_HEAD_ARCANUM_OF_THE_STALWART_PROTECTOR:
                case ENCHANT_HEAD_ARCANUM_OF_THE_SAVAGE_GLADIATOR:
                case ENCHANT_HEAD_MIND_AMPLIFICATION_DISH:
                {
                    if (pItemTemplate->Class != ITEM_CLASS_ARMOR)
                        return false;

                    if (pItemTemplate->SubClass != ITEM_SUBCLASS_ARMOR_CLOTH &&
                        pItemTemplate->SubClass != ITEM_SUBCLASS_ARMOR_LEATHER &&
                        pItemTemplate->SubClass != ITEM_SUBCLASS_ARMOR_MAIL &&
                        pItemTemplate->SubClass != ITEM_SUBCLASS_ARMOR_PLATE)
                        return false;

                    if (pItemTemplate->InventoryType != INVTYPE_HEAD)
                        return false;

                    break;
                }
                case ENCHANT_SHOULDER_INSCRIPTION_OF_TRIUMPH:
                case ENCHANT_SHOULDER_INSCRIPTION_OF_DOMINANCE:
                case ENCHANT_SHOULDER_GREATER_INSCRIPTION_OF_THE_AXE:
                case ENCHANT_SHOULDER_GREATER_INSCRIPTION_OF_THE_STORM:
                case ENCHANT_SHOULDER_GREATER_INSCRIPTION_OF_THE_CRAG:
                case ENCHANT_SHOULDER_GREATER_INSCRIPTION_OF_THE_PINNACLE:
                case ENCHANT_SHOULDERS_MASTERS_INSCRIPTION_OF_THE_AXE:
                case ENCHANT_SHOULDERS_MASTERS_INSCRIPTION_OF_THE_STORM:
                case ENCHANT_SHOULDERS_MASTERS_INSCRIPTION_OF_THE_CRAG:
                case ENCHANT_SHOULDERS_MASTERS_INSCRIPTION_OF_THE_PINNACLE:
                case ENCHANT_SHOULDER_GREATER_INSCRIPTION_OF_THE_GLADIATOR:
                {
                    if (pItemTemplate->Class != ITEM_CLASS_ARMOR)
                        return false;

                    if (pItemTemplate->SubClass != ITEM_SUBCLASS_ARMOR_CLOTH &&
                        pItemTemplate->SubClass != ITEM_SUBCLASS_ARMOR_LEATHER &&
                        pItemTemplate->SubClass != ITEM_SUBCLASS_ARMOR_MAIL &&
                        pItemTemplate->SubClass != ITEM_SUBCLASS_ARMOR_PLATE)
                        return false;

                    if (pItemTemplate->InventoryType != INVTYPE_SHOULDERS)
                        return false;

                    break;
                }
                case ENCHANT_CHEST_EXCEPTIONAL_RESILIENCE:
                case ENCHANT_CHEST_POWERFUL_STATS:
                case ENCHANT_CHEST_SUPER_HEALTH:
                {
                    if (pItemTemplate->Class != ITEM_CLASS_ARMOR)
                        return false;

                    if (pItemTemplate->SubClass != ITEM_SUBCLASS_ARMOR_CLOTH &&
                        pItemTemplate->SubClass != ITEM_SUBCLASS_ARMOR_LEATHER &&
                        pItemTemplate->SubClass != ITEM_SUBCLASS_ARMOR_MAIL &&
                        pItemTemplate->SubClass != ITEM_SUBCLASS_ARMOR_PLATE)
                        return false;

                    if (pItemTemplate->InventoryType != INVTYPE_CHEST &&
                        pItemTemplate->InventoryType != INVTYPE_ROBE)
                        return false;

                    break;
                }
                case ENCHANT_GLOVES_CRUSHER:
                case ENCHANT_GLOVES_EXCEPTIONAL_SPELLPOWER:
                case ENCHANT_GLOVES_MAJOR_AGILITY:
                case ENCHANT_GLOVES_PERCISION:
                case ENCHANT_GLOVES_SOCKET:
                case ENCHANT_GLOVES_HAND_MOUNTED_PRYO_ROCKET:
                case ENCHANT_GLOVES_HYPERSPEED_ACCELERATORS:
                {
                    if (pItemTemplate->Class != ITEM_CLASS_ARMOR)
                        return false;

                    if (pItemTemplate->SubClass != ITEM_SUBCLASS_ARMOR_CLOTH &&
                        pItemTemplate->SubClass != ITEM_SUBCLASS_ARMOR_LEATHER &&
                        pItemTemplate->SubClass != ITEM_SUBCLASS_ARMOR_MAIL &&
                        pItemTemplate->SubClass != ITEM_SUBCLASS_ARMOR_PLATE)
                        return false;

                    if (pItemTemplate->InventoryType != INVTYPE_HANDS)
                        return false;

                    break;
                }
                case ENCHANT_LEGS_ICESCALE_LEG_ARMOR:
                case ENCHANT_LEGS_SAPPHIRE_SPELLTHREAD:
                case ENCHANT_LEGS_BRILLIANT_SPELLTHREAD:
                case ENCHANT_LEGS_FROSTHIDE_LEG_ARMOR:
                case ENCHANT_LEGS_EARTHEN_LEG_ARMOR:
                {
                    if (pItemTemplate->Class != ITEM_CLASS_ARMOR)
                        return false;

                    if (pItemTemplate->SubClass != ITEM_SUBCLASS_ARMOR_CLOTH &&
                        pItemTemplate->SubClass != ITEM_SUBCLASS_ARMOR_LEATHER &&
                        pItemTemplate->SubClass != ITEM_SUBCLASS_ARMOR_MAIL &&
                        pItemTemplate->SubClass != ITEM_SUBCLASS_ARMOR_PLATE)
                        return false;

                    if (pItemTemplate->InventoryType != INVTYPE_LEGS)
                        return false;

                    break;
                }
                case ENCHANT_BRACERS_GREATER_ASSAULT:
                case ENCHANT_BRACERS_SUPERIOR_SPELLPOWER:
                case ENCHANT_BRACERS_EXCEPTIONAL_INTELLECT:
                case ENCHANT_BRACERS_MAJOR_SPIRIT:
                case ENCHANT_BRACERS_MAJOR_STAMINA:
                case ENCHANT_BRACER_SOCKET:
                case ENCHANT_BRACER_FUR_LINING_ATTACKPOWER:
                case ENCHANT_BRACER_FUR_LINING_SPELLPOWER:
                case ENCHANT_BRACER_FUR_LINING_AGILITY:
                case ENCHANT_BRACER_FUR_LINING_STAMINA:
                {
                    if (pItemTemplate->Class != ITEM_CLASS_ARMOR)
                        return false;

                    if (pItemTemplate->SubClass != ITEM_SUBCLASS_ARMOR_CLOTH &&
                        pItemTemplate->SubClass != ITEM_SUBCLASS_ARMOR_LEATHER &&
                        pItemTemplate->SubClass != ITEM_SUBCLASS_ARMOR_MAIL &&
                        pItemTemplate->SubClass != ITEM_SUBCLASS_ARMOR_PLATE)
                        return false;

                    if (pItemTemplate->InventoryType != INVTYPE_WRISTS)
                        return false;

                    break;
                }
                case ENCHANT_BELT_ETERNAL_BELT_BUCKLE:
                case ENCHANT_BELT_FRAG_BELT:
                {
                    if (pItemTemplate->Class != ITEM_CLASS_ARMOR)
                        return false;

                    if (pItemTemplate->SubClass != ITEM_SUBCLASS_ARMOR_CLOTH &&
                        pItemTemplate->SubClass != ITEM_SUBCLASS_ARMOR_LEATHER &&
                        pItemTemplate->SubClass != ITEM_SUBCLASS_ARMOR_MAIL &&
                        pItemTemplate->SubClass != ITEM_SUBCLASS_ARMOR_PLATE)
                        return false;

                    if (pItemTemplate->InventoryType != INVTYPE_WAIST)
                        return false;

                    break;
                }
                case ENCHANT_BOOTS_GREATER_ASSULT:
                case ENCHANT_BOOTS_SUPERIOR_AGILITY:
                case ENCHANT_BOOTS_ICEWALKER:
                case ENCHANT_BOOTS_TUSKARR_VITALITY:
                case ENCHANT_BOOTS_GREATER_VITALITY:
                case ENCHANT_BOOTS_GREATER_FORTITUDE:
                case ENCHANT_BOOTS_NITRO_BOOSTS:
                {
                    if (pItemTemplate->Class != ITEM_CLASS_ARMOR)
                        return false;

                    if (pItemTemplate->SubClass != ITEM_SUBCLASS_ARMOR_CLOTH &&
                        pItemTemplate->SubClass != ITEM_SUBCLASS_ARMOR_LEATHER &&
                        pItemTemplate->SubClass != ITEM_SUBCLASS_ARMOR_MAIL &&
                        pItemTemplate->SubClass != ITEM_SUBCLASS_ARMOR_PLATE)
                        return false;

                    if (pItemTemplate->InventoryType != INVTYPE_FEET)
                        return false;

                    break;
                }
                case ENCHANT_CLOAK_SPELL_PIERCING:
                case ENCHANT_CLOAK_MAJOR_AGILITY:
                case ENCHANT_CLOAK_GREATER_SPEED:
                case ENCHANT_CLOAK_SHADOW_ARMOR:
                case ENCHANT_CLOAK_FLEXWEAVE_UNDERLAY:
                case ENCHANT_CLOAK_SPRINGY_ARACHNOWEAVE:
                case ENCHANT_CLOAK_SWORDGUARD_EMBROIDERY:
                case ENCHANT_CLOAK_LIGHTWEAVE_EMBROIDERY:
                case ENCHANT_CLOAK_DARKGLOW_EMBROIDERY:
                {
                    if (pItemTemplate->Class != ITEM_CLASS_ARMOR)
                        return false;

                    if (pItemTemplate->SubClass != ITEM_SUBCLASS_ARMOR_CLOTH)
                        return false;

                    if (pItemTemplate->InventoryType != INVTYPE_CLOAK)
                        return false;

                    break;
                }
                case ENCHANT_2H_WEAPON_MASSACRE:
                {
                    if (pItemTemplate->Class != ITEM_CLASS_WEAPON)
                        return false;

                    if (pItemTemplate->SubClass != ITEM_SUBCLASS_WEAPON_AXE2 &&
                        pItemTemplate->SubClass != ITEM_SUBCLASS_WEAPON_MACE2 &&
                        pItemTemplate->SubClass != ITEM_SUBCLASS_WEAPON_SWORD2 &&
                        pItemTemplate->SubClass != ITEM_SUBCLASS_WEAPON_POLEARM &&
                        pItemTemplate->SubClass != ITEM_SUBCLASS_WEAPON_STAFF)
                        return false;

                    if (pItemTemplate->InventoryType != INVTYPE_2HWEAPON)
                        return false;

                    break;
                }
                case ENCHANT_STAFF_GREATER_SPELLPOWER:
                {
                    if (pItemTemplate->Class != ITEM_CLASS_WEAPON)
                        return false;

                    if (pItemTemplate->SubClass != ITEM_SUBCLASS_WEAPON_STAFF)
                        return false;

                    if (pItemTemplate->InventoryType != INVTYPE_2HWEAPON)
                        return false;

                    break;
                }
                case ENCHANT_WEAPON_BERSERKING:
                case ENCHANT_WEAPON_BLACK_MAGIC:
                case ENCHANT_WEAPON_BLOOD_DRAINING:
                case ENCHANT_WEAPON_BLADE_WARDING:
                case ENCHANT_WEAPON_TITANIUM_WEAPON_CHAIN:
                case ENCHANT_WEAPON_SUPERIOR_POTENCY:
                case ENCHANT_WEAPON_MIGHTY_SPELLPOWER:
                case ENCHANT_WEAPON_EXCEPTIONAL_AGILITY:
                case ENCHANT_WEAPON_EXCEPTIONAL_SPIRIT:
                case ENCHANT_WEAPON_TITANGUARD:
                case ENCHANT_WEAPON_ACCURACY:
                case ENCHANT_WEAPON_MONGOOSE:
                case ENCHANT_WEAPON_EXECUTIONER:
                case ENCHANT_WEAPON_DEATHFROST:
                case ENCHANT_WEAPON_FALLEN_CRUSADER:
                case ENCHANT_WEAPON_CINDERGLACIER:
                case ENCHANT_WEAPON_RAZORICE:
                case ENCHANT_WEAPON_LICHBANE:
                case ENCHANT_WEAPON_SPELLBREAKING:
                case ENCHANT_WEAPON_SPELLSHATTERING:
                case ENCHANT_WEAPON_SWORDBREAKING:
                case ENCHANT_WEAPON_SWORDSHATTERING:
                case ENCHANT_WEAPON_NERUBIAN_CARAPACE:
                case ENCHANT_WEAPON_STONESKIN_GARGOYLE:
                {
                    if (pItemTemplate->Class != ITEM_CLASS_WEAPON)
                        return false;

                    if (pItemTemplate->SubClass != ITEM_SUBCLASS_WEAPON_AXE &&
                        pItemTemplate->SubClass != ITEM_SUBCLASS_WEAPON_AXE2 &&
                        pItemTemplate->SubClass != ITEM_SUBCLASS_WEAPON_MACE &&
                        pItemTemplate->SubClass != ITEM_SUBCLASS_WEAPON_MACE2 &&
                        pItemTemplate->SubClass != ITEM_SUBCLASS_WEAPON_POLEARM &&
                        pItemTemplate->SubClass != ITEM_SUBCLASS_WEAPON_SWORD &&
                        pItemTemplate->SubClass != ITEM_SUBCLASS_WEAPON_SWORD2 &&
                        pItemTemplate->SubClass != ITEM_SUBCLASS_WEAPON_STAFF &&
                        pItemTemplate->SubClass != ITEM_SUBCLASS_WEAPON_FIST &&
                        pItemTemplate->SubClass != ITEM_SUBCLASS_WEAPON_DAGGER)
                        return false;

                    if (pItemTemplate->InventoryType != INVTYPE_2HWEAPON &&
                        pItemTemplate->InventoryType != INVTYPE_WEAPON &&
                        pItemTemplate->InventoryType != INVTYPE_WEAPONMAINHAND &&
                        pItemTemplate->InventoryType != INVTYPE_WEAPONOFFHAND)
                        return false;

                    break;
                }
                case ENCHANT_RANGED_HEARTSEEKER_SCOPE:
                case ENCHANT_RANGED_SUN_SCOPE:
                case ENCHANT_RANGED_DIAMOND_CUT_REFRACTOR_SCOPE:
                {
                    if (pItemTemplate->Class != ITEM_CLASS_WEAPON)
                        return false;

                    if (pItemTemplate->SubClass != ITEM_SUBCLASS_WEAPON_BOW &&
                        pItemTemplate->SubClass != ITEM_SUBCLASS_WEAPON_GUN &&
                        pItemTemplate->SubClass != ITEM_SUBCLASS_WEAPON_CROSSBOW)
                        return false;

                    if (pItemTemplate->InventoryType != INVTYPE_RANGED &&
                        pItemTemplate->InventoryType != INVTYPE_RANGEDRIGHT)
                        return false;

                    break;
                }
                case ENCHANT_SHIELD_TITANIUM_PLATING:
                case ENCHANT_SHIELD_GREATER_INTELLECT:
                case ENCHANT_SHIELD_RESILIENCE:
                {
                    if (pItemTemplate->Class != ITEM_CLASS_ARMOR)
                        return false;

                    if (pItemTemplate->SubClass != ITEM_SUBCLASS_ARMOR_SHIELD)
                        return false;

                    if (pItemTemplate->InventoryType != INVTYPE_SHIELD)
                        return false;

                    break;
                }
                case ENCHANT_RING_ASSAULT:
                case ENCHANT_RING_GREATER_SPELLPOWER:
                case ENCHANT_RING_STAMINA:
                {
                    if (pItemTemplate->Class != ITEM_CLASS_ARMOR)
                        return false;

                    if (pItemTemplate->SubClass != ITEM_SUBCLASS_ARMOR_MISC)
                        return false;

                    if (pItemTemplate->InventoryType != INVTYPE_FINGER)
                        return false;

                    break;
                }
            }

            switch (slot)
            {
                case EQUIPMENT_SLOT_HEAD:
                case EQUIPMENT_SLOT_SHOULDERS:
                case EQUIPMENT_SLOT_CHEST:
                case EQUIPMENT_SLOT_HANDS:
                case EQUIPMENT_SLOT_LEGS:
                case EQUIPMENT_SLOT_WRISTS:
                case EQUIPMENT_SLOT_WAIST:
                case EQUIPMENT_SLOT_FEET:
                {
                    if (pItemTemplate->Class != ITEM_CLASS_ARMOR)
                        return false;

                    if (pItemTemplate->Class == ITEM_CLASS_ARMOR)
                        if (pItemTemplate->SubClass != ITEM_SUBCLASS_ARMOR_CLOTH &&
                            pItemTemplate->SubClass != ITEM_SUBCLASS_ARMOR_LEATHER &&
                            pItemTemplate->SubClass != ITEM_SUBCLASS_ARMOR_MAIL &&
                            pItemTemplate->SubClass != ITEM_SUBCLASS_ARMOR_PLATE)
                            return false;

                    if (pItemTemplate->InventoryType != INVTYPE_HEAD &&
                        pItemTemplate->InventoryType != INVTYPE_SHOULDERS &&
                        pItemTemplate->InventoryType != INVTYPE_CHEST &&
                        pItemTemplate->InventoryType != INVTYPE_ROBE &&
                        pItemTemplate->InventoryType != INVTYPE_HANDS &&
                        pItemTemplate->InventoryType != INVTYPE_LEGS &&
                        pItemTemplate->InventoryType != INVTYPE_WRISTS &&
                        pItemTemplate->InventoryType != INVTYPE_WAIST &&
                        pItemTemplate->InventoryType != INVTYPE_FEET)
                        return false;

                    break;
                }
                case EQUIPMENT_SLOT_BACK:
                {
                    if (pItemTemplate->Class != ITEM_CLASS_ARMOR)
                        return false;

                    if (pItemTemplate->Class == ITEM_CLASS_ARMOR)
                        if (pItemTemplate->SubClass != ITEM_SUBCLASS_ARMOR_CLOTH)
                            return false;

                    if (pItemTemplate->InventoryType != INVTYPE_CLOAK)
                        return false;

                    break;
                }
                case EQUIPMENT_SLOT_FINGER1:
                case EQUIPMENT_SLOT_FINGER2:
                {
                    if (pItemTemplate->Class != ITEM_CLASS_ARMOR)
                        return false;

                    if (pItemTemplate->Class == ITEM_CLASS_ARMOR)
                        if (pItemTemplate->SubClass != ITEM_SUBCLASS_ARMOR_MISC)
                            return false;

                    if (pItemTemplate->InventoryType != INVTYPE_FINGER)
                        return false;

                    break;
                }
                case EQUIPMENT_SLOT_MAINHAND:
                {
                    if (pItemTemplate->Class != ITEM_CLASS_WEAPON)
                        return false;

                    if (pItemTemplate->Class == ITEM_CLASS_WEAPON)
                        if (pItemTemplate->SubClass != ITEM_SUBCLASS_WEAPON_AXE &&
                            pItemTemplate->SubClass != ITEM_SUBCLASS_WEAPON_AXE2 &&
                            pItemTemplate->SubClass != ITEM_SUBCLASS_WEAPON_MACE &&
                            pItemTemplate->SubClass != ITEM_SUBCLASS_WEAPON_MACE2 &&
                            pItemTemplate->SubClass != ITEM_SUBCLASS_WEAPON_SWORD &&
                            pItemTemplate->SubClass != ITEM_SUBCLASS_WEAPON_SWORD2 &&
                            pItemTemplate->SubClass != ITEM_SUBCLASS_WEAPON_POLEARM &&
                            pItemTemplate->SubClass != ITEM_SUBCLASS_WEAPON_STAFF &&
                            pItemTemplate->SubClass != ITEM_SUBCLASS_WEAPON_FIST &&
                            pItemTemplate->SubClass != ITEM_SUBCLASS_WEAPON_DAGGER)
                            return false;

                    if (pItemTemplate->InventoryType != INVTYPE_WEAPON &&
                        pItemTemplate->InventoryType != INVTYPE_WEAPONMAINHAND &&
                        pItemTemplate->InventoryType != INVTYPE_WEAPONOFFHAND &&
                        pItemTemplate->InventoryType != INVTYPE_2HWEAPON)
                        return false;

                    break;
                }
                case EQUIPMENT_SLOT_OFFHAND:
                {
                    if (pItemTemplate->Class != ITEM_CLASS_WEAPON && pItemTemplate->Class != ITEM_CLASS_ARMOR)
                        return false;

                    if (pItemTemplate->Class == ITEM_CLASS_WEAPON)
                        if (pItemTemplate->SubClass != ITEM_SUBCLASS_WEAPON_AXE &&
                            pItemTemplate->SubClass != ITEM_SUBCLASS_WEAPON_AXE2 &&
                            pItemTemplate->SubClass != ITEM_SUBCLASS_WEAPON_MACE &&
                            pItemTemplate->SubClass != ITEM_SUBCLASS_WEAPON_MACE2 &&
                            pItemTemplate->SubClass != ITEM_SUBCLASS_WEAPON_SWORD &&
                            pItemTemplate->SubClass != ITEM_SUBCLASS_WEAPON_SWORD2 &&
                            pItemTemplate->SubClass != ITEM_SUBCLASS_WEAPON_POLEARM &&
                            pItemTemplate->SubClass != ITEM_SUBCLASS_WEAPON_STAFF &&
                            pItemTemplate->SubClass != ITEM_SUBCLASS_WEAPON_FIST &&
                            pItemTemplate->SubClass != ITEM_SUBCLASS_WEAPON_DAGGER)
                            return false;

                    if (pItemTemplate->Class == ITEM_CLASS_ARMOR)
                        if (pItemTemplate->SubClass != ITEM_SUBCLASS_ARMOR_SHIELD)
                            return false;

                    if (pItemTemplate->InventoryType != INVTYPE_WEAPON &&
                        pItemTemplate->InventoryType != INVTYPE_WEAPONMAINHAND &&
                        pItemTemplate->InventoryType != INVTYPE_WEAPONOFFHAND &&
                        pItemTemplate->InventoryType != INVTYPE_2HWEAPON &&
                        pItemTemplate->InventoryType != INVTYPE_SHIELD)
                        return false;

                    break;
                }
                case EQUIPMENT_SLOT_RANGED:
                {
                    if (pItemTemplate->Class != ITEM_CLASS_WEAPON)
                        return false;

                    if (pItemTemplate->Class == ITEM_CLASS_WEAPON)
                        if (pItemTemplate->SubClass != ITEM_SUBCLASS_WEAPON_BOW &&
                            pItemTemplate->SubClass != ITEM_SUBCLASS_WEAPON_GUN &&
                            pItemTemplate->SubClass != ITEM_SUBCLASS_WEAPON_CROSSBOW)
                            return false;

                    if (pItemTemplate->InventoryType != INVTYPE_RANGED &&
                        pItemTemplate->InventoryType != INVTYPE_RANGEDRIGHT)
                        return false;

                    break;
                }
            }

            switch (enchantId)
            {
                case ENCHANT_2H_WEAPON_MASSACRE:
                {
                    if (pItemTemplate->Class == ITEM_CLASS_WEAPON && pItemTemplate->InventoryType != INVTYPE_2HWEAPON)
                        return false;

                    break;
                }
                case ENCHANT_STAFF_GREATER_SPELLPOWER:
                {
                    if (pItemTemplate->Class == ITEM_CLASS_WEAPON && pItemTemplate->SubClass != ITEM_SUBCLASS_WEAPON_STAFF)
                        return false;

                    break;
                }
                case ENCHANT_RANGED_HEARTSEEKER_SCOPE:
                case ENCHANT_RANGED_SUN_SCOPE:
                case ENCHANT_RANGED_DIAMOND_CUT_REFRACTOR_SCOPE:
                {
                    if (pItemTemplate->Class == ITEM_CLASS_WEAPON && (pItemTemplate->SubClass != ITEM_SUBCLASS_WEAPON_BOW &&
                        pItemTemplate->SubClass != ITEM_SUBCLASS_WEAPON_GUN && pItemTemplate->SubClass != ITEM_SUBCLASS_WEAPON_CROSSBOW))
                        return false;

                    break;
                }
                case ENCHANT_SHIELD_TITANIUM_PLATING:
                case ENCHANT_SHIELD_GREATER_INTELLECT:
                case ENCHANT_SHIELD_RESILIENCE:
                {
                    if (pItemTemplate->InventoryType != INVTYPE_SHIELD)
                        return false;

                    break;
                }
                case ENCHANT_WEAPON_FALLEN_CRUSADER:
                case ENCHANT_WEAPON_CINDERGLACIER:
                case ENCHANT_WEAPON_RAZORICE:
                case ENCHANT_WEAPON_LICHBANE:
                case ENCHANT_WEAPON_SPELLBREAKING:
                case ENCHANT_WEAPON_SPELLSHATTERING:
                case ENCHANT_WEAPON_SWORDBREAKING:
                case ENCHANT_WEAPON_SWORDSHATTERING:
                case ENCHANT_WEAPON_NERUBIAN_CARAPACE:
                case ENCHANT_WEAPON_STONESKIN_GARGOYLE:
                {
                    if (pItemTemplate->Class == ITEM_CLASS_WEAPON && player->getClass() != CLASS_DEATH_KNIGHT)
                        return false;

                    break;
                }
                case ENCHANT_GLOVES_SOCKET:
                case ENCHANT_BRACER_SOCKET:
                {
                    if (player->GetSkillValue(SKILL_BLACKSMITHING) < 450)
                        return false;

                    break;
                }
                case ENCHANT_RING_ASSAULT:
                case ENCHANT_RING_GREATER_SPELLPOWER:
                case ENCHANT_RING_STAMINA:
                {
                    if (player->GetSkillValue(SKILL_ENCHANTING) < 450)
                        return false;

                    break;
                }
                case ENCHANT_HEAD_MIND_AMPLIFICATION_DISH:
                case ENCHANT_GLOVES_HAND_MOUNTED_PRYO_ROCKET:
                case ENCHANT_GLOVES_HYPERSPEED_ACCELERATORS:
                case ENCHANT_BELT_FRAG_BELT:
                case ENCHANT_BOOTS_NITRO_BOOSTS:
                case ENCHANT_CLOAK_FLEXWEAVE_UNDERLAY:
                case ENCHANT_CLOAK_SPRINGY_ARACHNOWEAVE:
                {
                    if (player->GetSkillValue(SKILL_ENGINEERING) < 450)
                        return false;

                    for (uint8 i = 0; i < MAX_ITEM_SPELLS; ++i)
                        if (pItemTemplate->Spells[i].SpellId && !pItemTemplate->Spells[i].SpellTrigger)
                            return false;

                    break;
                }
                case ENCHANT_SHOULDERS_MASTERS_INSCRIPTION_OF_THE_AXE:
                case ENCHANT_SHOULDERS_MASTERS_INSCRIPTION_OF_THE_STORM:
                case ENCHANT_SHOULDERS_MASTERS_INSCRIPTION_OF_THE_CRAG:
                case ENCHANT_SHOULDERS_MASTERS_INSCRIPTION_OF_THE_PINNACLE:
                {
                    if (player->GetSkillValue(SKILL_INSCRIPTION) < 450)
                        return false;

                    break;
                }
                case ENCHANT_BRACER_FUR_LINING_ATTACKPOWER:
                case ENCHANT_BRACER_FUR_LINING_SPELLPOWER:
                case ENCHANT_BRACER_FUR_LINING_AGILITY:
                case ENCHANT_BRACER_FUR_LINING_STAMINA:
                {
                    if (player->GetSkillValue(SKILL_LEATHERWORKING) < 450)
                        return false;

                    break;
                }
                case ENCHANT_CLOAK_SWORDGUARD_EMBROIDERY:
                case ENCHANT_CLOAK_LIGHTWEAVE_EMBROIDERY:
                case ENCHANT_CLOAK_DARKGLOW_EMBROIDERY:
                {
                    if (player->GetSkillValue(SKILL_TAILORING) < 450)
                        return false;

                    break;
                }
            }

            return true;
        }

        bool HandleEnchants(Player* player, Creature* creature, uint32 enchantOption)
        {
            switch (enchantOption)
            {
                case MENU_ENCHANTS:
                {
                    if (player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_HEAD))
                        player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TABARD, "Head", MENU_ENCHANTS, ENCHANT_OPTION_HEAD);

                    if (player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_SHOULDERS))
                        player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TABARD, "Shoulders", MENU_ENCHANTS, ENCHANT_OPTION_SHOULDERS);

                    if (player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_CHEST))
                        player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TABARD, "Chest", MENU_ENCHANTS, ENCHANT_OPTION_CHEST);

                    if (player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_HANDS))
                        player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TABARD, "Gloves", MENU_ENCHANTS, ENCHANT_OPTION_GLOVES);

                    if (player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_LEGS))
                        player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TABARD, "Legs", MENU_ENCHANTS, ENCHANT_OPTION_LEGS);

                    if (player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_WRISTS))
                        player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TABARD, "Bracers", MENU_ENCHANTS, ENCHANT_OPTION_BRACERS);

                    if (player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_WAIST))
                        player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TABARD, "Belt", MENU_ENCHANTS, ENCHANT_OPTION_BELT);

                    if (player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_FEET))
                        player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TABARD, "Boots", MENU_ENCHANTS, ENCHANT_OPTION_BOOTS);

                    if (player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_BACK))
                        player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TABARD, "Cloak", MENU_ENCHANTS, ENCHANT_OPTION_CLOAK);

                    if (player->GetSkillValue(SKILL_ENCHANTING) >= 450)
                    {
                        if (player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_FINGER1))
                        player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TABARD, "Ring 1", MENU_ENCHANTS, ENCHANT_OPTION_RING1);

                        if (player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_FINGER2))
                        player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TABARD, "Ring 2", MENU_ENCHANTS, ENCHANT_OPTION_RING2);
                    }

                    if (player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_MAINHAND))
                    player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TABARD, "Main Hand", MENU_ENCHANTS, ENCHANT_OPTION_MAIN_HAND);

                    if (Item* item = player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_OFFHAND))
                        if (item->GetTemplate()->InventoryType != INVTYPE_HOLDABLE)
                            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TABARD, "Off Hand", MENU_ENCHANTS, ENCHANT_OPTION_OFF_HAND);

                    if (Item* item = player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_RANGED))
                        if (item->GetTemplate()->InventoryType != INVTYPE_RELIC && item->GetTemplate()->SubClass != ITEM_SUBCLASS_WEAPON_WAND)
                            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TABARD, "Ranged", MENU_ENCHANTS, ENCHANT_OPTION_RANGED);

                    player->SEND_GOSSIP_MENU(1, creature->GetGUID());
                    break;
                }
                case ENCHANT_OPTION_HEAD:
                {
                    Item* pItem = player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_HEAD);
                    if (!pItem)
                    {
                        player->GetSession()->SendNotification("You have no item equipped in your head slot");
                        player->CLOSE_GOSSIP_MENU();
                        return false;
                    }

                    player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TABARD, "+50 Attack Power & +20 Resilience", ENCHANT_OPTION_HEAD, ENCHANT_HEAD_ARCANUM_OF_TRIUMPH);
                    player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TABARD, "+29 Spell Power & +20 Resilience", ENCHANT_OPTION_HEAD, ENCHANT_HEAD_ARCANUM_OF_DOMINANCE);
                    player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TABARD, "+50 Attack Power & +20 Crit", ENCHANT_OPTION_HEAD, ENCHANT_HEAD_ARCANUM_OF_TORMENT);
                    player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TABARD, "+30 Spell Power & +20 Crit", ENCHANT_OPTION_HEAD, ENCHANT_HEAD_ARCANUM_OF_BURNING_MYSTERIES);
                    player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TABARD, "+30 Spell Power & +10 MP5", ENCHANT_OPTION_HEAD, ENCHANT_HEAD_ARCANUM_OF_BLISSFUL_MENDING);
                    player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TABARD, "+37 Stamina & +20 Dodge", ENCHANT_OPTION_HEAD, ENCHANT_HEAD_ARCANUM_OF_THE_STALWART_PROTECTOR);
                    player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TABARD, "+30 Stamina & +25 Resilience", ENCHANT_OPTION_HEAD, ENCHANT_HEAD_ARCANUM_OF_THE_SAVAGE_GLADIATOR);
                    player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TABARD, "Back", MENU_ENCHANTS, MENU_ENCHANTS);
                    player->SEND_GOSSIP_MENU(1, creature->GetGUID());
                    break;
                }
                case ENCHANT_OPTION_SHOULDERS:
                {
                    Item* pItem = player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_SHOULDERS);
                    if (!pItem)
                    {
                        player->GetSession()->SendNotification("You have no item equipped in your shoulder slot");
                        player->CLOSE_GOSSIP_MENU();
                        return false;
                    }

                    player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TABARD, "+40 Attack Power & +15 Resilience", ENCHANT_OPTION_SHOULDERS, ENCHANT_SHOULDER_INSCRIPTION_OF_TRIUMPH);
                    player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TABARD, "+23 Spell Power & +15 Resilience", ENCHANT_OPTION_SHOULDERS, ENCHANT_SHOULDER_INSCRIPTION_OF_DOMINANCE);
                    player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TABARD, "+40 Attack Power & +15 Crit", ENCHANT_OPTION_SHOULDERS, ENCHANT_SHOULDER_GREATER_INSCRIPTION_OF_THE_AXE);
                    player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TABARD, "+24 Spell Power & +15 Crit", ENCHANT_OPTION_SHOULDERS, ENCHANT_SHOULDER_GREATER_INSCRIPTION_OF_THE_STORM);
                    player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TABARD, "+24 Spell Power & +8 MP5", ENCHANT_OPTION_SHOULDERS, ENCHANT_SHOULDER_GREATER_INSCRIPTION_OF_THE_CRAG);
                    player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TABARD, "+20 Dodge & +22 Stamina", ENCHANT_OPTION_SHOULDERS, ENCHANT_SHOULDER_GREATER_INSCRIPTION_OF_THE_PINNACLE);
                    player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TABARD, "+30 Stamina & +15 Resilience", ENCHANT_OPTION_SHOULDERS, ENCHANT_SHOULDER_GREATER_INSCRIPTION_OF_THE_GLADIATOR);

                    if (player->GetSkillValue(SKILL_INSCRIPTION) >= 450)
                    {
                        player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TABARD, "+120 Attack Power & +15 Crit", ENCHANT_OPTION_SHOULDERS, ENCHANT_SHOULDERS_MASTERS_INSCRIPTION_OF_THE_AXE);
                        player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TABARD, "+70 Spell Power & +15 Crit", ENCHANT_OPTION_SHOULDERS, ENCHANT_SHOULDERS_MASTERS_INSCRIPTION_OF_THE_STORM);
                        player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TABARD, "+70 Spell Power & +8 MP5", ENCHANT_OPTION_SHOULDERS, ENCHANT_SHOULDERS_MASTERS_INSCRIPTION_OF_THE_CRAG);
                        player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TABARD, "+60 Dodge & +15 Parry", ENCHANT_OPTION_SHOULDERS, ENCHANT_SHOULDERS_MASTERS_INSCRIPTION_OF_THE_PINNACLE);
                    }

                    player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TABARD, "Back", MENU_ENCHANTS, MENU_ENCHANTS);
                    player->SEND_GOSSIP_MENU(1, creature->GetGUID());
                    break;
                }
                case ENCHANT_OPTION_CHEST:
                {
                    Item* pItem = player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_CHEST);
                    if (!pItem)
                    {
                        player->GetSession()->SendNotification("You have no item equipped in your chest slot");
                        player->CLOSE_GOSSIP_MENU();
                        return false;
                    }

                    player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TABARD, "+20 Resilience", ENCHANT_OPTION_CHEST, ENCHANT_CHEST_EXCEPTIONAL_RESILIENCE);
                    player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TABARD, "+10 Stats", ENCHANT_OPTION_CHEST, ENCHANT_CHEST_POWERFUL_STATS);
                    player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TABARD, "+275 Health", ENCHANT_OPTION_CHEST, ENCHANT_CHEST_SUPER_HEALTH);
                    player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TABARD, "Back", MENU_ENCHANTS, MENU_ENCHANTS);
                    player->SEND_GOSSIP_MENU(1, creature->GetGUID());
                    break;
                }
                case ENCHANT_OPTION_GLOVES:
                {
                    Item* pItem = player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_HANDS);
                    if (!pItem)
                    {
                        player->GetSession()->SendNotification("You have no item equipped in your hands slot");
                        player->CLOSE_GOSSIP_MENU();
                        return false;
                    }

                    player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TABARD, "+44 Attack Power", ENCHANT_OPTION_GLOVES, ENCHANT_GLOVES_CRUSHER);
                    player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TABARD, "+28 Spell Power", ENCHANT_OPTION_GLOVES, ENCHANT_GLOVES_EXCEPTIONAL_SPELLPOWER);
                    player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TABARD, "+20 Agility", ENCHANT_OPTION_GLOVES, ENCHANT_GLOVES_MAJOR_AGILITY);
                    player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TABARD, "+20 Hit Rating", ENCHANT_OPTION_GLOVES, ENCHANT_GLOVES_PERCISION);

                    if (player->GetSkillValue(SKILL_BLACKSMITHING) >= 450)
                        player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TABARD, "Socket Gloves [|cffFF0000Requires Engineering|r]", ENCHANT_OPTION_GLOVES, ENCHANT_GLOVES_SOCKET);

                    if (player->GetSkillValue(SKILL_ENGINEERING) >= 450)
                    {
                        player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TABARD, "Hand-Mounted Pyro Rocket", ENCHANT_OPTION_GLOVES, ENCHANT_GLOVES_HAND_MOUNTED_PRYO_ROCKET);
                        player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TABARD, "Hyperspeed Accelerators", ENCHANT_OPTION_GLOVES, ENCHANT_GLOVES_HYPERSPEED_ACCELERATORS);
                    }

                    player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TABARD, "Back", MENU_ENCHANTS, MENU_ENCHANTS);
                    player->SEND_GOSSIP_MENU(1, creature->GetGUID());
                    break;
                }
                case ENCHANT_OPTION_LEGS:
                {
                    Item* pItem = player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_LEGS);
                    if (!pItem)
                    {
                        player->GetSession()->SendNotification("You have no item equipped in your legs slot");
                        player->CLOSE_GOSSIP_MENU();
                        return false;
                    }

                    player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TABARD, "+75 Attack Power & +22 Crit", ENCHANT_OPTION_LEGS, ENCHANT_LEGS_ICESCALE_LEG_ARMOR);
                    player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TABARD, "+50 Spell Power & +30 Stamina", ENCHANT_OPTION_LEGS, ENCHANT_LEGS_SAPPHIRE_SPELLTHREAD);
                    player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TABARD, "+50 Spell Power & +20 Spirit", ENCHANT_OPTION_LEGS, ENCHANT_LEGS_BRILLIANT_SPELLTHREAD);
                    player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TABARD, "+55 Stamina & +22 Agility", ENCHANT_OPTION_LEGS, ENCHANT_LEGS_FROSTHIDE_LEG_ARMOR);
                    player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TABARD, "+40 Resilience & +28 Stamina", ENCHANT_OPTION_LEGS, ENCHANT_LEGS_EARTHEN_LEG_ARMOR);
                    player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TABARD, "Back", MENU_ENCHANTS, MENU_ENCHANTS);
                    player->SEND_GOSSIP_MENU(1, creature->GetGUID());
                    break;
                }
                case ENCHANT_OPTION_BRACERS:
                {
                    Item* pItem = player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_WRISTS);
                    if (!pItem)
                    {
                        player->GetSession()->SendNotification("You have no item equipped in your wrists slot");
                        player->CLOSE_GOSSIP_MENU();
                        return false;
                    }

                    player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TABARD, "+50 Attack Power", ENCHANT_OPTION_BRACERS, ENCHANT_BRACERS_GREATER_ASSAULT);
                    player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TABARD, "+30 Spell Power", ENCHANT_OPTION_BRACERS, ENCHANT_BRACERS_SUPERIOR_SPELLPOWER);
                    player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TABARD, "+16 Intellect", ENCHANT_OPTION_BRACERS, ENCHANT_BRACERS_EXCEPTIONAL_INTELLECT);
                    player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TABARD, "+18 Spirit", ENCHANT_OPTION_BRACERS, ENCHANT_BRACERS_MAJOR_SPIRIT);
                    player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TABARD, "+40 Stamina", ENCHANT_OPTION_BRACERS, ENCHANT_BRACERS_MAJOR_STAMINA);

                    if (player->GetSkillValue(SKILL_BLACKSMITHING) >= 450)
                        player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TABARD, "Socket Bracer [|cffFF0000Requires Leatherworking / Blacksmithing|r]", ENCHANT_OPTION_BRACERS, ENCHANT_BRACER_SOCKET);

                    if (player->GetSkillValue(SKILL_LEATHERWORKING) >= 450)
                    {
                        player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TABARD, "+130 Attack Power", ENCHANT_OPTION_BRACERS, ENCHANT_BRACER_FUR_LINING_ATTACKPOWER);
                        player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TABARD, "+76 Spell Power", ENCHANT_OPTION_BRACERS, ENCHANT_BRACER_FUR_LINING_SPELLPOWER);
                        player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TABARD, "+57 Agility", ENCHANT_OPTION_BRACERS, ENCHANT_BRACER_FUR_LINING_AGILITY);
                        player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TABARD, "+102 Stamina", ENCHANT_OPTION_BRACERS, ENCHANT_BRACER_FUR_LINING_STAMINA);
                    }

                    player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TABARD, "Back", MENU_ENCHANTS, MENU_ENCHANTS);
                    player->SEND_GOSSIP_MENU(1, creature->GetGUID());
                    break;
                }
                case ENCHANT_OPTION_BELT:
                {
                    Item* pItem = player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_WAIST);
                    if (!pItem)
                    {
                        player->GetSession()->SendNotification("You have no item equipped in your waist slot");
                        player->CLOSE_GOSSIP_MENU();
                        return false;
                    }

                    if (player->GetSkillValue(SKILL_ENGINEERING) >= 450)
                        player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TABARD, "Frag Belt [|cffFF0000Requires Engineering|r]", ENCHANT_OPTION_BELT, ENCHANT_BELT_FRAG_BELT);

                    player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TABARD, "Eternal Belt Buckle", ENCHANT_OPTION_BELT, ENCHANT_BELT_ETERNAL_BELT_BUCKLE);
                    player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TABARD, "Back", MENU_ENCHANTS, MENU_ENCHANTS);
                    player->SEND_GOSSIP_MENU(1, creature->GetGUID());
                    break;
                }
                case ENCHANT_OPTION_BOOTS:
                {
                    Item* pItem = player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_FEET);
                    if (!pItem)
                    {
                        player->GetSession()->SendNotification("You have no item equipped in your feet slot");
                        player->CLOSE_GOSSIP_MENU();
                        return false;
                    }

                    player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TABARD, "+32 Attack Power", ENCHANT_OPTION_BOOTS, ENCHANT_BOOTS_GREATER_ASSULT);
                    player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TABARD, "+22 Stamina", ENCHANT_OPTION_BOOTS, ENCHANT_BOOTS_GREATER_FORTITUDE);
                    player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TABARD, "+16 Agility", ENCHANT_OPTION_BOOTS, ENCHANT_BOOTS_SUPERIOR_AGILITY);
                    player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TABARD, "+12 Hit Rating & +12 Critical", ENCHANT_OPTION_BOOTS, ENCHANT_BOOTS_ICEWALKER);
                    player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TABARD, "+15 Stamina & +8% Movement Speed", ENCHANT_OPTION_BOOTS, ENCHANT_BOOTS_TUSKARR_VITALITY);
                    player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TABARD, "+7 MP5 & +7 Health every 5 seconds", ENCHANT_OPTION_BOOTS, ENCHANT_BOOTS_GREATER_VITALITY);

                    if (player->GetSkillValue(SKILL_ENGINEERING) >= 450)
                        player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TABARD, "Nitro Boosts [|cffFF0000Requires Engineering|r]", ENCHANT_OPTION_BOOTS, ENCHANT_BOOTS_NITRO_BOOSTS);

                    player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TABARD, "Back", MENU_ENCHANTS, MENU_ENCHANTS);
                    player->SEND_GOSSIP_MENU(1, creature->GetGUID());
                    break;
                }
                case ENCHANT_OPTION_CLOAK:
                {
                    Item* pItem = player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_BACK);
                    if (!pItem)
                    {
                        player->GetSession()->SendNotification("You have no item equipped in your back slot");
                        player->CLOSE_GOSSIP_MENU();
                        return false;
                    }

                    player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TABARD, "+35 Spell Penetration", ENCHANT_OPTION_CLOAK, ENCHANT_CLOAK_SPELL_PIERCING);
                    player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TABARD, "+22 Agility", ENCHANT_OPTION_CLOAK, ENCHANT_CLOAK_MAJOR_AGILITY);
                    player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TABARD, "+22 Haste", ENCHANT_OPTION_CLOAK, ENCHANT_CLOAK_GREATER_SPEED);
                    player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TABARD, "+10 Agility & +1 Steath Level", ENCHANT_OPTION_CLOAK, ENCHANT_CLOAK_SHADOW_ARMOR);

                    if (player->GetSkillValue(SKILL_ENGINEERING) >= 450)
                    {
                        player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TABARD, "Flexweave Underlay", ENCHANT_OPTION_CLOAK, ENCHANT_CLOAK_FLEXWEAVE_UNDERLAY);
                        player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TABARD, "Springy Arachnoweave [|cffFF0000Requires Tailoring|r]", ENCHANT_OPTION_CLOAK, ENCHANT_CLOAK_SPRINGY_ARACHNOWEAVE);
                    }

                    if (player->GetSkillValue(SKILL_TAILORING) >= 450)
                    {
                        player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TABARD, "Swordguard Embroidery", ENCHANT_OPTION_CLOAK, ENCHANT_CLOAK_SWORDGUARD_EMBROIDERY);
                        player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TABARD, "Lightweave Embroidery", ENCHANT_OPTION_CLOAK, ENCHANT_CLOAK_LIGHTWEAVE_EMBROIDERY);
                        player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TABARD, "Darkglow Embroidery", ENCHANT_OPTION_CLOAK, ENCHANT_CLOAK_DARKGLOW_EMBROIDERY);
                    }

                    player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TABARD, "Back", MENU_ENCHANTS, MENU_ENCHANTS);
                    player->SEND_GOSSIP_MENU(1, creature->GetGUID());
                    break;
                }
                case ENCHANT_OPTION_RING1:
                {
                    Item* pItem = player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_FINGER2);
                    if (!pItem)
                    {
                        player->GetSession()->SendNotification("You have no item equipped in your finger slot");
                        player->CLOSE_GOSSIP_MENU();
                        return false;
                    }

                    if (player->GetSkillValue(SKILL_ENCHANTING) >= 450)
                    {
                        player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TABARD, "+40 Attack Power", ENCHANT_OPTION_RING1, ENCHANT_RING_ASSAULT);
                        player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TABARD, "+23 Spell Power", ENCHANT_OPTION_RING1, ENCHANT_RING_GREATER_SPELLPOWER);
                        player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TABARD, "+30 Stamina", ENCHANT_OPTION_RING1, ENCHANT_RING_STAMINA);
                        player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TABARD, "Back", MENU_ENCHANTS, MENU_ENCHANTS);
                        player->SEND_GOSSIP_MENU(1, creature->GetGUID());
                    }
                    break;
                }
                case ENCHANT_OPTION_RING2:
                {
                    Item* pItem = player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_FINGER2);
                    if (!pItem)
                    {
                        player->GetSession()->SendNotification("You have no item equipped in your finger slot");
                        player->CLOSE_GOSSIP_MENU();
                        return false;
                    }

                    if (player->GetSkillValue(SKILL_ENCHANTING) >= 450)
                    {
                        player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TABARD, "+40 Attack Power", ENCHANT_OPTION_RING2, ENCHANT_RING_ASSAULT);
                        player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TABARD, "+23 Spell Power", ENCHANT_OPTION_RING2, ENCHANT_RING_GREATER_SPELLPOWER);
                        player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TABARD, "+30 Stamina", ENCHANT_OPTION_RING2, ENCHANT_RING_STAMINA);
                        player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TABARD, "Back", MENU_ENCHANTS, MENU_ENCHANTS);
                        player->SEND_GOSSIP_MENU(1, creature->GetGUID());
                    }
                    break;
                }
                case ENCHANT_OPTION_MAIN_HAND:
                {
                    Item* pItem = player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_MAINHAND);
                    if (!pItem)
                    {
                        player->GetSession()->SendNotification("You have no item equipped in your main hand slot");
                        player->CLOSE_GOSSIP_MENU();
                        return false;
                    }

                    const ItemTemplate* pItemTemplate = pItem->GetTemplate();
                    if (!pItemTemplate)
                    {
                        player->GetSession()->SendNotification("Unable to get template data for the item in your main hand slot");
                        player->CLOSE_GOSSIP_MENU();
                        return false;
                    }

                    if (pItemTemplate->InventoryType == INVTYPE_2HWEAPON)
                        player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TABARD, "+110 Attack Power", ENCHANT_OPTION_MAIN_HAND, ENCHANT_2H_WEAPON_MASSACRE);

                    if (pItemTemplate->SubClass == ITEM_SUBCLASS_WEAPON_STAFF)
                        player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TABARD, "+81 Spell Power", ENCHANT_OPTION_MAIN_HAND, ENCHANT_STAFF_GREATER_SPELLPOWER);

                    player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TABARD, "Berserking", ENCHANT_OPTION_MAIN_HAND, ENCHANT_WEAPON_BERSERKING);
                    player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TABARD, "Black Magic", ENCHANT_OPTION_MAIN_HAND, ENCHANT_WEAPON_BLACK_MAGIC);
                    player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TABARD, "Blood Draining", ENCHANT_OPTION_MAIN_HAND, ENCHANT_WEAPON_BLOOD_DRAINING);
                    player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TABARD, "Blade Warding", ENCHANT_OPTION_MAIN_HAND, ENCHANT_WEAPON_BLADE_WARDING);
                    player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TABARD, "Titanium Weapon Chain", ENCHANT_OPTION_MAIN_HAND, ENCHANT_WEAPON_TITANIUM_WEAPON_CHAIN);
                    player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TABARD, "+65 Attack Power", ENCHANT_OPTION_MAIN_HAND, ENCHANT_WEAPON_SUPERIOR_POTENCY);
                    player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TABARD, "+63 Spell Power", ENCHANT_OPTION_MAIN_HAND, ENCHANT_WEAPON_MIGHTY_SPELLPOWER);
                    player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TABARD, "+26 Agility", ENCHANT_OPTION_MAIN_HAND, ENCHANT_WEAPON_EXCEPTIONAL_AGILITY);
                    player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TABARD, "+45 Spirit", ENCHANT_OPTION_MAIN_HAND, ENCHANT_WEAPON_EXCEPTIONAL_SPIRIT);
                    player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TABARD, "+50 Stamina", ENCHANT_OPTION_MAIN_HAND, ENCHANT_WEAPON_TITANGUARD);
                    player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TABARD, "+25 Hit Rating & +25 Crit", ENCHANT_OPTION_MAIN_HAND, ENCHANT_WEAPON_ACCURACY);
                    player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TABARD, "Mongoose", ENCHANT_OPTION_MAIN_HAND, ENCHANT_WEAPON_MONGOOSE);
                    player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TABARD, "Executioner", ENCHANT_OPTION_MAIN_HAND, ENCHANT_WEAPON_EXECUTIONER);
                    player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TABARD, "Deathfrost", ENCHANT_OPTION_MAIN_HAND, ENCHANT_WEAPON_DEATHFROST);

                    if (player->getClass() == CLASS_DEATH_KNIGHT)
                    {
                        player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TABARD, "Fallen Crusader", ENCHANT_OPTION_MAIN_HAND, ENCHANT_WEAPON_FALLEN_CRUSADER);
                        player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TABARD, "Cinderglacier", ENCHANT_OPTION_MAIN_HAND, ENCHANT_WEAPON_CINDERGLACIER);
                        player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TABARD, "Razorice", ENCHANT_OPTION_MAIN_HAND, ENCHANT_WEAPON_RAZORICE);
                        player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TABARD, "Lichbane", ENCHANT_OPTION_MAIN_HAND, ENCHANT_WEAPON_LICHBANE);
                        player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TABARD, "Spellbreaking", ENCHANT_OPTION_MAIN_HAND, ENCHANT_WEAPON_SPELLBREAKING);
                        player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TABARD, "Spellshattering", ENCHANT_OPTION_MAIN_HAND, ENCHANT_WEAPON_SPELLSHATTERING);
                        player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TABARD, "Swordbreaking", ENCHANT_OPTION_MAIN_HAND, ENCHANT_WEAPON_SWORDBREAKING);
                        player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TABARD, "Swordshattering", ENCHANT_OPTION_MAIN_HAND, ENCHANT_WEAPON_SWORDSHATTERING);
                        player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TABARD, "Nerubian Carapace", ENCHANT_OPTION_MAIN_HAND, ENCHANT_WEAPON_NERUBIAN_CARAPACE);
                        player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TABARD, "Stoneskin Gargoyle", ENCHANT_OPTION_MAIN_HAND, ENCHANT_WEAPON_STONESKIN_GARGOYLE);
                    }

                    player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TABARD, "Back", MENU_ENCHANTS, MENU_ENCHANTS);
                    player->SEND_GOSSIP_MENU(1, creature->GetGUID());
                    break;
                }
                case ENCHANT_OPTION_OFF_HAND:
                {
                    Item* pItem = player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_OFFHAND);
                    if (!pItem)
                    {
                        player->GetSession()->SendNotification("You have no item equipped in your off hand slot");
                        player->CLOSE_GOSSIP_MENU();
                        return false;
                    }

                    const ItemTemplate* pItemTemplate = pItem->GetTemplate();
                    if (!pItemTemplate)
                    {
                        player->GetSession()->SendNotification("Unable to get template data for the item in your off hand slot");
                        player->CLOSE_GOSSIP_MENU();
                        return false;
                    }

                    if (pItemTemplate->InventoryType == INVTYPE_HOLDABLE)
                    {
                        player->GetSession()->SendNotification("Incorrect weapon type equipped");
                        player->CLOSE_GOSSIP_MENU();
                        return false;
                    }

                    if (pItemTemplate->InventoryType == INVTYPE_2HWEAPON)
                        player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TABARD, "+110 Attack Power", ENCHANT_OPTION_MAIN_HAND, ENCHANT_2H_WEAPON_MASSACRE);

                    if (pItemTemplate->SubClass == ITEM_SUBCLASS_WEAPON_STAFF)
                        player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TABARD, "+81 Spell Power", ENCHANT_OPTION_MAIN_HAND, ENCHANT_STAFF_GREATER_SPELLPOWER);

                    player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TABARD, "Berserking", ENCHANT_OPTION_OFF_HAND, ENCHANT_WEAPON_BERSERKING);
                    player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TABARD, "Black Magic", ENCHANT_OPTION_OFF_HAND, ENCHANT_WEAPON_BLACK_MAGIC);
                    player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TABARD, "Blood Draining", ENCHANT_OPTION_OFF_HAND, ENCHANT_WEAPON_BLOOD_DRAINING);
                    player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TABARD, "Blade Warding", ENCHANT_OPTION_OFF_HAND, ENCHANT_WEAPON_BLADE_WARDING);
                    player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TABARD, "Titanium Weapon Chain", ENCHANT_OPTION_OFF_HAND, ENCHANT_WEAPON_TITANIUM_WEAPON_CHAIN);
                    player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TABARD, "+65 Attack Power", ENCHANT_OPTION_OFF_HAND, ENCHANT_WEAPON_SUPERIOR_POTENCY);
                    player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TABARD, "+63 Spell Power", ENCHANT_OPTION_OFF_HAND, ENCHANT_WEAPON_MIGHTY_SPELLPOWER);
                    player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TABARD, "+26 Agility", ENCHANT_OPTION_OFF_HAND, ENCHANT_WEAPON_EXCEPTIONAL_AGILITY);
                    player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TABARD, "+45 Spirit", ENCHANT_OPTION_OFF_HAND, ENCHANT_WEAPON_EXCEPTIONAL_SPIRIT);
                    player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TABARD, "+50 Stamina", ENCHANT_OPTION_OFF_HAND, ENCHANT_WEAPON_TITANGUARD);
                    player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TABARD, "+25 Hit Rating & +25 Crit", ENCHANT_OPTION_OFF_HAND, ENCHANT_WEAPON_ACCURACY);
                    player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TABARD, "Mongoose", ENCHANT_OPTION_OFF_HAND, ENCHANT_WEAPON_MONGOOSE);
                    player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TABARD, "Executioner", ENCHANT_OPTION_OFF_HAND, ENCHANT_WEAPON_EXECUTIONER);
                    player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TABARD, "Deathfrost", ENCHANT_OPTION_OFF_HAND, ENCHANT_WEAPON_DEATHFROST);

                    if (player->getClass() == CLASS_DEATH_KNIGHT)
                    {
                        player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TABARD, "Fallen Crusader", ENCHANT_OPTION_OFF_HAND, ENCHANT_WEAPON_FALLEN_CRUSADER);
                        player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TABARD, "Cinderglacier", ENCHANT_OPTION_OFF_HAND, ENCHANT_WEAPON_CINDERGLACIER);
                        player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TABARD, "Razorice", ENCHANT_OPTION_OFF_HAND, ENCHANT_WEAPON_RAZORICE);
                        player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TABARD, "Lichbane", ENCHANT_OPTION_OFF_HAND, ENCHANT_WEAPON_LICHBANE);
                        player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TABARD, "Spellbreaking", ENCHANT_OPTION_OFF_HAND, ENCHANT_WEAPON_SPELLBREAKING);
                        player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TABARD, "Spellshattering", ENCHANT_OPTION_OFF_HAND, ENCHANT_WEAPON_SPELLSHATTERING);
                        player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TABARD, "Swordbreaking", ENCHANT_OPTION_OFF_HAND, ENCHANT_WEAPON_SWORDBREAKING);
                        player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TABARD, "Swordshattering", ENCHANT_OPTION_OFF_HAND, ENCHANT_WEAPON_SWORDSHATTERING);
                        player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TABARD, "Nerubian Carapace", ENCHANT_OPTION_OFF_HAND, ENCHANT_WEAPON_NERUBIAN_CARAPACE);
                        player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TABARD, "Stoneskin Gargoyle", ENCHANT_OPTION_OFF_HAND, ENCHANT_WEAPON_STONESKIN_GARGOYLE);
                    }

                    if (pItemTemplate->InventoryType == INVTYPE_SHIELD)
                    {
                        player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TABARD, "Titanium Plating", ENCHANT_OPTION_OFF_HAND, ENCHANT_SHIELD_TITANIUM_PLATING);
                        player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TABARD, "+25 Intellect", ENCHANT_OPTION_OFF_HAND, ENCHANT_SHIELD_GREATER_INTELLECT);
                        player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TABARD, "+12 Resilience", ENCHANT_OPTION_OFF_HAND, ENCHANT_SHIELD_RESILIENCE);
                    }

                    player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TABARD, "Back", MENU_ENCHANTS, MENU_ENCHANTS);
                    player->SEND_GOSSIP_MENU(1, creature->GetGUID());
                    break;
                }
                case ENCHANT_OPTION_RANGED:
                {
                    Item* pItem = player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_RANGED);
                    if (!pItem)
                    {
                        player->GetSession()->SendNotification("You have no item equipped in your ranged slot");
                        player->CLOSE_GOSSIP_MENU();
                        return false;
                    }

                    const ItemTemplate* pItemTemplate = pItem->GetTemplate();
                    if (!pItemTemplate)
                    {
                        player->GetSession()->SendNotification("Unable to get template data for the item in your off hand slot");
                        player->CLOSE_GOSSIP_MENU();
                        return false;
                    }

                    if (pItemTemplate->InventoryType == INVTYPE_RELIC || pItemTemplate->SubClass == ITEM_SUBCLASS_WEAPON_WAND)
                    {
                        player->GetSession()->SendNotification("Incorrect weapon type equipped");
                        player->CLOSE_GOSSIP_MENU();
                        return false;
                    }

                    player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TABARD, "+40 Ranged Crit", ENCHANT_OPTION_RANGED, ENCHANT_RANGED_HEARTSEEKER_SCOPE);
                    player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TABARD, "+40 Ranged Haste", ENCHANT_OPTION_RANGED, ENCHANT_RANGED_SUN_SCOPE);
                    player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TABARD, "+15 Ranged Weapon Damage", ENCHANT_OPTION_RANGED, ENCHANT_RANGED_DIAMOND_CUT_REFRACTOR_SCOPE);
                    player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TABARD, "Back", MENU_ENCHANTS, MENU_ENCHANTS);
                    player->SEND_GOSSIP_MENU(1, creature->GetGUID());
                    break;
                }
            }
            return true;
        }
};

#include "ScriptMgr.h"
#include "ArenaTeamMgr.h"
#include "Common.h"
#include "DisableMgr.h"
#include "BattlegroundMgr.h"
#include "Battleground.h"
#include "ArenaTeam.h"
#include "Language.h"

#define ARENA_1V1_MIN_LEVEL 20 // min level to create an arenateam
#define ARENA_1V1_COST 0 * 10000 // costs for create a team: 0 gold

class npc_1v1arena : public CreatureScript  
{
public:
    npc_1v1arena() : CreatureScript("npc_1v1arena") {}

	bool JoinQueueArena(Player* player, Creature* me)
	{
		if(ARENA_1V1_MIN_LEVEL > player->getLevel())
			return false;

		uint64 guid = player->GetGUID();
		uint8 arenaslot = ArenaTeam::GetSlotByType(ARENA_TEAM_1v1);
		uint8 arenatype = ARENA_TYPE_1v1;
		uint32 arenaRating = 0;
		uint32 matchmakerRating = 0;
		bool isRated = true;

		// ignore if we already in BG or BG queue
		if (player->InBattleground())
			return false;

		//check existance
		Battleground* bg = sBattlegroundMgr->GetBattlegroundTemplate(BATTLEGROUND_AA);
		if (!bg)
		{
			//TC_LOG_ERROR(LOG_FILTER_NETWORKIO, "Battleground: template bg (all arenas) not found");
			return false;
		}

		if (DisableMgr::IsDisabledFor(DISABLE_TYPE_BATTLEGROUND, BATTLEGROUND_AA, NULL))
		{
			ChatHandler(player->GetSession()).PSendSysMessage(LANG_ARENA_DISABLED);
			return false;
		}

		BattlegroundTypeId bgTypeId = bg->GetTypeID();
		BattlegroundQueueTypeId bgQueueTypeId = BattlegroundMgr::BGQueueTypeId(bgTypeId, arenatype);
		PvPDifficultyEntry const* bracketEntry = GetBattlegroundBracketByLevel(bg->GetMapId(), player->getLevel());
		if (!bracketEntry)
			return false;

		GroupJoinBattlegroundResult err = ERR_GROUP_JOIN_BATTLEGROUND_FAIL;

		// check if already in queue
        if (player->GetBattlegroundQueueIndex(bgQueueTypeId) < PLAYER_MAX_BATTLEGROUND_QUEUES)
            //player is already in this queue
            return false;
        // check if has free queue slots
        if (!player->HasFreeBattlegroundQueueId())
            return false;

		uint32 ateamId = 0;

		ateamId = player->GetArenaTeamId(arenaslot);
        ArenaTeam* at = sArenaTeamMgr->GetArenaTeamById(ateamId);
        if (!at)
        {
            player->GetSession()->SendNotInArenaTeamPacket(arenatype);
            return false;
        }
        // get the team rating for queueing
        arenaRating = at->GetRating();
        matchmakerRating = arenaRating;
        // the arenateam id must match for everyone in the group

        if (arenaRating <= 0)
            arenaRating = 1;

		BattlegroundQueue &bgQueue = sBattlegroundMgr->GetBattlegroundQueue(bgQueueTypeId);
		bg->SetRated(isRated);

		GroupQueueInfo* ginfo = bgQueue.AddGroup(player, NULL, bgTypeId, bracketEntry, arenatype, isRated, false, arenaRating, matchmakerRating, ateamId);
        uint32 avgTime = bgQueue.GetAverageQueueWaitTime(ginfo, bracketEntry->GetBracketId());
        uint32 queueSlot = player->AddBattlegroundQueueId(bgQueueTypeId);

        WorldPacket data;
        // send status packet (in queue)
        sBattlegroundMgr->BuildBattlegroundStatusPacket(&data, bg, queueSlot, STATUS_WAIT_QUEUE, avgTime, 0, arenatype, 0);
		player->GetSession()->SendPacket(&data);

		sBattlegroundMgr->ScheduleQueueUpdate(matchmakerRating, arenatype, bgQueueTypeId, bgTypeId, bracketEntry->GetBracketId());

		return true;
	}


	bool CreateArenateam(Player* player, Creature* me)
	{
		uint8 slot = ArenaTeam::GetSlotByType(ARENA_TEAM_1v1);
		if (slot >= MAX_ARENA_SLOT)
			return false;

		// Check if player is already in an arena team
		if (player->GetArenaTeamId(slot))
		{
			player->GetSession()->SendArenaTeamCommandResult(ERR_ARENA_TEAM_CREATE_S, player->GetName(), "", ERR_ALREADY_IN_ARENA_TEAM);
			return false;
		}


		// Teamname = playername
		// if teamname exist, we have to choose another name (playername  number)
		int i = 1;
		std::stringstream teamName;
		teamName << player->GetName();
		do
		{
			if(sArenaTeamMgr->GetArenaTeamByName(teamName.str()) != NULL) // teamname exist, so choose another name
			{
				teamName.str(std::string());
				teamName << player->GetName() << i;
			}
			else
				break;
		} while (i < 100); // should never happen

		// Create arena team
		ArenaTeam* arenaTeam = new ArenaTeam();

		if (!arenaTeam->Create(player->GetGUID(), ARENA_TEAM_1v1, teamName.str(), 0, 0, 0, 0, 0))
		{
			delete arenaTeam;
			return false;
		}

		// Register arena team
		sArenaTeamMgr->AddArenaTeam(arenaTeam);
		arenaTeam->AddMember(player->GetGUID());

		ChatHandler(player->GetSession()).SendSysMessage("1v1 Arenateam successful created!");

		return true;
	}


	bool OnGossipHello(Player* player, Creature* me)
	{
		if(player->GetArenaTeamId(ArenaTeam::GetSlotByType(ARENA_TEAM_1v1)) == NULL)
			player->ADD_GOSSIP_ITEM_EXTENDED(GOSSIP_ICON_CHAT, "|TInterface/ICONS/Achievement_Arena_2v2_7:30|t Create 1v1 Rated Arena Team", GOSSIP_SENDER_MAIN, 1, "Create 1v1 arena team?", ARENA_1V1_COST, false);
		else
		{
			if(player->InBattlegroundQueueForBattlegroundQueueType(BATTLEGROUND_QUEUE_1v1))
				player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "|TInterface/ICONS/Achievement_Arena_2v2_7:30|t Leave 1v1 Arena", GOSSIP_SENDER_MAIN, 3);
			else
			{
				player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "|TInterface/ICONS/Achievement_Arena_3v3_5:30|t Join 1v1 Rated Arena", GOSSIP_SENDER_MAIN, 2);
				player->ADD_GOSSIP_ITEM_EXTENDED(GOSSIP_ICON_CHAT, "|TInterface/ICONS/Achievement_Arena_2v2_7:30|t Disband Arena team", GOSSIP_SENDER_MAIN, 5, "Are you sure?", 0, false);
			}

			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "|TInterface/ICONS/INV_Misc_Coin_01:30|t Show statistics", GOSSIP_SENDER_MAIN, 4);
		}

		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "|TInterface/ICONS/INV_Misc_Coin_03:30|t How to Use NPC?", GOSSIP_SENDER_MAIN, 8);
		player->SEND_GOSSIP_MENU(68, me->GetGUID());
		return true;
	}



	bool OnGossipSelect(Player* player, Creature* me, uint32 /*uiSender*/, uint32 uiAction)
	{
		player->PlayerTalkClass->ClearMenus();

		switch (uiAction)
        {
		case 1: // Create new Arenateam
			{
				if(ARENA_1V1_MIN_LEVEL <= player->getLevel())
				{
					if(player->GetMoney() >= ARENA_1V1_COST && CreateArenateam(player, me))
						player->ModifyMoney(-(int32)ARENA_1V1_COST);
				}
				else
				{
					ChatHandler(player->GetSession()).PSendSysMessage("You need level %u+ to create an 1v1 Arena Team.", ARENA_1V1_MIN_LEVEL);
					player->CLOSE_GOSSIP_MENU();
					return true;
				}
			}
			break;

		case 2: // Join Queue Arena
			{
				if(JoinQueueArena(player, me) == false)
					ChatHandler(player->GetSession()).SendSysMessage("Something went wrong while join queue.");
				player->CLOSE_GOSSIP_MENU();
				return true;
			}
			break;

		case 3: // Leave Queue
			{
				WorldPacket Data;
				Data << (uint8)0x1 << (uint8)0x0 << (uint32)BATTLEGROUND_AA << (uint16)0x0 << (uint8)0x0;
				player->GetSession()->HandleBattleFieldPortOpcode(Data);
				player->CLOSE_GOSSIP_MENU();
				return true;
			}
			break;

		case 4: // get statistics
			{
				ArenaTeam* at = sArenaTeamMgr->GetArenaTeamById(player->GetArenaTeamId(ArenaTeam::GetSlotByType(ARENA_TEAM_1v1)));
				if(at)
				{
					std::stringstream s;
					s << "Rating: " << at->GetStats().Rating;
					s << "\nRank: " << at->GetStats().Rank;
					s << "\nSeason Games: " << at->GetStats().SeasonGames;
					s << "\nSeason Wins: " << at->GetStats().SeasonWins;
					s << "\nWeek Games: " << at->GetStats().WeekGames;
					s << "\nWeek Wins: " << at->GetStats().WeekWins;

					ChatHandler(player->GetSession()).PSendSysMessage(s.str().c_str());
				}
			}
			break;


		case 5: // Disband arenateam
			{
				WorldPacket Data;
				Data << (uint32)player->GetArenaTeamId(ArenaTeam::GetSlotByType(ARENA_TEAM_1v1));
				player->GetSession()->HandleArenaTeamLeaveOpcode(Data);
				ChatHandler(player->GetSession()).PSendSysMessage("Arena team deleted!");
				player->CLOSE_GOSSIP_MENU();
				return true;
			}
			break;

		case 8: // Script Info
			{
				player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Click on Create new 1v1 Arena team", GOSSIP_SENDER_MAIN, uiAction);
				player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Join 1v1 Arena and ready!", GOSSIP_SENDER_MAIN, uiAction);
				player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Enjoy!", GOSSIP_SENDER_MAIN, uiAction);
				player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "<- Back", GOSSIP_SENDER_MAIN, 7);
				player->SEND_GOSSIP_MENU(68, me->GetGUID());
				return true;
			}
			break;

		}

		OnGossipHello(player, me);
		return true;
	}
};

class Beastmaster_Trainer : public CreatureScript
{
public:
	Beastmaster_Trainer() : CreatureScript("Beastmaster_Trainer") {}

	void CreatePet(Player* player, Creature* creature, uint32 entry)
	{
		Creature* creatureTarget = creature->SummonCreature(entry, player->GetPositionX(), player->GetPositionY() + 2, player->GetPositionZ(), player->GetOrientation(), TEMPSUMMON_CORPSE_TIMED_DESPAWN, 500);
		if (!creatureTarget)
			return;

		Pet* pet = player->CreateTamedPetFrom(creatureTarget, 0);
		if (!pet)
			return;

		creatureTarget->setDeathState(JUST_DIED);
		creatureTarget->RemoveCorpse();
		creatureTarget->SetHealth(0);

		pet->SetPower(POWER_HAPPINESS, 10480000);

		pet->SetUInt64Value(UNIT_FIELD_CREATEDBY, player->GetGUID());
		pet->SetUInt64Value(UNIT_FIELD_FACTIONTEMPLATE, player->getFaction());
		pet->SetUInt64Value(UNIT_FIELD_LEVEL, player->getLevel());

		pet->GetMap()->AddToMap(pet->ToCreature());

		pet->GetCharmInfo()->SetPetNumber(sObjectMgr->GeneratePetNumber(), true);
		if (!pet->InitStatsForLevel(player->getLevel()))
			TC_LOG_INFO("misc", "Failure: No Init Stats for entry %u", entry);

		pet->UpdateAllStats();
		player->SetMinion(pet, true);

		pet->SavePetToDB(PET_SAVE_AS_CURRENT);
		pet->InitTalentForLevel();
		player->PetSpellInitialize();

		player->CLOSE_GOSSIP_MENU();
		//creature->Whisper("You have learned a way of the beast, congratulations.", LANG_UNIVERSAL, player);
	}

	bool OnGossipHello(Player* player, Creature* creature)
	{
		if (player->getClass() != CLASS_HUNTER)
		{
			//creature->Whisper("I can only train Hunters in the way of the beast.", LANG_UNIVERSAL, player);
			player->CLOSE_GOSSIP_MENU();
			return false;
		}

		if (player->GetPet())
		{
			//creature->Whisper("You must leave your current pet in order to gain a new one.", LANG_UNIVERSAL, player);
			player->CLOSE_GOSSIP_MENU();
			return false;
		}

		player->ADD_GOSSIP_ITEM(1, "Obtain a New Pet", GOSSIP_SENDER_MAIN, 30);
		if (player->CanTameExoticPets())
			player->ADD_GOSSIP_ITEM(2, "Obtain a New Exotic Pet", GOSSIP_SENDER_MAIN, 50);
		player->ADD_GOSSIP_ITEM(5, "Nevermind!", GOSSIP_SENDER_MAIN, 150);

		player->SEND_GOSSIP_MENU(1, creature->GetGUID());
		return true;
	}

	bool OnGossipSelect(Player* player, Creature* creature, uint32 /*sender*/, uint32 action)
	{
		player->PlayerTalkClass->ClearMenus();

		switch (action)
		{

		case 100:
			OnGossipHello(player, creature);
			break;

		case 150:
			player->CLOSE_GOSSIP_MENU();
			break;

		case 30:
			player->ADD_GOSSIP_ITEM(6, "Bat", GOSSIP_SENDER_MAIN, 18);
			player->ADD_GOSSIP_ITEM(6, "Bear", GOSSIP_SENDER_MAIN, 1);
			player->ADD_GOSSIP_ITEM(6, "Boar", GOSSIP_SENDER_MAIN, 2);
			player->ADD_GOSSIP_ITEM(6, "Cat", GOSSIP_SENDER_MAIN, 4);
			player->ADD_GOSSIP_ITEM(6, "Carrion Bird", GOSSIP_SENDER_MAIN, 5);
			player->ADD_GOSSIP_ITEM(6, "Crab", GOSSIP_SENDER_MAIN, 6);
			player->ADD_GOSSIP_ITEM(6, "Crocolisk", GOSSIP_SENDER_MAIN, 7);
			player->ADD_GOSSIP_ITEM(6, "Dragonhawk", GOSSIP_SENDER_MAIN, 17);
			player->ADD_GOSSIP_ITEM(6, "Gorilla", GOSSIP_SENDER_MAIN, 8);
			player->ADD_GOSSIP_ITEM(6, "Hound", GOSSIP_SENDER_MAIN, 9);
			player->ADD_GOSSIP_ITEM(6, "Hyena", GOSSIP_SENDER_MAIN, 10);
			player->ADD_GOSSIP_ITEM(6, "Moth", GOSSIP_SENDER_MAIN, 11);
			player->ADD_GOSSIP_ITEM(6, "Owl", GOSSIP_SENDER_MAIN, 12);
			player->ADD_GOSSIP_ITEM(4, "Next Page ->", GOSSIP_SENDER_MAIN, 31);
			player->ADD_GOSSIP_ITEM(4, "<- Main Menu", GOSSIP_SENDER_MAIN, 100);
			player->ADD_GOSSIP_ITEM(5, "Nevermind!", GOSSIP_SENDER_MAIN, 150);
			player->SEND_GOSSIP_MENU(1, creature->GetGUID());
			break;

		case 31:
			player->ADD_GOSSIP_ITEM(6, "Raptor", GOSSIP_SENDER_MAIN, 20);
			player->ADD_GOSSIP_ITEM(6, "Ravager", GOSSIP_SENDER_MAIN, 19);
			player->ADD_GOSSIP_ITEM(6, "Strider", GOSSIP_SENDER_MAIN, 13);
			player->ADD_GOSSIP_ITEM(6, "Scorpid", GOSSIP_SENDER_MAIN, 414);
			player->ADD_GOSSIP_ITEM(6, "Spider", GOSSIP_SENDER_MAIN, 16);
			player->ADD_GOSSIP_ITEM(6, "Serpent", GOSSIP_SENDER_MAIN, 21);
			player->ADD_GOSSIP_ITEM(6, "Turtle", GOSSIP_SENDER_MAIN, 15);
			player->ADD_GOSSIP_ITEM(6, "Wasp", GOSSIP_SENDER_MAIN, 93);
			player->ADD_GOSSIP_ITEM(4, "<- Previous Page", GOSSIP_SENDER_MAIN, 30);
			player->ADD_GOSSIP_ITEM(4, "<- Main Menu", GOSSIP_SENDER_MAIN, 100);
			player->ADD_GOSSIP_ITEM(5, "Nevermind!", GOSSIP_SENDER_MAIN, 150);
			player->SEND_GOSSIP_MENU(1, creature->GetGUID());
			break;

		case 50:
			player->ADD_GOSSIP_ITEM(6, "Chimaera", GOSSIP_SENDER_MAIN, 51);
			player->ADD_GOSSIP_ITEM(6, "Core Hound", GOSSIP_SENDER_MAIN, 52);
			player->ADD_GOSSIP_ITEM(6, "Devilsaur", GOSSIP_SENDER_MAIN, 53);
			player->ADD_GOSSIP_ITEM(6, "Rhino", GOSSIP_SENDER_MAIN, 54);
			player->ADD_GOSSIP_ITEM(6, "Silithid", GOSSIP_SENDER_MAIN, 55);
			player->ADD_GOSSIP_ITEM(6, "Worm", GOSSIP_SENDER_MAIN, 56);
			player->ADD_GOSSIP_ITEM(6, "Loque'nahak", GOSSIP_SENDER_MAIN, 57);
			player->ADD_GOSSIP_ITEM(6, "Skoll", GOSSIP_SENDER_MAIN, 58);
			player->ADD_GOSSIP_ITEM(6, "Gondri.", GOSSIP_SENDER_MAIN, 59);
			player->ADD_GOSSIP_ITEM(4, "<- Main Menu", GOSSIP_SENDER_MAIN, 100);
			player->ADD_GOSSIP_ITEM(5, "Nevermind!", GOSSIP_SENDER_MAIN, 150);
			player->SEND_GOSSIP_MENU(1, creature->GetGUID());
			break;

			// Credits to Serifaz who helped me with these ID's.
		case 51: // Chimera
			CreatePet(player, creature, 21879);
			break;

		case 52: // Core hound
			CreatePet(player, creature, 21108);
			break;

		case 53: // Devilsaur
			CreatePet(player, creature, 20931);
			break;

		case 54: // Rhino
			CreatePet(player, creature, 30445);
			break;

		case 55: // Silithid
			CreatePet(player, creature, 5460);
			break;

		case 56: // Worm
			CreatePet(player, creature, 30148);
			break;

		case 57: // Loque'nahak
			CreatePet(player, creature, 32517);
			break;

		case 58: // Skoll
			CreatePet(player, creature, 35189);
			break;

		case 59: // Gondria
			CreatePet(player, creature, 33776);
			break;

		case 16: // Spider
			CreatePet(player, creature, 2349);
			break;

		case 17: // Dragonhawk
			CreatePet(player, creature, 27946);
			break;

		case 18: // Bat
			CreatePet(player, creature, 28233);
			break;

		case 19: // Ravager
			CreatePet(player, creature, 17199);
			break;

		case 20: // Raptor
			CreatePet(player, creature, 14821);
			break;

		case 21: // Serpent
			CreatePet(player, creature, 28358);
			break;

		case 1: // Bear
			CreatePet(player, creature, 29319);
			break;

		case 2: // Boar
			CreatePet(player, creature, 29996);
			break;

		case 93: // Bug
			CreatePet(player, creature, 28085);
			break;

		case 4: // Cat
			CreatePet(player, creature, 28097);
			break;

		case 5: // Carrion
			CreatePet(player, creature, 26838);
			break;

		case 6: // Crab
			CreatePet(player, creature, 24478);
			break;

		case 7: // Crocolisk
			CreatePet(player, creature, 1417);
			break;

		case 8: // Gorila
			CreatePet(player, creature, 28213);
			break;

		case 9: // Hound
			CreatePet(player, creature, 29452);
			break;

		case 10: // Hyena
			CreatePet(player, creature, 13036);
			break;

		case 11: // Moth
			CreatePet(player, creature, 27421);
			break;

		case 12: // Owl
			CreatePet(player, creature, 23136);
			break;

		case 13: // Strider
			CreatePet(player, creature, 22807);
			break;

		case 414: // Scorpid
			CreatePet(player, creature, 9698);
			break;

		case 15: // Turtle
			CreatePet(player, creature, 25482);
			break;
		}

		return true;
	}
};

uint32 WARRIOR_GLYPH_VENDOR      = 0;
uint32 PALADIN_GLYPH_VENDOR      = 0;
uint32 HUNTER_GLYPH_VENDOR       = 0;
uint32 ROGUE_GLYPH_VENDOR        = 0;
uint32 PRIEST_GLYPH_VENDOR       = 0;
uint32 DEATH_KNIGHT_GLYPH_VENDOR = 0;
uint32 SHAMAN_GLYPH_VENDOR       = 0;
uint32 MAGE_GLYPH_VENDOR         = 0;
uint32 WARLOCK_GLYPH_VENDOR      = 0;
uint32 DRUID_GLYPH_VENDOR        = 0;

#include "ScriptPCH.h"

class npc_trainer : public CreatureScript
{
public:
  npc_trainer() : CreatureScript("npc_trainer") { }

  bool OnGossipHello(Player* player, Creature* creature)
  {
    player->ADD_GOSSIP_ITEM(3, "Train me", GOSSIP_SENDER_MAIN, 1);
    player->ADD_GOSSIP_ITEM(1, "Glyph Vendor.", GOSSIP_SENDER_MAIN, 2);
    player->SEND_GOSSIP_MENU(1, creature->GetGUID());
    return true;
  }

  bool OnGossipSelect(Player* player, Creature* creature, uint32 sender, uint32 action)
  {
    player->PlayerTalkClass->ClearMenus();
    
    if (sender == GOSSIP_SENDER_MAIN)
    {
      switch (action)
      {
      case 1: // Train Me.
        player->CastSpell(player,63680,true,NULL,NULL,player->GetGUID());
        player->CastSpell(player,63624,true,NULL,NULL,player->GetGUID());

        switch(player->getClass())
        {
        case CLASS_WARRIOR:
          player->learnSpell(7384, false);
          player->learnSpell(47436, false);
          player->learnSpell(47450, false);
          player->learnSpell(11578, false);
          player->learnSpell(47465, false);
          player->learnSpell(47502, false);
          player->learnSpell(34428, false);
          player->learnSpell(1715, false);
          player->learnSpell(2687, false);
          player->learnSpell(71, false);
          player->learnSpell(7386, false);
          player->learnSpell(355, false);
          player->learnSpell(72, false);
          player->learnSpell(47437, false);
          player->learnSpell(57823, false);
          player->learnSpell(694, false);
          player->learnSpell(2565, false);
          player->learnSpell(676, false);
          player->learnSpell(47520, false);
          player->learnSpell(20230, false);
          player->learnSpell(12678, false);
          player->learnSpell(47471, false);
          player->learnSpell(1161, false);
          player->learnSpell(871, false);
          player->learnSpell(2458, false);
          player->learnSpell(20252, false);
          player->learnSpell(47475, false);
          player->learnSpell(18499, false);
          player->learnSpell(1680, false);
          player->learnSpell(6552, false);
          player->learnSpell(47488, false);
          player->learnSpell(1719, false);
          player->learnSpell(23920, false);
          player->learnSpell(47440, false);
          player->learnSpell(3411, false);
          player->learnSpell(64382, false);
          player->learnSpell(55694, false);
          player->learnSpell(57755, false);
          player->learnSpell(42459, false);
          player->learnSpell(750, false);
          player->learnSpell(5246, false);
          player->learnSpell(3127, false);
          player->learnSpell(34093, false);
          player->learnSpell(33392, false);
          if(player->HasSpell(12294)) 
            player->learnSpell(47486, false);
          if(player->HasSpell(20243)) 
            player->learnSpell(47498, false);
          player->SaveToDB();
          player->SEND_GOSSIP_MENU(1, creature->GetGUID());
          player->PlayerTalkClass->SendCloseGossip();
          break;

        case CLASS_PALADIN:
          player->learnSpell(3127, false);
          player->learnSpell(19746, false);
          player->learnSpell(19752, false);
          player->learnSpell(750,   false);
          player->learnSpell(48942, false);
          player->learnSpell(48782, false);
          player->learnSpell(48932, false);
          player->learnSpell(20271, false);
          player->learnSpell(498,   false);
          player->learnSpell(10308, false);
          player->learnSpell(1152,  false);
          player->learnSpell(10278, false);
          player->learnSpell(48788, false);
          player->learnSpell(53408, false);
          player->learnSpell(48950, false);
          player->learnSpell(48936, false);
          player->learnSpell(31789, false);
          player->learnSpell(62124, false);
          player->learnSpell(54043, false);
          player->learnSpell(25780, false);
          player->learnSpell(1044,  false);
          player->learnSpell(20217, false);
          player->learnSpell(48819, false);
          player->learnSpell(48801, false);
          player->learnSpell(48785, false);
          player->learnSpell(5502,  false);
          player->learnSpell(20164, false);
          player->learnSpell(10326, false);
          player->learnSpell(1038,  false);
          player->learnSpell(53407, false);
          player->learnSpell(48943, false);
          player->learnSpell(20165, false);
          player->learnSpell(48945, false);
          player->learnSpell(642,   false);
          player->learnSpell(48947, false);
          player->learnSpell(20166, false);
          player->learnSpell(4987,  false);
          player->learnSpell(48806, false);
          player->learnSpell(6940,  false);
          player->learnSpell(48817, false);
          player->learnSpell(48934, false);
          player->learnSpell(48938, false);
          player->learnSpell(25898, false);
          player->learnSpell(32223, false);
          player->learnSpell(31884, false);
          player->learnSpell(54428, false);
          player->learnSpell(61411, false);
          player->learnSpell(53601, false);
          player->learnSpell(33388, false);
          player->learnSpell(33391, false);
          player->learnSpell(34093, false);
          player->learnSpell(33392, false);
          if(player->HasSpell(20925)) 
            player->learnSpell(48952, false);
          if(player->HasSpell(31935)) 
            player->learnSpell(48827, false);
          if(player->HasSpell(20911)) 
            player->learnSpell(25899, false); 
          if(player->HasSpell(20473)) 
            player->learnSpell(48825, false);
          if(player->GetTeam() == ALLIANCE)
          {
            player->learnSpell(31801, false); 
            player->learnSpell(13819, false); 
            player->learnSpell(23214, false); 
          }
          if(player->GetTeam() == HORDE)
          {
            player->learnSpell(53736, false); 
            player->learnSpell(34769, false); 
            player->learnSpell(34767, false); 
          }
          player->SaveToDB();
          player->SEND_GOSSIP_MENU(1, creature->GetGUID());
          player->PlayerTalkClass->SendCloseGossip();
          break;

        case CLASS_HUNTER:
          player->learnSpell(3043, false);
          player->learnSpell(3127, false);
          player->learnSpell(3045,  false);
          player->learnSpell(3034,  false);
          player->learnSpell(8737,  false);
          player->learnSpell(1494,  false);
          player->learnSpell(13163, false);
          player->learnSpell(48996, false);
          player->learnSpell(49001, false);
          player->learnSpell(49045, false);
          player->learnSpell(53338, false);
          player->learnSpell(5116,  false);
          player->learnSpell(27044, false);
          player->learnSpell(883,   false);
          player->learnSpell(2641,  false);
          player->learnSpell(6991,  false);
          player->learnSpell(982,   false);
          player->learnSpell(1515,  false);
          player->learnSpell(19883, false);
          player->learnSpell(20736, false);
          player->learnSpell(48990, false);
          player->learnSpell(2974,  false);
          player->learnSpell(6197,  false);
          player->learnSpell(1002,  false);
          player->learnSpell(14327, false);
          player->learnSpell(5118,  false);
          player->learnSpell(49056, false);
          player->learnSpell(53339, false);
          player->learnSpell(49048, false);
          player->learnSpell(19884, false);
          player->learnSpell(34074, false);
          player->learnSpell(781,   false);
          player->learnSpell(14311, false);
          player->learnSpell(1462,  false);
          player->learnSpell(19885, false);
          player->learnSpell(19880, false);
          player->learnSpell(13809, false);
          player->learnSpell(13161, false);
          player->learnSpell(5384,  false);
          player->learnSpell(1543,  false);
          player->learnSpell(19878, false);
          player->learnSpell(49067, false);
          player->learnSpell(3034,  false);
          player->learnSpell(13159, false);
          player->learnSpell(19882, false);
          player->learnSpell(58434, false);
          player->learnSpell(49071, false);
          player->learnSpell(49052, false);
          player->learnSpell(19879, false);
          player->learnSpell(19263, false);
          player->learnSpell(19801, false);
          player->learnSpell(34026, false);
          player->learnSpell(34600, false);
          player->learnSpell(34477, false);
          player->learnSpell(61006, false);
          player->learnSpell(61847, false);
          player->learnSpell(53271, false);
          player->learnSpell(60192, false);
          player->learnSpell(62757, false);
          player->learnSpell(34093, false);
          player->learnSpell(33392, false);
          if(player->HasSpell(19386)) 
            player->learnSpell(49012, false);
          if(player->HasSpell(53301)) 
            player->learnSpell(60053, false);
          if(player->HasSpell(19306)) 
            player->learnSpell(48999, false);
          if(player->HasSpell(19434)) 
            player->learnSpell(49050, false);
          player->SaveToDB();
          player->SEND_GOSSIP_MENU(1, creature->GetGUID());
          player->PlayerTalkClass->SendCloseGossip();
          break;

        case CLASS_MAGE:
          player->learnSpell(42921, false);
          player->learnSpell(42842, false);
          player->learnSpell(42995, false);
          player->learnSpell(42833, false);
          player->learnSpell(27090, false);
          player->learnSpell(33717, false);
          player->learnSpell(42873, false);
          player->learnSpell(42846, false);
          player->learnSpell(12826, false);
          player->learnSpell(28271, false);
          player->learnSpell(61780, false);
          player->learnSpell(61721, false);
          player->learnSpell(28272, false);
          player->learnSpell(42917, false);
          player->learnSpell(43015, false);
          player->learnSpell(130,   false);
          player->learnSpell(42926, false);
          player->learnSpell(43017, false);
          player->learnSpell(475,   false);
          player->learnSpell(1953,  false);
          player->learnSpell(42940, false);
          player->learnSpell(12051, false);
          player->learnSpell(43010, false);
          player->learnSpell(43020, false);
          player->learnSpell(43012, false);
          player->learnSpell(42859, false);
          player->learnSpell(2139,  false);
          player->learnSpell(42931, false);
          player->learnSpell(42985, false);
          player->learnSpell(43008, false);
          player->learnSpell(45438, false);
          player->learnSpell(43024, false);
          player->learnSpell(43002, false);
          player->learnSpell(43046, false);
          player->learnSpell(42897, false);
          player->learnSpell(42914, false);
          player->learnSpell(66,    false);
          player->learnSpell(58659, false);
          player->learnSpell(30449, false);
          player->learnSpell(42956, false);
          player->learnSpell(47610, false);
          player->learnSpell(61316, false);
          player->learnSpell(61024, false);
          player->learnSpell(55342, false);
          player->learnSpell(53142, false);
          player->learnSpell(7301,  false);
          player->learnSpell(34093, false);
          player->learnSpell(33392, false);
          if(player->GetTeam() == ALLIANCE)
          {
            player->learnSpell(32271, false); // Teleport: Exodar.
            player->learnSpell(49359, false); // Teleport: Theramore.
            player->learnSpell(3565,  false); // Teleport: Darnassus.
            player->learnSpell(33690, false); // Teleport: Shattrath.
            player->learnSpell(3562,  false); // Teleport: Ironforge.
            player->learnSpell(3561,  false); // Teleport: Stormwind.
            player->learnSpell(53140, false); // Teleport: Dalaran.
            player->learnSpell(53142, false); // Portal: Dalaran.
            player->learnSpell(10059, false); // Portal: Stormwind.
            player->learnSpell(11419, false); // Portal: Darnassus.
            player->learnSpell(32266, false); // Portal: Exodar.
            player->learnSpell(11416, false); // Portal: Ironforge.
            player->learnSpell(33691, false); // Portal: Shattrath.
            player->learnSpell(49360, false); // Portal: Theramore.
          }
          if(player->GetTeam() == HORDE)
          {
            player->learnSpell(3567,  false); // Teleport: Orgrimmar.
            player->learnSpell(35715, false); // Teleport: Shattrath.
            player->learnSpell(3566,  false); // Teleport: Thunder Bluff.
            player->learnSpell(49358, false); // Teleport: Stonard.
            player->learnSpell(32272, false); // Teleport: Silvermoon.
            player->learnSpell(3563,  false); // Teleport: Undercity.
            player->learnSpell(53140, false); // Teleport: Dalaran.
            player->learnSpell(53142, false); // Portal: Dalaran.
            player->learnSpell(11417, false); // Portal: Orgrimmar.
            player->learnSpell(35717, false); // Portal: Shattrath.
            player->learnSpell(32267, false); // Portal: Silvermoon.
            player->learnSpell(49361, false); // Portal: Stonard.
            player->learnSpell(11420, false); // Portal: Thunder Bluff.
            player->learnSpell(11418, false); // Portal: Undercity.
          }
          if(player->HasSpell(11366)) 
            player->learnSpell(42891, false);
          if(player->HasSpell(11426)) 
            player->learnSpell(43039, false);
          if(player->HasSpell(44457)) 
            player->learnSpell(55360, false);
          if(player->HasSpell(31661)) 
            player->learnSpell(42950, false);
          if(player->HasSpell(11113)) 
            player->learnSpell(42945, false);
          if(player->HasSpell(44425)) 
            player->learnSpell(44781, false);
          player->SaveToDB();
          player->PlayerTalkClass->SendCloseGossip();
          break;

        case CLASS_WARLOCK:
          player->learnSpell(696, false);
          player->learnSpell(47811, false);
          player->learnSpell(47809, false);
          player->learnSpell(688, false);
          player->learnSpell(47813, false);
          player->learnSpell(50511, false);
          player->learnSpell(57946, false);
          player->learnSpell(47864, false);
          player->learnSpell(6215, false);
          player->learnSpell(47878, false);
          player->learnSpell(47855, false);
          player->learnSpell(697, false);
          player->learnSpell(47856, false);
          player->learnSpell(47857, false);
          player->learnSpell(5697, false);
          player->learnSpell(47884, false);
          player->learnSpell(47815, false);
          player->learnSpell(47889, false);
          player->learnSpell(47820, false);
          player->learnSpell(698, false);
          player->learnSpell(712, false);
          player->learnSpell(126, false);
          player->learnSpell(5138, false);
          player->learnSpell(5500, false);
          player->learnSpell(11719, false);
          player->learnSpell(132, false);
          player->learnSpell(60220, false);
          player->learnSpell(18647, false);
          player->learnSpell(61191, false);
          player->learnSpell(47823, false);
          player->learnSpell(691, false);
          player->learnSpell(47865, false);
          player->learnSpell(47891, false);
          player->learnSpell(47888, false);
          player->learnSpell(17928, false);
          player->learnSpell(47860, false);
          player->learnSpell(47825, false);
          player->learnSpell(1122, false);
          player->learnSpell(47867, false);
          player->learnSpell(18540, false);
          player->learnSpell(47893, false);
          player->learnSpell(47838, false);
          player->learnSpell(29858, false);
          player->learnSpell(58887, false);
          player->learnSpell(47836, false);
          player->learnSpell(61290, false);
          player->learnSpell(48018, false);
          player->learnSpell(48020, false);
          player->learnSpell(33388, false);
          player->learnSpell(33391, false);
          player->learnSpell(23161, false);
          player->learnSpell(34093, false);
          player->learnSpell(33392, false);
          if(player->HasSpell(17877)) 
            player->learnSpell(47827, false);
          if(player->HasSpell(30283)) 
            player->learnSpell(47847, false);
          if(player->HasSpell(30108)) 
            player->learnSpell(47843, false);
          if(player->HasSpell(50796)) 
            player->learnSpell(59172, false);
          if(player->HasSpell(48181)) 
            player->learnSpell(59164, false);
          if(player->HasSpell(18220)) 
            player->learnSpell(59092, false);
          player->SaveToDB();
          player->PlayerTalkClass->SendCloseGossip();
          break;

        case CLASS_ROGUE:
          player->learnSpell(3127, false);
          player->learnSpell(42459, false);
          player->learnSpell(48668, false);
          player->learnSpell(48638, false);
          player->learnSpell(1784, false);
          player->learnSpell(48657, false);
          player->learnSpell(921, false);
          player->learnSpell(1776, false);
          player->learnSpell(26669, false);
          player->learnSpell(51724, false);
          player->learnSpell(6774, false);
          player->learnSpell(11305, false);
          player->learnSpell(1766, false);
          player->learnSpell(48676, false);
          player->learnSpell(48659, false);
          player->learnSpell(1804, false);
          player->learnSpell(8647, false);
          player->learnSpell(48691, false);
          player->learnSpell(51722, false);
          player->learnSpell(48672, false);
          player->learnSpell(1725, false);
          player->learnSpell(26889, false);
          player->learnSpell(2836, false);
          player->learnSpell(1833, false);
          player->learnSpell(1842, false);
          player->learnSpell(8643, false);
          player->learnSpell(2094, false);
          player->learnSpell(1860, false);
          player->learnSpell(57993, false);
          player->learnSpell(48674, false);
          player->learnSpell(31224, false);
          player->learnSpell(5938, false);
          player->learnSpell(57934, false);
          player->learnSpell(51723, false);
          player->learnSpell(34093, false);
          player->learnSpell(33392, false);
          if(player->HasSpell(16511)) 
            player->learnSpell(48660, false);
          if(player->HasSpell(1329)) 
            player->learnSpell(48666, false);
          player->SaveToDB();
          player->PlayerTalkClass->SendCloseGossip();
          break;

        case CLASS_PRIEST:
          player->learnSpell(528, false);
          player->learnSpell(2053, false);
          player->learnSpell(48161, false);
          player->learnSpell(48123, false);
          player->learnSpell(48125, false);
          player->learnSpell(48066, false);
          player->learnSpell(586, false);
          player->learnSpell(48068, false);
          player->learnSpell(48127, false);
          player->learnSpell(48171, false);
          player->learnSpell(48168, false);
          player->learnSpell(10890, false);
          player->learnSpell(6064, false);
          player->learnSpell(988, false);
          player->learnSpell(48300, false);
          player->learnSpell(6346, false);
          player->learnSpell(48071, false);
          player->learnSpell(48135, false);
          player->learnSpell(48078, false);
          player->learnSpell(453, false);
          player->learnSpell(10955, false);
          player->learnSpell(10909, false);
          player->learnSpell(8129, false);
          player->learnSpell(48073, false);
          player->learnSpell(605, false);
          player->learnSpell(48072, false);
          player->learnSpell(48169, false);
          player->learnSpell(552, false);
          player->learnSpell(1706, false);
          player->learnSpell(48063, false);
          player->learnSpell(48162, false);
          player->learnSpell(48170, false);
          player->learnSpell(48074, false);
          player->learnSpell(48158, false);
          player->learnSpell(48120, false);
          player->learnSpell(34433, false);
          player->learnSpell(48113, false);
          player->learnSpell(32375, false);
          player->learnSpell(64843, false);
          player->learnSpell(64901, false);
          player->learnSpell(53023, false);
          player->learnSpell(34093, false);
          player->learnSpell(33392, false);
          if(player->HasSpell(34914)) 
            player->learnSpell(48160, false);
          if(player->HasSpell(47540)) 
            player->learnSpell(53007, false);
          if(player->HasSpell(724)) 
            player->learnSpell(48087, false);
          if(player->HasSpell(19236)) 
            player->learnSpell(48173, false);
          if(player->HasSpell(34861)) 
            player->learnSpell(48089, false);
          if(player->HasSpell(15407)) 
            player->learnSpell(48156, false);
          player->SaveToDB();
          player->PlayerTalkClass->SendCloseGossip();
          break;

        case CLASS_DEATH_KNIGHT:
          player->learnSpell(3127, false);
          player->learnSpell(50842, false);
          player->learnSpell(49941, false);
          player->learnSpell(49930, false);
          player->learnSpell(47476, false);
          player->learnSpell(45529, false);
          player->learnSpell(3714, false);
          player->learnSpell(56222, false);
          player->learnSpell(48743, false);
          player->learnSpell(48263, false);
          player->learnSpell(49909, false);
          player->learnSpell(47528, false);
          player->learnSpell(45524, false);
          player->learnSpell(48792, false);
          player->learnSpell(57623, false);
          player->learnSpell(56815, false);
          player->learnSpell(47568, false);
          player->learnSpell(49895, false);
          player->learnSpell(50977, false);
          player->learnSpell(49576, false);
          player->learnSpell(49921, false);
          player->learnSpell(46584, false);
          player->learnSpell(49938, false);
          player->learnSpell(48707, false);
          player->learnSpell(48265, false);
          player->learnSpell(61999, false);
          player->learnSpell(42650, false);
          player->learnSpell(53428, false);
          player->learnSpell(53331, false);
          player->learnSpell(54447, false);
          player->learnSpell(53342, false);
          player->learnSpell(54446, false);
          player->learnSpell(53323, false);
          player->learnSpell(53344, false);
          player->learnSpell(70164, false);
          player->learnSpell(62158, false);
          player->learnSpell(33391, false);
          player->learnSpell(48778, false);
          player->learnSpell(51425, false);
          player->learnSpell(49924, false);
          player->learnSpell(34093, false);
          player->learnSpell(33392, false);
          if(player->HasSpell(55050)) 
            player->learnSpell(55262, false);
          if(player->HasSpell(49143)) 
            player->learnSpell(55268, false);
          if(player->HasSpell(49184)) 
            player->learnSpell(51411, false);
          if(player->HasSpell(55090)) 
            player->learnSpell(55271, false);
          if(player->HasSpell(49158)) 
            player->learnSpell(51328, false);
          player->SaveToDB();
          player->PlayerTalkClass->SendCloseGossip();
          break;

        case CLASS_SHAMAN:
          player->learnSpell(2062, false);
          player->learnSpell(8737, false);
          player->learnSpell(49273, false);
          player->learnSpell(49238, false);
          player->learnSpell(10399, false);
          player->learnSpell(49231, false);
          player->learnSpell(58753, false);
          player->learnSpell(2484, false);
          player->learnSpell(49281, false);
          player->learnSpell(58582, false);
          player->learnSpell(49233, false);
          player->learnSpell(58790, false);
          player->learnSpell(58704, false);
          player->learnSpell(58643, false);
          player->learnSpell(49277, false);
          player->learnSpell(61657, false);
          player->learnSpell(8012, false);
          player->learnSpell(526, false);
          player->learnSpell(2645, false);
          player->learnSpell(57994, false);
          player->learnSpell(8143, false);
          player->learnSpell(49236, false);
          player->learnSpell(58796, false);
          player->learnSpell(58757, false);
          player->learnSpell(49276, false);
          player->learnSpell(57960, false);
          player->learnSpell(131, false);
          player->learnSpell(58745, false);
          player->learnSpell(6196, false);
          player->learnSpell(58734, false);
          player->learnSpell(58774, false);
          player->learnSpell(58739, false);
          player->learnSpell(58656, false);
          player->learnSpell(546, false);
          player->learnSpell(556, false);
          player->learnSpell(66842, false);
          player->learnSpell(51994, false);
          player->learnSpell(8177, false);
          player->learnSpell(58749, false);
          player->learnSpell(20608, false);
          player->learnSpell(36936, false);
          player->learnSpell(36936, false);
          player->learnSpell(58804, false);
          player->learnSpell(49271, false);
          player->learnSpell(8512, false);
          player->learnSpell(6495, false);
          player->learnSpell(8170, false);
          player->learnSpell(66843, false);
          player->learnSpell(55459, false);
          player->learnSpell(66844, false);
          player->learnSpell(3738, false);
          player->learnSpell(2894, false);
          player->learnSpell(60043, false);
          player->learnSpell(51514, false);
          player->learnSpell(34093, false);
          player->learnSpell(33392, false);
          if(player->GetTeam() == ALLIANCE)
            player->learnSpell(32182, false);
          if(player->GetTeam() == HORDE)
            player->learnSpell(2825, false); 
          if(player->HasSpell(61295)) 
            player->learnSpell(61301, false);
          if(player->HasSpell(974)) 
            player->learnSpell(49284, false);
          if(player->HasSpell(30706)) 
            player->learnSpell(57722, false);
          if(player->HasSpell(51490)) 
            player->learnSpell(59159, false);
          player->SaveToDB();
          player->PlayerTalkClass->SendCloseGossip();
          break;

        case CLASS_DRUID:
          player->learnSpell(48378, false);
          player->learnSpell(48469, false);
          player->learnSpell(48461, false);
          player->learnSpell(48463, false);
          player->learnSpell(48441, false);
          player->learnSpell(53307, false);
          player->learnSpell(53308, false);
          player->learnSpell(5487, false);
          player->learnSpell(48560, false);
          player->learnSpell(6795, false);
          player->learnSpell(48480, false);
          player->learnSpell(53312, false);
          player->learnSpell(18960, false);
          player->learnSpell(5229, false);
          player->learnSpell(48443, false);
          player->learnSpell(50763, false);
          player->learnSpell(8983, false);
          player->learnSpell(8946, false);
          player->learnSpell(1066, false);
          player->learnSpell(48562, false);
          player->learnSpell(783, false);
          player->learnSpell(770, false);
          player->learnSpell(16857, false);
          player->learnSpell(18658, false);
          player->learnSpell(768, false);
          player->learnSpell(1082, false);
          player->learnSpell(16979, false);
          player->learnSpell(49376, false);
          player->learnSpell(5215, false);
          player->learnSpell(48477, false);
          player->learnSpell(49800, false);
          player->learnSpell(48465, false);
          player->learnSpell(48572, false);
          player->learnSpell(26995, false);
          player->learnSpell(48574, false);
          player->learnSpell(2782, false);
          player->learnSpell(50213, false);
          player->learnSpell(2893, false);
          player->learnSpell(33357, false);
          player->learnSpell(5209, false);
          player->learnSpell(48575, false);
          player->learnSpell(48447, false);
          player->learnSpell(48577, false);
          player->learnSpell(48579, false);
          player->learnSpell(5225, false);
          player->learnSpell(22842, false);
          player->learnSpell(49803, false);
          player->learnSpell(9634, false);
          player->learnSpell(20719, false);
          player->learnSpell(48467, false);
          player->learnSpell(29166, false);
          player->learnSpell(62600, false);
          player->learnSpell(22812, false);
          player->learnSpell(48470, false);
          player->learnSpell(48564, false);
          player->learnSpell(48566, false);
          player->learnSpell(33943, false);
          player->learnSpell(49802, false);
          player->learnSpell(48451, false);
          player->learnSpell(48568, false);
          player->learnSpell(33786, false);
          player->learnSpell(40120, false);
          player->learnSpell(62078, false);
          player->learnSpell(52610, false);
          player->learnSpell(50464, false);
          player->learnSpell(48570, false);
          player->learnSpell(34093, false);
          player->learnSpell(33392, false);
          if(player->HasSpell(50516)) 
            player->learnSpell(61384, false);
          if(player->HasSpell(48505)) 
            player->learnSpell(53201, false);
          if(player->HasSpell(48438)) 
            player->learnSpell(53251, false);
          if(player->HasSpell(5570)) 
            player->learnSpell(48468, false);
          player->SaveToDB();
          player->PlayerTalkClass->SendCloseGossip();
          break;
        }
        break;

                case 2: // Glyph Vendor.
          switch(player->getClass())
          {
          case CLASS_WARRIOR:
            player->GetSession()->SendListInventory(player->GetGUID(), WARRIOR_GLYPH_VENDOR);
            break;

          case CLASS_PALADIN:
            player->GetSession()->SendListInventory(player->GetGUID(), PALADIN_GLYPH_VENDOR);
            break;

          case CLASS_HUNTER:
            player->GetSession()->SendListInventory(player->GetGUID(), HUNTER_GLYPH_VENDOR);
            break;

          case CLASS_ROGUE:
            player->GetSession()->SendListInventory(player->GetGUID(), ROGUE_GLYPH_VENDOR);
            break;

          case CLASS_PRIEST:
            player->GetSession()->SendListInventory(player->GetGUID(), PRIEST_GLYPH_VENDOR);
            break;

          case CLASS_DEATH_KNIGHT:
            player->GetSession()->SendListInventory(player->GetGUID(), DEATH_KNIGHT_GLYPH_VENDOR);
            break;

          case CLASS_SHAMAN:
            player->GetSession()->SendListInventory(player->GetGUID(), SHAMAN_GLYPH_VENDOR);
            break;

          case CLASS_MAGE:
            player->GetSession()->SendListInventory(player->GetGUID(), MAGE_GLYPH_VENDOR);
            break;

          case CLASS_WARLOCK:
            player->GetSession()->SendListInventory(player->GetGUID(), WARLOCK_GLYPH_VENDOR);
            break;

          case CLASS_DRUID:
            player->GetSession()->SendListInventory(player->GetGUID(), DRUID_GLYPH_VENDOR);
            break;
          }
          break;
        }
            }
      return true;
  }
};

#include "ScriptPCH.h"

class System_OnKill : public PlayerScript
{
	public:
		System_OnKill() : PlayerScript("System_OnKill") {}

	void OnPVPKill(Player * Killer, Player * Victim)
	{
		uint64 KillerGUID = Killer->GetGUID();
		uint64 VictimGUID = Victim->GetGUID();

		struct KillStreak_Info
		{
			uint32 killstreak;
			uint64 lastkill;
		};

		static std::map<uint64, KillStreak_Info> KillStreakData;

		if( KillerGUID == VictimGUID || KillStreakData[KillerGUID].lastkill == VictimGUID )
			return;

		KillStreakData[KillerGUID].killstreak++;
		KillStreakData[KillerGUID].lastkill = VictimGUID;
		KillStreakData[VictimGUID].killstreak = 0;
		KillStreakData[VictimGUID].lastkill = 0;
	
		switch( KillStreakData[KillerGUID].killstreak )
		{
			char msg[250];
			case 1:
			case 2:
			case 3:
			case 4:
			case 5:
			case 6:
			case 7:
			case 8:
			case 9:
				//Killer->AddItem(29434, 1);
				break;
			
			case 10:
				snprintf(msg, 250, "|cFF0042FF[PvP System]:|CFF20C000[%u Kills]|cFFFF0303%s is on a hot streak!", KillStreakData[KillerGUID].killstreak, Killer->GetName());
				sWorld->SendServerMessage(SERVER_MSG_STRING, msg);
				//Killer->AddItem(29434, 2);
				break;
				
			case 11:
			case 12:
			case 13:
			case 14:
			case 15:
			case 16:
			case 17:
			case 18:
			case 19:
				//Killer->AddItem(29434, 2);
				break;

			case 20:
				snprintf(msg, 250, "[PvP System]:|CFF20C000[%u Kills]|cFFFF0303%s is on a RAMPAGE!!", KillStreakData[KillerGUID].killstreak, Killer->GetName());
				sWorld->SendServerMessage(SERVER_MSG_STRING, msg);
				//Killer->AddItem(29434, 4);
				break;
			
			case 21:
			case 22:
			case 23:
			case 24:
			case 25:
			case 26:
			case 27:
			case 28:
			case 29:
				//Killer->AddItem(29434, 3);
				break;
			
			case 30:
				snprintf(msg, 250, "[PvP System]:|CFF20C000[%u Kills]|cFFFF0303%s is UNSTOPABLE!", KillStreakData[KillerGUID].killstreak, Killer->GetName());
				sWorld->SendServerMessage(SERVER_MSG_STRING, msg);
				//Killer->AddItem(29434, 6);
				break;

			case 31:
			case 32:
			case 33:
			case 34:
			case 35:
			case 36:
			case 37:
			case 38:
			case 39:
				//Killer->AddItem(29434, 4);
				break;
			
			case 40:
				snprintf(msg, 250, "[PvP System]: |CFF20C000[%u Kills]|cFFFF0303%s is Dominating!", KillStreakData[KillerGUID].killstreak, Killer->GetName());
				sWorld->SendServerMessage(SERVER_MSG_STRING, msg);
				//Killer->AddItem(29434, 8);
				break;

			case 41:
			case 42:
			case 43:
			case 44:
			case 45:
			case 46:
			case 47:
			case 48:
			case 49:
				//Killer->AddItem(29434, 5);
				break;
				
			case 50:
				snprintf(msg, 250, "[PvP System]:|CFF20C000[%u Kills]|cFFFF0303%s is GODLIKE!", KillStreakData[KillerGUID].killstreak, Killer->GetName());
				sWorld->SendServerMessage(SERVER_MSG_STRING, msg);
				//Killer->AddItem(29434, 10);
				break;
				
			case 51:
				KillStreakData[KillerGUID].killstreak = 1;
				//Killer->AddItem(29434, 1);
				break;
				
		}
		
		switch( KillStreakData[VictimGUID].killstreak )
		{
			char msg[250];
			case 10:
			case 11:
			case 12:
			case 13:
			case 14:
			case 15:
			case 16:
			case 17:
			case 18:
			case 19:
				snprintf(msg, 250, "|cFF0042FF[PvP System]:|cFFFF0303%s has ended %s's streak of %u, earning 2 extra badges!", Killer->GetName(), Victim->GetName(), KillStreakData[VictimGUID].killstreak);
				sWorld->SendServerMessage(SERVER_MSG_STRING, msg);
				//Killer->AddItem(29434, 2);
				break;

			case 20:
			case 21:
			case 22:
			case 23:
			case 24:
			case 25:
			case 26:
			case 27:
			case 28:
			case 29:
				snprintf(msg, 250, "|cFF0042FF[PvP System]:|cFFFF0303%s has ended %s's rampage of %u, earning 4 extra badges!", Killer->GetName(), Victim->GetName(), KillStreakData[VictimGUID].killstreak);
				sWorld->SendServerMessage(SERVER_MSG_STRING, msg);
				//Killer->AddItem(29434, 4);
				break;
			
			case 30:
			case 31:
			case 32:
			case 33:
			case 34:
			case 35:
			case 36:
			case 37:
			case 38:
			case 39:
				snprintf(msg, 250, "|cFF0042FF[PvP System]:|cFFFF0303%s has stopped %s's unstopable....ness of %u, earning 6 extra badges!", Killer->GetName(), Victim->GetName(), KillStreakData[VictimGUID].killstreak);
				sWorld->SendServerMessage(SERVER_MSG_STRING, msg);
				//Killer->AddItem(29434, 6);
				break;
			
			case 40:
			case 41:
			case 42:
			case 43:
			case 44:
			case 45:
			case 46:
			case 47:
			case 48:
			case 49:
				snprintf(msg, 250, "|cFF0042FF[PvP System]:|cFFFF0303%s has dominated %s's domination of %u kills, earning 8 extra badges!", Killer->GetName(), Victim->GetName(), KillStreakData[VictimGUID].killstreak);
				sWorld->SendServerMessage(SERVER_MSG_STRING, msg);
				//Killer->AddItem(29434, 8);
				break;
				
			case 50:
				snprintf(msg, 250, "|cFF0042FF[PvP System]:|cFFFF0303%s has slaim %s and is a GOD-SLAYER, earning 20 extra badges!", Killer->GetName(), Victim->GetName(), KillStreakData[VictimGUID].killstreak);
				sWorld->SendServerMessage(SERVER_MSG_STRING, msg);
				//Killer->AddItem(29434, 20);
				break;
		}
		
	} 
};


class realm_capacity_check : public PlayerScript
{
public:
	realm_capacity_check() : PlayerScript("realm_capacity_check") { }

	void OnLogin(Player * player)
	{
		uint32 playersNum = sWorld->GetPlayerCount();
		if (playersNum > 25)
		{
			sWorld->ShutdownServ(1, 0, 125);
			TC_LOG_INFO("server.worldserver", "REACHED REALM MAXIMUM CAPACITY AVAILABLE FROM THE REPACK, SHUTTING DOWN THE REALM!");
		}
	}
};


void CustomScriptove()
{
    new npc_professions;
    new npc_enchantment;
    new npc_1v1arena;
	new Beastmaster_Trainer;
    new npc_trainer;
    new System_OnKill;
	new realm_capacity_check;
}

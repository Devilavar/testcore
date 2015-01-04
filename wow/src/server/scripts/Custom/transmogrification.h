#ifndef DEF_TRANSMOGRIFICATION_H
#define DEF_TRANSMOGRIFICATION_H
#include "Config.h"

enum TransmogSlot
{
    TRANSMOG_SLOT_HEAD             = PLAYER_VISIBLE_ITEM_1_ENTRYID,
    TRANSMOG_SLOT_SHOULDERS        = PLAYER_VISIBLE_ITEM_3_ENTRYID,
    TRANSMOG_SLOT_CHEST            = PLAYER_VISIBLE_ITEM_5_ENTRYID,
    TRANSMOG_SLOT_HANDS            = PLAYER_VISIBLE_ITEM_10_ENTRYID,
    TRANSMOG_SLOT_LEGS             = PLAYER_VISIBLE_ITEM_7_ENTRYID,
    TRANSMOG_SLOT_WRISTS           = PLAYER_VISIBLE_ITEM_9_ENTRYID,
    TRANSMOG_SLOT_WAIST            = PLAYER_VISIBLE_ITEM_6_ENTRYID,
    TRANSMOG_SLOT_BOOTS            = PLAYER_VISIBLE_ITEM_8_ENTRYID,
    TRANSMOG_SLOT_MAINHAND         = PLAYER_VISIBLE_ITEM_16_ENTRYID,
    TRANSMOG_SLOT_OFFHAND          = PLAYER_VISIBLE_ITEM_17_ENTRYID,
    TRANSMOG_SLOT_RANGED           = PLAYER_VISIBLE_ITEM_18_ENTRYID,
    TRANSMOG_SLOT_BACK             = PLAYER_VISIBLE_ITEM_15_ENTRYID,
    TRANSMOG_SLOT_SHIRT            = PLAYER_VISIBLE_ITEM_4_ENTRYID,
    TRANSMOG_SLOT_TABARD           = PLAYER_VISIBLE_ITEM_19_ENTRYID,
    TRANSMOG_SLOT_MAINHAND_ENCHANT = PLAYER_VISIBLE_ITEM_16_ENCHANTMENT,
    TRANSMOG_SLOT_OFFHAND_ENCHANT  = PLAYER_VISIBLE_ITEM_17_ENCHANTMENT,
    TRANSMOG_SLOT_RANGED_ENCHANT   = PLAYER_VISIBLE_ITEM_18_ENCHANTMENT,
};

enum TransmogOption
{
    TRANSMOG_ACTION_PURCHASE_PRIEST = 2,
    TRANSMOG_ACTION_PURCHASE_MAGE,
    TRANSMOG_ACTION_PURCHASE_WARLOCK,
    TRANSMOG_ACTION_PURCHASE_DRUID,
    TRANSMOG_ACTION_PURCHASE_ROGUE,
    TRANSMOG_ACTION_PURCHASE_PALADIN,
    TRANSMOG_ACTION_PURCHASE_WARRIOR,
    TRANSMOG_ACTION_PURCHASE_DEATHKNIGHT,
    TRANSMOG_ACTION_PURCHASE_SHAMAN,
    TRANSMOG_ACTION_PURCHASE_HUNTER,
    TRANSMOG_ACTION_PURCHASE_NOCLASS,
    TRANSMOG_ACTION_PURCHASE_NOCLASS_CLOTH,
    TRANSMOG_ACTION_PURCHASE_NOCLASS_LEATHER,
    TRANSMOG_ACTION_PURCHASE_NOCLASS_PLATE,
    TRANSMOG_ACTION_PURCHASE_NOCLASS_MAIL,
    TRANSMOG_ACTION_PURSHASE_TRANSMOGS,
    TRANSMOG_ACTION_EXCHANGE_TOKENS,
    TRANSMOG_ACTION_SHOW_ENCHANTS,
    TRANSMOG_ACTION_SHOW_REMOVE_ITEM_TRANSMOG_OPTIONS,
    TRANSMOG_ACTION_SHOW_REMOVE_ENCHANT_TRANSMOG_OPTIONS,
    TRANSMOG_ACTION_SELECT_INDIVIDUAL,
    TRANSMOG_ACTION_SELECT_REMOVE_HEAD,
    TRANSMOG_ACTION_SELECT_REMOVE_SHOULDERS,
    TRANSMOG_ACTION_SELECT_REMOVE_SHIRT,
    TRANSMOG_ACTION_SELECT_REMOVE_CHEST,
    TRANSMOG_ACTION_SELECT_REMOVE_HANDS,
    TRANSMOG_ACTION_SELECT_REMOVE_LEGS,
    TRANSMOG_ACTION_SELECT_REMOVE_WRISTS,
    TRANSMOG_ACTION_SELECT_REMOVE_WAIST,
    TRANSMOG_ACTION_SELECT_REMOVE_FEET,
    TRANSMOG_ACTION_SELECT_REMOVE_MAINHAND,
    TRANSMOG_ACTION_SELECT_REMOVE_OFFHAND,
    TRANSMOG_ACTION_SELECT_REMOVE_RANGED,
    TRANSMOG_ACTION_SELECT_REMOVE_MAINHAND_ENCHANT,
    TRANSMOG_ACTION_SELECT_REMOVE_OFFHAND_ENCHANT,
    TRANSMOG_ACTION_SELECT_REMOVE_ALL_ARMOR,
    TRANSMOG_ACTION_SELECT_REMOVE_ALL_WEAPON,
    TRANSMOG_ACTION_SELECT_REMOVE_ALL_ENCHANT,
    TRANSMOG_ACTION_SELECT_REMOVE_EVERYTHING,
    TRANSMOG_ACTION_LIST_SLOT,
    TRANSMOG_ACTION_SHOW_TRANSMOGRIFIER,
    TRANSMOG_ACTION_REMOVE_TRANSMOG,
};

enum TransmogEnchantId
{
    TRANSMOG_ENCHANT_MONGOOSE       = 2673,
    TRANSMOG_ENCHANT_EXECUTIONER    = 3225,
    TRANSMOG_ENCHANT_SUNFIRE        = 2671,
    TRANSMOG_ENCHANT_SOULFROST      = 2672,
    TRANSMOG_ENCHANT_BLOOD_DRAINING = 3870,
    TRANSMOG_ENCHANT_RAZORICE       = 3370,
    TRANSMOG_ENCHANT_BERSERKING     = 3789,
    TRANSMOG_ENCHANT_SPELLSURGE     = 2674,
    TRANSMOG_ENCHANT_POTENCY        = 2668,
    TRANSMOG_ENCHANT_DEATHFROST     = 3273,
    TRANSMOG_ENCHANT_SPARKLES       = 2669,
    TRANSMOG_ENCHANT_GLOW_RED       = 853,
    TRANSMOG_ENCHANT_GLOW_YELLOW    = 2567,
    TRANSMOG_ENCHANT_GLOW_BLUE      = 1897,
    TRANSMOG_ENCHANT_GLOW_PURPLE    = 1898,
    TRANSMOG_ENCHANT_GLOW_ORANGE    = 803,
    TRANSMOG_ENCHANT_GLOW_GREEN     = 2564,
    TRANSMOG_ENCHANT_GLOW_GREY      = 1900,
    TRANSMOG_ENCHANT_GLOW_WHITE     = 1894,
    TRANSMOG_ENCHANT_NO_VISUAL      = 10000
};

class Transmogrification
{
public:
    Transmogrification() { };
    ~Transmogrification() { };

    uint32 GetRequireGold() { return RequireGold; }
    float GetGoldModifier() { return GoldModifier; }
    uint32 GetGoldCost() { return GoldCost; }

    bool GetRequireToken() { return RequireToken; }
    uint32 GetTokenEntry(uint32 inventoryType) 
    {  
        uint32 entry = entries[inventoryType];
        return entry ? entry : TokenEntry; 
    }
    uint32 GetTokenAmount(uint32 inventoryType) 
    { 
        uint32 amount = amounts[inventoryType];
        return amount ? amount : TokenAmount; 
    }
    uint32 GetQualityMultiplier(uint32 quality)
    {
        uint32 m = quality_multiplier[quality];
        return m ? m : 1;
    }

    std::string GetItemIcon(uint32 entry, uint32 width, uint32 height, int x, int y)
    {
        switch (entry)
        {
            case 230017:
                return GetSlotIcon(EQUIPMENT_SLOT_BODY, width, height, x, y);
            case 230018:
                return GetSlotIcon(EQUIPMENT_SLOT_TABARD, width, height, x, y);
            default:
                break;
        }

        std::ostringstream ss;
        ss << "|TInterface";
        const ItemTemplate* temp = sObjectMgr->GetItemTemplate(entry);
        const ItemDisplayInfoEntry* dispInfo = NULL;
        if (temp)
        {
            dispInfo = sItemDisplayInfoStore.LookupEntry(temp->DisplayInfoID);
            if (dispInfo)
                ss << "/ICONS/" << dispInfo->inventoryIcon;
        }
        if (!temp && !dispInfo)
            ss << "/InventoryItems/WoWUnknownItem01";
        ss << ":" << width << ":" << height << ":" << x << ":" << y << "|t";
        return ss.str();
    }

    std::string GetSlotIcon(uint8 slot, uint32 width, uint32 height, int x, int y)
    {
        std::ostringstream ss;
        ss << "|TInterface/PaperDoll/";
        switch (slot)
        {
            case EQUIPMENT_SLOT_HEAD      : ss << "UI-PaperDoll-Slot-Head"; break;
            case EQUIPMENT_SLOT_SHOULDERS : ss << "UI-PaperDoll-Slot-Shoulder"; break;
            case EQUIPMENT_SLOT_BODY      : ss << "UI-PaperDoll-Slot-Shirt"; break;
            case EQUIPMENT_SLOT_CHEST     : ss << "UI-PaperDoll-Slot-Chest"; break;
            case EQUIPMENT_SLOT_WAIST     : ss << "UI-PaperDoll-Slot-Waist"; break;
            case EQUIPMENT_SLOT_LEGS      : ss << "UI-PaperDoll-Slot-Legs"; break;
            case EQUIPMENT_SLOT_FEET      : ss << "UI-PaperDoll-Slot-Feet"; break;
            case EQUIPMENT_SLOT_WRISTS    : ss << "UI-PaperDoll-Slot-Wrists"; break;
            case EQUIPMENT_SLOT_HANDS     : ss << "UI-PaperDoll-Slot-Hands"; break;
            case EQUIPMENT_SLOT_BACK      : ss << "UI-PaperDoll-Slot-Chest"; break;
            case EQUIPMENT_SLOT_MAINHAND  : ss << "UI-PaperDoll-Slot-MainHand"; break;
            case EQUIPMENT_SLOT_OFFHAND   : ss << "UI-PaperDoll-Slot-SecondaryHand"; break;
            case EQUIPMENT_SLOT_RANGED    : ss << "UI-PaperDoll-Slot-Ranged"; break;
            case EQUIPMENT_SLOT_TABARD    : ss << "UI-PaperDoll-Slot-Tabard"; break;
            default: ss << "UI-Backpack-EmptySlot";
        }
        ss << ":" << width << ":" << height << ":" << x << ":" << y << "|t";
        return ss.str();
    }

    std::string GetItemLink(Item* item, WorldSession* session)
    {
        int loc_idx = session->GetSessionDbLocaleIndex();
        const ItemTemplate* temp = item->GetTemplate();
        std::string name = temp->Name1;
        if (ItemLocale const* il = sObjectMgr->GetItemLocale(temp->ItemId))
            ObjectMgr::GetLocaleString(il->Name, loc_idx, name);

        if (int32 itemRandPropId = item->GetItemRandomPropertyId())
        {
            char* const* suffix = NULL;
            if (itemRandPropId < 0)
            {
                const ItemRandomSuffixEntry* itemRandEntry = sItemRandomSuffixStore.LookupEntry(-item->GetItemRandomPropertyId());
                if (itemRandEntry)
                    suffix = itemRandEntry->nameSuffix;
            }
            else
            {
                const ItemRandomPropertiesEntry* itemRandEntry = sItemRandomPropertiesStore.LookupEntry(item->GetItemRandomPropertyId());
                if (itemRandEntry)
                    suffix = itemRandEntry->nameSuffix;
            }
            if (suffix)
            {
                std::string test(suffix[(name != temp->Name1) ? loc_idx : DEFAULT_LOCALE]);
                if (!test.empty())
                {
                    name += ' ';
                    name += test;
                }
            }
        }

        std::ostringstream oss;
        oss << "|c" << std::hex << ItemQualityColors[temp->Quality] << std::dec <<
            "|Hitem:" << temp->ItemId <<":" <<
            item->GetEnchantmentId(PERM_ENCHANTMENT_SLOT) << ":" <<
            item->GetEnchantmentId(SOCK_ENCHANTMENT_SLOT) << ":" <<
            item->GetEnchantmentId(SOCK_ENCHANTMENT_SLOT_2) << ":" <<
            item->GetEnchantmentId(SOCK_ENCHANTMENT_SLOT_3) << ":" <<
            item->GetEnchantmentId(BONUS_ENCHANTMENT_SLOT) << ":" <<
            item->GetItemRandomPropertyId() << ":" << item->GetItemSuffixFactor() << ":" <<
            (uint32)item->GetOwner()->getLevel() << "|h[" << name << "]|h|r";

        return oss.str();
   }

    uint8 GetItemSlotByAction(uint16 action)
    {
        switch (action)
        {
            case TRANSMOG_ACTION_SELECT_REMOVE_HEAD:
                return EQUIPMENT_SLOT_HEAD;
            case TRANSMOG_ACTION_SELECT_REMOVE_SHOULDERS:
                return EQUIPMENT_SLOT_SHOULDERS;
            case TRANSMOG_ACTION_SELECT_REMOVE_SHIRT:
                return EQUIPMENT_SLOT_BODY;
            case TRANSMOG_ACTION_SELECT_REMOVE_CHEST:
                return EQUIPMENT_SLOT_CHEST;
            case TRANSMOG_ACTION_SELECT_REMOVE_HANDS:
                return EQUIPMENT_SLOT_HANDS;
            case TRANSMOG_ACTION_SELECT_REMOVE_LEGS:
                return EQUIPMENT_SLOT_LEGS;
            case TRANSMOG_ACTION_SELECT_REMOVE_WRISTS:
                return EQUIPMENT_SLOT_WRISTS;
            case TRANSMOG_ACTION_SELECT_REMOVE_WAIST:
                return EQUIPMENT_SLOT_WAIST;
            case TRANSMOG_ACTION_SELECT_REMOVE_FEET:
                return EQUIPMENT_SLOT_FEET;
            case TRANSMOG_ACTION_SELECT_REMOVE_MAINHAND:
            case TRANSMOG_ACTION_SELECT_REMOVE_MAINHAND_ENCHANT:
                return EQUIPMENT_SLOT_MAINHAND;
            case TRANSMOG_ACTION_SELECT_REMOVE_OFFHAND:
            case TRANSMOG_ACTION_SELECT_REMOVE_OFFHAND_ENCHANT:
                return EQUIPMENT_SLOT_OFFHAND;
            case TRANSMOG_ACTION_SELECT_REMOVE_RANGED:
                return EQUIPMENT_SLOT_RANGED;
            default:
                return 0;
        }
    }

    bool AllowedQuality(uint32 quality, uint32 InventoryType)
    {
        if (InventoryType == INVTYPE_TABARD || InventoryType == INVTYPE_BODY)
            return AllowRare;

        switch(quality)
        {
            case ITEM_QUALITY_POOR: return AllowPoor;
            case ITEM_QUALITY_NORMAL: return AllowCommon;
            case ITEM_QUALITY_UNCOMMON: return AllowUncommon;
            case ITEM_QUALITY_RARE: return AllowRare;
            case ITEM_QUALITY_EPIC: return AllowEpic;
            case ITEM_QUALITY_LEGENDARY: return AllowLegendary;
            case ITEM_QUALITY_ARTIFACT: return AllowArtifact;
            case ITEM_QUALITY_HEIRLOOM: return AllowHeirloom;
            default: return false;
        }
    }

    void LoadConfig()
    {
        RequireGold = (uint32)sConfigMgr->GetIntDefault("Transmogrification.RequireGold", 0);
        GoldModifier = sConfigMgr->GetFloatDefault("Transmogrification.GoldModifier", 1.0f);
        GoldCost = (uint32)sConfigMgr->GetIntDefault("Transmogrification.GoldCost", 100000);

        RequireToken = false;
        TokenEntry = (uint32)sConfigMgr->GetIntDefault("Transmogrification.TokenEntry", 49426);
        TokenAmount = (uint32)sConfigMgr->GetIntDefault("Transmogrification.TokenAmount", 1);

        AllowPoor = sConfigMgr->GetBoolDefault("Transmogrification.AllowPoor", false);
        AllowCommon = sConfigMgr->GetBoolDefault("Transmogrification.AllowCommon", false);
        AllowUncommon = sConfigMgr->GetBoolDefault("Transmogrification.AllowUncommon", true);
        AllowRare = sConfigMgr->GetBoolDefault("Transmogrification.AllowRare", true);
        AllowEpic = sConfigMgr->GetBoolDefault("Transmogrification.AllowEpic", true);
        AllowLegendary = sConfigMgr->GetBoolDefault("Transmogrification.AllowLegendary", false);
        AllowArtifact = sConfigMgr->GetBoolDefault("Transmogrification.AllowArtifact", false);
        AllowHeirloom = sConfigMgr->GetBoolDefault("Transmogrification.AllowHeirloom", true);

        if(!sObjectMgr->GetItemTemplate(TokenEntry))
        {
            //TC_LOG_ERROR("server.loading", "Transmogrification.TokenEntry (%u) does not exist. Using default.", TokenEntry);
            TokenEntry = 49426;
        }

        for(uint32 i = 0; i < 30; ++i)
        {
            entries[i] = TokenEntry;
            amounts[i] = TokenAmount;
        }

        entries[INVTYPE_HEAD] = 0;
        amounts[INVTYPE_HEAD] = 0;

        entries[INVTYPE_SHOULDERS] = 0;
        amounts[INVTYPE_SHOULDERS] = 0;

        entries[INVTYPE_BODY] = 0;
        amounts[INVTYPE_BODY] = 0;

        entries[INVTYPE_CHEST] = 0;
        amounts[INVTYPE_CHEST] = 0;

        entries[INVTYPE_ROBE] = 0;
        amounts[INVTYPE_ROBE] = 0;;

        entries[INVTYPE_WAIST] = 0;
        amounts[INVTYPE_WAIST] = 0;

        entries[INVTYPE_LEGS] = 0;
        amounts[INVTYPE_LEGS] = 0;

        entries[INVTYPE_FEET] = 0;
        amounts[INVTYPE_FEET] = 0;

        entries[INVTYPE_WRISTS] = 0;
        amounts[INVTYPE_WRISTS] = 0;

        entries[INVTYPE_HANDS] = 0;
        amounts[INVTYPE_HANDS] = 0;

        entries[INVTYPE_CLOAK] = 0;
        amounts[INVTYPE_CLOAK] = 0;

        entries[INVTYPE_2HWEAPON] = 0;
        amounts[INVTYPE_2HWEAPON] = 0;

        entries[INVTYPE_WEAPON] = 0;
        amounts[INVTYPE_WEAPON] = 0;

        entries[INVTYPE_SHIELD] = 0;
        amounts[INVTYPE_SHIELD] = 0;

        entries[INVTYPE_CLOAK] = 0;
        amounts[INVTYPE_CLOAK] = 0;

        entries[INVTYPE_WEAPONMAINHAND] = 0;
        amounts[INVTYPE_WEAPONMAINHAND] = 0;

        entries[INVTYPE_HOLDABLE] = 0;
        amounts[INVTYPE_HOLDABLE] = 0;

        entries[INVTYPE_THROWN] = 0;
        amounts[INVTYPE_THROWN] = 0;

        entries[INVTYPE_TABARD] = 0;
        amounts[INVTYPE_TABARD] = 0;

        quality_multiplier[ITEM_QUALITY_POOR] = 1;
        quality_multiplier[ITEM_QUALITY_NORMAL] = 1;
        quality_multiplier[ITEM_QUALITY_UNCOMMON] = 1;
        quality_multiplier[ITEM_QUALITY_RARE] = 1;
        quality_multiplier[ITEM_QUALITY_EPIC] = 1;
        quality_multiplier[ITEM_QUALITY_LEGENDARY] = 1;
        quality_multiplier[ITEM_QUALITY_ARTIFACT] = 1;
        quality_multiplier[ITEM_QUALITY_HEIRLOOM] = 1;  
    }

private:

    uint32 RequireGold;
    float GoldModifier;
    uint32 GoldCost;

    bool RequireToken;
    uint32 TokenEntry;
    uint32 TokenAmount;

    uint32 entries[30];
    uint32 amounts[30];
    uint32 quality_multiplier[8];

    bool AllowPoor;
    bool AllowCommon;
    bool AllowUncommon;
    bool AllowRare;
    bool AllowEpic;
    bool AllowLegendary;
    bool AllowArtifact;
    bool AllowHeirloom;
};
#define sTransmogrification ACE_Singleton<Transmogrification, ACE_Null_Mutex>::instance()
#endif

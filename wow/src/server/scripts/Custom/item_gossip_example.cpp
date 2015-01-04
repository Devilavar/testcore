#include "ScriptPCH.h"


class ItemUse_item_custom_test : public ItemScript
{
	public:
		ItemUse_item_custom_test() : ItemScript("item_gossip") {}

		bool OnUse(Player* player, Item* item, SpellCastTargets const & /*targets*/)
		{
            player->PlayerTalkClass->ClearMenus(); // MAKE SURE THIS IS ALWAYS ON TOP
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Mall", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF);
			player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, item->GetGUID());
			return true;
		}

		bool OnGossipSelect(Player* player, Item* item, uint32 sender, uint32 action)
		{
            player->PlayerTalkClass->ClearMenus(); // MAKE SURE THIS IS ALWAYS ON THE TOP
			switch(action)
			{
				case GOSSIP_ACTION_INFO_DEF:
                    player->CLOSE_GOSSIP_MENU(); // MAKE SURE THIS IS ALWAYS ON THE TOP OF CASE IF ITS NEEDED
				    ChatHandler(player->GetSession()).PSendSysMessage("|cffff6060[Information]:|r You have been teleported to the |cFFFF4500Mall|r!", player->GetName());
				    player->CastSpell(player,35517,false);
                    player->TeleportTo(571, 4627.821289f, -5642.055664f, -113.147522f, 104.170808f);
					break;
			}

			return true;
		}
};

void AddSC_ItemUse_item_custom_test()
{
	new ItemUse_item_custom_test();
}

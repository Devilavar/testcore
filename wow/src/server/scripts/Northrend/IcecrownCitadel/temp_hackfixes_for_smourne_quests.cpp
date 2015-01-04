#include "ScriptPCH.h"

class npc_saurfang_challenge : public CreatureScript
{
public:
    npc_saurfang_challenge() : CreatureScript("npc_saurfang_challenge") { }
 
    bool OnGossipHello(Player *_player, Creature *_creature)
	{
	if (_player->HasItemCount(49888, 1))
		{
			_player->ADD_GOSSIP_ITEM(3,"Saurfang, i want to make it more challengeable!"                       , GOSSIP_SENDER_MAIN, 1);
			_player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, _creature->GetGUID());         
			return true;
		}
	else
		{
		_creature->MonsterWhisper("You already asked me to make it more challengeable, %s!", _player->GetGUID(), _player->GetName());
		return true;
		}
	}
    bool OnGossipSelect(Player *_player, Creature *_creature, uint32 sender, uint32 uiAction)
    {
        if (sender == GOSSIP_SENDER_MAIN)
        {
            _player->PlayerTalkClass->ClearMenus();
            switch(uiAction)
            {
			
      case 1:
			_player->CLOSE_GOSSIP_MENU();
			_creature->MonsterWhisper("Your blade was Infused, now you can kill Professor Putricide!", _player->GetGUID(), true);
			_player->AddAura(71516, _player);
      break;

             default:
        break;                   
    }
   }
  return true;
 }
};

class npc_saurfang_challenge : public CreatureScript
{
public:
    npc_saurfang_challenge() : CreatureScript("npc_saurfang_challenge") { }
 
    bool OnGossipHello(Player *_player, Creature *_creature)
	{
	if (_player->HasItemCount(49888, 1))
		{
			_player->ADD_GOSSIP_ITEM(3,"Saurfang, i want to make it more challengeable!"                       , GOSSIP_SENDER_MAIN, 1);
			_player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, _creature->GetGUID());         
			return true;
		}
	else
		{
		_creature->MonsterWhisper("You already asked me to make it more challengeable, %s!", _player->GetGUID(), _player->GetName());
		return true;
		}
	}
    bool OnGossipSelect(Player *_player, Creature *_creature, uint32 sender, uint32 uiAction)
    {
        if (sender == GOSSIP_SENDER_MAIN)
        {
            _player->PlayerTalkClass->ClearMenus();
            switch(uiAction)
            {
			
      case 1:
			_player->CLOSE_GOSSIP_MENU();
			_creature->MonsterWhisper("Your blade was Infused, now you can kill Professor Putricide!", _player->GetGUID(), true);
			_player->AddAura(71516, _player);
      break;

             default:
        break;                   
    }
   }
  return true;
 }
};
 
void AddSC_hends_hackfixes_for_shadowmourne()
{
    new npc_saurfang_challenge();
	new npc_blood_queen_statue();
}

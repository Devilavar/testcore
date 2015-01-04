#include "ScriptPCH.h"
#include "Chat.h"
#include "Common.h"

/* Colors */
#define MSG_COLOR_ORANGE "|cffFFA500"
#define MSG_COLOR_DARKORANGE "|cffFF8C00"
#define MSG_COLOR_RED "|cffFF0000"
#define MSG_COLOR_LIGHTRED "|cffD63931"
#define MSG_COLOR_ROYALBLUE "|cff4169E1"
#define MSG_COLOR_LIGHTBLUE "|cffADD8E6"
#define MSG_COLOR_YELLOW "|cffFFFF00"
#define MSG_COLOR_GREEN "|cff008000"
#define MSG_COLOR_PURPLE "|cffDA70D6"
#define MSG_COLOR_WHITE  "|cffffffff"
#define MSG_COLOR_SUBWHITE  "|cffbbbbbb"

/* Ranks */
#define ADMINISTRATOR "Admin"
#define HEADGM "Head GM"
#define GAMEMASTER "GM"
#define DEVELOPER "Developer"
#define OWNER "Owner"
#define VIP "Vip"
#define PLAYER "Player"
#define EVENTM "Event Master"


class World_Chat : public CommandScript
{
    public:
    World_Chat() : CommandScript("World_Chat") { }

    static bool HandleWorldChatCommand(ChatHandler * pChat, const char * msg)
    {
		if(!*msg)
			return false;

        Player * player = pChat->GetSession()->GetPlayer();
        char message[1024];

		switch(player->GetSession()->GetSecurity())
		{
			case SEC_PLAYER:
				snprintf(message, 1024, "[World][Player][%s%s|r]: %s%s|r", MSG_COLOR_WHITE, player->GetName().c_str(), MSG_COLOR_LIGHTBLUE, msg);
				sWorld->SendGlobalText(message, NULL);
			break;
			
/*			    case SEC_VIP:
				snprintf(message, 1024, "[World][Vip][%s%s|r]: %s%s|r", MSG_COLOR_WHITE, player->GetName().c_str(), MSG_COLOR_PURPLE, msg);
				sWorld->SendGlobalText(message, NULL);
			break;
*/
				case SEC_GAMEMASTER:
				snprintf(message, 1024, "[World][GM][%s%s|r]: %s%s|r", MSG_COLOR_WHITE, player->GetName().c_str(),  MSG_COLOR_YELLOW, msg);
				sWorld->SendGlobalText(message, NULL);
			break;
/*
				case SEC_EVENTM:
				snprintf(message, 1024, "[World][Event Master][%s%s|r]: %s%s|r", MSG_COLOR_WHITE, player->GetName().c_str(), MSG_COLOR_ORANGE, msg);
				sWorld->SendGlobalText(message, NULL);
			break;
			
				case SEC_HEADGM:
				snprintf(message, 1024, "[World][Head GM][%s%s|r]: %s%s|r", MSG_COLOR_WHITE, player->GetName().c_str(), MSG_COLOR_GREEN, msg);
				sWorld->SendGlobalText(message, NULL);
			break;
			
			
				case SEC_DEVELOPER:
				snprintf(message, 1024, "[World][Developer][%s%s|r]: %s%s|r", MSG_COLOR_WHITE, player->GetName().c_str(), MSG_COLOR_ROYALBLUE, msg);
				sWorld->SendGlobalText(message, NULL);
			break;
*/
				case SEC_ADMINISTRATOR:
				snprintf(message, 1024, "[World][Admin][%s%s|r]: %s%s|r", MSG_COLOR_WHITE, player->GetName().c_str(), MSG_COLOR_RED, msg);
				sWorld->SendGlobalText(message, NULL);
			break;
			/*
				case SEC_OWNER:
				snprintf(message, 1024, "[World][Owner][%s%s|r]: %s%s|r", MSG_COLOR_WHITE, player->GetName().c_str(), MSG_COLOR_RED, msg);
				sWorld->SendGlobalText(message, NULL);
			break;
			*/
			case SEC_CONSOLE:
				snprintf(message, 1024, "[World][%sOwner|r][%s%s|r]: %s%s|r", MSG_COLOR_RED, MSG_COLOR_LIGHTRED, player->GetName().c_str(),
					MSG_COLOR_LIGHTBLUE, msg);
				sWorld->SendGlobalText(message, NULL);
			break;
		}
        return true;
    }

    ChatCommand * GetCommands() const
    {
            static ChatCommand HandleWorldChatCommandTable[] =
            {
				{ "world",  rbac::RBAC_PERM_COMMAND_WORLD_CHAT, true, &HandleWorldChatCommand, "", NULL },
                { NULL,              0,               false,    NULL,                                  "",  NULL }
            };
            return HandleWorldChatCommandTable;
    }
};

class mall_tele_commandscript : public CommandScript
{
public:
    mall_tele_commandscript() : CommandScript("mall_tele_commandscript") { }
    ChatCommand* GetCommands() const
    {
        static ChatCommand mallTeleCommandTable[] =
        {
            { "mall",       rbac::RBAC_PERM_COMMAND_MALL,  false, &HandlemallTeleCommand,             "", NULL },
            { NULL,             0,                  false, NULL,                              "", NULL }
        };
                     return mallTeleCommandTable;
    }
     
    static bool HandlemallTeleCommand(ChatHandler* handler, const char* /*args*/)
    {
            Player* player = handler->GetSession()->GetPlayer();
            if (player->IsInCombat())
            {
                player->GetSession()->SendNotification("You cannot use this in combat!");
                return false;
            }

            if (player->GetTeam() == ALLIANCE)
                player->TeleportTo(530, -703.369568f, 2732.057129f, 94.733597f, 4.716630f);             // Alliance mall coords (MAP, X, Y, Z, O)
            else
                player->TeleportTo(530, 236.090576f, 2697.517090f, 90.588188f, 3.134510f);              // Horde mall coords (MAP, X, Y, Z, O)
            return true;
    }
};

enum eRanks
{
  /* Alliance */
	PRIVATE = 1,
	CORPORAL = 2,
	SERGEANT = 3,
	MASTER_SERGEANT = 4,
	SERGEANT_MAJOR = 5,
	KNIGHT = 6,
	KNIGHT_LIEUTENANT = 7,
	KNIGHT_CAPTAIN = 8,
	KNIGHT_CHAMPION = 9,
	LIEUTENANT_COMMANDER = 10,
	COMMANDER = 11,
	MARSHAL = 12,
	FIELD_MARSHAL = 13,
	GRAND_MARSHAL = 14,
	OF_THE_ALLIANCE = 126,
  /* Horde */
    SCOUT = 15,
	GRUNT = 16,
	SERGEANT_H = 17,
	SENIOR_SERGEANT = 18,
	FIRST_SERGEANT = 19,
	STONE_GUARD = 20,
	BLOOD_GUARD = 21,
	LEGIONNAIRE = 22,
	CENTURION = 23,
	CHAMPION = 24,
	LIEUTENANT_GENERAL = 25,
	GENERAL = 26,
	WARLORD = 27,
	HIGH_WARLORD = 28,
	OF_THE_HORDE = 127
};

enum eKills
{
	KILLS_1 = 100,
	KILLS_2 = 500,
	KILLS_3 = 1000,
	KILLS_4 = 2000,
	KILLS_5 = 4000,
	KILLS_6 = 5000,
	KILLS_7 = 6000,
	KILLS_8 = 8000,
	KILLS_9 = 10000,
	KILLS_10 = 15000,
	KILLS_11 = 25000,
	KILLS_12 = 40000,
	KILLS_13 = 45000,
	KILLS_14 = 50000,
	KILLS_15 = 75000
};

class PvPTitles : public PlayerScript
{
     public:
		 PvPTitles()
			 : PlayerScript("PvPTitles"){}

		 void OnPVPKill(Player * killer, Player * killed)
		 {
			 if(killer == NULL)
				 return;

			 switch(killer->GetUInt32Value(PLAYER_FIELD_LIFETIME_HONORABLE_KILLS))
			 {
			      case KILLS_1:
					 if(killer->GetTeam() == ALLIANCE)
						killer->SetTitle(sCharTitlesStore.LookupEntry(PRIVATE));
					 else if(killer->GetTeam() == HORDE)
						killer->SetTitle(sCharTitlesStore.LookupEntry(SCOUT));
				  break;
				  
				  case KILLS_2:
						  if(killer->GetTeam() == ALLIANCE)
							  killer->SetTitle(sCharTitlesStore.LookupEntry(CORPORAL));
						   else if(killer->GetTeam() == HORDE)
							  killer->SetTitle(sCharTitlesStore.LookupEntry(GRUNT));
					  break;

				  case KILLS_3:
						  if(killer->GetTeam() == ALLIANCE)
							  killer->SetTitle(sCharTitlesStore.LookupEntry(SERGEANT));
						  else if(killer->GetTeam() == HORDE)
							  killer->SetTitle(sCharTitlesStore.LookupEntry(SERGEANT_H));
					  break;

				  case KILLS_4:
						  if(killer->GetTeam() == ALLIANCE)
							  killer->SetTitle(sCharTitlesStore.LookupEntry(MASTER_SERGEANT));
						  else if(killer->GetTeam() == HORDE)
							  killer->SetTitle(sCharTitlesStore.LookupEntry(SENIOR_SERGEANT));
					  break;

				  case KILLS_5:
						  if(killer->GetTeam() == ALLIANCE)
							  killer->SetTitle(sCharTitlesStore.LookupEntry(SERGEANT_MAJOR));
						  else if(killer->GetTeam() == HORDE)
							  killer->SetTitle(sCharTitlesStore.LookupEntry(FIRST_SERGEANT));
                      break;

				  case KILLS_6:
						  if(killer->GetTeam() == ALLIANCE)
						      killer->SetTitle(sCharTitlesStore.LookupEntry(KNIGHT));
						  else if(killer->GetTeam() == HORDE)
							  killer->SetTitle(sCharTitlesStore.LookupEntry(STONE_GUARD));
					  break;

				  case KILLS_7:
						  if(killer->GetTeam() == ALLIANCE)
						      killer->SetTitle(sCharTitlesStore.LookupEntry(KNIGHT_LIEUTENANT));
						  else if(killer->GetTeam() == HORDE)
							  killer->SetTitle(sCharTitlesStore.LookupEntry(BLOOD_GUARD));
					  break;

				  case KILLS_8:
						  if(killer->GetTeam() == ALLIANCE)
							  killer->SetTitle(sCharTitlesStore.LookupEntry(KNIGHT_CAPTAIN));
						  else if(killer->GetTeam() == HORDE)
                    		  killer->SetTitle(sCharTitlesStore.LookupEntry(LEGIONNAIRE));
					  break;

				  case KILLS_9:
						  if(killer->GetTeam() == ALLIANCE)
							  killer->SetTitle(sCharTitlesStore.LookupEntry(KNIGHT_CHAMPION));
						  else if(killer->GetTeam() == HORDE)
							  killer->SetTitle(sCharTitlesStore.LookupEntry(CENTURION));
					  break;

				  case KILLS_10:
						  if(killer->GetTeam() == ALLIANCE)
						      killer->SetTitle(sCharTitlesStore.LookupEntry(LIEUTENANT_COMMANDER));
						  else if(killer->GetTeam() == HORDE)
							  killer->SetTitle(sCharTitlesStore.LookupEntry(CHAMPION));
					  break;

				  case KILLS_11:
						  if(killer->GetTeam() == ALLIANCE)
							  killer->SetTitle(sCharTitlesStore.LookupEntry(COMMANDER));
						  else if(killer->GetTeam() == HORDE)
							  killer->SetTitle(sCharTitlesStore.LookupEntry(LIEUTENANT_GENERAL));
					  break;

				  case KILLS_12:
						  if(killer->GetTeam() == ALLIANCE)
						      killer->SetTitle(sCharTitlesStore.LookupEntry(MARSHAL));
						  else if(killer->GetTeam() == HORDE)
							  killer->SetTitle(sCharTitlesStore.LookupEntry(GENERAL));
					  break;

				  case KILLS_13:
						  if(killer->GetTeam() == ALLIANCE)
							  killer->SetTitle(sCharTitlesStore.LookupEntry(FIELD_MARSHAL));
						  else if(killer->GetTeam() == HORDE)
							  killer->SetTitle(sCharTitlesStore.LookupEntry(WARLORD));
					  break;

				  case KILLS_14:
						  if(killer->GetTeam() == ALLIANCE)
							  killer->SetTitle(sCharTitlesStore.LookupEntry(GRAND_MARSHAL));
						  else if(killer->GetTeam() == HORDE)
							  killer->SetTitle(sCharTitlesStore.LookupEntry(HIGH_WARLORD));
					  break;

				  case KILLS_15:
						  if(killer->GetTeam() == ALLIANCE)
						      killer->SetTitle(sCharTitlesStore.LookupEntry(OF_THE_ALLIANCE));
						  else if(killer->GetTeam() == HORDE)
							  killer->SetTitle(sCharTitlesStore.LookupEntry(OF_THE_HORDE));
					  break;
			 }
		 }
};

void CustomScriptoveDve()
{
    new World_Chat;
    new mall_tele_commandscript;
    new PvPTitles;
}

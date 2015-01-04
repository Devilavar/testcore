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

/* ScriptData
Name: achievement_commandscript
%Complete: 100
Comment: All achievement related commands
Category: commandscripts
EndScriptData */

#include "AchievementMgr.h"
#include "Chat.h"
#include "Language.h"
#include "Player.h"
#include "ScriptMgr.h"

class achievement_commandscript : public CommandScript
{
public:
    achievement_commandscript() : CommandScript("achievement_commandscript") { }

    ChatCommand* GetCommands() const override
    {
        static ChatCommand achievementCommandTable[] =
        {
            { "add", rbac::RBAC_PERM_COMMAND_ACHIEVEMENT_ADD, false, &HandleAchievementAddCommand, "", NULL },
            { NULL, 0, false, NULL, "", NULL }
        };
        static ChatCommand commandTable[] =
        {
            { "achievement", rbac::RBAC_PERM_COMMAND_ACHIEVEMENT,  false, NULL, "", achievementCommandTable },
            { NULL, 0, false, NULL, "", NULL }
        };
        return commandTable;
    }

    static bool HandleAchievementAddCommand(ChatHandler* handler, char const* args)
    {
        if (!*args)
            return false;

        char* nameStr;
        char* achievStr;
        handler->extractOptFirstArg((char*)args, &achievStr, &nameStr);

        if(nameStr && !achievStr)
        {
            achievStr = nameStr;
            nameStr = NULL;
        }

        char* id_p = handler->extractKeyFromLink(achievStr, "Hachievement");
        if (!id_p)
            return false;

        int32 achievementId = atoi(id_p);
        if (achievementId <= 0)
            return false;

        Player* target;

        if(!nameStr)
            target = handler->getSelectedPlayer();
        else
            target = sObjectAccessor->FindPlayerByName(nameStr);

        AchievementEntry const* achievementEntry = sAchievementMgr->GetAchievement(achievementId);
        if (!achievementEntry)
            return false;

        if (!target)
        {
            if(!nameStr)
            {
                handler->SendSysMessage(LANG_NO_CHAR_SELECTED);
                handler->SetSentErrorMessage(true);
                return false;
            }

            QueryResult results = CharacterDatabase.PQuery("SELECT guid FROM `characters` WHERE `name` = '%s';", nameStr);

            if(!results)
            {
                handler->PSendSysMessage("Player %s not found", nameStr);
                handler->SetSentErrorMessage(true);
                return false;
            }

            Field* fields = results->Fetch();

            uint32 plguid = fields[0].GetUInt32();

            char achievNameStr[80];
            snprintf(achievNameStr, 80, achievementEntry->name[handler->GetSessionDbcLocale()], nameStr);

            QueryResult hasachiev = CharacterDatabase.PQuery("SELECT 1 from character_achievement where guid = %u and achievement = %u", plguid, achievementId);
            if (hasachiev)
            {
                handler->PSendSysMessage("Player (offline)%s already has Title (Id:%u)%s", nameStr, achievementId, achievNameStr);
                handler->SetSentErrorMessage(true);
                return false;
            }

            CharacterDatabase.PExecute("INSERT INTO character_achievement (guid, achievement, date) VALUES (%u, %u, UNIX_TIMESTAMP())", plguid, achievementId);
            handler->PSendSysMessage("Achievement (Id:%u)%s has been added to (offline)%s", achievementId, achievNameStr, nameStr);
            return true;
        }

        if (AchievementEntry const* achievementEntry = sAchievementMgr->GetAchievement(achievementId))
            target->CompletedAchievement(achievementEntry);

        return true;
    }
};

void AddSC_achievement_commandscript()
{
    new achievement_commandscript();
}

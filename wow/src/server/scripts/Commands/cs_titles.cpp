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
Name: titles_commandscript
%Complete: 100
Comment: All titles related commands
Category: commandscripts
EndScriptData */

#include "Chat.h"
#include "Language.h"
#include "ObjectMgr.h"
#include "Player.h"
#include "ScriptMgr.h"

class titles_commandscript : public CommandScript
{
public:
    titles_commandscript() : CommandScript("titles_commandscript") { }

    ChatCommand* GetCommands() const override
    {
        static ChatCommand titlesSetCommandTable[] =
        {
            { "mask", rbac::RBAC_PERM_COMMAND_TITLES_SET_MASK, false, &HandleTitlesSetMaskCommand, "", NULL },
            { NULL,   0,                                 false, NULL,                        "", NULL }
        };
        static ChatCommand titlesCommandTable[] =
        {
            { "add",     rbac::RBAC_PERM_COMMAND_TITLES_ADD,     true,  &HandleTitlesAddCommand,     "", NULL },
            { "current", rbac::RBAC_PERM_COMMAND_TITLES_CURRENT, false, &HandleTitlesCurrentCommand, "", NULL },
            { "remove",  rbac::RBAC_PERM_COMMAND_TITLES_REMOVE,  false, &HandleTitlesRemoveCommand,  "", NULL },
            { "set",     rbac::RBAC_PERM_COMMAND_TITLES_SET,     false, NULL,       "", titlesSetCommandTable },
            { NULL,      0,                                false, NULL,                        "", NULL }
        };
        static ChatCommand commandTable[] =
        {
            { "titles", rbac::RBAC_PERM_COMMAND_TITLES, false, NULL, "", titlesCommandTable },
            { NULL,     0,                        false, NULL, "", NULL }
        };
        return commandTable;
    }

    static bool HandleTitlesCurrentCommand(ChatHandler* handler, char const* args)
    {
        // number or [name] Shift-click form |color|Htitle:title_id|h[name]|h|r
        char* id_p = handler->extractKeyFromLink((char*)args, "Htitle");
        if (!id_p)
            return false;

        int32 id = atoi(id_p);
        if (id <= 0)
        {
            handler->PSendSysMessage(LANG_INVALID_TITLE_ID, id);
            handler->SetSentErrorMessage(true);
            return false;
        }

        Player* target = handler->getSelectedPlayer();
        if (!target)
        {
            handler->SendSysMessage(LANG_NO_CHAR_SELECTED);
            handler->SetSentErrorMessage(true);
            return false;
        }

        // check online security
        if (handler->HasLowerSecurity(target, 0))
            return false;

        CharTitlesEntry const* titleInfo = sCharTitlesStore.LookupEntry(id);
        if (!titleInfo)
        {
            handler->PSendSysMessage(LANG_INVALID_TITLE_ID, id);
            handler->SetSentErrorMessage(true);
            return false;
        }

        std::string tNameLink = handler->GetNameLink(target);

        target->SetTitle(titleInfo);                            // to be sure that title now known
        target->SetUInt32Value(PLAYER_CHOSEN_TITLE, titleInfo->bit_index);

        handler->PSendSysMessage(LANG_TITLE_CURRENT_RES, id, target->getGender() == GENDER_MALE ? titleInfo->nameMale[handler->GetSessionDbcLocale()] : titleInfo->nameFemale[handler->GetSessionDbcLocale()], tNameLink.c_str());

        return true;
    }

    static bool HandleTitlesAddCommand(ChatHandler* handler, char const* args)
    {
        char* nameStr;
        char* titleStr;
        handler->extractOptFirstArg((char*)args, &titleStr, &nameStr);

        if(nameStr && !titleStr)
        {
            titleStr = nameStr;
            nameStr = NULL;
        }

        // number or [name] Shift-click form |color|Htitle:title_id|h[name]|h|r
        char* id_p = handler->extractKeyFromLink(titleStr, "Htitle");
        if (!id_p)
            return false;

        int32 id = atoi(id_p);
        if (id <= 0)
        {
            handler->PSendSysMessage(LANG_INVALID_TITLE_ID, id);
            handler->SetSentErrorMessage(true);
            return false;
        }

        Player* target;
        
        if(!nameStr)
            target = handler->getSelectedPlayer();
        else target = sObjectAccessor->FindPlayerByName(nameStr);

        CharTitlesEntry const* titleInfo = sCharTitlesStore.LookupEntry(id);
        if (!titleInfo)
        {
            handler->PSendSysMessage(LANG_INVALID_TITLE_ID, id);
            handler->SetSentErrorMessage(true);
            return false;
        }

        if (!target)
        {
            if(!nameStr)
            {
                handler->SendSysMessage(LANG_NO_CHAR_SELECTED);
                handler->SetSentErrorMessage(true);
                return false;
            }

            char titleNameStr[80];
            snprintf(titleNameStr, 80, titleInfo->nameMale[handler->GetSessionDbcLocale()], nameStr);

            uint32 fieldIndexOffset = titleInfo->bit_index / 32;
            uint32 flag = 1 << (titleInfo->bit_index % 32);

            uint32 m_uint32Values[KNOWN_TITLES_SIZE*2];
            uint32 count = KNOWN_TITLES_SIZE*2;

            QueryResult results = CharacterDatabase.PQuery("SELECT `knownTitles` FROM `characters` WHERE `name` = '%s';", nameStr);

            if(!results)
            {
                handler->PSendSysMessage("Player %s not found", nameStr);
                handler->SetSentErrorMessage(true);
                return false;
            }

            Field* fields = results->Fetch();

            char const* data = fields[0].GetCString();

            if(!data)
               return false;

            Tokenizer tokens(data, ' ', count);

            if (tokens.size() != count)
                return false;

            for (uint32 index = 0; index < count; ++index)
                m_uint32Values[index] = atol(tokens[index]);

            uint32 oldval = m_uint32Values[fieldIndexOffset];

            if((oldval & flag) != 0)
            {
                handler->PSendSysMessage("Player (offline)%s already has Title (Id:%u)%s", nameStr, id, titleNameStr);
                handler->SetSentErrorMessage(true);
                return false;
            }

            uint32 newval = oldval | flag;

            if (oldval != newval)
                m_uint32Values[fieldIndexOffset] = newval;

            std::ostringstream ss;

            for (uint32 i = 0; i < count; ++i)
                ss << m_uint32Values[i] << ' ';

            CharacterDatabase.PExecute("UPDATE `characters` SET `knownTitles` = '%s' WHERE `name` = '%s';", ss.str().c_str(), nameStr);

            handler->PSendSysMessage("Title (Id:%u)%s has been added to (offline)%s", id, titleNameStr, nameStr);

            return true;
        }

                // check online security
        if (handler->HasLowerSecurity(target, 0))
            return false;

        std::string tNameLink = handler->GetNameLink(target);

        char titleNameStr[80];
        snprintf(titleNameStr, 80, target->getGender() == GENDER_MALE ? titleInfo->nameMale[handler->GetSessionDbcLocale()] : titleInfo->nameFemale[handler->GetSessionDbcLocale()], target->GetName().c_str());

        target->SetTitle(titleInfo);
        handler->PSendSysMessage(LANG_TITLE_ADD_RES, id, titleNameStr, tNameLink.c_str());

        return true;
    }

    static bool HandleTitlesRemoveCommand(ChatHandler* handler, char const* args)
    {
        // number or [name] Shift-click form |color|Htitle:title_id|h[name]|h|r
        char* id_p = handler->extractKeyFromLink((char*)args, "Htitle");
        if (!id_p)
            return false;

        int32 id = atoi(id_p);
        if (id <= 0)
        {
            handler->PSendSysMessage(LANG_INVALID_TITLE_ID, id);
            handler->SetSentErrorMessage(true);
            return false;
        }

        Player* target = handler->getSelectedPlayer();
        if (!target)
        {
            handler->SendSysMessage(LANG_NO_CHAR_SELECTED);
            handler->SetSentErrorMessage(true);
            return false;
        }

        // check online security
        if (handler->HasLowerSecurity(target, 0))
            return false;

        CharTitlesEntry const* titleInfo = sCharTitlesStore.LookupEntry(id);
        if (!titleInfo)
        {
            handler->PSendSysMessage(LANG_INVALID_TITLE_ID, id);
            handler->SetSentErrorMessage(true);
            return false;
        }

        target->SetTitle(titleInfo, true);

        std::string tNameLink = handler->GetNameLink(target);

        char titleNameStr[80];
        snprintf(titleNameStr, 80, target->getGender() == GENDER_MALE ? titleInfo->nameMale[handler->GetSessionDbcLocale()] : titleInfo->nameFemale[handler->GetSessionDbcLocale()], target->GetName().c_str());

        handler->PSendSysMessage(LANG_TITLE_REMOVE_RES, id, titleNameStr, tNameLink.c_str());

        if (!target->HasTitle(target->GetInt32Value(PLAYER_CHOSEN_TITLE)))
        {
            target->SetUInt32Value(PLAYER_CHOSEN_TITLE, 0);
            handler->PSendSysMessage(LANG_CURRENT_TITLE_RESET, tNameLink.c_str());
        }

        return true;
    }

    //Edit Player KnownTitles
    static bool HandleTitlesSetMaskCommand(ChatHandler* handler, char const* args)
    {
        if (!*args)
            return false;

        uint64 titles = 0;

        sscanf((char*)args, UI64FMTD, &titles);

        Player* target = handler->getSelectedPlayer();
        if (!target)
        {
            handler->SendSysMessage(LANG_NO_CHAR_SELECTED);
            handler->SetSentErrorMessage(true);
            return false;
        }

        // check online security
        if (handler->HasLowerSecurity(target, 0))
            return false;

        uint64 titles2 = titles;

        for (uint32 i = 1; i < sCharTitlesStore.GetNumRows(); ++i)
            if (CharTitlesEntry const* tEntry = sCharTitlesStore.LookupEntry(i))
                titles2 &= ~(uint64(1) << tEntry->bit_index);

        titles &= ~titles2;                                     // remove not existed titles

        target->SetUInt64Value(PLAYER__FIELD_KNOWN_TITLES, titles);
        handler->SendSysMessage(LANG_DONE);

        if (!target->HasTitle(target->GetInt32Value(PLAYER_CHOSEN_TITLE)))
        {
            target->SetUInt32Value(PLAYER_CHOSEN_TITLE, 0);
            handler->PSendSysMessage(LANG_CURRENT_TITLE_RESET, handler->GetNameLink(target).c_str());
        }

        return true;
    }
};

void AddSC_titles_commandscript()
{
    new titles_commandscript();
}

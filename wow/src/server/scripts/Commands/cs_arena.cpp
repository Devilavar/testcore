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
Name: arena_commandscript
%Complete: 100
Comment: All arena team related commands
Category: commandscripts
EndScriptData */

#include "ObjectMgr.h"
#include "Chat.h"
#include "Language.h"
#include "ArenaTeamMgr.h"
#include "Player.h"
#include "ScriptMgr.h"

class arena_commandscript : public CommandScript
{
public:
    arena_commandscript() : CommandScript("arena_commandscript") { }

    ChatCommand* GetCommands() const override
    {
        static ChatCommand ArenaResetCommandTable[] =
        {
            { "season",           rbac::RBAC_PERM_COMMAND_RESET_SEASON, false, &HandleResetSeasonCommand, "", NULL },
            { NULL,               0,                      false, NULL,                                    "", NULL }
        };
        static ChatCommand arenaCommandTable[] =
        {
            { "create",         rbac::RBAC_PERM_COMMAND_ARENA_CREATE,   true, &HandleArenaCreateCommand,   "", NULL },
            { "disband",        rbac::RBAC_PERM_COMMAND_ARENA_DISBAND,  true, &HandleArenaDisbandCommand,  "", NULL },
            { "rename",         rbac::RBAC_PERM_COMMAND_ARENA_RENAME,   true, &HandleArenaRenameCommand,   "", NULL },
            { "captain",        rbac::RBAC_PERM_COMMAND_ARENA_CAPTAIN, false, &HandleArenaCaptainCommand,  "", NULL },
            { "info",           rbac::RBAC_PERM_COMMAND_ARENA_INFO,     true, &HandleArenaInfoCommand,     "", NULL },
            { "lookup",         rbac::RBAC_PERM_COMMAND_ARENA_LOOKUP,  false, &HandleArenaLookupCommand,   "", NULL },
            { "reset",          rbac::RBAC_PERM_COMMAND_RESET_SEASON,  false, NULL,                        "", ArenaResetCommandTable },
            { NULL, 0, false, NULL, "", NULL }
        };
        static ChatCommand commandTable[] =
        {
            { "arena",          rbac::RBAC_PERM_COMMAND_ARENA,     false, NULL,                       "", arenaCommandTable },
            { NULL, 0, false, NULL, "", NULL }
        };
        return commandTable;
    }

    static bool HandleArenaCreateCommand(ChatHandler* handler, char const* args)
    {
        if (!*args)
            return false;

        Player* target;
        if (!handler->extractPlayerTarget(*args != '"' ? (char*)args : NULL, &target))
            return false;

        char* tailStr = *args != '"' ? strtok(NULL, "") : (char*)args;
        if (!tailStr)
            return false;

        char* name = handler->extractQuotedArg(tailStr);
        if (!name)
            return false;

        char* typeStr = strtok(NULL, "");
        if (!typeStr)
            return false;

        int8 type = atoi(typeStr);
        if (sArenaTeamMgr->GetArenaTeamByName(name))
        {
            handler->PSendSysMessage(LANG_ARENA_ERROR_NAME_EXISTS, name);
            handler->SetSentErrorMessage(true);
            return false;
        }

        if (type == 2 || type == 3 || type == 5 )
        {
            if (Player::GetArenaTeamIdFromDB(target->GetGUID(), type) != 0)
            {
                handler->PSendSysMessage(LANG_ARENA_ERROR_SIZE, target->GetName().c_str());
                handler->SetSentErrorMessage(true);
                return false;
            }

            ArenaTeam* arena = new ArenaTeam();

            if (!arena->Create(target->GetGUID(), type, name, 4293102085, 101, 4293253939, 4, 4284049911))
            {
                delete arena;
                handler->SendSysMessage(LANG_BAD_VALUE);
                handler->SetSentErrorMessage(true);
                return false;
            }

            sArenaTeamMgr->AddArenaTeam(arena);
            handler->PSendSysMessage(LANG_ARENA_CREATE, arena->GetName().c_str(), arena->GetId(), arena->GetType(), arena->GetCaptain());
        }
        else
        {
            handler->SendSysMessage(LANG_BAD_VALUE);
            handler->SetSentErrorMessage(true);
            return false;
        }

        return true;
    }

    static bool HandleArenaDisbandCommand(ChatHandler* handler, char const* args)
    {
        if (!*args)
            return false;

        uint32 teamId = atoi((char*)args);
        if (!teamId)
            return false;

        ArenaTeam* arena = sArenaTeamMgr->GetArenaTeamById(teamId);

        if (!arena)
        {
            handler->PSendSysMessage(LANG_ARENA_ERROR_NOT_FOUND, teamId);
            handler->SetSentErrorMessage(true);
            return false;
        }

        if (arena->IsFighting())
        {
            handler->SendSysMessage(LANG_ARENA_ERROR_COMBAT);
            handler->SetSentErrorMessage(true);
            return false;
        }

        std::string name = arena->GetName();
        arena->Disband();
        if (handler->GetSession())
            TC_LOG_DEBUG("bg.arena", "GameMaster: %s [GUID: %u] disbanded arena team type: %u [Id: %u].",
                handler->GetSession()->GetPlayer()->GetName().c_str(), handler->GetSession()->GetPlayer()->GetGUIDLow(), arena->GetType(), teamId);
        else
            TC_LOG_DEBUG("bg.arena", "Console: disbanded arena team type: %u [Id: %u].", arena->GetType(), teamId);

        delete(arena);

        handler->PSendSysMessage(LANG_ARENA_DISBAND, name.c_str(), teamId);
        return true;
    }

    static bool HandleResetSeasonCommand(ChatHandler* handler, const char* args)
    {
        if (!*args)
            return false;

        char* _ArenaType;
        char* _rewards;

        handler->extractOptFirstArg((char*)args, &_ArenaType, &_rewards);

        if (!_ArenaType || !_rewards)
            return false;

        uint8 ArenaType = (uint8)atof(_ArenaType);
        uint8 m_rewards = (uint8)atof(_rewards);

        if ((ArenaType != 2 && ArenaType != 3 && ArenaType != 5) || (m_rewards != 0 && m_rewards != 1))
            return false;

        handler->SendGlobalSysMessage("Resetting Arena Season..... You may experience some lag");

        bool rewards = m_rewards == 0 ? false : true;

        QueryResult result;
        Field* fields;

        uint32 lastrating = 0;
        uint32 maxrating;
        ArenaTeamMgr::ArenaTeamContainer::const_iterator i = sArenaTeamMgr->GetArenaTeamMapBegin();
        for (; i != sArenaTeamMgr->GetArenaTeamMapEnd(); ++i) // loop through existing teams in core rather than DB query
        {
            if (i->second->GetRating() > lastrating && i->second->GetType() == ArenaType)
            {
                maxrating = i->second->GetRating();
                lastrating = maxrating;
            }
        }

        uint32 top01rating = (maxrating - (maxrating * 0.01)); // top 1% of teams -- Wrathful Gladiator's Frost Wyrm & Gladiator title (rating > Top03rating)
        uint32 Top03rating = (maxrating - (maxrating * 0.03)); // top 1% - 3% of teams -- Duelist title
        uint32 Top10rating = (maxrating - (maxrating * 0.1)); // top 3% - 10% of teams -- Rival title
        uint32 Top30rating = (maxrating - (maxrating * 0.3)); // top 10% - 30% of teams -- Challenger title
        std::string top01teams = "", top03teams = "", top10teams = "", top30teams = "";
        i = sArenaTeamMgr->GetArenaTeamMapBegin();
        for (; i != sArenaTeamMgr->GetArenaTeamMapEnd(); ++i)
        {
            if (i->second->GetType() == ArenaType)
            {
                uint32 TeamId = i->second->GetId();
                uint32 TotalTeamGames = i->second->GetStats().SeasonGames;
                result = CharacterDatabase.PQuery("SELECT guid, seasonGames FROM arena_team_member WHERE arenaTeamId = %u", TeamId); // put it first so i can reference it at the end of the loop
                if (i->second->GetStats().Rating >= top01rating && rewards)
                {
                    if (top01teams == "")
                        top01teams = i->second->GetName();
                    else
                        top01teams = top01teams + ", " + i->second->GetName();
                    if (result)
                    {
                        do
                        {
                            fields = result->Fetch();
                            uint32 guid = fields[0].GetUInt32();
                            uint16 totalgames = fields[1].GetUInt16();
                            if (totalgames >= TotalTeamGames / 4) // only give rewards to people who participated in at least 25% of the teams total games
                            {
                                if (Player* player = sObjectMgr->GetPlayerByLowGUID(guid))
                                {
                                    if (i->second->GetStats().Rank == 1)
                                    {
                                        player->learnSpell(71810, false); // Wrathful Gladiator's Frost Wyrm // 

                                        if (AchievementEntry const * WrathMount = sAchievementStore.LookupEntry(4600))
                                            player->CompletedAchievement(WrathMount); // Wrathful Gladiator's Frost Wyrm achievement // 

                                        if (CharTitlesEntry const* WrathGladiatorTitle = sCharTitlesStore.LookupEntry(177))
                                            player->SetTitle(WrathGladiatorTitle); // Wrathful Gladiator <name>

                                        if (AchievementEntry const * WrathGladiatorTAchiev = sAchievementStore.LookupEntry(4599))
                                            player->CompletedAchievement(WrathGladiatorTAchiev); // Wrathful Gladiator title achievement
                                    }

                                    if (CharTitlesEntry const* GladiatorTitle = sCharTitlesStore.LookupEntry(42))
                                        player->SetTitle(GladiatorTitle); // Gladiator <name>

                                    if (AchievementEntry const * GLADIATORACHIEV = sAchievementStore.LookupEntry(2091))
                                        player->CompletedAchievement(GLADIATORACHIEV); // Gladiator title achievement
                                }
                                else
                                {
                                    if (Player* _handler = sObjectMgr->GetPlayerByLowGUID(handler->GetSession()->GetGuidLow()))
                                    {
                                        // no way to optimize this... one way or another it MUST be added to DB, regardless of offline or online
                                        time_t CurrTime = time(NULL);
                                        // learn frost wyrm spell(71810)
                                        if (i->second->GetStats().Rank == 1) // only rank 1 team gets wrathful frost wyrm & wrathful gladiator title
                                        {
                                            CharacterDatabase.PExecute("INSERT IGNORE INTO character_spell VALUES (%u, 71810, 1, 0)", guid);
                                            CharacterDatabase.PExecute("INSERT IGNORE INTO character_achievement VALUES (%u, 4599, %u), (%u, 4600, %u)", guid, CurrTime, guid, CurrTime);
                                            _handler->SetOfflineTitle(_handler->GetSession(), guid, 177); // add Wrathful Gladiator title(177)
                                        }
                                        // add frost wyrm achievement(4600)
                                        // add wrathful gladiator achievement(4599)
                                        // add Gladiator achievement(2091)
                                        CharacterDatabase.PExecute("INSERT IGNORE INTO character_achievement VALUES (%u, 2091, %u)", guid, CurrTime, guid, CurrTime, guid, CurrTime);
                                        _handler->SetOfflineTitle(_handler->GetSession(), guid, 42);
                                    }
                                }
                                i->second->DelMember(guid, true);
                            }
                        }
                        while (result->NextRow()); // loop through arena team members
                    }
                }
                else if (i->second->GetStats().Rating >= Top03rating && i->second->GetStats().Rating < top01rating && rewards)
                {
                    if (top03teams == "")
                        top03teams = i->second->GetName();
                    else
                        top03teams = top03teams + ", " + i->second->GetName();

                    if (result)
                    {
                        do
                        {
                            fields = result->Fetch();
                            uint32 guid = fields[0].GetUInt32();
                            uint16 totalgames = fields[1].GetUInt16();
                            if (totalgames >= TotalTeamGames / 4)
                            {
                                if (Player* player = sObjectMgr->GetPlayerByLowGUID(guid))
                                {
                                    if (CharTitlesEntry const* DUELISTTITLE = sCharTitlesStore.LookupEntry(43))
                                        player->SetTitle(DUELISTTITLE); // Duelist <name>
                                    if (AchievementEntry const * DUELISTACHIEV = sAchievementStore.LookupEntry(2092))
                                        player->CompletedAchievement(DUELISTACHIEV); // duelist title achievement
                                }
                                else
                                {
                                    if (Player* _handler = sObjectMgr->GetPlayerByLowGUID(handler->GetSession()->GetGuidLow()))
                                    {
                                        // no way to optimize this... one way or another it MUST be added to DB, regardless of offline or online
                                        time_t CurrTime = time(NULL);
                                        // add duelist achievement(2092)
                                        CharacterDatabase.PExecute("INSERT IGNORE INTO character_achievement VALUES (%u, 2092, %u)", guid, CurrTime, guid, CurrTime);
                                        player->SetOfflineTitle(handler->GetSession(), guid, 43); // add Duelist title(43)
                                    }
                                }
                                i->second->DelMember(guid, true);
                            }
                        }
                        while (result->NextRow());
                    }
                }
                else if (i->second->GetStats().Rating >= Top10rating && i->second->GetStats().Rating < Top03rating && rewards)
                {
                    if (top10teams == "")
                        top10teams = i->second->GetName();
                    else
                        top10teams = top10teams + ", " + i->second->GetName();
                        
                    if (result)
                    {
                        do
                        {
                            fields = result->Fetch();
                            uint32 guid = fields[0].GetUInt32();
                            uint16 totalgames = fields[1].GetUInt16();
                            if (totalgames >= TotalTeamGames / 4)
                            {
                                if (Player* player = sObjectMgr->GetPlayerByLowGUID(guid)) // online or offline?
                                {
                                    if (CharTitlesEntry const* titleInfo = sCharTitlesStore.LookupEntry(44))
                                        player->SetTitle(titleInfo); // Rival <name>
                                    if (AchievementEntry const * RIVALACHIEV = sAchievementStore.LookupEntry(2093))
                                        player->CompletedAchievement(RIVALACHIEV); // rival title achievement
                                }
                                else
                                {
                                    // no way to optimize this... one way or another it MUST be added to DB, regardless of offline or online
                                    time_t CurrTime = time(NULL);
                                    // add rival achievement(2093)
                                    CharacterDatabase.PExecute("INSERT IGNORE INTO character_achievement VALUES (%u, 2093, %u)", guid, CurrTime, guid, CurrTime);
                                    player->SetOfflineTitle(handler->GetSession(), guid, 44); // add rival title(43)
                                }
                            }
                            i->second->DelMember(guid, true);
                        }
                        while (result->NextRow());
                    }
                }
                else if (i->second->GetStats().Rating >= Top30rating && i->second->GetStats().Rating < Top10rating && rewards)
                {
                    if (top30teams == "")
                        top30teams = i->second->GetName();
                    else
                        top30teams = top30teams + ", " + i->second->GetName();
                        
                    if (result)
                    {
                        do
                        {
                            fields = result->Fetch();
                            uint32 guid = fields[0].GetUInt32();
                            uint16 totalgames = fields[1].GetUInt16();
                            if (totalgames >= TotalTeamGames / 4)
                            {
                                if (Player* player = sObjectMgr->GetPlayerByLowGUID(guid))
                                {
                                    if (CharTitlesEntry const* titleInfo = sCharTitlesStore.LookupEntry(45))
                                        player->SetTitle(titleInfo); // Challenger <name>
                                    if (AchievementEntry const * CHALLENGERACHIEV = sAchievementStore.LookupEntry(2090))
                                        player->CompletedAchievement(CHALLENGERACHIEV); // rival title achievement
                                }
                                else
                                {
                                    // no way to optimize this... one way or another it MUST be added to DB, regardless of offline or online
                                    time_t CurrTime = time(NULL);
                                    // add rival achievement(2090)
                                    CharacterDatabase.PExecute("INSERT IGNORE INTO character_achievement VALUES (%u, 2090, %u)", guid, CurrTime, guid, CurrTime);
                                    player->SetOfflineTitle(handler->GetSession(), guid, 45); // add challenger title(43)
                                }
                            }
                            i->second->DelMember(guid, true);
                        }
                        while (result->NextRow());
                    }
                }
                else
                {
                    if (result)
                    {
                        do
                        {
                            fields = result->Fetch();
                            uint32 guid = fields[0].GetUInt32();
                            i->second->DelMember(guid, true); // isn't in a top ranking team, still need to remove them
                        }
                        while (result->NextRow());
                    }
                }
            }
        }

        SessionMap sessions = sWorld->GetAllSessions();
        for (SessionMap::const_iterator itr = sessions.begin(); itr != sessions.end(); ++itr)
            if (Player* player = itr->second->GetPlayer())
                player->SetArenaPoints(0);

        // GG BRO
        TC_LOG_INFO("server.loading", "Deleting arena team members...");
        handler->SendGlobalGMSysMessage("Resetting Arena Season... (part 1 of 5)");
        CharacterDatabase.PExecute("DELETE FROM arena_team_member WHERE arenaTeamId IN (select arenaTeamId from arena_team where type = %u)", ArenaType);
        TC_LOG_INFO("server.loading", "Deleting arena teams...");
        handler->SendGlobalGMSysMessage("Resetting Arena Season... (part 2 of 5)");
        CharacterDatabase.PExecute("DELETE FROM arena_team WHERE type = %u", ArenaType);
        TC_LOG_INFO("server.loading", "Updating arena points...");
        handler->SendGlobalGMSysMessage("Resetting Arena Season... (part 3 of 5)");
        CharacterDatabase.PExecute("UPDATE characters SET arenaPoints = 0 WHERE arenaPoints <> 0");
        TC_LOG_INFO("server.loading", "Resetting arena stats...");
        handler->SendGlobalGMSysMessage("Resetting Arena Season... (part 4 of 5)");
        switch (ArenaType)
        {
            case 2:
                CharacterDatabase.PExecute("DELETE FROM character_arena_stats WHERE slot = 0");
                break;
            case 3:
                CharacterDatabase.PExecute("DELETE FROM character_arena_stats WHERE slot = 1");
                break;
            case 5:
                CharacterDatabase.PExecute("DELETE FROM character_arena_stats WHERE slot = 2");
                break;
        }
        TC_LOG_INFO("server.loading", "Reloading Arena Teams...");
        handler->SendGlobalGMSysMessage("Resetting Arena Season... (part 5 of 5)");
        sArenaTeamMgr->LoadArenaTeams(ArenaType); // in theory this will always load 0 teams

        std::string m_ArenaType = _ArenaType;
        std::string AllTeams = "|cffff00ee" + top01teams + ", " + top03teams + ", " + top10teams + ", " + top30teams;
        std::string notifymessage = "|cffffffffA new Arena Season has started for " + m_ArenaType + "v" + m_ArenaType + ", you may need to relog to see changes.";
        std::string congratmsg = "|cffff00eeCongratulations to the following teams for ranking in the top 1-30% for " + m_ArenaType + "v" + m_ArenaType + ":";

        handler->SendGlobalSysMessage(notifymessage.c_str());
        WorldPacket data(SMSG_NOTIFICATION, (notifymessage.size()+1));
        data << notifymessage;
        sWorld->SendGlobalMessage(&data);

        if (rewards)
        {
            sWorld->SendServerMessage(SERVER_MSG_STRING, congratmsg.c_str());
            sWorld->SendServerMessage(SERVER_MSG_STRING, AllTeams.c_str());
        }
        return true;
    }

    static bool HandleArenaRenameCommand(ChatHandler* handler, char const* _args)
    {
        if (!*_args)
            return false;

        char* args = (char *)_args;

        char const* oldArenaStr = handler->extractQuotedArg(args);
        if (!oldArenaStr)
        {
            handler->SendSysMessage(LANG_BAD_VALUE);
            handler->SetSentErrorMessage(true);
            return false;
        }

        char const* newArenaStr = handler->extractQuotedArg(strtok(NULL, ""));
        if (!newArenaStr)
        {
            handler->SendSysMessage(LANG_BAD_VALUE);
            handler->SetSentErrorMessage(true);
            return false;
        }

        ArenaTeam* arena = sArenaTeamMgr->GetArenaTeamByName(oldArenaStr);
        if (!arena)
        {
            handler->PSendSysMessage(LANG_AREAN_ERROR_NAME_NOT_FOUND, oldArenaStr);
            handler->SetSentErrorMessage(true);
            return false;
        }

        if (sArenaTeamMgr->GetArenaTeamByName(newArenaStr))
        {
            handler->PSendSysMessage(LANG_ARENA_ERROR_NAME_EXISTS, oldArenaStr);
            handler->SetSentErrorMessage(true);
            return false;
        }

        if (arena->IsFighting())
        {
            handler->SendSysMessage(LANG_ARENA_ERROR_COMBAT);
            handler->SetSentErrorMessage(true);
            return false;
        }

        if (!arena->SetName(newArenaStr))
        {
            handler->SendSysMessage(LANG_BAD_VALUE);
            handler->SetSentErrorMessage(true);
            return false;
        }

        handler->PSendSysMessage(LANG_ARENA_RENAME, arena->GetId(), oldArenaStr, newArenaStr);
        if (handler->GetSession())
            TC_LOG_DEBUG("bg.arena", "GameMaster: %s [GUID: %u] rename arena team \"%s\"[Id: %u] to \"%s\"",
                handler->GetSession()->GetPlayer()->GetName().c_str(), handler->GetSession()->GetPlayer()->GetGUIDLow(), oldArenaStr, arena->GetId(), newArenaStr);
        else
            TC_LOG_DEBUG("bg.arena", "Console: rename arena team \"%s\"[Id: %u] to \"%s\"", oldArenaStr, arena->GetId(), newArenaStr);

        return true;
    }

    static bool HandleArenaCaptainCommand(ChatHandler* handler, char const* args)
    {
        if (!*args)
            return false;

        char* idStr;
        char* nameStr;
        handler->extractOptFirstArg((char*)args, &idStr, &nameStr);
        if (!idStr)
            return false;

        uint32 teamId = atoi(idStr);
        if (!teamId)
            return false;

        Player* target;
        uint64 targetGuid;
        if (!handler->extractPlayerTarget(nameStr, &target, &targetGuid))
            return false;

        ArenaTeam* arena = sArenaTeamMgr->GetArenaTeamById(teamId);

        if (!arena)
        {
            handler->PSendSysMessage(LANG_ARENA_ERROR_NOT_FOUND, teamId);
            handler->SetSentErrorMessage(true);
            return false;
        }

        if (!target)
        {
            handler->PSendSysMessage(LANG_PLAYER_NOT_EXIST_OR_OFFLINE, nameStr);
            handler->SetSentErrorMessage(true);
            return false;
        }

        if (arena->IsFighting())
        {
            handler->SendSysMessage(LANG_ARENA_ERROR_COMBAT);
            handler->SetSentErrorMessage(true);
            return false;
        }

        if (!arena->IsMember(targetGuid))
        {
            handler->PSendSysMessage(LANG_ARENA_ERROR_NOT_MEMBER, nameStr, arena->GetName().c_str());
            handler->SetSentErrorMessage(true);
            return false;
        }

        if (arena->GetCaptain() == targetGuid)
        {
            handler->PSendSysMessage(LANG_ARENA_ERROR_CAPTAIN, nameStr, arena->GetName().c_str());
            handler->SetSentErrorMessage(true);
            return false;
        }

        arena->SetCaptain(targetGuid);

        CharacterNameData const* oldCaptainNameData = sWorld->GetCharacterNameData(GUID_LOPART(arena->GetCaptain()));
        if (!oldCaptainNameData)
        {
            handler->SetSentErrorMessage(true);
            return false;
        }

        handler->PSendSysMessage(LANG_ARENA_CAPTAIN, arena->GetName().c_str(), arena->GetId(), oldCaptainNameData->m_name.c_str(), target->GetName().c_str());
        if (handler->GetSession())
            TC_LOG_DEBUG("bg.arena", "GameMaster: %s [GUID: %u] promoted player: %s [GUID: %u] to leader of arena team \"%s\"[Id: %u]",
                handler->GetSession()->GetPlayer()->GetName().c_str(), handler->GetSession()->GetPlayer()->GetGUIDLow(), target->GetName().c_str(), target->GetGUIDLow(), arena->GetName().c_str(), arena->GetId());
        else
            TC_LOG_DEBUG("bg.arena", "Console: promoted player: %s [GUID: %u] to leader of arena team \"%s\"[Id: %u]",
                target->GetName().c_str(), target->GetGUIDLow(), arena->GetName().c_str(), arena->GetId());

        return true;
    }

    static bool HandleArenaInfoCommand(ChatHandler* handler, char const* args)
    {
        if (!*args)
            return false;

        uint32 teamId = atoi((char*)args);
        if (!teamId)
            return false;

        ArenaTeam* arena = sArenaTeamMgr->GetArenaTeamById(teamId);

        if (!arena)
        {
            handler->PSendSysMessage(LANG_ARENA_ERROR_NOT_FOUND, teamId);
            handler->SetSentErrorMessage(true);
            return false;
        }

        handler->PSendSysMessage(LANG_ARENA_INFO_HEADER, arena->GetName().c_str(), arena->GetId(), arena->GetRating(), arena->GetType(), arena->GetType());
        for (ArenaTeam::MemberList::iterator itr = arena->m_membersBegin(); itr != arena->m_membersEnd(); ++itr)
            handler->PSendSysMessage(LANG_ARENA_INFO_MEMBERS, itr->Name.c_str(), GUID_LOPART(itr->Guid), itr->PersonalRating, (arena->GetCaptain() == itr->Guid ? "- Captain" : ""));

        return true;
    }

    static bool HandleArenaLookupCommand(ChatHandler* handler, char const* args)
    {
        if (!*args)
            return false;

        std::string namepart = args;
        std::wstring wnamepart;

        if (!Utf8toWStr(namepart, wnamepart))
            return false;

        wstrToLower(wnamepart);

        bool found = false;
        ArenaTeamMgr::ArenaTeamContainer::const_iterator i = sArenaTeamMgr->GetArenaTeamMapBegin();
        for (; i != sArenaTeamMgr->GetArenaTeamMapEnd(); ++i)
        {
            ArenaTeam* arena = i->second;

            if (Utf8FitTo(arena->GetName(), wnamepart))
            {
                if (handler->GetSession())
                {
                    handler->PSendSysMessage(LANG_ARENA_LOOKUP, arena->GetName().c_str(), arena->GetId(), arena->GetType(), arena->GetType());
                    found = true;
                    continue;
                }
             }
        }

        if (!found)
            handler->PSendSysMessage(LANG_AREAN_ERROR_NAME_NOT_FOUND, namepart.c_str());

        return true;
    }
};

void AddSC_arena_commandscript()
{
    new arena_commandscript();
}

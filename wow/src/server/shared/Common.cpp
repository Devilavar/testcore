/*
 * Copyright (C) 2008-2014 TrinityCore 
 * Copyright (C) 2005-2009 MaNGOS <http://getmangos.com/>
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

#include "Common.h"

char const* localeNames[TOTAL_LOCALES] = {
  "enUS",
  "koKR",
  "frFR",
  "deDE",
  "zhCN",
  "zhTW",
  "esES",
  "esMX",
  "ruRU"
};

LocaleConstant GetLocaleByName(const std::string& name)
{
    for (uint32 i = 0; i < TOTAL_LOCALES; ++i)
        if (name==localeNames[i])
            return LocaleConstant(i);

    return LOCALE_enUS;                                     // including enGB case
}

namespace AuthHelper
{
    static RealmBuildInfo const MopAcceptedClientBuilds[] =
    {
        {18019, 5, 4, 7, ' '},
        {17956, 5, 4, 7, ' '},
        {17930, 5, 4, 7, ' '},
        {17898, 5, 4, 7, ' '},

        {17688, 5, 4, 2, ' '},
        {17658, 5, 4, 2, ' '},

        {17538, 5, 4, 1, ' '},

        {17359, 5, 4, 0, ' '},

        {17128, 5, 3, 0, 'a'},
        {16981, 5, 3, 0, ' '},

        {16826, 5, 2, 0, 'j'},
        {16769, 5, 2, 0, 'i'},
        {16760, 5, 2, 0, 'h'},
        {16733, 5, 2, 0, 'g'},
        {16716, 5, 2, 0, 'f'},
        {16709, 5, 2, 0, 'e'},
        {16701, 5, 2, 0, 'd'},
        {16685, 5, 2, 0, 'c'},
        {16683, 5, 2, 0, 'b'},
        {16669, 5, 2, 0, 'a'},
        {16650, 5, 2, 0, ' '},

        {16357, 5, 1, 0, 'a'},
        {16309, 5, 1, 0, ' '},

        {16135, 5, 0, 5, 'b'},

        {0,     0, 0, 0, ' '}                                   // terminator
    };

    static RealmBuildInfo const CataAcceptedClientBuilds[] =
    {
        {15595, 4, 3, 4, ' '},
        {14545, 4, 2, 2, ' '},
        {13623, 4, 0, 6, 'a'},
        {0,     0, 0, 0, ' '}                                   // terminator
    };

    static RealmBuildInfo const WotlkAcceptedClientBuilds[] =
    {
        {12340, 3, 3, 5, 'a'},
        {11723, 3, 3, 3, 'a'},
        {11403, 3, 3, 2, ' '},
        {11159, 3, 3, 0, 'a'},
        {10505, 3, 2, 2, 'a'},
        {9947,  3, 1, 3, ' '},
        {0,     0, 0, 0, ' '}                                   // terminator
    };

    static RealmBuildInfo const BcAcceptedClientBuilds[] =
    {
        {8606,  2, 4, 3, ' '},
        {0,     0, 0, 0, ' '}                                   // terminator
    };

    static RealmBuildInfo const VanillaAcceptedClientBuilds[] =
    {
        {6005,  1, 12, 2, ' '},
        {5875,  1, 12, 1, ' '},
        {0,     0,  0, 0, ' '}                                   // terminator
    };

    RealmBuildInfo const* GetBuildInfo(int build)
    {
        for (int i = 0; MopAcceptedClientBuilds[i].Build; ++i)
            if (MopAcceptedClientBuilds[i].Build == build)
                return &MopAcceptedClientBuilds[i];

        for (int i = 0; CataAcceptedClientBuilds[i].Build; ++i)
            if (CataAcceptedClientBuilds[i].Build == build)
                return &CataAcceptedClientBuilds[i];

        for (int i = 0; WotlkAcceptedClientBuilds[i].Build; ++i)
            if (WotlkAcceptedClientBuilds[i].Build == build)
                return &WotlkAcceptedClientBuilds[i];

        for (int i = 0; BcAcceptedClientBuilds[i].Build; ++i)
            if (BcAcceptedClientBuilds[i].Build == build)
                return &BcAcceptedClientBuilds[i];

        for (int i = 0; VanillaAcceptedClientBuilds[i].Build; ++i)
            if (VanillaAcceptedClientBuilds[i].Build == build)
                return &VanillaAcceptedClientBuilds[i];

        return NULL;
    }

    int GetExpansionFlag(int build)
    {
        if (RealmBuildInfo const* buildInfo = GetBuildInfo(build))
            return buildInfo->MajorVersion;

        return NO_VALID_EXP_FLAG;
    }

    bool CanConnectTo(int ClientBuild, int RealmBuild, int ClientExpFlag)
    {
        int RealmExpFlag = GetExpansionFlag(RealmBuild);

        switch (RealmExpFlag)
        {
            case NO_VALID_EXP_FLAG:
                return false;
            case VANILLA_EXP_FLAG:
            case BC_EXP_FLAG:
            case WOTLK_EXP_FLAG:
            case CATA_EXP_FLAG:
            case MOP_EXP_FLAG:
                if (ClientBuild == RealmBuild)
                    return true;
                return false;
                /*
            case MOP_EXP_FLAG:
                if (ClientExpFlag < 0)
                    ClientExpFlag = GetExpansionFlag(ClientBuild);

                if (ClientBuild < RealmBuild)
                    return false;

                if (ClientExpFlag == RealmExpFlag)
                    return true;
                return false;
                */
            default:
                return false;
        }
    }
};

// Copyright BattleDash. All Rights Reserved.

#pragma once

#include <string>

namespace Kyber
{
struct GameLevel
{
    const char* level;
    const char* name;
};

static GameLevel s_game_levels[] = {
    { "XP_Offline/Levels/Arctic/Arctic_01_Offline/Arctic_01_Offline", "OFFLINE WA" },
    { "Levels/Arctic/Arctic_01/Arctic_01", "OUTPOST BETA" },
    { "Levels/Arctic/Arctic_02/Arctic_02", "ICE CAVES" },
    { "Levels/Arctic/Arctic_03/Arctic_03", "REBEL BASE" },
    { "Levels/Arctic/Arctic_04/Arctic_04", "TWILIGHT ON HOTH" },
    { "Levels/Desert/Desert_01/Desert_01", "JUNDLAND WASTES" },
    { "Levels/Desert/Desert_02/Desert_02", "JAWA REFUGE" },
    { "Levels/Desert/Desert_03/Desert_03", "REBEL DEPOT" },
    { "Levels/Desert/Desert_13_MP/Desert_13_MP", "RAIDER CAMP" },
    { "Levels/Desert/Desert_04/Desert_04", "DUNE SEA EXCHANGE" },
    { "Levels/Forest/Forest_01/Forest_01", "FOREST MOON OF ENDOR" },
    { "Levels/Forest/Forest_02/Forest_02", "SWAMP CRASH SITE" },
    { "Levels/Forest/Forest_03/Forest_03", "IMPERIAL STATION" },
    { "Levels/Forest/Forest_06/Forest_06", "SURVIVORS OF ENDOR" },
    { "Levels/Volcanic/Volcanic_01/Volcanic_01", "SOROSUUB CENTROPLEX" },
    { "Levels/Volcanic/Volcanic_02/Volcanic_02", "SULFER FIELDS" },
    { "Levels/Volcanic/Volcanic_03/Volcanic_03", "IMPERIAL HANGER" },
    { "XP0/Levels/Valley/Valley_01/Valley_01", "GRAVEYARD OF GIANTS" },
    { "XP0/Levels/Valley/Valley_02/Valley_02", "GOAZON BADLANDS" },
    { "XP1/Levels/Industrial_01/Industrial_01", "SOROSUUB  REFINERY" },
    { "XP1/Levels/Industrial_02/Industrial_02", "SOROSUUB  PIPELINES" },
    { "XP1/Levels/Palace_01/Palace_01", "JABBAS PALACE" },
    { "XP1/Levels/Palace_02/Palace_02", "PALACE GARAGE" },
    { "XP2/Levels/Clouds/Clouds_01/Clouds_01", "CLOUD CITY" },
    { "XP2/Levels/Clouds/Clouds_02/Clouds_02", "ADMINISTRATORS PALACE" },
    { "XP2/Levels/Clouds/Clouds_03/Clouds_03", "BIONIIP LABORATORIES" },
    { "XP2/Levels/Clouds/Clouds_04/Clouds_04", "CARBONITE-FREEZING CHAMBERS" },
    { "XP2/Levels/Clouds/Clouds_05/Clouds_05", "BESPIN AIRSPACE" },
    { "XP3/Levels/Space/Space_01/Space_01", "DEFENSE SECTOR" },
    { "XP3/Levels/Space/Space_02/Space_02", "POWER SECTOR" },
    { "XP3/Levels/Space/Space_03/Space_03", "COMMAND SECTOR" },
    { "XP4/Levels/Beach/Beach_01/Beach_01", "SCARIF BEACH" },
    { "XP4/Levels/Beach/Beach_02/Beach_02", "LANDING PAD 13" },
    { "XP4/Levels/Beach/Beach_03/Beach_03", "SCARIF JUNGLE" },
    { "XP_Offline/Levels/Arctic/Arctic_04_Offline/Arctic_04_Offline", "OFFLINE TWILIGHT ON HOTH" },
};

GameLevel GetGameLevel(const char* level);
} // namespace Kyber

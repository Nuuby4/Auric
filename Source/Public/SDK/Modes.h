// Copyright BattleDash. All Rights Reserved.

#pragma once

#include <SDK/Levels.h>

#include <string>
#include <vector>

namespace Kyber
{
struct GameMode
{
    const char* mode;
    const char* name;
    std::vector<const char*> levels;
    std::vector<GameLevel> levelOverrides;
};

//clang-format off
// What if you wanted to format something nicely, but clang-format said:
static GameMode s_game_modes[] = {
    { "Cargo",   // InclusionSetting
        "CARGO", // The Display Name
        { "Levels/Arctic/Arctic_03/Arctic_03", "Levels/Desert/Desert_03/Desert_03", "Levels/Volcanic/Volcanic_03/Volcanic_03",
            "Levels/Forest/Forest_03/Forest_03", "XP0/Levels/Valley/Valley_02/Valley_02", "XP1/Levels/Industrial_01/Industrial_01",
            "XP1/Levels/Industrial_02/Industrial_02", "XP1/Levels/Palace_01/Palace_01", "XP1/Levels/Palace_02/Palace_02",
            "XP2/Levels/Clouds/Clouds_02/Clouds_02", "XP2/Levels/Clouds/Clouds_04/Clouds_04", "XP3/Levels/Space/Space_01/Space_01",
            "XP3/Levels/Space/Space_02/Space_02", "XP3/Levels/Space/Space_03/Space_03" } },
    { "DropZone",    // InclusionSetting
        "DROP ZONE", // The Display Name
        { "Levels/Arctic/Arctic_02/Arctic_02", "Levels/Desert/Desert_02/Desert_02", "Levels/Forest/Forest_02/Forest_02",
            "Levels/Desert/Desert_03/Desert_03", "Levels/Volcanic/Volcanic_02/Volcanic_02", "Levels/Desert/Desert_04/Desert_04",
            "Levels/Desert/Desert_13_MP/Desert_13_MP", "XP0/Levels/Valley/Valley_02/Valley_02", "Levels/Arctic/Arctic_04/Arctic_04",
            "XP2/Levels/Clouds/Clouds_03/Clouds_03" } },
    { "Domination",  // InclusionSetting
        "DROID RUN", // The Display Name
        { "Levels/Forest/Forest_02/Forest_02", "Levels/Volcanic/Volcanic_02/Volcanic_02", "Levels/Arctic/Arctic_02/Arctic_02",
            "Levels/Desert/Desert_02/Desert_02", "Levels/Arctic/Arctic_03/Arctic_03", "Levels/Desert/Desert_04/Desert_04",
            "XP0/Levels/Valley/Valley_02/Valley_02", "Levels/Desert/Desert_13_MP/Desert_13_MP", "XP1/Levels/Industrial_01/Industrial_01",
            "XP1/Levels/Industrial_02/Industrial_02", "XP1/Levels/Palace_01/Palace_01", "XP1/Levels/Palace_02/Palace_02",
            "XP2/Levels/Clouds/Clouds_02/Clouds_02", "XP2/Levels/Clouds/Clouds_03/Clouds_03", "XP2/Levels/Clouds/Clouds_04/Clouds_04",
            "XP3/Levels/Space/Space_01/Space_01", "XP3/Levels/Space/Space_02/Space_02", "XP3/Levels/Space/Space_03/Space_03",

            "Levels/Volcanic/Volcanic_03/Volcanic_03", "Levels/Forest/Forest_03/Forest_03", "Levels/Desert/Desert_03/Desert_03" },
        { { "Levels/Volcanic/Volcanic_03/Volcanic_03", "IMPERIAL HANGER - UNFINISHED" },
            { "Levels/Forest/Forest_03/Forest_03", "IMPERIAL STATION - UNFINISHED" },
            { "Levels/Desert/Desert_03/Desert_03", "REBEL DEPOT - UNFINISHED" } } },
    { "TeamDeathMatch", // InclusionSetting
        "BLAST",        // The Display Name
        { "Levels/Desert/Desert_02/Desert_02", "Levels/Forest/Forest_02/Forest_02", "Levels/Arctic/Arctic_02/Arctic_02",
            "Levels/Desert/Desert_03/Desert_03", "Levels/Volcanic/Volcanic_03/Volcanic_03", "Levels/Forest/Forest_03/Forest_03",
            "Levels/Volcanic/Volcanic_02/Volcanic_02", "Levels/Desert/Desert_04/Desert_04", "Levels/Arctic/Arctic_03/Arctic_03",
            "XP0/Levels/Valley/Valley_02/Valley_02", "Levels/Desert/Desert_13_MP/Desert_13_MP", "Levels/Arctic/Arctic_04/Arctic_04",
            "XP1/Levels/Industrial_01/Industrial_01", "XP1/Levels/Industrial_02/Industrial_02", "XP1/Levels/Palace_01/Palace_01",
            "XP1/Levels/Palace_02/Palace_02", "XP2/Levels/Clouds/Clouds_02/Clouds_02", "XP2/Levels/Clouds/Clouds_03/Clouds_03",
            "XP2/Levels/Clouds/Clouds_04/Clouds_04", "XP3/Levels/Space/Space_01/Space_01", "XP3/Levels/Space/Space_02/Space_02",
            "XP3/Levels/Space/Space_03/Space_03", "XP4/Levels/Beach/Beach_01/Beach_01", "XP4/Levels/Beach/Beach_02/Beach_02",
            "XP4/Levels/Beach/Beach_03/Beach_03" } },
    { "HeroesVersusVillains", "HEROES VS. VILLAINS",
        { "Levels/Arctic/Arctic_02/Arctic_02", "Levels/Desert/Desert_02/Desert_02", "Levels/Forest/Forest_02/Forest_02",
            "Levels/Volcanic/Volcanic_02/Volcanic_02", "Levels/Desert/Desert_04/Desert_04", "Levels/Forest/Forest_03/Forest_03",
            "XP0/Levels/Valley/Valley_02/Valley_02", "Levels/Desert/Desert_13_MP/Desert_13_MP", "Levels/Arctic/Arctic_04/Arctic_04",
            "XP1/Levels/Industrial_01/Industrial_01", "XP1/Levels/Industrial_02/Industrial_02", "XP1/Levels/Palace_01/Palace_01",
            "XP1/Levels/Palace_02/Palace_02" } },
    { "HeroHunt",    // InclusionSetting
        "HERO HUNT", // The Display Name
        { "Levels/Desert/Desert_02/Desert_02", "Levels/Forest/Forest_02/Forest_02", "Levels/Volcanic/Volcanic_02/Volcanic_02",
            "Levels/Arctic/Arctic_02/Arctic_02", "Levels/Desert/Desert_04/Desert_04", "Levels/Forest/Forest_03/Forest_03",
            "Levels/Volcanic/Volcanic_03/Volcanic_03", "Levels/Desert/Desert_03/Desert_03", "Levels/Arctic/Arctic_03/Arctic_03",
            "Levels/Desert/Desert_13_MP/Desert_13_MP", "XP1/Levels/Industrial_01/Industrial_01", "XP1/Levels/Industrial_02/Industrial_02",
            "XP1/Levels/Palace_01/Palace_01", "XP1/Levels/Palace_02/Palace_02" } },
    { "HeroTeamDeathMatch", // InclusionSetting
        "HERO BLAST",       // The Display Name
        { "Levels/Desert/Desert_02/Desert_02", "Levels/Forest/Forest_02/Forest_02", "Levels/Arctic/Arctic_02/Arctic_02",
            "Levels/Desert/Desert_03/Desert_03", "Levels/Volcanic/Volcanic_03/Volcanic_03", "Levels/Forest/Forest_03/Forest_03",
            "Levels/Volcanic/Volcanic_02/Volcanic_02", "Levels/Desert/Desert_04/Desert_04", "Levels/Arctic/Arctic_03/Arctic_03",
            "XP0/Levels/Valley/Valley_02/Valley_02", "Levels/Desert/Desert_13_MP/Desert_13_MP", "Levels/Arctic/Arctic_04/Arctic_04",
            "XP1/Levels/Industrial_01/Industrial_01", "XP1/Levels/Industrial_02/Industrial_02", "XP1/Levels/Palace_01/Palace_01",
            "XP1/Levels/Palace_02/Palace_02", "XP2/Levels/Clouds/Clouds_02/Clouds_02", "XP2/Levels/Clouds/Clouds_03/Clouds_03",
            "XP2/Levels/Clouds/Clouds_04/Clouds_04", "XP3/Levels/Space/Space_01/Space_01", "XP3/Levels/Space/Space_02/Space_02",
            "XP3/Levels/Space/Space_03/Space_03", "XP4/Levels/Beach/Beach_01/Beach_01", "XP4/Levels/Beach/Beach_02/Beach_02",
            "XP4/Levels/Beach/Beach_03/Beach_03" } },
    { "WalkerAssault",    // InclusionSetting
        "WALKER ASSAULT", // The Display Name
        { "Levels/Arctic/Arctic_01/Arctic_01", "Levels/Desert/Desert_01/Desert_01", "Levels/Forest/Forest_01/Forest_01",
            "Levels/Volcanic/Volcanic_01/Volcanic_01", "XP0/Levels/Valley/Valley_01/Valley_01", "Levels/Arctic/Arctic_04/Arctic_04",
            "Levels/Forest/Forest_06/Forest_06", "XP2/Levels/Clouds/Clouds_01/Clouds_01", "XP4/Levels/Beach/Beach_01/Beach_01",
            "XP4/Levels/Beach/Beach_02/Beach_02" } },
    { "Supremacy",   // InclusionSetting
        "SUPREMACY", // The Display Name
        { "Levels/Arctic/Arctic_01/Arctic_01", "Levels/Desert/Desert_01/Desert_01", "Levels/Forest/Forest_01/Forest_01",
            "Levels/Volcanic/Volcanic_01/Volcanic_01", "XP0/Levels/Valley/Valley_01/Valley_01", "Levels/Arctic/Arctic_04/Arctic_04",
            "Levels/Forest/Forest_06/Forest_06", "XP2/Levels/Clouds/Clouds_01/Clouds_01" } },
    { "RebelAssault",    // InclusionSetting
        "TURNING POINT", // The Display Name
        { "XP0/Levels/Valley/Valley_01/Valley_01", "Levels/Arctic/Arctic_01/Arctic_01", "Levels/Forest/Forest_01/Forest_01",
            "Levels/Volcanic/Volcanic_01/Volcanic_01", "Levels/Desert/Desert_01/Desert_01", "Levels/Arctic/Arctic_04/Arctic_04",
            "Levels/Forest/Forest_06/Forest_06", "XP2/Levels/Clouds/Clouds_01/Clouds_01", "XP4/Levels/Beach/Beach_01/Beach_01",
            "XP4/Levels/Beach/Beach_02/Beach_02", "XP4/Levels/Beach/Beach_03/Beach_03" } },
    { "FighterSquadron",    // InclusionSetting
        "FIGHTER SQUADRON", // The Display Name
        {
            "Levels/Desert/Desert_01/Desert_01",
            "Levels/Volcanic/Volcanic_01/Volcanic_01",
            "Levels/Desert/Desert_02/Desert_02",
            "Levels/Arctic/Arctic_01/Arctic_01",
            "XP0/Levels/Valley/Valley_01/Valley_01",
            "Levels/Arctic/Arctic_04/Arctic_04",
            "XP2/Levels/Clouds/Clouds_05/Clouds_05",
        } },
    { "Payload",      // InclusionSetting
        "EXTRACTION", // The Display Name
        { "XP1/Levels/Industrial_01/Industrial_01", "XP1/Levels/Industrial_02/Industrial_02", "XP1/Levels/Palace_01/Palace_01",
            "XP1/Levels/Palace_02/Palace_02" } },
    { "HeroAssault", // InclusionSetting
        "SABOTAGE",  // The Display Name
        { "XP2/Levels/Clouds/Clouds_01/Clouds_01", "XP2/Levels/Clouds/Clouds_02/Clouds_02", "XP2/Levels/Clouds/Clouds_03/Clouds_03",
            "XP2/Levels/Clouds/Clouds_04/Clouds_04" } },
    { "SpaceBattle",      // InclusionSetting
        "BATTLE STATION", // The Display Name
        { "XP3/Levels/Space/Space_01/Space_01", "XP3/Levels/Space/Space_02/Space_02", "XP3/Levels/Space/Space_03/Space_03" } },
    { "SpaceToGround",  // InclusionSetting
        "INFILTRATION", // The Display Name
        { "XP4/Levels/Beach/Beach_01/Beach_01", "XP4/Levels/Beach/Beach_02/Beach_02", "XP4/Levels/Beach/Beach_03/Beach_03" } },
};

GameMode GetGameMode(const char* mode);
GameLevel GetGameLevel(GameMode mode, const char* level);
} // namespace Kyber

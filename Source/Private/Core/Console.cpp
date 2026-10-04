// Copyright Nuuby. All Rights Reserved

#include <Core/Console.h>
#include <Core/Program.h>

#include <Hook/Func.h>
#include <Hook/HookManager.h>
#include <Base/Log.h>

#include <string>
#include <iostream>

namespace Kyber
{
TL_DECLARE_FUNC(0x14334B160, void, ConsoleRegistry_registerConsoleMethods, const char* groupName, ConsoleMethod* methods, int count);

void Console::RegisterConsoleCommand(StaticConsoleMethodPtr_t func, const char* name, const char* description)
{
    ConsoleMethod* method = new ConsoleMethod{ func, name, 0, description };
    ConsoleRegistry_registerConsoleMethods("Auric", method, 1);
}   

void ExtentDebug(ConsoleContext& cc)
{
    ServerPlayer* serverPlayer = ServerGameContext::Get()->GetPlayerManager()->m_players[0];
    if (serverPlayer == nullptr)
    {
        cc << "Couldn't find serverPlayer ";
        return;
    }

    ClientPlayer* clientPlayer = ClientGameContext::Get()->GetPlayerManager()->m_players[0];
    if (clientPlayer == nullptr)
    {
        cc << "Couldn't find clientPlayer ";
        return;
    }

    serverPlayer->LogExtents();
    clientPlayer->LogExtents();
}

void IncreaseChargeAmount(ConsoleContext& cc)
{
    auto stream = cc.stream();
    std::string playerName;
    int numCharges;
    stream >> playerName >> numCharges;

    ServerPlayer* player = ServerGameContext::Get()->GetPlayerManager()->GetPlayer(playerName.c_str());
    if (player == nullptr)
    {
        cc << "Couldn't find player " << playerName;
        return;
    }

    player->GetForceCardServerPlayerExtent()->IncreaseChargeCount(numCharges);
    /*
    KYBER_LOG(LogLevel::Info, "ForceCardServerPlayerExtent: " << std::hex << player->GetForceCardServerPlayerExtent());
    KYBER_LOG(LogLevel::Info, "Slot1 Offset: " << &player->GetForceCardServerPlayerExtent()->m_slot1asset);
    KYBER_LOG(LogLevel::Info, "Slot1: " << player->GetForceCardServerPlayerExtent()->m_slot1asset->Name);
    KYBER_LOG(LogLevel::Info, "Slot2: " << player->GetForceCardServerPlayerExtent()->m_slot2asset->Name);
    KYBER_LOG(LogLevel::Info, "Slot3: " << player->GetForceCardServerPlayerExtent()->m_slot3asset->Name);*/
}

void PrintPlayerManagers(ConsoleContext& cc)
{
    KYBER_LOG(LogLevel::Info, "ServerPlayerManager: " << std::hex << ServerGameContext::Get()->GetPlayerManager());
    KYBER_LOG(LogLevel::Info, "ClientPlayerManager: " << std::hex << ClientGameContext::Get()->GetPlayerManager());
    KYBER_LOG(LogLevel::Info, "FirstServerPlayer: " << std::hex << ServerGameContext::Get()->GetPlayerManager()->m_players[0]);
    KYBER_LOG(LogLevel::Info, "FirstClientPlayer: " << std::hex << ClientGameContext::Get()->GetPlayerManager()->m_players[0]);
}

void TestCommand(ConsoleContext& cc)
{ 
    g_program->m_server->Start("XP2/Levels/Clouds/Clouds_01/Clouds_01", "WalkerAssault", 40, SocketSpawnInfo(false, "", ""));
}

void LoadLevelCommand(ConsoleContext& cc)
{
    auto stream = cc.stream();
    std::string levelPath;
    std::string gamemode;
    stream >> levelPath >> gamemode;

    if (g_program->m_server->m_running)
    {
        g_program->m_server->LoadLevel(levelPath.c_str(), gamemode.c_str(), "");
    }
    else
    {
        g_program->m_server->Start(levelPath.c_str(), gamemode.c_str(), 40, SocketSpawnInfo(false, "", ""));
    }
}

void SetTeamCommand(ConsoleContext& cc)
{
    if (!g_program->m_server->m_running)
    {
        cc << "This is a server command, start a server to use it!";
    }

    auto stream = cc.stream();
    std::string playerName;
    int team;
    stream >> playerName >> team;

    ServerPlayer* player = ServerGameContext::Get()->GetPlayerManager()->GetPlayer(playerName.c_str());
    if (player == nullptr)
    {
        cc << "Couldn't find player " << playerName;
        return;
    }

    player->SetTeam(team);
    cc << "Set " << playerName << " to team " << team;
}

Console::Console()
{
    KYBER_LOG(LogLevel::Debug, "[Console] Initializing Console Commands");

    RegisterConsoleCommand(&ExtentDebug, "ExtentDebug", "");
    RegisterConsoleCommand(&IncreaseChargeAmount, "IncreaseCharges", "<Player> <NumCharges>");
    RegisterConsoleCommand(&PrintPlayerManagers, "PrintPlayerManagers", "");
    RegisterConsoleCommand(&TestCommand, "Test", "");
    RegisterConsoleCommand(&LoadLevelCommand, "LoadLevel", "<LevelPath> <GameMode>");
    RegisterConsoleCommand(&SetTeamCommand, "SetTeam", "<Player> <Team>");
}
} // namespace Kyber

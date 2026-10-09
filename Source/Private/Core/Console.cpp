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
#define SINGLE_ARG(...) __VA_ARGS__

TL_DECLARE_FUNC(0x14334B160, void, ConsoleRegistry_registerConsoleMethods, const char* groupName, ConsoleMethod* methods, int count);
TL_DECLARE_FUNC(0x14334C370, SINGLE_ARG(eastl::fixed_vector<InstanceMethod, 128>&), ConsoleRegistry_getInstanceMethods);

void Console::RegisterConsoleCommand(StaticConsoleMethodPtr_t func, const char* name, const char* description)
{
    ConsoleMethod* method = new ConsoleMethod{ func, name, 0, description };
    ConsoleRegistry_registerConsoleMethods("Auric", method, 1);
}   

void ConsoleRegistry_registerInstanceMethod(
    fastdelegate::FastDelegate1<ConsoleContext&, void>& method, const char* name, const char* groupName)
{
    for (InstanceMethod& m : ConsoleRegistry_getInstanceMethods())
    {
        if (strcmp(m.name, name) == 0)
        {
            m.func = method;
            return;
        }
    }
    InstanceMethod& m = ConsoleRegistry_getInstanceMethods().push_back();
    m.description = "";
    m.func = method;
    m.groupName = groupName;
    m.name = name;
}

void Console::UnregisterCommand(const char* name)
{
    eastl::fixed_vector<InstanceMethod, 128>& methods = ConsoleRegistry_getInstanceMethods();
    for (eastl::fixed_vector<InstanceMethod, 128>::iterator it = methods.begin(); it != methods.end();)
    {
        if (strcmp(name, it->name) != 0)
        {
            ++it;
            continue;
        }

        it = methods.erase(it);
        return;
    }
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
    g_program->m_server->Start("Levels/Desert/Desert_04/Desert_04", "DropZone", 40, SocketSpawnInfo(false, "", ""));
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

void Console::EnqueueCommand(const char* cmd)
{
    auto delegate = fastdelegate::FastDelegate<void(const char*)>([](const char* result) {
        if (strlen(result) == 0)
        {
            return;
        }

        KYBER_LOG(LogLevel::Info, "[Console] " << result);
    });

    KYBER_LOG(LogLevel::Info, "[Console] > " << cmd);
    Console_enqueueCommand(cmd, delegate);
}
} // namespace Kyber

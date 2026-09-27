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

    RegisterConsoleCommand(&LoadLevelCommand, "LoadLevel", "<LevelPath> <GameMode>");
    RegisterConsoleCommand(&SetTeamCommand, "SetTeam", "<Player> <Team>");
}
} // namespace Kyber

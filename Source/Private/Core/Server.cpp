// Copyright BattleDash. All Rights Reserved.

#define _WINSOCKAPI_
#include <Core/Server.h>

#include <Core/Program.h>
#include <Hook/HookManager.h>
#include <Base/Log.h>
#include <Utilities/ErrorUtils.h>
#include <Utilities/MemoryUtils.h>
#include <Utilities/PlatformUtils.h>
#include <SDK/TypeInfo.h>
#include <SDK/SDK.h>

#include <ws2tcpip.h>
#include <iomanip>
#include <iostream>
#include <minhook/MinHook.h>
#include <sstream>
#include <stdio.h>
#include <thread>

#define OFFSET_SERVER_CONSTRUCTOR HOOK_OFFSET(0x143C3D3C0)
#define OFFSET_SERVER_START HOOK_OFFSET(0x143C466F0)
#define OFFSET_CREATESERVERBACKEND HOOK_OFFSET(0x1441B0470)
#define OFFSET_APPLY_SETTINGS HOOK_OFFSET(0x14334AE20)

namespace Kyber
{
Server::Server()
    : m_socketSpawnInfo(SocketSpawnInfo(false, "", ""))
    , m_socketManager(new SocketManager(ProtocolDirection::Clientbound, SocketSpawnInfo(false, "", "")))
    , m_running(false)
    , m_hooksRemoved(false)
{}

Server::~Server()
{
    KYBER_LOG(LogLevel::Debug, "Destroying Server");
}

void Server::Start(const char* level, const char* mode, int maxPlayers, SocketSpawnInfo info)
{
    EnableGameHooks();

    ClientLobbyInformation* clientLobby = ClientLobbyInformation::Get();

    ClientSettings* clientSettings = Settings<ClientSettings>("Client");
    GameSettings* gameSettings = Settings<GameSettings>("Game");
    PersistenceSettings* persistenceSettings = SettingsLookup<PersistenceSettings>(OFFSET_TYPEINFO_PERSISTENCESETTINGS);

    clientSettings->ServerIp = "";

    clientLobby->info->playerCountTeam1 = maxPlayers;
    clientLobby->info->playerCountTeam2 = maxPlayers;
    clientLobby->info->gamemode = mode;
    clientLobby->info->level = level;

    gameSettings->Level = const_cast<char*>(level);
    gameSettings->StartPoint = "Offline_WalkerAssault";
    std::string gameMode = "GameMode=" + std::string(mode);
    gameSettings->DefaultLayerInclusion = _strdup(gameMode.c_str());

    Guid newGuid = Guid::FromString("b0e13818-7192-e311-873e-d06628a59c0a");
    persistenceSettings->PersistenceConfigurationGuid = newGuid;

    info.serverMode = "SERVER";
    m_socketSpawnInfo = info;

    g_program->m_client->ChangeState(ClientState_Startup);

    m_running = true;
    m_hooksRemoved = false;
}

void InitLevelSetup(LevelSetup* levelSetup, const char* level, const char* mode, const char* startPoint)
{
    KYBER_LOG(LogLevel::Debug,
        "[Server] Loading Level '" << level << "' with mode '" << (mode ? mode : "none") << "' and startpoint '" << startPoint << "'");

    LevelSetup_ctor(levelSetup);
    levelSetup->Name = strdup(level);
    levelSetup->StartPoint = strdup(startPoint);

    if (mode)
    {
        LevelSetup_setInclusionOption(levelSetup, "GameMode", strdup(mode));
    }
}

void Server::LoadLevel(const char* level, const char* mode, const char* startpoint)
{
    LevelSetup levelSetup;
    InitLevelSetup(&levelSetup, level, mode, startpoint);
    ServerLoadLevelMessage_post(&levelSetup, true, true);
}

__int64 ServerCtorHk(__int64 inst, ServerSpawnInfo& info, SocketManager* socketManager)
{
    static const auto trampoline = HookManager::Call(ServerCtorHk);

    info.isLocalHost = false;

    return trampoline(inst, info, socketManager);
}

__int64 ServerStartHk(__int64 inst, ServerSpawnInfo* info, ServerSpawnOverrides* spawnOverrides)
{
    static const auto trampoline = HookManager::Call(ServerStartHk);

    Server* server = g_program->m_server;
    spawnOverrides->socketManager = server->m_socketManager;

    return trampoline(inst, info, spawnOverrides);
}

__int64 SettingsManagerApplyHk(__int64 inst, __int64* a2, char* script, BYTE* a4)
{
    static const auto trampoline = HookManager::Call(SettingsManagerApplyHk);
    Server* server = g_program->m_server;
    if (server->m_running && !server->m_hooksRemoved)
    {
        server->m_hooksRemoved = true;
        server->DisableGameHooks();
    }
    KYBER_LOG(LogLevel::DebugPlusPlus, "SettingsManagerApplyHk(" << script << ")");
    if (strstr(script, "InstallationLevel"))
    {
        return 0;
    }
    return trampoline(inst, a2, script, a4);
}

__int64 CreateServerBackendHk(BackendType backendType, __int64 serverArena, __int64 configuration)
{
    static const auto trampoline = HookManager::Call(CreateServerBackendHk);

    BackendType newBackendType = Backend_Peer;

    KYBER_LOG(LogLevel::Debug, "Initialzed Server with Backend " << BackendTypeToString(newBackendType));
    return trampoline(newBackendType, serverArena, configuration);
}

HookTemplate server_hook_offsets[] = {
    { OFFSET_SERVER_CONSTRUCTOR, ServerCtorHk },
    { OFFSET_SERVER_START, ServerStartHk },
    { OFFSET_APPLY_SETTINGS, SettingsManagerApplyHk },
    { OFFSET_CREATESERVERBACKEND, CreateServerBackendHk },
};

void Server::InitializeGameHooks()
{
    for (HookTemplate& hook : server_hook_offsets)
    {
        HookManager::CreateHook(hook.offset, hook.hook);
    }
    Hook::ApplyQueuedActions();
}

void Server::EnableGameHooks()
{
    HookManager::EnableHook(OFFSET_SERVER_CONSTRUCTOR);
    HookManager::EnableHook(OFFSET_SERVER_START);
    Hook::ApplyQueuedActions();
}

void Server::DisableGameHooks()
{
    HookManager::DisableHook(OFFSET_SERVER_CONSTRUCTOR);
    HookManager::DisableHook(OFFSET_SERVER_START);
    Hook::ApplyQueuedActions();
}

void Server::InitializeGamePatches()
{
    BYTE ptch[] = { 0xB9, 0x01, 0x00, 0x00, 0x00 };
    MemoryUtils::Patch((void*)0x143A9C8BE, (void*)ptch, sizeof(ptch));

    BYTE ptch2[] = { 0x90, 0x90 };
    MemoryUtils::Patch((void*)(0x143A9C8BE + 0x5), (void*)ptch2, sizeof(ptch2));
}

void Server::Initialize()
{
    InitializeGameHooks();
    DisableGameHooks();
    InitializeGamePatches();
}

void Server::Stop()
{
    m_running = false;
    UDPSocket* socket = m_socketManager->m_sockets.back();
    m_socketManager->Close(socket);
    socket->Close();
}
} // namespace Kyber

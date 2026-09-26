// Copyright Nuuby. All Rights Reserved.

#include <Core/Client.h>

#include <Core/Program.h>
#include <Hook/HookManager.h>
#include <Base/Log.h>
#include <SDK/TypeInfo.h>
#include <SDK/SDK.h>

#define OFFSET_CLIENTCHANGESTATE HOOK_OFFSET(0x143A95BB0)
#define OFFSET_CLIENT_INIT_NETWORK HOOK_OFFSET(0x143A97630)
#define OFFSET_CLIENT_CONNECTTOADDRESS HOOK_OFFSET(0x143B136F0)

#define OFFSET_GET_CLIENT_INSTANCE 0x14659DE50

namespace Kyber
{
Client::Client()
    : m_clientState(ClientState_None)
{
    KYBER_LOG(LogLevel::Info, "[Client] Initializing");
}

__int64 ClientChangeStateHk(__int64 inst, ClientState currentClientState, ClientState lastClientState)
{
    static const auto trampoline = HookManager::Call(ClientChangeStateHk);

    g_program->m_client->m_clientState = currentClientState;

    KYBER_LOG(LogLevel::Debug, "ClientState changed to " << ClientStateToString(currentClientState));

    Server* server = g_program->m_server;
    if (currentClientState == ClientState_Startup)
    {
        if (server->m_running)
        {
            server->Stop();

            GameSettings* gameSettings = Settings<GameSettings>("Game");
            gameSettings->Level = "Levels/FrontEnd/FrontEnd";
            gameSettings->DefaultLayerInclusion = "";
        }
        else
        {
            if (!g_program->m_joining)
            {
                Settings<ClientSettings>("Client")->ServerIp = "";
            }
            else
            {
                g_program->m_joining = false;
            }
        }
    }

    return trampoline(inst, currentClientState, lastClientState);
}

bool ClientInitNetworkHk(__int64 inst, bool singleplayer, bool localhost, bool coop, bool hosted)
{
    static const auto trampoline = HookManager::Call(ClientInitNetworkHk);
    if (g_program->m_server->m_running || strlen(Settings<ClientSettings>("Client")->ServerIp) > 0)
    {
        *reinterpret_cast<__int64*>(inst + 0xB8) =
            reinterpret_cast<__int64>(new SocketManagerCreator(g_program->m_server->m_socketSpawnInfo));
    }
    return trampoline(inst, singleplayer, localhost, coop, hosted);
}

void ClientConnectToAddressHk(__int64 inst, const char* ipAddress, const char* serverPassword)
{
    static const auto trampoline = HookManager::Call(ClientConnectToAddressHk);
    SocketSpawnInfo info = g_program->m_server->m_socketSpawnInfo;
    if (info.isProxied)
    {
        trampoline(inst, (std::string(info.proxyAddress) + ":25200").c_str(), serverPassword);
    }
    else
    {
        trampoline(inst, ipAddress, serverPassword);
    }
}

__int64 Client::GetGameClient()
{ 
    return *reinterpret_cast<__int64*>(*reinterpret_cast<__int64*>(((__int64 (*)(void))OFFSET_GET_CLIENT_INSTANCE)() + 0x20) + 0x28);
}

void Client::ChangeState(ClientState newState)
{
    ClientChangeStateHk(GetGameClient(), newState, m_clientState);
}

void Client::Initialize()
{ 
	InitializeHooks(); 
}

// clang-format off
HookTemplate client_hook_offsets[] = {
    { OFFSET_CLIENTCHANGESTATE, ClientChangeStateHk },
    { OFFSET_CLIENT_CONNECTTOADDRESS, ClientConnectToAddressHk },
    { OFFSET_CLIENT_INIT_NETWORK, ClientInitNetworkHk },
};

void Client::InitializeHooks()
{
	for (HookTemplate& hook : client_hook_offsets)
    {
        HookManager::CreateHook(hook.offset, hook.hook);
    }
    Hook::ApplyQueuedActions();
}

Client::~Client()
{ 
	KYBER_LOG(LogLevel::Debug, "[Client] Destroying"); 
}
}
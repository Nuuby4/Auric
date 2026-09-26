// Copyright BattleDash. All Rights Reserved.

#include <Core/Program.h>

#include <Base/Version.h>
#include <Base/Log.h>
#include <Render/Renderer.h>
#include <Utilities/ErrorUtils.h>
#include <Utilities/PlatformUtils.h>
#include <Utilities/MemoryUtils.h>
#include <Hook/HookManager.h>
#include <SDK/Funcs.h>
#include <SDK/TypeInfo.h>
#include <SDK/SDK.h>
#include <Network/SocketManager.h>
#include <API/KyberAPIService.h>

#include <MinHook/MinHook.h>

#include <Windows.h>
#include <cstdio>
#include <chrono>
#include <thread>

#define OFFSET_GET_SETTINGS_OBJECT HOOK_OFFSET(0x143363D80)
#define OFFSET_MESSAGEMANAGERDISPATCHMESSAGE HOOK_OFFSET(0x1432FF410)
#define OFFSET_SETTINGS_CTR HOOK_OFFSET(0x143363EE0)

Kyber::Program* g_program;

namespace Kyber
{
Program::Program(HMODULE module)
    : m_module(module)
    , m_server(nullptr)
    , m_clientState(ClientState_None)
    , m_joining(false)
{
    if (g_program || MH_Initialize() != MH_OK)
    {
        ErrorUtils::ThrowException("Initialization failed. Please restart Battlefront and try again!");
    }

    // Open a console
    AllocConsole();
    FILE* pFile;
    freopen_s(&pFile, "CONOUT$", "w", stdout);

    // ANSI Colors
    HANDLE stdoutHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD dwMode;
    GetConsoleMode(stdoutHandle, &dwMode);
    dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    SetConsoleMode(stdoutHandle, dwMode);

    SetConsoleTitleA(("Kyber v" + KYBER_VERSION).c_str());

    new std::thread(&Program::InitializationThread, this);
}

Program::~Program()
{
    KYBER_LOG(LogLevel::Info, "Destroying Kyber");
    HookManager::RemoveHooks();
    delete m_server;
}

DWORD WINAPI Program::InitializationThread()
{
    KYBER_LOG(LogLevel::Info, "Initializing...");
    KYBER_LOG(LogLevel::Info, " __ __              __           ");
    KYBER_LOG(LogLevel::Info, "|  \\  | _  _  _  _ |  |, __  _   ");
    KYBER_LOG(LogLevel::Info, "|  |  || || || || ||  o \\\\ \\| |   ");
    KYBER_LOG(LogLevel::Info, "|__\\__|\\____|\\____||____| \\  //   ");
    KYBER_LOG(LogLevel::Info, "                           ///    ");

    Initialize();
    m_client = new Client();
    //m_server = new Server();

    KYBER_LOG(LogLevel::Info, "Initialized Kyber v" << KYBER_VERSION);
    KYBER_LOG(LogLevel::Warning, "Press [INSERT] on your Keyboard to use Kyber!");

    while (1)
    {
        if (GetAsyncKeyState(VK_END) & 1)
        {
            KYBER_LOG(LogLevel::Info, "Ejecting Kyber");
            FreeLibrary(m_module);
            delete this;
            break;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    return 0;
}

__int64 GetSettingsObjectHk(__int64 inst, const char* identifier)
{
    static const auto trampoline = HookManager::Call(GetSettingsObjectHk);
    return trampoline(inst, identifier);
}

void MessageManagerDispatchMessageHk(void* inst, Message* message)
{
    static const auto trampoline = HookManager::Call(MessageManagerDispatchMessageHk);

    if (!message)
    {
        return;
    }

    TypeInfo* type = message->getType();
    if (!type || !type->typeInfoData)
    {
        trampoline(inst, message);
        return;
    }

    std::string typeName = type->getName();

    if (typeName == "NetworkCreatePlayerMessage")
    {
        NetworkCreateJoiningPlayerMessage* msg = (NetworkCreateJoiningPlayerMessage*)message;
        msg->isSpectator = false;
    }

    if (typeName == "ServerLevelCompletedMessage")
    {
        KYBER_LOG(LogLevel::Info, "Game ended, moving to next level");

        auto& mapList = g_program->m_server->m_mapList;
        GameSettings* gameSettings = Settings<GameSettings>("Game");

        // @TODO Research Game's Actual Map Rotation Implementation
        NextLevelInfo nextLevel;

        if (!mapList.empty())
        {
            MapRotation* map = mapList.front();

            nextLevel.level = map->Level;
            nextLevel.gameMode = map->GameMode;

            gameSettings->Level = strdup(map->Level);
            std::string gameModeStr = "GameMode=" + std::string(map->GameMode);
            gameSettings->DefaultLayerInclusion = strdup(gameModeStr.c_str());

            delete map;
            mapList.erase(mapList.begin());
        }
        else
        {
            nextLevel.level = gameSettings->Level;
            nextLevel.gameMode = strchr(gameSettings->DefaultLayerInclusion, '=') + 1;
        }
        ServerMapSequencer_LoadNextLevel(nextLevel);
    }
    trampoline(inst, message);
}

HookTemplate program_hook_offsets[] = { 
    { OFFSET_GET_SETTINGS_OBJECT, GetSettingsObjectHk }, 
    { OFFSET_MESSAGEMANAGERDISPATCHMESSAGE, MessageManagerDispatchMessageHk } 
};

void Program::InitializeGamePatches()
{
    BYTE alwaysTruePatch[6] = { 0x90, 0x90, 0x90, 0x90, 0x90, 0x90 };
    MemoryUtils::Patch((void*)0x143338021, alwaysTruePatch, sizeof(alwaysTruePatch)); // Enable All Console Commands
    MemoryUtils::Patch((void*)0x143362261, alwaysTruePatch, sizeof(alwaysTruePatch)); // Expose Hidden Settings to Console

    BYTE alwaysFalsePatch[2] = { 0xEB, 0x0F };
    MemoryUtils::Patch((void*)0x1432F28C6, alwaysFalsePatch, sizeof(alwaysFalsePatch)); // Allow Multiple Game Instances 
}

void Program::InitializeGameHooks()
{
    for (HookTemplate& hook : program_hook_offsets)
    {
        HookManager::CreateHook(hook.offset, hook.hook);
    }
    Hook::ApplyQueuedActions();
}

void Program::Initialize()
{
    InitializeGamePatches();
    InitializeGameHooks();

    //m_server->Initialize();
    m_client->Initialize();
}
} // namespace Kyber
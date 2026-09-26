// Copyright BattleDash. All Rights Reserved.

#pragma once

#include <Network/SocketManager.h>
#include <SDK/SDK.h>
#include <SDK/TypeInfo.h>

#include <Windows.h>
#include <string>

namespace Kyber
{
class Server
{
public:
    Server();
    ~Server();

    void Initialize();
    void InitializeGameHooks();
    void InitializeGamePatches();
    void EnableGameHooks();
    void DisableGameHooks();

    void LoadLevel(const char* level, const char* mode, const char* startpoint);
    void Start(const char* level, const char* mode, int maxPlayers, SocketSpawnInfo info);
    void Stop();

    SocketManager* m_socketManager;
    SocketSpawnInfo m_socketSpawnInfo;

    std::vector<MapRotation*> m_mapList;

    bool m_running;
    bool m_hooksRemoved;
};
} // namespace Kyber

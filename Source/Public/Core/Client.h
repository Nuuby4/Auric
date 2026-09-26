// Copyright Nuuby. Licensed under GPLv3.

#pragma once

#include <SDK/TypeInfo.h>

namespace Kyber
{
class Client
{
public:
    Client();
    ~Client();

    void Initialize();
    void InitializeHooks();

    __int64 GetGameClient();
    void ChangeState(ClientState newState);

    ClientState m_clientState;

    //@TODO Setup Client SocketManager here
};
} // namespace Kyber

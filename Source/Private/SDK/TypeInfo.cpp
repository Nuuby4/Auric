// Copyright Nuuby. All Rights Reserved.

#include <SDK/TypeInfo.h>

#include <SDK/Funcs.h>

namespace Kyber
{
std::string ClientStateToString(ClientState state)
{
    switch (state)
    {
    case ClientState_WaitingForStaticBundleLoad:
        return "WaitingForStaticBundleLoad";
    case ClientState_LoadProfileOptions:
        return "LoadProfileOptions";
    case ClientState_LostConnection:
        return "LostConnection";
    case ClientState_WaitingForUnload:
        return "WaitingForUnload";
    case ClientState_Startup:
        return "Startup";
    case ClientState_StartServer:
        return "StartServer";
    case ClientState_WaitingForLevel:
        return "WaitingForLevel";
    case ClientState_StartLoadingLevel:
        return "StartLoadingLevel";
    case ClientState_WaitingForLevelLoaded:
        return "WaitingForLevelLoaded";
    case ClientState_WaitingForLevelLink:
        return "WaitingForLevelLink";
    case ClientState_LevelLinked:
        return "LevelLinked";
    case ClientState_WaitingForGhosts:
        return "WaitingForGhosts";
    case ClientState_Ingame:
        return "Ingame";
    case ClientState_LeaveIngame:
        return "LeaveIngame";
    case ClientState_ConnectToServer:
        return "ConnectToServer";
    case ClientState_ShuttingDown:
        return "ShuttingDown";
    case ClientState_Shutdown:
        return "Shutdown";
    case ClientState_None:
        return "None";
    default:
        return "Unknown";
    }
}

std::string BackendTypeToString(BackendType type)
{
    switch (type)
    {
    case Backend_Lan:
        return "Lan";
    case Backend_Blaze:
        return "Blaze";
    case Backend_Peer:
        return "Peer";
    case Backend_Local:
        return "Local";
    case Backend_Playground:
        return "Playground";
    case Backend_Count_:
        return "Count_";
    default:
        return "Unknown";
    }
}
} // namespace Kyber
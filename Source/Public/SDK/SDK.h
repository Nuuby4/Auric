// Copyright BattleDash. All Rights Reserved.

#pragma once

#ifndef WIN32_LEAN_AND_MEAN
    #define WIN32_LEAN_AND_MEAN
#endif

#ifndef NOMINMAX
    #define NOMINMAX
#endif

// Prevent winsock.h from being included (forces winsock2.h only)
#ifndef _WINSOCKAPI_
    #define _WINSOCKAPI_

#endif
#include <stddef.h>
#include <string>
#include <vector>
#include <rpc.h>
#include <iostream>

namespace Kyber
{
struct Guid
{
    union
    {
        uint8_t data[16];

        struct
        {
            uint32_t data1;
            uint16_t data2;
            uint16_t data3;
            uint8_t data4[8];
        };
    };

    std::string ToString() const
    {
        char buffer[37];
        sprintf_s(buffer, "%08x-%04x-%04x-%02x%02x-%02x%02x%02x%02x%02x%02x", data1, data2, data3, data4[0], data4[1], data4[2], data4[3],
            data4[4], data4[5], data4[6], data4[7]);
        return std::string(buffer);
    }

    __forceinline bool Equals(const Guid& guid) const
    { return memcmp(data, guid.data, 16) == 0; }

    __forceinline bool IsZero() const
    {
        for (int i = 0; i < 16; ++i)
        {
            if (data[i] == 0)
            {
                continue;
            }

            return false;
        }

        return true;
    }

    __forceinline bool operator==(const Guid& guid) const
    { 
        return Equals(guid);
    }

    __forceinline bool operator!=(const Guid& guid) const
    { 
        return !Equals(guid);
    }

    __forceinline bool operator<(const Guid& guid) const
    { 
        return memcmp(this, &guid, sizeof(Guid)) < 0;
    }

    static Guid FromString(const char* str)
    {
        Guid guid{};
        sscanf_s(str, "%08x-%04hx-%04hx-%02hhx%02hhx-%02hhx%02hhx%02hhx%02hhx%02hhx%02hhx", &guid.data1, &guid.data2, &guid.data3,
            &guid.data4[0], &guid.data4[1], &guid.data4[2], &guid.data4[3], &guid.data4[4], &guid.data4[5], &guid.data4[6], &guid.data4[7]);
        return guid;
    }

    static Guid FromString(const std::string& str)
    { 
        return FromString(str.c_str());
    }

    //static Guid FromString(const eastl::string& str)
    //{ 
    //    return FromString(str.c_str());
    //}

    static Guid FromFrostyLE(uint8_t* b)
    {
        Guid guid{};
        memcpy(guid.data, b, 16);
        return guid;
    }

    static Guid FromFrostyBE(uint8_t* b)
    {
        Guid guid{};
        guid.data[0] = b[3];
        guid.data[1] = b[2];
        guid.data[2] = b[1];
        guid.data[3] = b[0];
        guid.data[4] = b[5];
        guid.data[5] = b[4];
        guid.data[6] = b[7];
        guid.data[7] = b[6];
        guid.data[8] = b[8];
        guid.data[9] = b[9];
        guid.data[10] = b[10];
        guid.data[11] = b[11];
        guid.data[12] = b[12];
        guid.data[13] = b[13];
        guid.data[14] = b[14];
        guid.data[15] = b[15];
        return guid;
    }

    void ToFrostyLE(uint8_t* b) const
    { 
        memcpy(b, data, 16);
    }

    void ToFrostyBE(uint8_t* b) const
    {
        b[0] = data[3];
        b[1] = data[2];
        b[2] = data[1];
        b[3] = data[0];

        b[4] = data[5];
        b[5] = data[4];

        b[6] = data[7];
        b[7] = data[6];

        for (int i = 0; i < 8; i++)
        {
            b[8 + i] = data[8 + i];
        }
    }
    static Guid Generate()
    {
        UUID uuid;
        RPC_CSTR uuid_str;
        std::string uuid_out;

        RPC_STATUS status = UuidCreate(&uuid);
        if (status != RPC_S_OK)
        {
            std::cout << "couldn't create uuid\nError code: " << status << std::endl;
        }

        status = UuidToStringA(&uuid, &uuid_str);
        if (status != RPC_S_OK)
        {
            std::cout << "couldn't convert uuid to string\nError code: " << status << std::endl;
        }

        uuid_out = reinterpret_cast<char*>(uuid_str);
        RpcStringFreeA(&uuid_str);
        return FromString(uuid_out);
    }
};

struct PlayerInformation2
{
    char pad_0000[0x10];
    uint64_t userId;
};

struct PlayerInformation
{
    uint64_t personaId;
    char name[0x8];
    PlayerInformation2* info;
    char pad_0010[0x10];
    uint64_t userId;
};

struct UIPlayerInformation
{
    char pad_0000[0x48];
    PlayerInformation m_players[64];
};

struct GameModeInformation
{
    const char* level;
    const char* gamemode;
    uint32_t playerCountTeam1;
    uint32_t playerCountTeam2;
};

struct ClientLobbyInformation
{
    char pad_0000[0x30];

    UIPlayerInformation* team1;
    UIPlayerInformation* team2;
    char pad_0040[0x28];
    GameModeInformation* info;

    static ClientLobbyInformation* Get()
    { 
        return *(ClientLobbyInformation**)0x142CDE3A0;
    }
};

class NextLevelInfo
{
public:
    char pad_0000[16];
    const char* level;
    const char* gameMode;
};

struct MapRotation
{
public:
    const char* Level;
    const char* GameMode;
    const char* Name;
};

class NetworkCreateJoiningPlayerMessage
{
public:
    char pad_0000[72];
    char* playerName;
    bool isSpectator;
};

struct TypeObject
{
    virtual class TypeInfo* getType() const = 0;

protected:
    virtual ~TypeObject() = default;
};

class Message : public TypeObject
{
public:
    const int category;
    const int type;
};

enum TypeCodeEnum : uint16_t
{
    kTypeCode_Void = 0,
    kTypeCode_DbObject = 1,
    kTypeCode_ValueType = 2,
    kTypeCode_Class = 3,
    kTypeCode_Array = 4,
    kTypeCode_CString = 7,
    kTypeCode_Enum = 8,
    kTypeCode_Boolean = 10,
    kTypeCode_Int8 = 11,
    kTypeCode_Uint8 = 12,
    kTypeCode_Int16 = 13,
    kTypeCode_Uint16 = 14,
    kTypeCode_Int32 = 15,
    kTypeCode_Uint32 = 16,
    kTypeCode_Int64 = 17,
    kTypeCode_Uint64 = 18,
    kTypeCode_Float32 = 19,
    kTypeCode_Float64 = 20,
};

class ModuleInfo
{
public:
    char* moduleName;             // 0x0000
    class ModuleInfo* nextModule; // 0x0008
};

class MemberInfoData
{
public:
    char* name;     // 0x0000
    uint16_t flags; // 0x0008

    static const uint32_t kFlagIsBlittable = 1 << 15;

    bool IsBlittable() const
    { 
        return (flags & kFlagIsBlittable) == kFlagIsBlittable; 
    }
}; // Size: 0x000A

class TypeInfoData : public MemberInfoData
{
public:
    uint16_t totalSize;       // 0x000A
    uint32_t guid;            // 0x000C
    class ModuleInfo* module; // 0x0010
    // class TypeInfo* arrayTypeInfo; // 0x0018 // Not in swbf 2015?
    uint16_t alignment;
    uint16_t fieldCount;
    uint32_t signature;
};

class TypeInfo
{
public:
    class TypeInfoData* typeInfoData; // 0x0000
    TypeInfo* next;

    // @TODO - Verify ClassInfo, fields below are all ClassInfoType specific
    char pad[0x28];
    // const TypeInfo* m_super;
    const TypeObject* m_defaultInstance;
    uint16_t m_classId;
    uint16_t m_lastClassId;

    TypeCodeEnum getBasicType() const;
    const char* getName() const;

    bool isKindOf(const TypeInfo* other) const;
}; // Size: 0x0008

class LevelSetupOptions
{
public:
    char* Criterion;
    char* Value;
};

class LevelSetup
{
public:
    char* Name;
    LevelSetupOptions* InclusionOptions;
    uint32_t DifficultyIndex;
    char pad_001A[0x4];
    char* StartPoint;
    bool IsSaveGame;
    bool HasPersistentSave;
    bool ForceReloadResources;
    char pad_0023[0x5];
};

class SocketManager;

struct ServerSpawnOverrides
{
    LevelSetup* levelSetup;
    SocketManager* socketManager;
    __int64 connectionCreator;
    __int64 peerCreator;
};

class ServerPlayerManager
{
public:
    char pad_0000[8];                  // 0x0000
    class PlayerData* m_playerData;    // 0x0008
    uint32_t m_maxPlayerCount;         // 0x0010
    uint32_t m_playerCountBitCount;    // 0x0014
    uint32_t m_playerIdBitCount;       // 0x0018
    char pad_001C[212];                // 0x001C
    class ServerPlayer* m_players[64]; // 0x00F0
    char pad_02F0[1276];               // 0x02F0
};                                     // Size: 0x07EC

struct ServerCharacter
{
    void* vtable;
    char pad_0000[0x7B8];
};

class ServerPlayer
{
public:
    void* vtable;                           // 0x0000
    class PlayerData* m_data;               // 0x0008
    class MemoryArena* m_memoryArena;       // 0x0010
    char* m_name;                           // 0x0018
    char pad_0020[0x14];                    // 0x0020
    uint64_t m_id;                          // 0x0034
    char pad_003C[0x2B68];                  // 0x2B9C
    bool m_isAIPlayer;                      // 0x2BA8
    char pad_0003[0x3];
    uint32_t m_teamId;                      // 0x2BAC
    char pad_0004[0xD8];
    ServerCharacter* m_serverCharacter;     // 0x2C88

    //bool Teleport(const LinearTransform& transform);
};

class MemoryArena
{
public:
    char pad_0000[136]; // 0x0000
};                      // Size: 0x0088

struct ServerSpawnInfo
{
    ServerSpawnInfo(LevelSetup& setup)
        : levelSetup(setup)
    {}

    void* fileSystem = nullptr;
    void* damageArbitrator = nullptr;
    ServerPlayerManager* playerManager = nullptr;
    LevelSetup& levelSetup;
    unsigned int tickFrequency = 0;
    bool isSinglePlayer = false;
    bool isLocalHost = false;
    bool isDedicated = false;
    bool isEncrypted = false;
    bool isCoop = false;
    bool isMenu = false;
    bool keepResources = false;
    void* saveData;
    void* serverCallbacks = nullptr;
    void* runtimeModules = nullptr;
};

struct SocketSpawnInfo
{
    SocketSpawnInfo(bool isProxied, const char* proxyAddress, const char* serverName)
        : isProxied(isProxied)
        , proxyAddress(proxyAddress)
        , serverName(serverName)
    {}

    bool isProxied;
    const char* proxyAddress;
    const char* serverName;
    const char* serverMode;
    const char* serverLevel;
};
} // namespace Kyber

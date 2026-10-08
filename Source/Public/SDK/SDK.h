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

#include <EASTL/fixed_vector.h>
#include <Utilities/StringUtils.h>

#include <stddef.h>
#include <string>
#include <vector>
#include <rpc.h>
#include <iostream>

namespace Kyber
{
#define FB_CLIENT_ARENA (reinterpret_cast<MemoryArena*>(0x142A3B390))
#define FB_SERVER_ARENA (reinterpret_cast<MemoryArena*>(0x142A3CEB0))

#define STRIP_PARENS(...) __VA_ARGS__
#define AU_DECLARE_GAMEMEMBERFUNC(ptr, returnType, name, args, ...)                                                                         \
    inline returnType name(__VA_ARGS__)                                                                                                     \
    {                                                                                                                                       \
        return reinterpret_cast<returnType(__fastcall*)(void*, __VA_ARGS__)>(ptr)(this, STRIP_PARENS args);                                 \
    }   
    
#define AU_DECLARE_GAMEMEMBERFUNC_NOARGS(ptr, returnType, name)                                                                            \
    inline returnType name()                                                                                                                    \
{                                                                                                                                           \
    return reinterpret_cast<returnType(__fastcall*)(void*)>(ptr)(this);                                                                     \
}

enum Realm
{
    Realm_Client,          // 0x0000
    Realm_Server,          // 0x0001
    Realm_ClientAndServer, // 0x0002
    Realm_None,            // 0x0003
    Realm_Pipeline,        // 0x0004
    Realm_Count
};

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

class SocketManagerCreator;

class GameClient
{
public:
    void* vtable;
    class GameSettings* m_gameSettings;       // 0x8
    char pad_0010[0x28];                      // 0x10
    class ClientGameContext* m_clientGameContext;   // 0x38
    class ClientSettings* m_clientSettings;   // 0x40
    char pad_0048[0x70];                      // 0x48
    SocketManagerCreator* m_socketManagerCreator;           // 0xB8

    static GameClient* Get()
    {
        return *reinterpret_cast<GameClient**>(
            *reinterpret_cast<uintptr_t*>(reinterpret_cast<uintptr_t (*)()>(0x143B9F260)() + 0x38) + 0x20);
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
    uint32_t category;
    uint32_t type;
    enum LocalPlayerId localPlayerId;
    char pad_0018[24];
    bool ownedByMessageManager;
    char pad_0028[3];
private:
    AU_DECLARE_GAMEMEMBERFUNC_NOARGS(0x1432FCFD0, void*, Ctor)
}; // Size: 0x30

typedef int32_t EventId;
class EntityEvent : TypeObject
{
public:
    enum Sender
    {
        Sender_External,
        Sender_Parent,
        Sender_Child
    };

    EntityEvent(const char* event);

    EntityEvent(EventId eventId)
        : eventId(eventId)
        , sender(Sender_Child)
    {}

    TypeInfo* getType() const override
    { return (TypeInfo*)0x142F95420; }

    bool Is(const char* event) const
    { return eventId == StringUtils::HashQuick(event); }

    mutable EventId eventId;
    mutable Sender sender;
};

enum ResourceCompartment : uint16_t
{
    ResourceCompartment_Static = 0,
    ResourceCompartment_Frontend = 1,
    ResourceCompartment_LoadingScreen = 2,
    ResourceCompartment_GameStatic = 3,
    ResourceCompartment_Game = 4,
    ResourceCompartment_Dynamic_Begin_,
    ResourceCompartment_Synchronized_Begin_ = ResourceCompartment_Dynamic_Begin_,
    ResourceCompartment_Synchronized_End_ = ResourceCompartment_Synchronized_Begin_ + 2000,
    ResourceCompartment_NonSynchronized_Begin_,
    ResourceCompartment_NonSynchronized_End_ = ResourceCompartment_NonSynchronized_Begin_ + 1000,
    ResourceCompartment_Count_ = ResourceCompartment_NonSynchronized_End_,
    ResourceCompartment_Forbidden_ = ResourceCompartment_Count_,
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

class DataContainer : public TypeObject
{
public:
    struct GuidEntry
    {
        Guid guid;
    };

    enum
    {
        Exported = 0x1000,
        HasGuid = 0x0100,
    };

    inline uint32_t IsExported() const
    { 
        return m_dcFlags & Exported;
    }

    inline const Guid* GetInstanceGuid() const
    {
        if ((m_dcFlags & HasGuid) == 0)
        {
            return nullptr;
        }

        const GuidEntry* guidEntry = reinterpret_cast<const GuidEntry*>(this) + -1;
        return &guidEntry->guid;
    }

    inline void SetInstanceGuid(Guid& guid)
    {
        if ((m_dcFlags & HasGuid) == 0)
        {
            m_dcFlags |= HasGuid;
        }

        GuidEntry* guidEntry = reinterpret_cast<GuidEntry*>(this) + -1;
        guidEntry->guid = guid;
    }

    inline int addRef() const
    { 
        return InterlockedIncrement((volatile unsigned __int32*)&m_refCount);
    }

    void release();

    // Override this when necessary, this is just the base DataContainer TypeInfo
    //TypeInfo* getType() const override
    //{ 
    //    return m_dcType != nullptr ? m_dcType : (TypeInfo*)0x142F59D40;
    //}

    //TypeInfo* m_dcType = nullptr;
    uint32_t m_refCount = 1;
    uint16_t m_dcFlags = 0;
    ResourceCompartment m_compartment = ResourceCompartment_Static;
};

class GameDataContainer : public DataContainer
{};

class DataBusPeer : public GameDataContainer
{
public:
    uint32_t Flags;
    char _0x001C[4];
};

class GameObjectData : public DataBusPeer
{};

class EntityData : public GameObjectData
{
public:
};

class EntityBase : public TypeObject
{
public:
    void* m_linkPrev;
    void* m_linkNext;
    uint64_t m_flags;

    //bool IsSpatial() const;
    //bool IsComponent() const;

    void Init();

    void* GetEntityBus() const;
    const GameObjectData* GetData() const;

    void FireEvent(EntityEvent* event);
    void Event(EntityEvent* event);
    //KB_DECLARE_VIRTUALFUNC(5, void, PropertyChanged, (modification), struct PropertyModification* modification)
    //KB_DECLARE_VIRTUALFUNC(5, void, PropertyChanged, (modification), const struct PropertyModification* modification)

    Realm GetRealm() const
    {
        return Realm(m_flags & (1 << 0));
    }
};

class NativeEntity : public EntityBase
{
public:
    class EntityBus* m_entityBus;
    const GameObjectData* m_data;
};

class EntityBus : public TypeObject
{
public:
    // Can't confirm the accuracy of these to 2015, however we don't need to really
    void* owner;
    EntityBus* m_parentBus;
    void* m_transformSpace;
    EntityBus* m_prevSibling;
    EntityBus* m_nextSibling;
    EntityBus* m_firstChild;
    int m_refCount;
    Realm realm;
    NativeEntity** m_peers;
    char pad[0x30];
    uintptr_t m_entityBusBridgeOrExposedObject;
};

struct ArrayBase
{
    static void* emptyArrayBegin();
};

template<typename T>
class FBArray : public ArrayBase
{
public:
    T* m_data = nullptr;

    FBArray()
    {
        reset();
    }

    void reset()
    {
        m_data = (T*)emptyArrayBegin();
    }

    inline void cloneFromVec(const std::vector<T>& vec)
    {
        init(vec.size());
        for (int i = 0; i < vec.size(); i++)
        {
            m_data[i] = vec[i];
        }
    }

    inline void init(uint32_t size)
    {
        if (size == 0)
        {
            reset();
            return;
        }

        size_t headerSize = sizeof(uint32_t) > __alignof(T) ? sizeof(uint32_t) : __alignof(T);
        m_data = (T*)(reinterpret_cast<uint8_t*>(FB_GLOBAL_ARENA->alloc(headerSize + size * sizeof(T))) + headerSize);
        memset(m_data, 0, size * sizeof(T));

        uint32_t* data = reinterpret_cast<uint32_t*>(reinterpret_cast<char*>(m_data));
        data[-1] = size;
    }

    inline void extend(uint32_t amount)
    {
        uint32_t prevSize = size();
        constexpr size_t headerSize = sizeof(uint32_t) > __alignof(T) ? sizeof(uint32_t) : __alignof(T);

        T* dest = (T*)(reinterpret_cast<uint8_t*>(FB_GLOBAL_ARENA->alloc(headerSize + ((prevSize + amount) * sizeof(T)))) + headerSize);
        memcpy(dest, m_data, prevSize * sizeof(T));
        memset(dest + prevSize, 0, amount * sizeof(T));

        // @TODO: free previous array (requires proper padding when alloc-ing tho)
        // FB_GLOBAL_ARENA->free(reinterpret_cast<uint8_t*>(m_data) - headerSize);
        if (MemoryArena* arena = ArenaMap::FindArenaForObject(this, false))
        {
            // cant seem to figure out which ptr its alloc'd to
            // arena->free(reinterpret_cast<void*>((reinterpret_cast<uintptr_t>(m_data) - headerSize) & ~15ul));
            MemoryLeakDb::AddEntry(prevSize * sizeof(T), "FBArray::extend original free fail");
        }
        else
        {
            // Leak!
            MemoryLeakDb::AddEntry(prevSize * sizeof(T), "FBArray::extend original free fail");
        }

        m_data = dest;

        uint32_t* data = reinterpret_cast<uint32_t*>(reinterpret_cast<char*>(m_data));
        data[-1] = prevSize + amount;
    }

    inline uint32_t size() const
    {
        auto* data = reinterpret_cast<uint32_t*>(m_data);
        if (!data)
        {
            return 0;
        }

        return reinterpret_cast<uint32_t*>(m_data)[-1];
    }

    inline T& at(uint32_t index)
    {
        return m_data[index];
    }

    inline T& operator[](uint32_t index)
    {
        return m_data[index];
    }

    class Iterator
    {
    public:
        using iterator_category = std::forward_iterator_tag;
        using value_type = T;
        using difference_type = std::ptrdiff_t;
        using pointer = T*;
        using reference = T&;

        Iterator(pointer ptr)
            : ptr(ptr)
        {}

        reference operator*() const
        {
            return *ptr;
        }
        pointer operator->()
        {
            return ptr;
        }

        Iterator& operator++()
        {
            ptr++;
            return *this;
        }

        Iterator operator++(int)
        {
            Iterator tmp = *this;
            ++(*this);
            return tmp;
        }

        friend bool operator==(const Iterator& a, const Iterator& b)
        {
            return a.ptr == b.ptr;
        };
        friend bool operator!=(const Iterator& a, const Iterator& b)
        {
            return a.ptr != b.ptr;
        };

    private:
        pointer ptr;
    };

    Iterator begin()
    {
        return Iterator(m_data);
    }
    Iterator end()
    {
        return Iterator(m_data + size());
    }

    Iterator begin() const
    {
        return Iterator(m_data);
    }
    Iterator end() const
    {
        return Iterator(m_data + size());
    }
};

class FBBitArray
{
public:
    FBBitArray();
    FBBitArray(uint32_t bitCount, void* arena = nullptr);
    virtual ~FBBitArray() = default;

    AU_DECLARE_GAMEMEMBERFUNC(0x143364CD0, void*, Init, (bitCount, arena), uint32_t bitCount, void* arena)
    AU_DECLARE_GAMEMEMBERFUNC(0x143360BB0, void*, Destroy, (arena), void* arena)
    AU_DECLARE_GAMEMEMBERFUNC_NOARGS(0x143365D20, void, Reset)
    AU_DECLARE_GAMEMEMBERFUNC_NOARGS(0x143366100, void, SetAllBits)

    uint32_t* m_bits;       // 0x08
    uint32_t m_defaultBits; // 0x10
    uint32_t m_bitCount;    // 0x14
    int32_t m_dwordCount;   // 0x18

private:
    AU_DECLARE_GAMEMEMBERFUNC_NOARGS(0x1433605E0, void*, Ctor)
}; // Size: 0x20

class PlayerExtent : public TypeObject
{};

struct PlayerExtentRegistration
{
    using ctorFunc_t = PlayerExtent* (*)(PlayerExtent*);
    using dtorFunc_t = ctorFunc_t;

    uint32_t offset;
    uint32_t size;
    uint32_t alignment;
    char pad_0C[4];
    const char* typeName;
    ctorFunc_t ctorFunc;
    dtorFunc_t dtorFunc;
    void* nullFunction;
    PlayerExtentRegistration* next;
};

class ServerPlayer;
class ServerPlayerExtent : public PlayerExtent
{};

#define AU_DECLARE_SERVERPLAYEREXTENT_MEMBERS()                                                                                            \
    static PlayerExtentRegistration* s_registration;                                                                                       \
    ServerPlayer* GetPlayer()                                                                                                              \
    {                                                                                                                                      \
        return reinterpret_cast<ServerPlayer*>(reinterpret_cast<uint8_t*>(this) - s_registration->offset);                                 \
    }

class ClientPlayer;
class ClientPlayerExtent : public PlayerExtent
{};

#define AU_DECLARE_CLIENTPLAYEREXTENT_MEMBERS()                                                                                            \
    static PlayerExtentRegistration* s_registration;                                                                                       \
    ClientPlayer* GetPlayer()                                                                                                              \
    {                                                                                                                                      \
        return reinterpret_cast<ClientPlayer*>(reinterpret_cast<uint8_t*>(this) - s_registration->offset);                                 \
    }

struct ForceCardSlot
{
    class ForceCardAsset* m_asset;
    char pad_08[0x10];
    float m_cooldown;
    char pad_1C[0xC];
};

class ForceCardServerPlayerExtent : public ServerPlayerExtent
{
public:
    AU_DECLARE_SERVERPLAYEREXTENT_MEMBERS();

    char pad_0000[0x50];
    ForceCardSlot m_slots[4];

    void IncreaseChargeCount(int numCharges);
};

class OnlineServerPlayerExtent : public ServerPlayerExtent
{
public:
    AU_DECLARE_SERVERPLAYEREXTENT_MEMBERS();
    char pad_0008[0x30];
    uint64_t m_currentPartner;

    void OnlineServerPlayerExtent::SetPartner(ServerPlayer* newPartner);
};

class PersistenceServerPlayerExtent : public ServerPlayerExtent
{
public:
    AU_DECLARE_SERVERPLAYEREXTENT_MEMBERS();

    char pad_0008[0x28];
    uint32_t m_rank;        // 0x30
    uint32_t m_kills;       // 0x34
    uint32_t m_deaths;      // 0x38
    uint32_t m_score;       // 0x3C
    uint32_t m_assists;     // 0x40
    uint32_t m_unk;         // 0x44


    AU_DECLARE_GAMEMEMBERFUNC(0x14445BA30, void, SetRank, (rank), uint32_t rank)
    AU_DECLARE_GAMEMEMBERFUNC(0x14445B940, void, SetKills, (kills), uint32_t kills)
    AU_DECLARE_GAMEMEMBERFUNC(0x14445B8F0, void, SetDeaths, (deaths), uint32_t deaths)
    AU_DECLARE_GAMEMEMBERFUNC(0x14445BA60, void, SetScore, (score), uint32_t score)
    AU_DECLARE_GAMEMEMBERFUNC(0x14445BA00, void, SetAssists, (assists), uint32_t assists) // maybe
    AU_DECLARE_GAMEMEMBERFUNC(0x14445B8B0, void, SetUnk, (something), uint32_t something)
};

class ServerSoldierPlayerExtent : public ServerPlayerExtent
{
    AU_DECLARE_SERVERPLAYEREXTENT_MEMBERS();

};

class PersistenceClientPlayerExtent : public ClientPlayerExtent
{
public:
    AU_DECLARE_CLIENTPLAYEREXTENT_MEMBERS();

    char pad_[0x70];
    uint32_t m_rank;
};

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

struct ServerCharacter
{
    void* vtable;
    char pad_0000[0x7B8];
};

#define AU_DECLARE_SERVERPLAYEREXTENT(name)                                                                                                \
    inline name* Get##name() const                                                                                                         \
    {                                                                                                                                      \
        return reinterpret_cast<name*>(GetExtent(name::s_registration));                                                                   \
    }

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

    void LogExtents();
    TypeObject* GetExtent(const char* name);

    ServerPlayerExtent* GetExtent(const PlayerExtentRegistration* registrar) const
    {
        return reinterpret_cast<ServerPlayerExtent*>(reinterpret_cast<uintptr_t>(this) + registrar->offset);
    }

    AU_DECLARE_GAMEMEMBERFUNC(0x143CBE6F0, void, SetTeam, (teamId), int teamId)

    AU_DECLARE_SERVERPLAYEREXTENT(ForceCardServerPlayerExtent)
    AU_DECLARE_SERVERPLAYEREXTENT(OnlineServerPlayerExtent)
    AU_DECLARE_SERVERPLAYEREXTENT(PersistenceServerPlayerExtent)
};

class ServerPlayerManager
{
public:
    char pad_0000[8];               // 0x0000
    class PlayerData* m_playerData; // 0x0008
    uint32_t m_maxPlayerCount;      // 0x0010
    uint32_t m_playerCountBitCount; // 0x0014
    uint32_t m_playerIdBitCount;    // 0x0018
    char pad_001C[0x24C];
    eastl::fixed_vector<ServerPlayer*, 64> m_players;
    eastl::fixed_vector<ServerPlayer*, 64> m_spectators;
    eastl::fixed_vector<ServerPlayer*, 64> m_localPlayers;

    ServerPlayer* GetPlayerOrSpectator(uint64_t id);
    ServerPlayer* GetPlayerOrSpectator(const char* name);

    ServerPlayer* GetPlayer(const char* name);
    ServerPlayer* GetSpectator(const char* name);

    ServerPlayer* GetPlayer(uint64_t id, bool includeAI = false);
    ServerPlayer* GetSpectator(uint64_t id);
}; // Size: 0x07EC

class ServerGameContext
{
public:
    char pad_0000[0x8];
    void* m_realm;
    __int64 m_messageManager;
    char pad_0018[0x50];
    ServerPlayerManager* m_serverPlayerManager;     // 0x68
    __int64 m_serverPeer;

    static ServerGameContext* Get()
    { 
        return *(ServerGameContext**)0x142C20A00;
    }

    ServerPlayerManager* GetPlayerManager()
    {
        return m_serverPlayerManager;
    }
};

class ClientPlayer
{
public:
    void* vtable;                     // 0x0000
    class PlayerData* m_data;         // 0x0008
    class MemoryArena* m_memoryArena; // 0x0010
    char* m_name;                     // 0x0018
    char pad_0020[0x14];              // 0x0020
    uint64_t m_id;
    char pad_003C[0x2B68];
    bool isAiPlayer;
    char pad_0003[0x3];
    uint32_t teamId; // 2BAC
    char pad_0000[0x108];

    void ClientPlayer::LogExtents();

    ClientPlayerExtent* GetExtent(const PlayerExtentRegistration* registrar) const
    { return reinterpret_cast<ClientPlayerExtent*>(reinterpret_cast<uintptr_t>(this) + registrar->offset); }

    AU_DECLARE_SERVERPLAYEREXTENT(PersistenceClientPlayerExtent);
};

class ClientPlayerManager
{
public:
    char pad_0000[8];
    class PlayerData* m_playerData;
    uint32_t m_maxPlayerCount;      // 0x0010
    uint32_t m_playerCountBitCount; // 0x0014
    uint32_t m_playerIdBitCount;    // 0x0018
    char pad_001C[0xBC];
    eastl::fixed_vector<ClientPlayer*, 64> m_players;
    eastl::fixed_vector<ClientPlayer*, 64> m_spectators;
    eastl::fixed_vector<ClientPlayer*, 64> m_localPlayers;

    ClientPlayer* GetPlayerById(__int64 id)
    {
        for (const auto& player : m_players)
        {
            if (player == nullptr)
                continue;

            if (player->m_id == id)
                return player;
        }
        return nullptr;
    }
};

class ClientConnection
{
public:
    char pad_0[0x38];
    void* client_connection;
};

class ClientGameContext
{
public:
    char pad_0000[0x8];                 // 0x0
    void* m_realm;                      // 0x8
    void* messageManager;               // 0x10
    char pad_0018[0x10];                // 0x20
    __int64 physicsManager;             // 0x28
    char pad_0030[0x8];                 // 0x30
    void* clientLevel;                  // 0x38
    char pad_0040[0x28];                // 0x60
    ClientPlayerManager* playerManager; // 0x68
    ClientConnection* client;

    ClientPlayerManager* GetPlayerManager()
    {
        if (this != nullptr && this->playerManager != nullptr)
        {
            return this->playerManager;
        }

        return nullptr;
    }

    static ClientGameContext* Get()
    { 
        return *(ClientGameContext**)0x142AE8080;
    }
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

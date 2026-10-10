// Copyright Nuuby. All Rights Reserved

#pragma once

#include <SDK/SDK.h>
#include <SDK/TypeInfo.h>
#include <Hook/Func.h>

#include <mutex>
#include <unordered_set>
#include <functional>
#include <map>

namespace Kyber
{
#define AU_INTERNAL_IMPLEMENT_ENTITY(name, dataType, override)                                                                             \
    AuricEntityBase* __entityCreator__##name##_##dataType(EntityManager* entityManager, NativeEntity* entity, DataContainer* data)         \
    {                                                                                                                                      \
        return new name(entityManager, entity, reinterpret_cast<dataType*>(data));                                                         \
    }                                                                                                                                      \
                                                                                                                                           \
    EntityStaticRegistrar _entityRegistrar_##name##_##dataType(#dataType, __entityCreator__##name##_##dataType, override)

#define AU_IMPLEMENT_ENTITY_OVERRIDE(name, dataType) AU_INTERNAL_IMPLEMENT_ENTITY(name, dataType, true)

typedef void(__fastcall* Entity_propertyChanged_t)(void* entity, void* modification);
typedef void(__fastcall* Entity_onDestroy_t)(void* entity);
typedef void(__fastcall* Entity_event_t)(void* entity, EntityEvent* event);
typedef void(__fastcall* Entity_deinit_t)(void* entity, void* info);
typedef void(__fastcall* Entity_dtor_t)(void* entity);

struct DataContext
{
    const void* bus;
    const void* data;
    const void* exposed;

    DataContext(const void* bus = 0, const void* data = 0, const void* exposed = 0)
        : bus(bus)
        , data(data)
        , exposed(exposed)
    {}
};

TL_DECLARE_FUNC(0x143A48FB0, void, DataContext_ctor, void* entityBus, DataContext* dcOut);

class CacheData
{
public:
    void* value;                // 0x0000
    class TypeInfo* valueType;  // 0x0008
    char pad[0xC];              // 0x0010
    uint32_t flags;             // 0x001C
};

enum CacheDataFlags
{
    CacheDataFlags_IsValueSetFlag = 1 << 5,
    CacheDataFlags_IsValueWrittenFlag = 1 << 6,
};

struct PropertyReaderBase
{
    CacheData* m_cache = nullptr;
    void* m_defaultValue = nullptr;

    bool HasConnection() const
    {
        return m_cache != nullptr;
    }

    bool HasConnectionValue() const
    {
        return m_cache != nullptr ? (m_cache->flags & CacheDataFlags_IsValueWrittenFlag) != 0 : false;
    }

    const void* Get() const;
};

template<typename T>
struct PropertyReader : PropertyReaderBase
{
    const T Get() const
    { 
        return *reinterpret_cast<const T*>(PropertyReaderBase::Get());
    }
};

struct PropertyWriterBase
{

    CacheData* m_cache = nullptr;

    bool HasConnection() const
    {
        return m_cache != nullptr; 
    }

    bool HasConnectionValue() const
    {
        return m_cache != nullptr ? (m_cache->flags & (CacheDataFlags_IsValueSetFlag | CacheDataFlags_IsValueWrittenFlag)) != 0 : false;
    }
};

TL_DECLARE_FUNC(0x143342000, void, DataBus_CreateFieldOverride, const void* bus, PropertyWriterBase* inst, const DataContainer* data,
    int fieldNameHash, const TypeInfo* typeInfo, const void* defaultValue, bool writeValue);
TL_DECLARE_FUNC(0x14330B8B0, void*, PropertyRefWriterBase_set, CacheData* cache, const void* value, bool callListeners);
TL_DECLARE_FUNC(0x14330C470, void, PropertyReaderBase_set, PropertyReaderBase* inst, const DataContext* dc, const DataContainer* data,
    int fieldNameHash, const TypeInfo* typeInfo, const void* defaultValue);

template<typename T>
struct PropertyWriter : PropertyWriterBase
{
    const T* Get() const
    {
        return reinterpret_cast<const T*>(m_cache->value);
    }

    void init(const DataContext* dc, const DataContainer* data, int fieldNameHash, const TypeInfo* typeInfo, const void* defaultValue,
        bool writeValue)
    {
        DataBus_CreateFieldOverride(dc->bus,this, data, fieldNameHash, typeInfo, defaultValue, writeValue);
    }

    void Set(T* value) const
    {
        if (m_cache == nullptr)
        {
            return;
        }

        PropertyRefWriterBase_set(this->m_cache, value, true);
    }

    void operator=(T* value) const
    {
        Set(value);
    }

    void operator=(T& value) const
    {
        Set(&value);
    }
};

class AuricEntityBase
{
    friend class EntityManager;

public:
    AuricEntityBase(NativeEntity* entity, DataContainer* data);
    virtual ~AuricEntityBase() = default;

    void FireEvent(EventId entityEvent);
    void FireEvent(const char* event);

    
    template<typename T>
    PropertyWriter<T> CreateFieldOverride(const char* fieldName, const TypeInfo* type, const void* defaultValue)
    {
        int fieldHash = StringUtils::HashQuick(fieldName);

        DataContext dc;
        DataContext_ctor(m_nativeEntity->m_entityBus, &dc);

        PropertyWriter<T> writer;
        writer.init(&dc, m_data, fieldHash, type, defaultValue, defaultValue != nullptr);
        return writer;
    }

    template<typename T>
    PropertyWriter<T> CreateFieldOverride(const char* fieldName, const TypeInfo* type)
    { 
        return CreateFieldOverride<T>(fieldName, type, nullptr); 
    }

    template<typename T>
    PropertyReader<T> GetFieldReader(const char* fieldName) const
    {
        int fieldHash = StringUtils::HashQuick(fieldName);

        DataContext dc;
        DataContext_ctor(m_nativeEntity->m_entityBus, &dc);

        PropertyReader<T> reader;
        PropertyReaderBase_set(&reader, &dc, m_data, fieldHash, nullptr, nullptr);
        return reader;
    }

    template<typename T>
    T* ReadField(const char* fieldName)
    { return reinterpret_cast<T*>(ReadField(fieldName)); }

    virtual void OnDestroy()
    {
        m_isInitialized = false;

        if (m_origOnDestroyFn == nullptr)
        {
            return;
        }

        m_origOnDestroyFn(m_nativeEntity);
    }

    virtual void Event(EntityEvent* event)
    {
        if (m_origEventFn == nullptr)
        {
            return;
        }

        m_origEventFn(m_nativeEntity, event);
    }

    virtual void PropertyChanged(void* modification)
    {
        if (m_origPropertyChangedFn == nullptr)
        {
            return;
        }

        m_origPropertyChangedFn(m_nativeEntity, modification);
    }

    virtual void Deinit(void* info)
    {
        m_isInitialized = false;

        if (m_origDeinitFn == nullptr)
        {
            return;
        }

        m_origDeinitFn(m_nativeEntity, info);
    }

    virtual void Update(const void* params) {};

    const DataContainer* GetData() const
    {
        return m_data;
    }

    Entity_propertyChanged_t m_origPropertyChangedFn = nullptr;

    Entity_onDestroy_t m_origOnDestroyFn = nullptr;
    Entity_event_t m_origEventFn = nullptr;

protected:
    NativeEntity* m_nativeEntity;
    const DataContainer* m_data;

    void SetWantUpdates(bool wantUpdates)
    {
        m_wantUpdates = wantUpdates;
    }

private:
    bool m_isSpatialEntity;
    bool m_isInitialized;
    bool m_wantUpdates;

    Entity_deinit_t m_origDeinitFn = nullptr;
    Entity_dtor_t m_origDtorFn = nullptr;
};

template<typename T>
class AuricEntity : public AuricEntityBase
{
public:
    AuricEntity(NativeEntity* entity, T* data)
        : AuricEntityBase(entity, data)
    {}

    const T* GetData() const
    { 
        return static_cast<const T*>(m_data);
    }
};

using AuricEntityCreator = std::function<AuricEntityBase*(class EntityManager*, NativeEntity*, DataContainer*)>;

class EntityManagerStaticData
{
public:
    static EntityManagerStaticData& Get()
    {
        static EntityManagerStaticData instance;
        return instance;
    }

    void RegisterEntity(const std::string& dataName, AuricEntityCreator creator, bool override = false)
    {
        m_creators[dataName] = creator;

        if (override)
        {
            m_overrideCreators.insert(dataName);
        }
    }

    bool IsOverrideCreator(const std::string& dataName)
    {
        return m_overrideCreators.count(dataName);
    }

    AuricEntityCreator GetCreator(const std::string& dataName)
    {
        if (!m_creators.count(dataName))
        {
            return nullptr;
        }

        return m_creators[dataName];
    }

private:
    std::map<std::string, AuricEntityCreator> m_creators;

    std::unordered_set<std::string> m_overrideCreators;
};

class EntityStaticRegistrar
{
public:
    EntityStaticRegistrar(const std::string& dataName, AuricEntityCreator creator, bool override)
    {
        EntityManagerStaticData& data = EntityManagerStaticData::Get();
        data.RegisterEntity(dataName, creator, override);
    }
};

struct AuricEntityBindings
{
    NativeEntity* native;
    AuricEntityBase* auric;
};

class EntityManager
{
public:
    EntityManager();

    void InitializeHooks();

    TypeObject* EntityManager::CreateEntity(void* params, DataContainer* data);
    void OnEntityCreated(NativeEntity* entity);

    AuricEntityBase* GetAuricEntity(NativeEntity* nativeEntity);
    void RemoveEntity(NativeEntity* nativeEntity);

    void UpdateEntities(Realm realm, const void* params);

    void RegisterNativeTypeInfo();

    const TypeInfo* GetNativeType(const std::string& name)
    {
        return m_nativeTypeInfo.count(name) ? m_nativeTypeInfo[name] : nullptr;
    }

private:
    std::recursive_mutex m_mutex;
    std::vector<AuricEntityBindings> m_bindings;

    std::unordered_map<std::string, TypeInfo*> m_nativeTypeInfo;
};
}

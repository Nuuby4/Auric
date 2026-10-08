// Copyright Nuuby. All Rights Reserved

#pragma once

#include <SDK/SDK.h>
#include <SDK/TypeInfo.h>

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

class AuricEntityBase
{
    friend class EntityManager;

public:
    AuricEntityBase(NativeEntity* entity, DataContainer* data);
    virtual ~AuricEntityBase() = default;

    void FireEvent(EventId entityEvent);
    void FireEvent(const char* event);

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

private:
    std::recursive_mutex m_mutex;
    std::vector<AuricEntityBindings> m_bindings;
};
}

// Copyright Nuuby. All Rights Reserved.

#include <Entity/EntityManager.h>
#include <Core/Program.h>
#include <Utilities/PlatformUtils.h>

#include <Base/Log.h>
#include <Hook/HookManager.h>
#include <SDK/Funcs.h>
#include <SDK/SDK.h>

#include <iostream>

#define ENTITYFACTORY_INTERNALCREATEENTITY HOOK_OFFSET(0x143A68CC0)

namespace Kyber
{
TL_DECLARE_FUNC(0x143BED980, NativeEntity*, DefaultEntityCreator_ConsoleCommandEntity_Create, void* entityCreator, void* creationInfo)
TL_DECLARE_FUNC(0x143A4A870, void, EntityBus_FireEvent, void* inst, const DataContainer* data, const int eventHash);
TL_DECLARE_FUNC(0x143307D80, void*, PropertyReaderBaseGetHk, const PropertyReaderBase* inst);

const void* PropertyReaderBase::Get() const
{
    return PropertyReaderBaseGetHk(this);
}

AuricEntityBase::AuricEntityBase(NativeEntity* entity, DataContainer* data)
    : m_nativeEntity(entity)
    , m_data(data)
    , m_isSpatialEntity(false)
    , m_isInitialized(true)
    , m_wantUpdates(false)
    {}

void EntityManagerPropertyChangedHk(NativeEntity* entity, void* modification)
{
    if (g_program->m_entityManager == nullptr)
    {
        return;
    }

    AuricEntityBase* auricEntity = g_program->m_entityManager->GetAuricEntity(entity);
    if (auricEntity == nullptr)
    {
        return;
    }

    auricEntity->PropertyChanged(modification);
}

void EntityManagerOnDestroyHk(NativeEntity* entity)
{
    if (g_program->m_entityManager == nullptr)
    {
        return;
    }

    AuricEntityBase* auricEntity = g_program->m_entityManager->GetAuricEntity(entity);
    if (auricEntity == nullptr)
    {
        return;
    }

    auricEntity->OnDestroy();

    g_program->m_entityManager->RemoveEntity(entity);
    delete auricEntity;
}

void EntityManagerEventHk(NativeEntity* entity, EntityEvent* entityEvent)
{
    if (g_program->m_entityManager == nullptr)
    {
        return;
    }

    AuricEntityBase* auricEntity = g_program->m_entityManager->GetAuricEntity(entity);
    if (auricEntity == nullptr)
    {
        return;
    }

    auricEntity->Event(entityEvent);
}

void EntityManagerDeinitHk(NativeEntity* entity, void* info)
{
    if (g_program->m_entityManager == nullptr)
    {
        return;
    }

    AuricEntityBase* auricEntity = g_program->m_entityManager->GetAuricEntity(entity);
    if (auricEntity == nullptr)
    {
        return;
    }

    auricEntity->Deinit(info);
}

EntityManager::EntityManager()
{
    InitializeHooks();
    RegisterNativeTypeInfo();

    KYBER_LOG(LogLevel::Info, "[Entity] Initialized EntityManager");
}

TypeObject* EntityManager::CreateEntity(void* params, DataContainer* data)
{
    const TypeInfo* typeInfo = data->getType();
    if (typeInfo == nullptr)
    {
        return nullptr;
    }


    bool isOverrideCreator = EntityManagerStaticData::Get().IsOverrideCreator(typeInfo->getName());
    if (!isOverrideCreator)
    {
        return nullptr;
    }

    const char* name = typeInfo->getName();

    NativeEntity* entity = DefaultEntityCreator_ConsoleCommandEntity_Create(nullptr, params);
    if (entity == nullptr)
    {
        KYBER_LOG(LogLevel::Warning, "Failed to create entity for " << name);
        return nullptr;
    }

    PlatformUtils::DuplicateVTable(entity, 23);

    AuricEntityBase* auricEntity = EntityManagerStaticData::Get().GetCreator(typeInfo->getName())(this, entity, data);

    void* origPropertyChangedFn = PlatformUtils::HookVTableFunction(entity, EntityManagerPropertyChangedHk, 2);
    void* origEventFn = PlatformUtils::HookVTableFunction(entity, EntityManagerEventHk, 4);
    void* origOnDestroyFn = PlatformUtils::HookVTableFunction(entity, EntityManagerOnDestroyHk, 7);
    void* origDeinitFn = PlatformUtils::HookVTableFunction(entity, EntityManagerDeinitHk, 17);

    if (isOverrideCreator) // Research why this is a !isOverrideCreator in Kyber V2
    {
        auricEntity->m_origPropertyChangedFn = reinterpret_cast<Entity_propertyChanged_t>(origPropertyChangedFn);
        auricEntity->m_origOnDestroyFn = reinterpret_cast<Entity_onDestroy_t>(origOnDestroyFn);
        auricEntity->m_origEventFn = reinterpret_cast<Entity_event_t>(origEventFn);
        auricEntity->m_origDeinitFn = reinterpret_cast<Entity_deinit_t>(origDeinitFn);
    }
    else
    {
        auricEntity->m_origPropertyChangedFn = nullptr;
        auricEntity->m_origOnDestroyFn = nullptr;
        auricEntity->m_origEventFn = nullptr;
        auricEntity->m_origDeinitFn = nullptr;
        auricEntity->m_origDtorFn = nullptr;
    }

    KYBER_LOG(LogLevel::DebugPlusPlus, "Created custom entity for " << typeInfo->getName() << " with data at " << data);

    std::lock_guard<std::recursive_mutex> lock(m_mutex);
    m_bindings.push_back({ entity, auricEntity });
    return entity;
}

void EntityManager::OnEntityCreated(NativeEntity* entity)
{

}

void EntityManager::UpdateEntities(Realm realm, const void* params)
{
    std::lock_guard<std::recursive_mutex> lock(m_mutex);
    for (const auto& binding : m_bindings)
    {
        if (!binding.auric->m_wantUpdates || !binding.auric->m_isInitialized)
        {
            continue;
        }

        if (binding.native->GetRealm() != realm)
        {
            continue;
        }

        binding.auric->Update(params);
    }
}

void EntityManager::RegisterNativeTypeInfo()
{
    TypeInfo* firstTypeInfo = (TypeInfo*)0x1430703F0;
    for (TypeInfo* info = firstTypeInfo; info; info = info->next)
    {
        m_nativeTypeInfo[info->getName()] = info;
    }
}

void AuricEntityBase::FireEvent(EventId entityEvent)
{
    KYBER_LOG(LogLevel::Info, "Firing event " << entityEvent << " NativeEntity: " << m_nativeEntity);

    EntityEvent event = entityEvent;
    EntityBus_FireEvent(m_nativeEntity->m_entityBus, reinterpret_cast<const DataContainer*>(m_data), entityEvent);
}

void AuricEntityBase::FireEvent(const char* event)
{
    FireEvent(StringUtils::HashQuick(event));
}

AuricEntityBase* EntityManager::GetAuricEntity(NativeEntity* nativeEntity)
{
    std::lock_guard<std::recursive_mutex> lock(m_mutex);
    for (const auto& binding : m_bindings)
    {
        if (binding.native != nativeEntity)
        {
            continue;
        }

        return binding.auric;
    }

    return nullptr;
}

void EntityManager::RemoveEntity(NativeEntity* entity)
{
    std::lock_guard<std::recursive_mutex> lock(m_mutex);
    m_bindings.erase(
        std::remove_if(m_bindings.begin(), m_bindings.end(), [&](AuricEntityBindings const& binding) { return binding.native == entity; }),
        m_bindings.end());
}

void* EntityFactory_InternalCreateEntityHk(void* params, void* datacontext)
{
    static const auto trampoline = HookManager::Call(EntityFactory_InternalCreateEntityHk);
    DataContainer* data = *(DataContainer**)((__int64)params + 0xD0);
    const char* name = data->getType()->getName();

    if (g_program->m_entityManager != nullptr)
    {
        TypeObject* entityManagerEntity = g_program->m_entityManager->CreateEntity(params, data);
        if (entityManagerEntity != nullptr)
        {
            return entityManagerEntity;
        }
    }

    void* entity = trampoline(params, datacontext);

    if (entity != nullptr && g_program->m_entityManager != nullptr)
    {
        g_program->m_entityManager->OnEntityCreated(reinterpret_cast<NativeEntity*>(entity));
    }
    
    return entity;
}

void EntityManager::InitializeHooks()
{
    HookManager::CreateHook(ENTITYFACTORY_INTERNALCREATEENTITY, EntityFactory_InternalCreateEntityHk);
    Hook::ApplyQueuedActions();
}
}

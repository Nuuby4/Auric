// Copyright Nuuby. All Rights Reserved.

#include <Entity/Overrides/ConsoleCommandTriggerEntity.h>
#include <Core/Program.h>
#include <Base/Log.h>

namespace Kyber
{
static void** g_realmContext = (void**)0x142AC3D68;
TL_DECLARE_FUNC(0x143A34B30, void*, RealmContext_setCurrentContext, void* context);

AU_IMPLEMENT_ENTITY_OVERRIDE(ConsoleCommandTriggerEntity, ConsoleCommandTriggerEntityData);

ConsoleCommandTriggerEntity::ConsoleCommandTriggerEntity(EntityManager* entityManager, NativeEntity* entity, ConsoleCommandTriggerEntityData* data)
    : AuricEntity(entity, data)
    , m_commandCount(0)
{
    auto delegate = fastdelegate::MakeDelegate(this, &ConsoleCommandTriggerEntity::Execute);

    m_name = data->CommandName;

    ConsoleRegistry_registerInstanceMethod(delegate, m_name.c_str(), "Auric");

    SetWantUpdates(true);
}

ConsoleCommandTriggerEntity::~ConsoleCommandTriggerEntity() 
{ 
    g_program->m_console->UnregisterCommand(m_name.c_str()); 
}

void ConsoleCommandTriggerEntity::Execute(ConsoleContext& cc)
{
    m_commandCount++;
}

void ConsoleCommandTriggerEntity::Update(const void* params)
{
    if (m_commandCount == 0)
    {
        return;
    }
    
    void* prev = RealmContext_setCurrentContext(g_realmContext[m_nativeEntity->GetRealm()]);

    for (int i = 0; i < m_commandCount; ++i)
    {
        FireEvent("OnCommand");
    }

    RealmContext_setCurrentContext(prev);

    m_commandCount = 0;
}

} // namespace Kyber

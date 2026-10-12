// Copyright Nuuby. All Rights Reserved

#include <Entity/Overrides/KillSwitchEntity.h>

#include <Core/Program.h>
#include <Base/Log.h>

namespace Kyber
{
AU_IMPLEMENT_ENTITY_OVERRIDE(KillSwitchEntity, KillSwitchEntityData);

KillSwitchEntity::KillSwitchEntity(EntityManager* entityManager, NativeEntity* entity, KillSwitchEntityData* data)
    : AuricEntity(entity, data)
{ 
    m_out = CreateFieldOverride<bool>(0x4984d719, g_program->m_entityManager->GetNativeType("Boolean"));

    // @TODO Create System Settings for this
    m_result = true;
    m_out = &m_result;
}
} // namespace Kyber

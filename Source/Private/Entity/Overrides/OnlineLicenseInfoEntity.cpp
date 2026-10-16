// Copyright Nuuby. All Rights Reserved

#include <Entity/Overrides/OnlineLicenseInfoEntity.h>

#include <Core/Program.h>
#include <Base/Log.h>

namespace Kyber
{
AU_IMPLEMENT_ENTITY_OVERRIDE(OnlineLicenseInfoEntity, OnlineLicenseInfoEntityData);

OnlineLicenseInfoEntity::OnlineLicenseInfoEntity(EntityManager* entityManager, NativeEntity* entity, OnlineLicenseInfoEntityData* data)
    : AuricEntity(entity, data)
{
    m_out = CreateFieldOverride<bool>("Available", g_program->m_entityManager->GetNativeType("Boolean"));
    
    // @TODO Create System Settings for this
    m_out = true;
}
} // namespace Kyber

// Copyright Nuuby. All Rights Reserved.

#include <Entity/EntityManager.h>
#include <SDK/SDK.h>

namespace Kyber
{
class KillSwitchEntity : public AuricEntity<KillSwitchEntityData>
{
public:
    KillSwitchEntity(EntityManager* entityManager, NativeEntity* entity, KillSwitchEntityData* data);

private:
    PropertyWriter<bool> m_out;
    bool m_result;
};
} // namespace Kyber

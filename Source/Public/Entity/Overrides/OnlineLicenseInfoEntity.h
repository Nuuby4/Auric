// Copyright Nuuby. All Rights Reserved.

#include <Entity/EntityManager.h>
#include <SDK/SDK.h>

namespace Kyber
{
class OnlineLicenseInfoEntity : public AuricEntity<OnlineLicenseInfoEntityData>
{
public:
    OnlineLicenseInfoEntity(EntityManager* entityManager, NativeEntity* entity, OnlineLicenseInfoEntityData* data);

private:
    PropertyWriter<bool> m_out;
    bool m_result;
};
} // namespace Kyber

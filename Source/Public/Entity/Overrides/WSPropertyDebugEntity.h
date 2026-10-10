// Copyright Nuuby. All Rights Reserved.

#include <Entity/EntityManager.h>
#include <SDK/SDK.h>

#include <string>

namespace Kyber
{
class WSPropertyDebugEntity : public AuricEntity<WSPropertyDebugEntityData>
{
public:
    WSPropertyDebugEntity(EntityManager* entityManager, NativeEntity* entity, WSPropertyDebugEntityData* data);

    void Update(const void* params);

private:
    bool m_showOnScreen;
    std::string m_str;
};
} // namespace Kyber

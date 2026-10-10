// Copyright Nuuby. All Rights Reserved.

#include <Entity/EntityManager.h>
#include <SDK/SDK.h>

#include <string>

namespace Kyber
{
class PropertyDebugEntity : public AuricEntity<PropertyDebugEntityData>
{
public:
    PropertyDebugEntity(EntityManager* entityManager, NativeEntity* entity, PropertyDebugEntityData* data);

    void Event(EntityEvent* event);
    void Update(const void* params);

private:
    bool m_visible;
    std::string m_str;
};
} // namespace Kyber

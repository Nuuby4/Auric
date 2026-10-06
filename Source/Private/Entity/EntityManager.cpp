// Copyright Nuuby. All Rights Reserved.

#include <Entity/EntityManager.h>

namespace Kyber
{
AuricEntityBase::AuricEntityBase(NativeEntity* entity, DataContainer* data)
    : m_nativeEntity(entity)
    , m_data(data)
    {}
}

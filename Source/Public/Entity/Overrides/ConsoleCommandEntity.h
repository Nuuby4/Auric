// Copyright Nuuby. All Rights Reserved.

#pragma once

#include <Entity/Overrides/ConsoleCommandEntity.h>
#include <Entity/EntityManager.h>

#include <SDK/SDK.h>
#include <SDK/TypeInfo.h>

namespace Kyber
{
class ConsoleCommandEntity : public AuricEntity<ConsoleCommandEntityData>
{
public:
    ConsoleCommandEntity(NativeEntity* entity, ConsoleCommandEntityData* data);
    ~ConsoleCommandEntity();

    void Event(EntityEvent* event) override;
};
}

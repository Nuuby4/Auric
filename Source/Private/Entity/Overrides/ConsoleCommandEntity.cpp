// Copyright Nuuby. All Rights Reserved.

#include <Entity/Overrides/ConsoleCommandEntity.h>
#include <Core/Program.h>
#include <Base/Log.h>

namespace Kyber
{
AU_IMPLEMENT_ENTITY_OVERRIDE(ConsoleCommandEntity, ConsoleCommandEntityData);

ConsoleCommandEntity::ConsoleCommandEntity(EntityManager* entityManager, NativeEntity* entity, ConsoleCommandEntityData* data)
    : AuricEntity(entity, data)
{}

ConsoleCommandEntity::~ConsoleCommandEntity()
{}

void ConsoleCommandEntity::Event(EntityEvent* event)
{
    KYBER_LOG(LogLevel::Info, "Got Event: " << std::hex << event->eventId << " from ConsoleCommandEntity!")
    return;
}
}

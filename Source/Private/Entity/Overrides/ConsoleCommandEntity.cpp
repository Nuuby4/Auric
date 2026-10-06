// Copyright Nuuby. All Rights Reserved.

#include <Entity/Overrides/ConsoleCommandEntity.h>
#include <Core/Program.h>

namespace Kyber
{
ConsoleCommandEntity::ConsoleCommandEntity(NativeEntity* entity, ConsoleCommandEntityData* data)
    : AuricEntity(entity, data)
{}

ConsoleCommandEntity::~ConsoleCommandEntity()
{}

void ConsoleCommandEntity::Event(EntityEvent* event)
{
    if (!event->Is("Execute"))
    {
        return;
    }

    for (char* cmd : GetData()->Commands)
    {
        g_program->m_console->EnqueueCommand(cmd);
    }
}
}

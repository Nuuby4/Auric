// Copyright Nuuby. All Rights Reserved

#include <Entity/Overrides/WSPrintDebugTextEntity.h>

#include <Base/Log.h>

namespace Kyber
{
AU_IMPLEMENT_ENTITY_OVERRIDE(WSPrintDebugTextEntity, WSPrintDebugTextEntityData);

WSPrintDebugTextEntity::WSPrintDebugTextEntity(EntityManager* entityManager, NativeEntity* entity, WSPrintDebugTextEntityData* data)
    : AuricEntity(entity, data)
{
    SetWantUpdates(false);
}

void WSPrintDebugTextEntity::Event(EntityEvent* event)
{
    for (PrintDebugTextInput input : GetData()->Events) 
    {
        if (event->Is(input.NameInternal))
        {
            KYBER_LOG(LogLevel::Info, "[WSPrintDebugTextEntity] " << input.Name);
        }
    }
}
} // namespace Kyber

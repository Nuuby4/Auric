// Copyright Nuuby. All Rights Reserved

#pragma once

#include <Entity/EntityManager.h>

#include <Core/DebugHooks.h>

namespace Kyber
{
class WSPrintDebugTextEntity : public AuricEntity<WSPrintDebugTextEntityData>
{
public:
    WSPrintDebugTextEntity(EntityManager* entityManager, NativeEntity* entity, WSPrintDebugTextEntityData* data);

    void Event(EntityEvent* event) override;

private:
    std::string m_text;
};
} // namespace Kyber

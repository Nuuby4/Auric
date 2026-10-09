// Copyright Nuuby. All Rights Reserved.

#pragma once

#include <Entity/EntityManager.h>

#include <SDK/SDK.h>
#include <SDK/TypeInfo.h>

namespace Kyber
{
class ConsoleCommandEntity : public AuricEntity<ConsoleCommandEntityData>
{
public:
    ConsoleCommandEntity(EntityManager* entityManager, NativeEntity* entity, ConsoleCommandEntityData* data);
    ~ConsoleCommandEntity();

    void Event(EntityEvent* event) override;
    void CommandCallback(const char* cmdResult);

private:
    PropertyWriter<bool> m_boolOut;
    PropertyWriter<int> m_intOut;
    PropertyWriter<float> m_floatOut;
    PropertyWriter<char*> m_stringOut;

    bool m_pendingCallback;
};
}

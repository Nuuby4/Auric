// Copyright Nuuby. All Rights Reserved.

#pragma once

#include <Entity/EntityManager.h>
#include <Core/Console.h>

#include <SDK/SDK.h>
#include <SDK/TypeInfo.h>

#include <string>

namespace Kyber
{
class ConsoleCommandTriggerEntity : public AuricEntity<ConsoleCommandTriggerEntityData>
{
public:
    ConsoleCommandTriggerEntity(EntityManager* entityManager, NativeEntity* entity, ConsoleCommandTriggerEntityData* data);
    ~ConsoleCommandTriggerEntity();

    void Execute(ConsoleContext& cc);
    void Update(const void* params) override;

private:
    std::string m_name;

    int m_commandCount;
};
} // namespace Kyber

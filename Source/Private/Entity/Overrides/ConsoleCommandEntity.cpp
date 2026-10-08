// Copyright Nuuby. All Rights Reserved.

#include <Entity/Overrides/ConsoleCommandEntity.h>

#include <Core/Console.h>
#include <Core/Program.h>
#include <Base/Log.h>

namespace Kyber
{
AU_IMPLEMENT_ENTITY_OVERRIDE(ConsoleCommandEntity, ConsoleCommandEntityData);

ConsoleCommandEntity::ConsoleCommandEntity(EntityManager* entityManager, NativeEntity* entity, ConsoleCommandEntityData* data)
    : AuricEntity(entity, data)
{
    /* m_boolOut = CreateFieldOverride<bool>("BoolResult", g_program->m_entityManager->GetNativeType("Boolean"));
    m_intOut = CreateFieldOverride<int>("IntResult", g_program->m_entityManager->GetNativeType("Int32"));
    m_floatOut = CreateFieldOverride<float>("FloatResult", g_program->m_entityManager->GetNativeType("Float32"));
    m_stringOut = CreateFieldOverride<char*>("StringResult", g_program->m_entityManager->GetNativeType("CString"));*/
}

ConsoleCommandEntity::~ConsoleCommandEntity()
{}

void ConsoleCommandEntity::Event(EntityEvent* event)
{
    if (!event->Is("Execute"))
    {
        return;
    }

    //Realm currentRealm = m_nativeEntity->GetRealm();
    //if (currentRealm != Realm_Client)
    //{
    //    return;
    //}

    for (char* cmd : GetData()->Commands)
    {
        Console_enqueueCommand(cmd, nullptr);
        // We don't want to update the property outputs of every command if there are multiple commands
        //if (i == 0 && !m_pendingCallback)
        //{
        //    m_pendingCallback = true;
        //    Console_enqueueCommand(cmd, ExecuteConsoleCommandCallback_t(this, &ConsoleCommandEntity::CommandCallback));
        //}
        //else
        //{
        //    Console_enqueueCommand(cmd, nullptr);
        //}
    }
    /*
    PropertyReader<char*> dynamicCommand = GetFieldReader<char*>("DynamicCommand");
    if (dynamicCommand.HasConnectionValue())
    {
        if (!m_pendingCallback)
        {
            m_pendingCallback = true;
            Console_enqueueCommand(dynamicCommand.Get(), ExecuteConsoleCommandCallback_t(this, &ConsoleCommandEntity::CommandCallback));
        }
        else
        {
            Console_enqueueCommand(dynamicCommand.Get(), nullptr);
        }
    }*/
}
}

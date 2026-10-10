// Copyright Nuuby. All Rights Reserved.

#include <Entity/Overrides/ConsoleCommandEntity.h>
#include <Entity/EntityManager.h>

#include <Core/Console.h>
#include <Core/Program.h>
#include <Base/Log.h>

namespace Kyber
{
AU_IMPLEMENT_ENTITY_OVERRIDE(ConsoleCommandEntity, ConsoleCommandEntityData);

ConsoleCommandEntity::ConsoleCommandEntity(EntityManager* entityManager, NativeEntity* entity, ConsoleCommandEntityData* data)
    : AuricEntity(entity, data)
{
    m_boolOut = CreateFieldOverride<bool>("BoolResult", g_program->m_entityManager->GetNativeType("Boolean"));
    m_intOut = CreateFieldOverride<int>("IntResult", g_program->m_entityManager->GetNativeType("Int32"));
    m_floatOut = CreateFieldOverride<float>("FloatResult", g_program->m_entityManager->GetNativeType("Float32"));
    m_stringOut = CreateFieldOverride<char*>("StringResult", g_program->m_entityManager->GetNativeType("CString"));

    m_pendingCallback = false;
}

ConsoleCommandEntity::~ConsoleCommandEntity()
{}

void ConsoleCommandEntity::Event(EntityEvent* event)
{
    if (!event->Is("Execute"))
    {
        return;
    }

    for (const char* cmd : GetData()->Commands)
    {
        if (cmd && !m_pendingCallback)
        {
            m_pendingCallback = true;
            Console_enqueueCommand(cmd, ExecuteConsoleCommandCallback_t(this, &ConsoleCommandEntity::CommandCallback));

            // Temporary Until Maxima or Alternative is integrated (Callback cannot get settings not accessible in the console)
            WSGameSettings* wsSettings = Settings<WSGameSettings>("WhiteShark");
            if (strcmp(cmd, "Whiteshark.SkipLobby") == 0)
            {
                bool SkipLobby = wsSettings->SkipLobby;
                m_boolOut = &SkipLobby;
            }
            else if (strcmp(cmd, "Whiteshark.SkipPreRound") == 0)
            {
                bool SkipPreRound = wsSettings->SkipPreRound;
                m_boolOut = &SkipPreRound;
            }
            else if (strcmp(cmd, "Whiteshark.LobbyThreshold") == 0)
            {
                int LobbyThreshold = wsSettings->LobbyThreshold;
                m_intOut = &LobbyThreshold;
            }
            else if (strcmp(cmd, "Whiteshark.LobbyMaxTeamDiff") == 0)
            {
                int LobbyMaxTeamDiff = wsSettings->LobbyMaxTeamDiff;
                m_intOut = &LobbyMaxTeamDiff;
            }
        }
        else if (cmd)
        {
            Console_enqueueCommand(cmd, nullptr);
        }
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
void ConsoleCommandEntity::CommandCallback(const char* cmdResult)
{
    m_pendingCallback = false;
    if (strlen(cmdResult) == 0)
    {
        return;
    }

    KYBER_LOG(LogLevel::Info, "[ConsoleCommand] Got result: " << cmdResult);

    // stringResult = const_cast<char*>(cmdResult);
    // tempBoolResult = (cmdResult == 0) || (strcmp(cmdResult, "true") == 0);
    // tempIntResult = atoi(cmdResult);
    // float floatResult = std::stof(cmdResult);

    // m_boolOut = &return1;
    // m_intOut = &tempIntResult;
    // m_floatOut = &floatResult;
    // m_stringOut = &stringResult;
    FireEvent("OnResult");
}
}

// Copyright Nuuby. All Rights Reserved

#pragma once

#include <SDK/SDK.h>
#include <SDK/TypeInfo.h>
#include <Hook/Func.h>

#include <EASTL/string.h>
#include <FastDelegate/FastDelegate/FastDelegate.h>

#include <string>
#include <sstream>

namespace Kyber
{

typedef fastdelegate::FastDelegate<void(const char*)> ExecuteConsoleCommandCallback_t;
TL_DECLARE_FUNC(0x143336A90, void, Console_enqueueCommand, const char* cmdString, ExecuteConsoleCommandCallback_t callback);

class ConsoleStream
{
private:
    std::string input;
    std::string delimiter;
    std::istringstream stream;

public:
    ConsoleStream(const std::string& inputStr, const std::string& delim)
        : input(inputStr)
        , delimiter(delim)
        , stream(input)
    {}

    template<typename T>
    ConsoleStream& operator>>(T& variable)
    {
        std::string token;
        if (std::getline(stream, token, delimiter[0]))
        {
            std::istringstream(token) >> std::boolalpha >> variable;
        }
        return *this;
    }

    ConsoleStream& operator>>(eastl::string& variable)
    {
        std::string token;
        if (std::getline(stream, token, delimiter[0]))
        {
            variable = token.c_str();
        }
        return *this;
    }
};

class ConsoleContext
{
public:
    char pad_0000[352]; // 0x0000
    char* rawArguments; // 0x0160

    template<typename T>
    ConsoleContext& operator<<(T&& variable)
    {
        std::ostringstream stream;
        stream << std::forward<T>(variable);
        return *this;
    }

    ConsoleStream stream()
    { return ConsoleStream(rawArguments, " "); }
};

typedef void (*StaticConsoleMethodPtr_t)(ConsoleContext&);

struct InstanceMethod
{
    const char* name;
    const char* groupName;
    const char* description;
    fastdelegate::FastDelegate<void(ConsoleContext&)> func;
};

struct ConsoleMethod
{
    StaticConsoleMethodPtr_t fptr;
    const char* name;
    const char* groupName;
    const char* description;
};

void ConsoleRegistry_registerInstanceMethod(
    fastdelegate::FastDelegate1<ConsoleContext&, void>& method, const char* name, const char* groupName);

class Console
{
public:
    Console();

    void EnqueueCommand(const char* cmd);
    void RegisterConsoleCommand(StaticConsoleMethodPtr_t func, const char* name, const char* description);
    void UnregisterCommand(const char* name);
};
} // namespace Kyber

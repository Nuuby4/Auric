// Copyright Nuuby. All Rights Reserved

#include <Utilities/StringUtils.h>

#include <stdio.h>
#include <string.h>

namespace Kyber
{
char* StringUtils::CopyWithArena(const std::string& src, MemoryArena* arena)
{
    char* dest = (char*)arena->alloc(src.length() + 1);
    strcpy(dest, src.c_str());
    return dest;
}
}

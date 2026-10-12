// Copyright Nuuby. Licensed under GPLv3.

#include <Utilities/Memory.h>
#include <SDK/Funcs.h>

#include <Core/Program.h>
#include <Base/Log.h>
#include <Hook/HookManager.h>

#include <EASTL/internal/config.h>
#include <winnt.h>

void* operator new(size_t size, Kyber::MemoryArena* arena) 
{
    return arena->alloc(size);
}

void* operator new[](size_t size, Kyber::MemoryArena* arena)
{
    return arena->alloc(size);
}

void operator delete(void* ptr, Kyber::MemoryArena* arena)
{
    return arena->free(ptr);
}

namespace Kyber
{
TL_DECLARE_FUNC(0x143330CA0, void*, MemoryArena_alloc, MemoryArena* arena, size_t size, size_t alignment);
TL_DECLARE_FUNC(0x143331830, void, MemoryArena_free, MemoryArena* arena, void* mem);

void* MemoryArena::alloc(size_t size, size_t align)
{
    return MemoryArena_alloc(this, size, align);
}

void* MemoryArena::alloc(size_t size)
{
    return alloc(size, (((size - 0x10) >> 0x3F) & 0xFFFFFFFFFFFFFFF8L) + 0x10);
}

void MemoryArena::free(void* mem)
{
    return MemoryArena_free(this, mem);
}
} // namespace Kyber

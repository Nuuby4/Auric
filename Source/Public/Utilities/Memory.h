// Copyright Nuuby. All Rights Reserved

#pragma once


namespace Kyber
{
class MemoryArena
{
public:
    char pad_0000[120]; // 0x0000

    void* alloc(size_t size, size_t align);
    void* alloc(size_t size);

    void free(void* mem);

    template<typename T>
    void del(T* mem)
    {
        if (mem != nullptr)
        {
            mem->~T();
            this->free(mem);
        }
    }
};

#define FB_CLIENT_ARENA (reinterpret_cast<MemoryArena*>(0x142A3B390))
#define FB_SERVER_ARENA (reinterpret_cast<MemoryArena*>(0x142A3CEB0))
#define FB_STATIC_ARENA (reinterpret_cast<MemoryArena*>(0x142A3A140))
#define FB_GLOBAL_ARENA (reinterpret_cast<MemoryArena*>(0x142A3A120))

//void* operator new(size_t size, Kyber::MemoryArena* arena);
//void* operator new[](size_t size, Kyber::MemoryArena* arena);
//void operator delete(void* ptr, Kyber::MemoryArena* arena);
}

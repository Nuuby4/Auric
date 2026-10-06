// Copyright Nuuby. All Rights Reserved

#pragma once

#include <SDK/SDK.h>
#include <SDK/TypeInfo.h>

namespace Kyber
{
class AuricEntityBase : public TypeObject
{
public:
    AuricEntityBase(NativeEntity* entity, DataContainer* data);
    virtual ~AuricEntityBase() = default;

    virtual void unk0() = 0;
    virtual void PropertyChanged() = 0;
    virtual __int64 unk1() { return 0; };
    virtual void Event(EntityEvent* event) = 0;
    virtual void unk2() = 0;
    virtual void unk3() = 0;
    virtual __int64 unk4(__int64 inst) { return *reinterpret_cast<__int64*>(inst + 0x28); };
    virtual __int64 unk5(__int64 inst) { return *reinterpret_cast<__int64*>(inst + 0x20); };
    virtual __int64 internalGetChildEntityBusInfo(__int64 a1, __int64 a2) = 0;
    virtual __int64 internalGetWeakPtr(__int64 a1, __int64 a2) = 0;
    virtual void unk6() = 0;
    virtual void OnCreate() = 0;
    virtual void OnSaveCreate() = 0;
    virtual void OnInit() = 0;
    virtual void unk7() = 0;
    virtual void onDeInit() = 0;
    virtual void unk8() = 0;
    virtual void unk9() = 0;
    virtual void unk10() = 0;
    virtual void internalGetOwner() = 0;

protected:
    NativeEntity* m_nativeEntity;
    const DataContainer* m_data;
};

template<typename T>
class AuricEntity : public AuricEntityBase
{
public:
    AuricEntity(NativeEntity* entity, T* data)
        : AuricEntityBase(entity, data)
    {}

    const T* GetData() const
    { 
        return static_cast<const T*>(m_data);
    }
};
}

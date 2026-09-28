// Copyright Nuuby. All Rights Reserved

#include <SDK/SDK.h>
#include <SDK/TypeInfo.h>

#include <Base/Log.h>
#include <SDK/Funcs.h>

namespace Kyber
{
TL_DECLARE_FUNC(0x1444EEDD0, bool, ForceCard_IncreaseChargeCount, ForceCardSlot* slot, int numCharges)
TL_DECLARE_FUNC(0x144500E80, uint64_t, ForceCard_SyncChargeCount, void* a, int unk)

PlayerExtentRegistration* ForceCardServerPlayerExtent::s_registration = reinterpret_cast<PlayerExtentRegistration*>(0x1427C3F38);

void ForceCardServerPlayerExtent::IncreaseChargeCount(int numCharges)
{
    ForceCardSlot* slot = &m_slots[1];
    if (slot->m_asset && ForceCard_IncreaseChargeCount(slot, numCharges))
    {
        ForceCard_SyncChargeCount(reinterpret_cast<uint8_t*>(this) + 0x38, 1);
    }
}

const char* TypeInfo::getName() const
{ 
	return typeInfoData->name; 
}

TypeCodeEnum TypeInfo::getBasicType() const
{ 
	return TypeCodeEnum((typeInfoData->flags >> 5) & 31); 
}

ServerPlayer* ServerPlayerManager::GetPlayerOrSpectator(uint64_t id)
{
    ServerPlayer* player = GetPlayer(id);
    if (player != nullptr)
    {
        return player;
    }

    player = GetSpectator(id);
    if (player != nullptr)
    {
        return player;
    }

    return nullptr;
}

ServerPlayer* ServerPlayerManager::GetPlayerOrSpectator(const char* name)
{
    ServerPlayer* player = GetPlayer(name);
    if (player != nullptr)
    {
        return player;
    }

    player = GetSpectator(name);
    if (player != nullptr)
    {
        return player;
    }

    return nullptr;
}

ServerPlayer* ServerPlayerManager::GetPlayer(const char* name)
{
    for (const auto& player : m_players)
    {
        if (player->m_isAIPlayer)
        {
            continue;
        }

        if (strcmp(player->m_name, name) != 0)
        {
            continue;
        }

        return player;
    }

    return nullptr;
}

ServerPlayer* ServerPlayerManager::GetSpectator(const char* name)
{
    for (const auto& player : m_spectators)
    {
        if (player->m_isAIPlayer)
        {
            continue;
        }

        if (strcmp(player->m_name, name) != 0)
        {
            continue;
        }

        return player;
    }

    return nullptr;
}

ServerPlayer* ServerPlayerManager::GetPlayer(uint64_t id, bool includeAI)
{
    for (const auto& player : m_players)
    {
        if (!includeAI && player->m_isAIPlayer)
        {
            continue;
        }

        if (player->m_id != id)
        {
            continue;
        }

        return player;
    }

    return nullptr;
}

ServerPlayer* ServerPlayerManager::GetSpectator(uint64_t id)
{
    for (const auto& player : m_spectators)
    {
        if (player->m_isAIPlayer)
        {
            continue;
        }

        if (player->m_id != id)
        {
            continue;
        }

        return player;
    }

    return nullptr;
}
}
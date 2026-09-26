#pragma once

#include <SDK/TypeInfo.h>
#include <SDK/SDK.h>

#include <Hook/Func.h>

namespace Kyber
{

TL_DECLARE_FUNC(0x1445049A0, void, ServerMapSequencer_LoadNextLevel, NextLevelInfo a1);
TL_DECLARE_FUNC(0x143363EE0, __int64, Settings_LookupObject, __int64 inst, __int64 typeInfo);

TL_DECLARE_FUNC(0x145437E50, void, LevelSetup_ctor, LevelSetup* levelSetup);
TL_DECLARE_FUNC(0x143A34B70, void, LevelSetup_setInclusionOption, LevelSetup*, char* category, char* mode);
TL_DECLARE_FUNC(0x145438BE0, void, LevelSetup_setEqual, LevelSetup* m_setup, LevelSetup* setup_2);
TL_DECLARE_FUNC(0x143C44820, void, ServerLoadLevelMessage_post, LevelSetup* levelSetup, bool fadeOut, bool forceReloadResources);

TL_DECLARE_FUNC(0x143C7F140, void, ServerConnection_KickPlayer, __int64 inst, __int64 reason, const std::string& reasonText);
TL_DECLARE_FUNC(0x143CBE6F0, void, ServerConnection_SetPlayerTeam, ServerPlayer* player, int teamId);
TL_DECLARE_FUNC(0x143CD4180, __int64, ServerPeer_ConnectionForPlayer, __int64 inst, ServerPlayer* player);

TL_DECLARE_FUNC(0x143CB0C50, ServerPlayer*, ServerPlayerManager_createPlayer, ServerPlayerManager* playerManager, uint64_t playerId, uint32_t localPlayerId, uint32_t connectionId, bool isSpectator);
TL_DECLARE_FUNC(0x143CAE830, void, ServerPlayerManager_addPlayer, ServerPlayerManager* playerManager, ServerPlayer* player, const char* nickname);
TL_DECLARE_FUNC(0x143CB1CB0, void, ServerPlayerManager_deletePlayer, ServerPlayerManager* playerManager, ServerPlayer* player);

} // namespace Kyber

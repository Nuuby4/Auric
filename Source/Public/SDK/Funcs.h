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

} // namespace Kyber

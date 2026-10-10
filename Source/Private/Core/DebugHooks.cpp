// Copyright Nuuby. All Rights Reserved.

#include <Core/DebugHooks.h>
#include <Hook/Func.h>

namespace Kyber
{
TL_DECLARE_FUNC(0x1433151D0, DebugRenderer*, DebugRenderer_GetCurrent);
TL_DECLARE_FUNC(0x143317710, void, DebugRenderer_draw2dText, DebugRenderer* inst, int x, int y, const char* text, Color32 color, float scale);

DebugRenderer* DebugRenderer::Get()
{ 
    return DebugRenderer_GetCurrent();
}

void DebugRenderer::Draw2dText(int x, int y, Color32 color, const std::string& text, float scale)
{
    DebugRenderer_draw2dText(Get(), x, y, text.c_str(), color, scale);
}
}
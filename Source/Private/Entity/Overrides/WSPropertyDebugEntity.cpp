// Copyright Nuuby. All Rights Reserved.

#include <Entity/Overrides/WSPropertyDebugEntity.h>
#include <Entity/EntityManager.h>
#include <Core/DebugHooks.h>

#include <Core/Program.h>
#include <Base/Log.h>
#include <cstdint>
#include <string>
#include <map>

namespace Kyber
{
AU_IMPLEMENT_ENTITY_OVERRIDE(WSPropertyDebugEntity, WSPropertyDebugEntityData);

WSPropertyDebugEntity::WSPropertyDebugEntity(EntityManager* entityManager, NativeEntity* entity, WSPropertyDebugEntityData* data)
    : AuricEntity(entity, data)
    , m_showOnScreen(data->ShowOnScreen)
{
    // Disabled because it's not very useful
    SetWantUpdates(false);
}

void WSPropertyDebugEntity::Update(const void* params)
{
    if (!m_showOnScreen)
    {
        return;
    }

    auto* data = GetData();
    for (PropertyDebugInput input : data->Inputs)
    {
        std::string str = "";
        std::string prefix = std::string(input.NameInternal) + " | ";
        bool hasValue = false;

        switch (input.TypeClass)
        {
        case PropertyDebugTypeClass_String:
        {
            PropertyReader<char*> stringValue = GetFieldReader<char*>(input.NameInternal);
            if (stringValue.HasConnection())
            {
                hasValue = stringValue.HasConnectionValue();

                if (hasValue)
                {
                    str = StringUtils::Format("%s %s", prefix, stringValue.Get());
                }
            }

            break;
        }
        case PropertyDebugTypeClass_Float: 
        {
            PropertyReader<float> floatValue = GetFieldReader<float>(input.NameInternal);
            if (floatValue.HasConnection())
            {
                hasValue = floatValue.HasConnectionValue();

                if (hasValue)
                {
                    str = StringUtils::Format("%s %.3f", prefix, floatValue.Get());
                }
            }

            break;
        }
        case PropertyDebugTypeClass_Vec2:
        {
            PropertyReader<Vec2> vec2Value = GetFieldReader<Vec2>(input.NameInternal);
            if (vec2Value.HasConnection())
            {
                hasValue = vec2Value.HasConnectionValue();

                if (hasValue)
                {
                    Vec2 v = vec2Value.Get();
                    str = StringUtils::Format("%s %.3f, %.3f", prefix, v.x, v.y);
                }
            }

            break;
        }
        case PropertyDebugTypeClass_Vec3: 
        {
            PropertyReader<Vec3> vec3Value = GetFieldReader<Vec3>(input.NameInternal);
            if (vec3Value.HasConnection())
            {
                hasValue = vec3Value.HasConnectionValue();

                if (hasValue)
                {
                    Vec3 v = vec3Value.Get();
                    str = StringUtils::Format("%s %.3f, %.3f, %.3f", prefix, v.x, v.y, v.z);
                }

                break;
            }
        }
        case PropertyDebugTypeClass_Vec4: 
        {
            PropertyReader<Vec4> vec4Value = GetFieldReader<Vec4>(input.NameInternal);
            if (vec4Value.HasConnection())
            {
                hasValue = vec4Value.HasConnectionValue();

                if (hasValue)
                {
                    Vec4 v = vec4Value.Get();
                    str = StringUtils::Format("%s %.3f, %.3f, %.3f %.3f", prefix, v.x, v.y, v.z, v.w);
                }
            }

            break;
        }
        case PropertyDebugTypeClass_Uint64:
        {
            PropertyReader<uint64_t> uint64Value = GetFieldReader<uint64_t>(input.NameInternal);
            if (uint64Value.HasConnection())
            {
                hasValue = uint64Value.HasConnectionValue();

                if (hasValue)
                {
                    str = StringUtils::Format("%s %llu", prefix, uint64Value.Get());
                }
            }
            break;
        }
        case PropertyDebugTypeClass_Uint32:
        {
            PropertyReader<uint32_t> uint32Value = GetFieldReader<uint32_t>(input.NameInternal);
            if (uint32Value.HasConnection())
            {
                hasValue = uint32Value.HasConnectionValue();

                if (hasValue)
                {
                    str = StringUtils::Format("%s %u", prefix, uint32Value.Get());
                }
            }
            break;
        }
        case PropertyDebugTypeClass_LinearTransform:
            str = StringUtils::Format("Object Not Implemented");
            break;
        case PropertyDebugTypeClass_Bool:
        {
            PropertyReader<bool> boolValue = GetFieldReader<bool>(input.NameInternal);
            if (boolValue.HasConnection())
            {
                hasValue = boolValue.HasConnectionValue();

                if (hasValue)
                {
                    StringUtils::Format("%s %s", prefix, boolValue.Get() ? "true" : "false");
                }
            }
            break;
        }
        case PropertyDebugTypeClass_Int: 
        {
            PropertyReader<int32_t> int32Value = GetFieldReader<int32_t>(input.NameInternal);
            if (int32Value.HasConnection())
            {
                hasValue = int32Value.HasConnectionValue();

                if (hasValue)
                {
                    str = StringUtils::Format("%s %i", prefix, int32Value.Get());
                }
            }
            break;
        }
        case PropertyDebugTypeClass_Enum:
            str = StringUtils::Format("Enum Not Implemented");
            break;
        case PropertyDebugTypeClass_Object:
            str = StringUtils::Format("Object Not Implemented");
            break;
        default:
            return;
        }

        m_str = str;

        if (m_str.empty())
        {
            return;
        }

        Vec3 color(0, 0, 0);
        DebugRenderer::Draw2dText(1620, 100 + (20 * input.Index), Color32(color.x * 255, color.y * 255, color.z * 255, 200), m_str, 1.0);
    }
}
} // namespace Kyber

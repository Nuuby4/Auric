#pragma once

#define TL_LINE_NUMBER() __LINE__
#define TL_CONCAT(a, b) a##b
#define TL_EXPAND_CONCAT(a, b) TL_CONCAT(a, b)
#define TL_DECLARE_FUNC(offset, returnType, name, ...)                                                                                     \
    namespace TL_EXPAND_CONCAT(__tlFunc__, TL_LINE_NUMBER())                                                                               \
    {                                                                                                                                      \
    typedef returnType(__fastcall* name##_t)(__VA_ARGS__);                                                                                 \
    }                                                                                                                                      \
    static TL_EXPAND_CONCAT(__tlFunc__, TL_LINE_NUMBER())::name##_t name =                                                                 \
        reinterpret_cast<TL_EXPAND_CONCAT(__tlFunc__, TL_LINE_NUMBER())::name##_t>(offset);

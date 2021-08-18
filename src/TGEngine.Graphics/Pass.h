#pragma once

#include "Core/NonCopyable.h"

#include "Graphics.h"

TG_NAMESPACE_BEGIN

class Pass :
    private NonCopyable
{
public:
    bool _enableZTest;
    bool _enableZWrite;
    bool _enableLight;
    CullMode _cullMode = CullMode::Back;
    BlendMode _srcBlendMode = BlendMode::One;
    BlendMode _destBlendMode = BlendMode::One;
};

}

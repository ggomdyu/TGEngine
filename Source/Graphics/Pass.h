#pragma once

#include "Core/NonCopyable.h"

#include "Graphics.h"

namespace tg
{

class Pass :
    private NonCopyable
{
/**@section Constructor */
public:
    bool m_enableZTest;
    bool m_enableZWrite;
    bool m_enableLight;
    CullMode m_cullMode = CullMode::Back;
    BlendMode m_srcBlendMode = BlendMode::One;
    BlendMode m_destBlendMode = BlendMode::One;
};

}

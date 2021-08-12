#pragma once

#include "Math/Vector3.h"
#include "Math/Vector2.h"
#include "Math/Color.h"

namespace tg
{

struct V3F_C2F
{
    Vector3 position;
    Vector3 color;
};

struct V3F_T2F
{
    Vector3 position;
    Vector2 uv;
};

struct V3F_C4F_T2F
{
    Vector3 position;
    Color color;
    Vector2 uv;
};

struct V3F_T2F_Quad
{
    V3F_T2F tl;
    V3F_T2F tr;
    V3F_T2F bl;
    V3F_T2F br;
};

struct V3F_N3F_T2F
{
    Vector3 position;
    Vector3 normal;
    Vector2 uv;
};

}

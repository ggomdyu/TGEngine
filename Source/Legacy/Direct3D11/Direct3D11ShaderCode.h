/**
 * @filename    D3D11ShaderCode.h
 * @author      ggomdyu
 * @since       02/06/2018
 */

#pragma once

constexpr const char g_positionColorVert[] =
"                                                           \n\
cbuffer MatrixBuffer                                        \n\
{                                                           \n\
    matrix g_matWorld;                                      \n\
    matrix g_matVP;                                         \n\
};                                                          \n\
                                                            \n\
struct VS_INPUT                                             \n\
{                                                           \n\
    float4 position : POSITION;                             \n\
    float4 color : COLOR;                                   \n\
};                                                          \n\
                                                            \n\
struct VS_OUTPUT                                            \n\
{                                                           \n\
    float4 position : SV_POSITION;                          \n\
    float4 color : COLOR;                                   \n\
};                                                          \n\
                                                            \n\
VS_OUTPUT main()                                            \n\
{                                                           \n\
    VS_OUTPUT output;                                       \n\
    output.
}                                                           \n\
";

constexpr const char g_positionColorFrag[] =
"                                                           \n\
#version 330 core                                           \n\
                                                            \n\
in vec4 g_fColor;                                           \n\
out vec4 g_outColor;                                        \n\
                                                            \n\
void main()                                                 \n\
{                                                           \n\
    g_outColor = g_fColor;                                  \n\
}                                                           \n\
";

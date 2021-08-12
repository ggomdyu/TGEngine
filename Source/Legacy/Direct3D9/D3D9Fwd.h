/*
* Author : Cha Junho
* Date : 05/17/2016
* Latest author :
* Latest date : 
*/


#pragma once
#include "../../../Core/Platform/Windows/Misc/shared_comptr.h"


struct IDirect3D9Ex;
struct IDirect3DDevice9Ex;
struct IDirect3DVertexBuffer9;
struct IDirect3DTexture9;
struct ID3DXSprite;
struct ID3DXEffect;
struct ID3DXMesh;


namespace tgon
{


using SpD3D9Ex = shared_comptr<IDirect3D9Ex>;
using SpD3D9DeviceEx = shared_comptr<IDirect3DDevice9Ex>;
using SpD3D9VertexBuffer = shared_comptr<IDirect3DVertexBuffer9>;
using SpD3D9Texture = shared_comptr<IDirect3DTexture9>;
using SpD3DXSprite = shared_comptr<ID3DXSprite>;
using SpD3DXEffect = shared_comptr<ID3DXEffect>;
using SpD3DXMesh = shared_comptr<ID3DXMesh>;


}
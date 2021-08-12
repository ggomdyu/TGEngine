/*
* Author : Cha Junho
* Date : 01/17/2016
* Latest author :
* Latest date :
*/

#pragma once
#include <d3dx9.h>

#include "../GraphicsType.h"


namespace tgon
{
	
	UINT GetD3D9ColorChannelBits( const D3DFORMAT );
	
	UINT GetD3D9DepthBits( const D3DFORMAT );
	
	UINT GetD3D9StencilBits( const D3DFORMAT );

	//D3DPRIMITIVETYPE ConvertPrimitiveToD3D9( PrimitiveType from );
}
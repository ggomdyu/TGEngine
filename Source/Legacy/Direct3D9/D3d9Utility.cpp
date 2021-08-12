#include "PrecompiledHeader.h"
#include "D3d9Utility.h"


//
//SpD3d9Effect tgon::LoadShader( const SpD3d9DeviceEx& device, const wchar_t* shaderPath )
//{
//	SpD3d9Effect retShader;
//	LPD3DXBUFFER errBuffer;
//	DWORD shaderFlag = 0;
//
//#if defined( _DEBUG ) | defined( DEBUG )
//	shaderFlag |= D3DXSHADER_DEBUG;
//#endif
//
//	HRESULT hr = D3DXCreateEffectFromFileW( device, shaderPath,
//			nullptr,		// #define definition to use when compiling the shader
//			nullptr,		// ?
//			shaderFlag,
//			nullptr,		// ?
//			&retShader,
//			&errBuffer );
//
//	if ( FAILED( hr ))
//	{
//		std::wstring errString = L"DX Invoke Error : Failed to load shader. [";
//		errString += shaderPath;
//		errString += L']';
//
//		MessageBoxW( GetFocus( ),
//					 errString.c_str( ),
//					 L"MB_OK",
//					 MB_OK );
//	}
//
//	if ( errBuffer && !retShader )
//	{
//		//int32_t size = errBuffer->GetBufferSize( );
//		void* errMsg = errBuffer->GetBufferPointer( );
//
//		if ( errMsg )
//		{
//			MessageBoxA( GetFocus( ),
//						 static_cast<const char*>( errMsg ),
//						 "WARNING!",
//						 MB_OK );
//		}
//	}
//
//	return retShader;
//}
//
//SpD3d9Mesh tgon::LoadMesh( const SpD3d9DeviceEx& device, const wchar_t* meshPath )
//{
//	SpD3d9Mesh spMesh;
//	HRESULT result;
//	
//	V( D3DXLoadMeshFromXW( meshPath,
//				D3DXMESH_SYSTEMMEM,
//				device,
//				nullptr,		// I think it's useless
//				nullptr,		// Material
//				nullptr,		// Effect instance?
//				nullptr,		// Material count
//				&spMesh
//			));
//
//	if ( FAILED( result ))
//	{
//		DxTraceW( __FILEW__, __LINE__, result, nullptr, true );
//	}
//
//	return spMesh;
//}
//
//
//SpD3d9Texture tgon::LoadTexture( const SpD3d9DeviceEx& device, const wchar_t* texturePath )
//{
//	SpD3d9Texture spTexture;
//	HRESULT result = D3DXCreateTextureFromFileW( device, texturePath, &spTexture );
//
//	if ( FAILED( result ))
//	{
//		DxTraceW( __FILEW__, __LINE__, result, nullptr, true );
//	}
//
//	return spTexture;
//}


UINT tgon::GetD3D9ColorChannelBits( const D3DFORMAT fmt )
{
	switch ( fmt )
	{
	case D3DFMT_R8G8B8:
		return 8;
	case D3DFMT_A8R8G8B8:
		return 8;
	case D3DFMT_X8R8G8B8:
		return 8;
	case D3DFMT_R5G6B5:
		return 5;
	case D3DFMT_X1R5G5B5:
		return 5;
	case D3DFMT_A1R5G5B5:
		return 5;
	case D3DFMT_A4R4G4B4:
		return 4;
	case D3DFMT_R3G3B2:
		return 2;
	case D3DFMT_A8R3G3B2:
		return 2;
	case D3DFMT_X4R4G4B4:
		return 4;
	case D3DFMT_A2B10G10R10:
		return 10;
	case D3DFMT_A8B8G8R8:
		return 8;
	case D3DFMT_A2R10G10B10:
		return 10;
	case D3DFMT_A16B16G16R16:
		return 16;
	default:
		return 0;
	}
}

UINT tgon::GetD3D9DepthBits( const D3DFORMAT fmt )
{
	switch ( fmt )
	{
	case D3DFMT_D32F_LOCKABLE:
	case D3DFMT_D32:
		return 32;

	case D3DFMT_D24X8:
	case D3DFMT_D24S8:
	case D3DFMT_D24X4S4:
	case D3DFMT_D24FS8:
		return 24;

	case D3DFMT_D16_LOCKABLE:
	case D3DFMT_D16:
		return 16;

	case D3DFMT_D15S1:
		return 15;

	default:
		return 0;
	}
}

UINT tgon::GetD3D9StencilBits( const D3DFORMAT fmt )
{
    switch( fmt )
    {
        case D3DFMT_D16_LOCKABLE:
        case D3DFMT_D16:
        case D3DFMT_D32F_LOCKABLE:
        case D3DFMT_D32:
        case D3DFMT_D24X8:
            return 0;

        case D3DFMT_D15S1:
            return 1;

        case D3DFMT_D24X4S4:
            return 4;

        case D3DFMT_D24S8:
        case D3DFMT_D24FS8:
            return 8;

        default:
            return 0;
    }
}
//
//D3DPRIMITIVETYPE tgon::ConvertPrimitiveToD3D9( PrimitiveType from )
//{
//	switch ( from )
//	{
//	case PrimitiveType::kPointList:
//		return D3DPRIMITIVETYPE::D3DPT_POINTLIST;
//		break;
//	
//	case PrimitiveType::kLineList:
//		return D3DPRIMITIVETYPE::D3DPT_LINELIST;
//		break;
//
//	case PrimitiveType::kLineStrip:
//		return D3DPRIMITIVETYPE::D3DPT_LINESTRIP;
//		break;
//	
//	case PrimitiveType::kTriangleList:
//		return D3DPRIMITIVETYPE::D3DPT_TRIANGLELIST;
//		break;
//
//	case PrimitiveType::kTriangleStrip:
//		return D3DPRIMITIVETYPE::D3DPT_TRIANGLESTRIP;
//		break;
//
//	case PrimitiveType::kTriangleFan:
//		return D3DPRIMITIVETYPE::D3DPT_TRIANGLEFAN;
//		break;
//
//	default:
//		abort( );
//		break;
//	}
//}

//
//
//bool tgon::IsDeviceAcceptable( IDirect3D9Ex* d3dInterface,
//							   D3DCAPS9* pCaps,
//							   D3DFORMAT AdapterFormat,
//							   D3DFORMAT BackBufferFormat )
//{
//	const BOOL isSucceed = d3dInterface->CheckDeviceFormat(
//					pCaps->AdapterOrdinal,
//					pCaps->DeviceType,
//					AdapterFormat,
//					D3DUSAGE_QUERY_POSTPIXELSHADER_BLENDING, // return false backbuffer formats that don't support alpha blending.
//					D3DRTYPE_TEXTURE,
//					BackBufferFormat );
//	
//	return ( isSucceed == S_OK );
//}
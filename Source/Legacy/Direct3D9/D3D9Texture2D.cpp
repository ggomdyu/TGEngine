#include "PrecompiledHeader.h"
#include "D3D9Texture2D.h"

//
////tgon::D3d9Texture::D3d9Texture( const SpTGraphicsDevice& gd, const wchar_t* texturePath ) :
////	m_texture( LoadTexture( gd->GetD3dDevice(), texturePath ))
////{
////	assert( gd.get());
////	assert( texturePath );
////}
////void tgon::D3d9Texture::SetTexture( const SpTGraphicsDevice& gd, const wchar_t* texturePath )
////{
////	assert( gd.get());
////	assert( texturePath );
////
////	m_texture = LoadTexture( gd->GetD3dDevice(), texturePath );
////}
//
//decltype ( tgon::D3D9Texture::ms_texPool ) tgon::D3D9Texture::ms_texPool;
//
//tgon::D3D9Texture::D3D9Texture(
//	const SpTGraphics& grp,
//	const wchar_t* texPath )
//{
//	//const auto findIter = ms_texPool.find( texPath );
//	//if ( findIter == ms_texPool.end( ))
//	//{
//	//	IDirect3DTexture9* texture = nullptr;
//	//	HRESULT hr = D3DXCreateTextureFromFileW( 
//	//		grp->GetD3dDevice( ),
//	//		texPath,
//	//		&texture
//	//	);
//
//	//	if ( FAILED( hr ))
//	//	{
//	//		MessageBox( 
//	//			GetFocus( ),
//	//			L"Failed to invoke D3DXCreateTextureFromFileW.",
//	//			L"WARNING!",
//	//			MB_OK | MB_ICONEXCLAMATION 
//	//		);
//	//	}
//	//}
//}
//
//tgon::D3D9Texture::~D3D9Texture( )
//{
//}

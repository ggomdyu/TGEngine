#include "PrecompiledHeader.h"
#include "D3d9Shader.h"
#include "D3d9Util.h"
//
//
//tgon::D3d9Shader::D3d9Shader( const SpTGraphicsDevice& device, const wchar_t* shaderPath ) :
//	m_shader( LoadShader( device->GetD3dDevice(), shaderPath ))
//{
//	assert( device.get());
//	assert( shaderPath );
//}
//
//
//tgon::D3d9Shader::D3d9Shader( const SpTGraphicsDevice& device, const wchar_t* shaderPath, const wchar_t* texturePath ) :
//	m_shader( LoadShader( device->GetD3dDevice(), shaderPath )),
//	m_texture( device, texturePath )
//{
//	assert( device.get());
//	assert( shaderPath );
//}
//
//void tgon::D3d9Shader::SetTexture( const D3d9Texture& texture )
//{
//}
//
//
//tgon::D3d9Shader::~D3d9Shader( )
//{
//}
//
//
//void tgon::D3d9Shader::BeginDisplay( )
//{
//	//uint32_t numPasses = 0;
//	//GetShader( )->Begin( &numPasses, NULL );
//
//	//for ( uint32_t i = 0; i < numPasses; ++i )
//	//{
//	//	GetShader( )->BeginPass( i );
//	//}
//
//}
//
//
//void tgon::D3d9Shader::EndDisplay( )
//{
//	//GetShader( )->EndPass( );
////	GetShader( )->End( );
//}
//
//
//void tgon::D3d9Shader::SetVector( const char* pVectorStr, const D3DXVECTOR4* rVector )
//{
//	this->GetShader( )->SetVector( pVectorStr, rVector );
//}
//
//
//void tgon::D3d9Shader::SetMatrix( const D3d9Camera& camera )
//{
//	HRESULT result;
//
//	V( GetShader( )->SetMatrix( "gWorldMatrix", &camera.GetWorld( )));
//	V( GetShader( )->SetMatrix( "gViewMatrix", &camera.GetView( )));
//	V( GetShader( )->SetMatrix( "gProjectionMatrix", &camera.GetProjection( )));
//}
/*
* 작성자 : 차준호
* 작성일 : 2016-01-21
* 최종 수정 :
* 최종 수정일 :
*/


#pragma once
//#include "Graphics/TGraphicsDevice.h"
//#include "D3d9Type.h"
//#include "D3d9Texture.h"
//#include "D3d9Camera.h"
//
//namespace tgon {
//
//
//class D3d9Shader;
//typedef D3d9Shader ShaderImpl;
//
//
//class D3d9Shader
//{
//public:
//	explicit D3d9Shader( const SpTGraphicsDevice&, const wchar_t* shaderPath );
//	explicit D3d9Shader( const SpTGraphicsDevice&, const wchar_t* shaderPath,
//							const wchar_t* texturePath );
//	virtual ~D3d9Shader( );
//
//
//public:
//	void BeginDisplay( );
//	void EndDisplay( );
//
//	void SetTexture( const D3d9Texture& );
//	void SetMatrix( const D3d9Camera& );
//	void SetVector( const char* pVectorStr, const D3DXVECTOR4* rVector );
//
//public:
//	const SpD3d9Effect& GetShader( ) const	{ return m_shader; };
//
//
//private:
//	D3d9Texture		m_texture;
//	SpD3d9Effect	m_shader;
//};
//
//
//}
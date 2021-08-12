#include "PrecompiledHeader.h"
#include "ModelScene.h"

//#include "GraphicsManager.h"
//
//#include "D3d9Device.h"
//#include "D3d9Quad.h"
//#include "D3d9Camera.h"
//#include "D3d9Shader.h"
//#include "D3d9Mesh.h"
//
//#include "D3d92DSprite.h"
//
//std::unique_ptr<tgon::D3d9Quad> d3d9Tex;
//std::unique_ptr<tgon::D3d9Camera> d3d9Camera;
//std::unique_ptr<tgon::D3d9Shader> d3d9Shader;
//std::unique_ptr<tgon::D3d9Mesh> d3d9Mesh;
//std::unique_ptr<tgon::D3d9Texture> d3d9TextureDiffuse;
//std::unique_ptr<tgon::D3d9Texture> d3d9TextureSpecular;
//
//std::unique_ptr<tgon::D3d92DSprite> d3d9Sprite;

tgon::ModelScene::ModelScene( )
{
	/*d3d9Shader.reset( new D3d9Shader( GeTGraphics(),
			L"Resources/ColorShader.fx" ));

	d3d9Mesh.reset( new D3d9Mesh( GeTGraphics(),
			L"Resources/sphere.x" ));
			*/
	//d3d9Camera.reset( new D3d9Camera( GeTGraphics()));
///
	//d3d9Sprite.reset( new D3d92DSprite( GeTGraphics( ),
	//							L"D:\\1.PNG" ));
}


tgon::ModelScene::~ModelScene( )
{
}


void tgon::ModelScene::Update( float tickTime )
{
	//d3d9Shader->SetMatrix( *d3d9Camera );

}


void tgon::ModelScene::Render( )
{
	//GeTGraphics( )->BeginDraw( );
	//{
	//	d3d9Sprite->Render( );
	//}
	//GeTGraphics( )->EndDraw( );



	/*uint32_t passNum =0;

	GeTGraphics( )->BeginDraw( );
	{
		UINT numPasses = 0;
		d3d9Shader->GetShader( )->Begin( &numPasses, NULL );
		{
			for ( UINT i = 0; i < numPasses; ++i )
			{
				d3d9Shader->GetShader( )->BeginPass( i );
				{
					d3d9Mesh->GetMesh( )->DrawSubset( 0 );
				}
				d3d9Shader->GetShader( )->EndPass( );
			}
		}
		d3d9Shader->GetShader( )->End( );
	}
	GeTGraphics( )->EndDraw( );*/
}


/*
GeTGraphics( )->BeginDraw( );

	d3d9Shader->SetMatrix( *d3d9Camera.get( ));
	
	// Specular light
	d3d9Shader->SetVector( "gWorldLightPosition", &gWorldLightPosition );
	d3d9Shader->SetVector( "gWorldCameraPosition", &gWorldCameraPosition );

	// Specular mapping
	D3DXVECTOR4 gLightColor( 0.7f, 0.7f, 1.0f, 1.0f );
	d3d9Shader->SetVector( "gLightColor", &gLightColor );
	d3d9Shader->GetShader( )->SetTexture( "DiffuseMap_Tex", d3d9TextureDiffuse->GetTexture( ));
	d3d9Shader->GetShader( )->SetTexture( "SpecularMap_Tex", d3d9TextureSpecular->GetTexture( ));

	static float f = 0.0f;
	f += 0.01f;
	
	D3DXMATRIX matrix;
	D3DXMatrixIdentity( &matrix );
//	D3DXMatrixRotationX( &matrix, f );

//	GeTGraphics( )->GetD3dDevice( )->SetTransform( D3DTS_WORLD, &matrix );


	UINT numPasses = 0;
	d3d9Shader->GetShader( )->Begin( &numPasses, NULL );
	{
		for ( UINT i = 0; i < numPasses; ++i )
		{
			d3d9Shader->GetShader( )->BeginPass( i );
			{
				d3d9Mesh->GetMesh( )->DrawSubset( 0 );
			}
			d3d9Shader->GetShader( )->EndPass( );
		}
	}
	d3d9Shader->GetShader( )->End( );


	GeTGraphics( )->EndDraw( );


*/
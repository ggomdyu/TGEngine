/*
* Author : Kim Taewoo
* Date : 04/26/2015
* Latest author : Cha Junho
* Latest date : 01/17/2016
*/


#pragma once
#include "../Abstract/AbstractGraphicsDevice.h"

#include <d3d9.h>
#include "D3D9Fwd.h"


namespace tgon
{


class D3D9GraphicsDevice;
using GraphicsDeviceImpl = D3D9GraphicsDevice;

class TGON_API D3D9GraphicsDevice : 
	public AbstractGraphicsDevice
{
protected:
	/*
		Ctor/Dtor
	*/
	explicit D3D9GraphicsDevice( /*In*/ TPlatformWindow* deviceWindow, /*In*/ const GraphicsProperty& grpProp );
	
	D3D9GraphicsDevice( /*In*/ const D3D9GraphicsDevice& ) = delete;

	virtual ~D3D9GraphicsDevice( ) = default;

	/*
		Operators
	*/
	D3D9GraphicsDevice& operator=( const D3D9GraphicsDevice& ) = delete;


public:
	/*
		Command
	*/
	// Begin scene rendering. Return true if device can render.
	virtual bool BeginScene( ) override;
	
	// End scene rendering. This function must be called after BeginScene
	virtual bool EndScene( ) override;
	
	// Clear the back buffer scene.
	virtual bool Clear( ) override;

	// Draw the back buffer to visible screen.
	virtual bool Present( ) override;

	//
	virtual void DrawPrimitive( PrimitiveType primitiveType, uint32_t startVertex, uint32_t primitiveCount ) override;


	/*
		Sets
	*/
	virtual void SetCullMode( TCullMode cullMode ) override;
	
	virtual void EnableDepthBuffer( bool isEnable ) override;
	

	/*
		Gets
	*/
	const SpD3D9Ex& GetD3D( ) const;

	const SpD3D9DeviceEx& GetD3DDevice( ) const;
	

private:
	/*
		Internal works
	*/

	// Initialize IDirect3D9Ex and Caps
	void InitD3DInterface( );

	// Initialize IDirect3DDevice9Ex
	void InitD3DDevice( const GraphicsProperty& grpProp );


private:
	shared_comptr<IDirect3D9Ex> m_d3d;

	shared_comptr<IDirect3DDevice9Ex> m_d3dDevice;

	std::unique_ptr<D3DCAPS9> m_deviceCaps;
};


inline const SpD3D9Ex& D3D9GraphicsDevice::GetD3D( ) const
{
	return m_d3d;
}

inline const SpD3D9DeviceEx& D3D9GraphicsDevice::GetD3DDevice( ) const
{
	return m_d3dDevice;
}

inline void D3D9GraphicsDevice::DrawPrimitive( PrimitiveType primitiveType, uint32_t startVertex, uint32_t primitiveCount )
{
	m_d3dDevice->DrawPrimitive(
		static_cast<D3DPRIMITIVETYPE>( primitiveType ),
		startVertex,
		primitiveCount
	);
}

inline void D3D9GraphicsDevice::SetCullMode( TCullMode cullMode )
{
	m_currMode = m_currMode;

	if ( cullMode == TCullMode::kCW )
	{
		m_d3dDevice->SetRenderState( D3DRENDERSTATETYPE::D3DRS_CULLMODE, D3DCULL::D3DCULL_CW );
	}
	else if ( cullMode == TCullMode::kCCW )
	{
		m_d3dDevice->SetRenderState( D3DRENDERSTATETYPE::D3DRS_CULLMODE, D3DCULL::D3DCULL_CCW );
	}
	else if ( cullMode == TCullMode::kNone )
	{
		m_d3dDevice->SetRenderState( D3DRENDERSTATETYPE::D3DRS_CULLMODE, D3DCULL::D3DCULL_NONE );
	}
}

inline void tgon::D3D9GraphicsDevice::EnableDepthBuffer( bool isEnable )
{
	m_d3dDevice->SetRenderState( D3DRS_ZENABLE, ( isEnable ) ? D3DZB_TRUE : D3DZB_FALSE );
}


}

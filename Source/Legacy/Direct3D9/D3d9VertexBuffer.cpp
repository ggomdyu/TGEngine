#include "PrecompiledHeader.h"
#include "D3D9VertexBuffer.h"


tgon::D3D9VertexBuffer::D3D9VertexBuffer( int32_t vertexSize, int32_t vertexCount, bool isDynamic ) :
	m_usage( 0 ),
	m_lockFlag( 0 ),
	m_pool( D3DPOOL::D3DPOOL_MANAGED )
{
	if ( isDynamic )
	{
		m_pool = D3DPOOL::D3DPOOL_DEFAULT;
		m_usage |= D3DUSAGE_DYNAMIC;
		m_lockFlag |= D3DLOCK_DISCARD;
	}
}

tgon::D3D9VertexBuffer::~D3D9VertexBuffer( )
{
	if ( m_vertexBuffer )
	{
		m_vertexBuffer->Release( );
	}
}

void* tgon::D3D9VertexBuffer::Lock( int32_t lockOffset, int32_t lockSize, bool isDiscard )
{
	DWORD flag = 0;
	
	if ( isDiscard )
	{
		flag |= D3DLOCK_DISCARD;
	}

	void* vertices = nullptr;
	m_vertexBuffer->Lock( lockOffset, lockSize, &vertices, 0 );

	return vertices;
}

bool tgon::D3D9VertexBuffer::Unlock( )
{
	return SUCCEEDED( m_vertexBuffer->Unlock( ));
}



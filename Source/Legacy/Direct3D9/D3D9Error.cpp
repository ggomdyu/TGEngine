#include "PrecompiledHeader.h"
#include "D3d9Error.h"


tgon::ErrorStrings<tgon::D3D9ErrorCode>::D3D9ErrorStringsProxy tgon::ErrorStrings<tgon::D3D9ErrorCode>::m_errStrings;


namespace tgon {
namespace 
{


void AssembleErrorString( 
	_Out_ wchar_t* outErrorString,
	LPCWSTR functionName,
	LPCWSTR fileNameOfCode,
	UINT numLine, 
	HRESULT result )
{
	assert( fileNameOfCode );
	assert( numLine >= 0 );
	assert( result != S_OK );


	wsprintf( outErrorString, 
L"An error has occurred [%s]\n\
[Function] : %s\n\
[File] : %s\n\
[Line] : %d lines", 
		GetErrorString( static_cast<D3D9ErrorCode>( result )),
		functionName, 
		fileNameOfCode, 
		numLine );

}

}
}


void tgon::DxTraceW(
	LPCWSTR functionName,
	LPCWSTR fileNameOfCode,
	UINT numLine,
	HRESULT result,
	bool doPopup )
{
#if defined( DEBUG ) | defined( _DEBUG )
	wchar_t errString[512] {0};
	AssembleErrorString( errString, functionName, fileNameOfCode, numLine, result );

	if ( doPopup )
	{
		MessageBoxW( GetFocus( ), errString, L"WARNING!",
			MB_OK | MB_ICONEXCLAMATION );
	}
	else
	{
		OutputDebugStringW( errString );
	}
#endif
}
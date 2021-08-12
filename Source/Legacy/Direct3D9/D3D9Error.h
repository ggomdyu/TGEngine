/*
* Author : Cha Junho
* Date : 05/08/2016
* Latest author :
* Latest date :
*/


#pragma once
#include "./Core/Platform/OSAL/PlatformInclude.h"
#include "./Core/Error/ErrorCode.h"
#include <d3d9.h>


namespace tgon
{


enum class D3D9ErrorCode : HRESULT
{
	kOk = S_OK,
	kFalse = S_FALSE,
	kOutOfVideoMemory = D3DERR_OUTOFVIDEOMEMORY,
	kWrongTextureFormat = D3DERR_WRONGTEXTUREFORMAT,
	kNotFound = D3DERR_NOTFOUND,
	kInvalidCall = D3DERR_INVALIDCALL,
	kInvalidDevice = D3DERR_INVALIDDEVICE,
	kMoreData = D3DERR_MOREDATA,
	kDeviceLost = D3DERR_DEVICELOST,
	kDeviceNotReset = D3DERR_DEVICENOTRESET,
	kNotAvailable = D3DERR_NOTAVAILABLE,
	kDeviceRemoved = D3DERR_DEVICEREMOVED,
	kUnsupportedColorOperation = D3DERR_UNSUPPORTEDCOLOROPERATION,
	kUnsupportedColorArg = D3DERR_UNSUPPORTEDCOLORARG,
	kUnsupportedAlphaArg = D3DERR_UNSUPPORTEDALPHAARG,
	kTooManyOperations = D3DERR_TOOMANYOPERATIONS,
	kConflictingTextureFilter = D3DERR_CONFLICTINGTEXTUREFILTER,
	kUnsupportedTextureFilter = D3DERR_UNSUPPORTEDTEXTUREFILTER,
	kUnsupportedFactorValue = D3DERR_UNSUPPORTEDFACTORVALUE,
	kConflictingRenderState = D3DERR_CONFLICTINGRENDERSTATE,
	kConflictingTexturePalette = D3DERR_CONFLICTINGTEXTUREPALETTE,
	kDriverInternalError = D3DERR_DRIVERINTERNALERROR,
	kDriverInvalidCall = D3DERR_DRIVERINVALIDCALL,
	kWasStillDrawing = D3DERR_WASSTILLDRAWING,
	kDeviceHung = D3DERR_DEVICEHUNG,
	kUnsupportedOverlay = D3DERR_UNSUPPORTEDOVERLAY,
	kUnsupportedOverlayFormat = D3DERR_UNSUPPORTEDOVERLAYFORMAT,
	kCannotProtectContent = D3DERR_CANNOTPROTECTCONTENT,
	kUnsupportedCrypto = D3DERR_UNSUPPORTEDCRYPTO,
	kPresentStatisticsDisjoint = D3DERR_PRESENT_STATISTICS_DISJOINT,
};


template<>
class ErrorStrings<D3D9ErrorCode>
{
	using _UTy = typename std::underlying_type<D3D9ErrorCode>::type;

	struct D3D9ErrorStringsProxy
	{
		D3D9ErrorStringsProxy( )
		{
			RegisterErrorString( S_OK, L"S_OK" );
			RegisterErrorString( S_FALSE, L"S_FALSE" );
			RegisterErrorString( D3DERR_INVALIDCALL, L"D3DERR_INVALIDCALL" );
			RegisterErrorString( D3DERR_OUTOFVIDEOMEMORY, L"D3DERR_OUTOFVIDEOMEMORY" );
			RegisterErrorString( D3DERR_WRONGTEXTUREFORMAT, L"D3DERR_WRONGTEXTUREFORMAT" );
			RegisterErrorString( D3DERR_NOTFOUND, L"D3DERR_NOTFOUND" );
			RegisterErrorString( D3DERR_INVALIDDEVICE, L"D3DERR_INVALIDDEVICE" );
			RegisterErrorString( D3DERR_MOREDATA, L"D3DERR_MOREDATA" );
			RegisterErrorString( D3DERR_DEVICELOST, L"D3DERR_DEVICELOST" );
			RegisterErrorString( D3DERR_DEVICENOTRESET, L"D3DERR_DEVICENOTRESET" );
			RegisterErrorString( D3DERR_NOTAVAILABLE, L"D3DERR_NOTAVAILABLE" );
			RegisterErrorString( D3DERR_DEVICEREMOVED, L"D3DERR_DEVICEREMOVED" );
			RegisterErrorString( D3DERR_UNSUPPORTEDCOLOROPERATION, L"D3DERR_UNSUPPORTEDCOLOROPERATION" );
			RegisterErrorString( D3DERR_UNSUPPORTEDCOLORARG, L"D3DERR_UNSUPPORTEDCOLORARG" );
			RegisterErrorString( D3DERR_UNSUPPORTEDALPHAARG, L"D3DERR_UNSUPPORTEDALPHAARG" );
			RegisterErrorString( D3DERR_TOOMANYOPERATIONS, L"D3DERR_TOOMANYOPERATIONS" );
			RegisterErrorString( D3DERR_CONFLICTINGTEXTUREFILTER, L"D3DERR_CONFLICTINGTEXTUREFILTER" );
			RegisterErrorString( D3DERR_UNSUPPORTEDFACTORVALUE, L"D3DERR_UNSUPPORTEDFACTORVALUE" );
			RegisterErrorString( D3DERR_CONFLICTINGRENDERSTATE, L"D3DERR_CONFLICTINGRENDERSTATE" );
			RegisterErrorString( D3DERR_UNSUPPORTEDTEXTUREFILTER, L"D3DERR_UNSUPPORTEDTEXTUREFILTER" );
			RegisterErrorString( D3DERR_CONFLICTINGTEXTUREPALETTE, L"D3DERR_CONFLICTINGTEXTUREPALETTE" );
			RegisterErrorString( D3DERR_DRIVERINTERNALERROR, L"D3DERR_DRIVERINTERNALERROR" );
			RegisterErrorString( D3DERR_DRIVERINVALIDCALL, L"D3DERR_DRIVERINVALIDCALL" );
			RegisterErrorString( D3DERR_WASSTILLDRAWING, L"D3DERR_WASSTILLDRAWING" );
			RegisterErrorString( D3DERR_DEVICEHUNG, L"D3DERR_DEVICEHUNG" );
			RegisterErrorString( D3DERR_UNSUPPORTEDOVERLAY, L"D3DERR_UNSUPPORTEDOVERLAY" );
			RegisterErrorString( D3DERR_UNSUPPORTEDOVERLAYFORMAT, L"D3DERR_UNSUPPORTEDOVERLAYFORMAT" );
			RegisterErrorString( D3DERR_CANNOTPROTECTCONTENT, L"D3DERR_CANNOTPROTECTCONTENT" );
			RegisterErrorString( D3DERR_UNSUPPORTEDCRYPTO, L"D3DERR_UNSUPPORTEDCRYPTO" );
			RegisterErrorString( D3DERR_PRESENT_STATISTICS_DISJOINT, L"D3DERR_PRESENT_STATISTICS_DISJOINT" );
		}		
		const wchar_t* operator[]( _UTy errCode )
		{
			return m_errStrings[errCode];
		}
	private:
		void RegisterErrorString( _UTy errCode, const wchar_t* errString )
		{
			// Is errCode duplicated?
			if ( m_errStrings.find( errCode ) != m_errStrings.end( ))
			{
				//TConsole::WriteLine( L"A error code has duplicated. [", errString, L"]" );
			}
			// Otherwise, insert the error string
			else
			{
				m_errStrings[errCode] = errString;
			}
		};
	private:
		std::unordered_map<_UTy, const wchar_t*> m_errStrings;
	};

public:
	static D3D9ErrorStringsProxy m_errStrings;
};


// Show Direct3D Error that contains debug info
// ( Calling this function directly is not recommended. Use the V or V_RETURN macro. )
void DxTraceW( LPCWSTR functionName, LPCWSTR fileNameOfCode, UINT numLine, HRESULT result, bool doPopup );



}


#if defined( DEBUG ) || defined( _DEBUG )
#	ifndef V
#		define V(x) {{ HRESULT result = (x); if ( FAILED( result )) { tgon::DxTraceW( __FUNCTIONW__, __FILEW__, __LINE__, result, true ); }}}
#	endif
#	ifndef V_RETURN
#		define V_RETURN(x) {{ HRESULT result = (x); if ( FAILED( result )) { tgon::DxTraceW( __FUNCTIONW__, __FILEW__, __LINE__, result, true ); return result; }}}
#	endif
#else
#	ifndef V
#		define V(x)
#	endif
#	ifndef V_RETURN
#		define V_RETURN(x) {{ HRESULT result = (x); if ( FAILED( result )) { return result; }}}
#	endif
#endif
#include "PrecompiledHeader.pch"
#include "MiniDump.h"

//#include <DbgHelp.h>

void tgon::SetMiniDump( )
{
	static bool isAlreadyDumped = false;
	if ( isAlreadyDumped )
	{
		return;
	}

	isAlreadyDumped = true;
/*
	MINIDUMP_EXCEPTION_INFORMATION exInfo;
	exceptInfo.ThreadId = GetCurrentThreadId( );
	exceptInfo.ExceptionPointers = ( EXCEPTION_POINTERS* )exceptionPointers;
	exceptInfo.ClientPointers = TRUE;
*/
}

#ifndef DX_MACROS_H
#define DX_MACROS_H

// --------------------------------------------------------
// Convenience macro for releasing COM objects.
//
// Any time you get a reference from the DirectX API, you
// must release that reference.  This macro simplifies that.
// --------------------------------------------------------
#define ReleaseMacro(x) { if(x){ x->Release(); x = 0; } }

// --------------------------------------------------------
// Macro for checking the result of a DirectX function call.  This will
// pop up a message box on a failed result and then quit.  This macro
// depends on the "dxerr" (DirectX Error) helper files.
//
// In release mode this macro effectively does nothing.
// --------------------------------------------------------
#if defined(DEBUG) | defined(_DEBUG)
#ifndef HR
#include "dxerr.h"
#define HR(x)												\
	{															\
		HRESULT hr = (x);										\
		if(FAILED(hr))											\
		{														\
			DXTrace(__FILEW__, (DWORD)__LINE__, hr, L#x, true);	\
			PostQuitMessage(0);									\
		}														\
    }
#endif
#else
#ifndef HR
#define HR(x) (x) // Do nothing special!
#endif
#endif

#endif

#pragma once

/*************************************************************/
/* System includes                                           */
/*************************************************************/

#define _CRTDBG_MAP_ALLOC   // memory leak detection
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <cstdlib>  // wcstombs_s
#include <stdlib.h>  
#include <stdio.h>
#include <crtdbg.h>  
#include <assert.h>
#include <string>
#include <fstream>

#include "Core/Logger.h"


/**************************************************************/
/* Platform specifications                                    */
/**************************************************************/
#if defined( _WIN32 ) || defined ( _WIN64 )

#include <DirectXMath.h>	// XMFLOAT3, XMFLOAT4X4
#include <d3d11.h>

typedef DirectX::XMFLOAT3           VEC3;
typedef DirectX::XMFLOAT4           VEC4;
typedef DirectX::XMFLOAT4X4         VEC4x4;

#else 

// Using other another graphics library


#endif



#define  ENABLE_UI

#if defined( ENABLE_UI )

#include "imgui/imgui.h"
#include "imgui/imgui_impl_win32.h"
#include "imgui/imgui_impl_dx11.h"

#endif

/*
#if defined(_DEBUG)

#define DEBUG_NEW new(_NORMAL_BLOCK, __FILE__, __LINE__)
#define new DEBUG_NEW

#endif

*/

// Replace the new keyword with the debug new 

// Debug Macros


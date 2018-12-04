#pragma once

#include <iostream>

#include <stdlib.h>
#include <stdio.h>
#include <cstdlib>  // wcstombs_s
#define _CRTDBG_MAP_ALLOC  

#include <stdlib.h>  
#include <stdio.h>
#include <crtdbg.h>  
#include <assert.h>
#include <string>
#include <fstream>

#define  ENABLE_UI

#if defined(ENABLE_UI)

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

#include "Core/Logger.h"

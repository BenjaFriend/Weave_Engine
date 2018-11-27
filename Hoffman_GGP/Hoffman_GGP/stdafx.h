#pragma once

#include <iostream>

#include <stdlib.h>
#include <stdio.h>

#define _CRTDBG_MAP_ALLOC  

#include <stdlib.h>  
#include <stdio.h>
#include <crtdbg.h>  
#include <assert.h>
#include <string>

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

#ifdef _DEBUG

#define DEBUG_PRINT(a, ...) printf("%s: %d(): " a "\n", __FILE__, __LINE__, __VA_ARGS__);

#else

#define DEBUG_PRINT(a, ...)

#endif
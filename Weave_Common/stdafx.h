#pragma once

/// <summary>
/// Precompiled header for this application.
/// Contains flags for different options and typedef options
/// for different platforms, and other common application libraries
/// or tools
/// </summary>
/// <author>Ben Hoffman</author>

/*************************************************************/
/* System includes                                           */
/*************************************************************/
#include <iostream>
#include <cstdlib>          // wcstombs_s
#include <stdlib.h>  
#include <stdio.h>
#include <assert.h>
#include <string>
#include <fstream>
#include <tuple>

#include "Logging/Logger.h"    // Logging definitions to only happen in debug
#include "MathHelper.h"     // Math definitions and typedefs

/**************************************************************/
/* Runtime Options                                            */
/**************************************************************/
#define ENABLE_UI   // Flag for if we want ImGui to be included anywhere.
                    // Should only turn off if you cannot use ImGui
#define EDITOR_ON   // Flag for turning the editor on and off

#define MAX_ENTITY_COUNT    32

/**************************************************************/
/* Platform specifications                                    */
/**************************************************************/
#if defined( _WIN32 ) || defined ( _WIN64 )

// Windows uses wide chars for their file names
typedef std::wstring                FileName;
#define FORCE_INLINE                __forceinline
#define W_UNUSED

#else 

// Using other another graphics library
typedef std::string                FileName;
#define FORCE_INLINE                inline
#define W_UNUSED                    __attribute__ ((unused))

#endif

/**************************************************************/
/* Debug Macros                                               */
/**************************************************************/
#if defined( _DEBUG ) || defined ( DEBUG )

#if defined( _WIN32 ) || defined ( _WIN64 )

#define _CRTDBG_MAP_ALLOC   // memory leak detection
#include <crtdbg.h> 

// Debug new macro for tracking exactly where the leaks are
#define DEBUG_NEW new(_NORMAL_BLOCK, __FILE__, __LINE__)

//#define new DEBUG_NEW // This breaks IMGui, but gives you a 
                        // great memory leak detector

#endif  // _WIN32

#else 

// Define DEBUG_NEW as just new
#define DEBUG_NEW new

#endif


/**************************************************************/
/* Util Macros                                                */
/**************************************************************/
#define SAFE_DELETE( obj ) if( obj != nullptr ){ delete obj; obj = nullptr; }  
#pragma once
/// <summary>
/// Provide the math definitions that we will need in the future
/// for cross platform development
/// </summary>
/// <author>Ben Hoffman</author>

#include <inttypes.h>

// GLM Manual: https://github.com/g-truc/glm/blob/master/manual.md
#define GLM_FORCE_INLINE
#define GLM_FORCE_INTRINSICS

// #TODO Experiment with the performance of different precisions
#define GLM_FORCE_PRECISION_LOWP_FLOAT
#define GLM_FORCE_PRECISION_LOWP_DOUBLE
#define GLM_FORCE_PRECISION_LOWP_INT
#define GLM_FORCE_PRECISION_LOWP_UINT
#define GLM_ENABLE_EXPERIMENTAL

#include <glm/glm.hpp>
#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/gtc/quaternion.hpp>

// Windows libraries
#if defined( _WIN32 ) || defined ( _WIN64 )

#include <d3d11.h>          // Any directX functionality
#include <Windows.h>        // Windows callbacks
#include <string>

// Windows uses wide chars for their file names
typedef std::wstring               FileName;

#else 

// Using other another graphics library
typedef std::string                FileName;

// #TODO 
// Glm defintions 

#endif

// Integer typedefs for ease of use
typedef uint8_t                 UINT8;
typedef unsigned short          UINT16;
typedef uint32_t                UINT32;
typedef uint64_t                UINT64;

typedef int8_t                  INT8;
typedef signed short            INT16;
typedef int32_t                 INT32;
typedef int64_t                 INT64;
#pragma once
/// <summary>
/// Provide the math definitions that we will need in the future
/// for cross platform development
/// </summary>
/// <author>Ben Hoffman</author>
#if defined( _WIN32 ) || defined ( _WIN64 )

#include <DirectXMath.h>	// XMFLOAT3, XMFLOAT4X4
#include <d3d11.h>          // Any directX functionality
#include <Windows.h>        // Windows callbacks

typedef DirectX::XMFLOAT2           VEC2;
typedef DirectX::XMFLOAT3           VEC3;
typedef DirectX::XMFLOAT4           VEC4;
typedef DirectX::XMFLOAT4X4         VEC4x4;

// Windows uses wide chars for their file names
typedef std::wstring                FileName;

#else 

// Using other another graphics library
typedef std::string                FileName;

// #TODO 
// Glm defintions 

#endif

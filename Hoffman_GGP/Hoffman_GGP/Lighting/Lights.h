#pragma once

#include <DirectXMath.h>	// XMFLOAT3, XMFLOAT4X4

// Comment this out to stop drawing lights
#define DRAW_LIGHTS

/// <summary>
/// Directional light with ambient and diffuse color
/// </summary>
/// <author>Ben Hoffman</author>
struct DirectionalLight
{
    DirectX::XMFLOAT4 AmbientColor;
    DirectX::XMFLOAT4 DiffuseColor;
    DirectX::XMFLOAT3 Direction;
    float Padding;      // We have to add this here because of the
                        // way that SimpleShader works
};

/// <summary>
/// Point light with a color, range, position, and intensity
/// </summary>
/// <author>Ben Hoffman</autor>
struct PointLight
{
    DirectX::XMFLOAT3 Color; // 12 bytes
    float Range;                    // 16 bytes

    DirectX::XMFLOAT3 Position;     // 28 bytes
    float Intensity;                // 32 bytes
};
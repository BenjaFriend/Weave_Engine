#pragma once

#include <DirectXMath.h>	// XMFLOAT3, XMFLOAT4X4

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
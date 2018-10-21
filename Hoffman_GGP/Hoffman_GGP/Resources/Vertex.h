#pragma once

#include <DirectXMath.h>

struct Vertex
{
    DirectX::XMFLOAT3 Position;	    // The position of the vertex
    DirectX::XMFLOAT2 UV;           // The UV position of the vertex
    DirectX::XMFLOAT3 Normal;       // Normal Direction of the vertex
    DirectX::XMFLOAT3 Tangent;		// Tangent for normal mapping
};
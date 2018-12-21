#pragma once

#include "../stdafx.h"

struct Vertex
{
    VEC3 Position;  // The position of the vertex
    VEC2 UV;        // The UV position of the vertex
    VEC3 Normal;    // Normal Direction of the vertex
    VEC3 Tangent;	// Tangent for normal mapping
};
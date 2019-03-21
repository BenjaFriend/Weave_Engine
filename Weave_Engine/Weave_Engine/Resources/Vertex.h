#pragma once

#include "stdafx.h"

struct Vertex
{
    glm::vec3 Position;  // The position of the vertex
    glm::vec2 UV;        // The UV position of the vertex
    glm::vec3 Normal;    // Normal Direction of the vertex
    glm::vec3 Tangent;	// Tangent for normal mapping
};
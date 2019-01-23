#pragma once

#include "../Resources/Mesh.h"

namespace Editor
{
    /// <summary>
    /// A gizmo is a 3D object that can be draw in the game world 
    /// to help with debugging. They have no collision on them
    /// </summary>
    /// <author>Ben Hoffman</author>
    class Gizmo
    {
    public:

        Gizmo( Mesh* aMesh, glm::vec3 aColor );

        virtual ~Gizmo();

    private:

        Mesh* GizmoMesh = nullptr;

    };

}
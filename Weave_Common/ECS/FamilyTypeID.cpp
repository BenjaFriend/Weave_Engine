#include "stdafx.h"

#include "FamilyTypeID.h"

namespace ECS
{
    class IComponent;

    namespace Util
    {
        TypeID FamilyTypeID<IComponent>::s_count = 0u;

        template class FamilyTypeID<IComponent>;
    }
}
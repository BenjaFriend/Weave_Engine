#pragma once

// ECS System based off of Tobias Stein's implementation 
// https://github.com/tobias-stein/EntityComponentSystem

namespace ECS
{
    namespace Util
    {
        using TypeID = size_t;

        template<class T>
        class FamilyTypeID
        {
            static TypeID s_count;

        public:

            template<class U>
            static const TypeID Get()
            {
                static const TypeID STATIC_TYPE_ID{ s_count++ };
                return STATIC_TYPE_ID;
            }

            static const TypeID Get()
            {
                return s_count;
            }
        };

    }   // namespace Util

}   // namespace ECS
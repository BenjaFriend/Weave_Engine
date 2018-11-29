#pragma once

#include "IComponent.h"
#include "../FamilyTypeID.h"

namespace ECS
{

    using ComponentTypeId = size_t;

    template <class T>
    class Component : public IComponent
    {

    public:

        /** Count of component types */
        static const ComponentTypeId STATIC_COMPONENT_TYPE_ID;

        Component() {}

        virtual ~Component() {}


        ComponentTypeId GetStaticComponentTypeID() const
        {
            return STATIC_COMPONENT_TYPE_ID;
        }

        virtual const char* ComponentName() = 0;

        virtual void SaveObject( nlohmann::json & aOutFile ) = 0;

    };  // Component

    // This private member only exists to force the compiler to create an instance of Component T,
    // which will set its unique identifier.
    template<class T>
    const ComponentTypeId ECS::Component<T>::STATIC_COMPONENT_TYPE_ID = ECS::Util::FamilyTypeID<IComponent>::Get<T>();

}
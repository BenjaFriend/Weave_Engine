#pragma once

#include "ECS/IComponent.h"
#include "ECS/FamilyTypeID.h"
#include "Utils/SaveFileDefs.h"

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

        Component( nlohmann::json const & aInitData )
        {
            LOG_WARN( "Component does not implement scene file constructor!" );
        }

        virtual ComponentTypeId GetStaticComponentTypeID() const override
        {
            return STATIC_COMPONENT_TYPE_ID;
        }

        virtual const char* ComponentName() override = 0;

    protected:

        virtual ~Component() {}

    };  // Component

    // This private member only exists to force the compiler to create an instance of Component T,
    // which will set its unique identifier.
    template<class T>
    const ComponentTypeId ECS::Component<T>::STATIC_COMPONENT_TYPE_ID = ECS::Util::FamilyTypeID<IComponent>::Get<T>();
}
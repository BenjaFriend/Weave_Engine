#pragma once

#include "ECS/Component.h"

class TestComponent : public ECS::Component<TestComponent>
{
public:

    TestComponent( float aVal )
    {
        myData = aVal;
    }

    ~TestComponent()
    {

    }

    float GetData() { return myData; }

private:

    float myData = 21.f;

};
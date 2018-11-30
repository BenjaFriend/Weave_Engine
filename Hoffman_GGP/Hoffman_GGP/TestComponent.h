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

    virtual const char* ComponentName() { return "TestComponent"; }

    virtual void DrawEditorGUI() override
    {
        ImGui::InputFloat( "Test Data", &this->myData );
    }

    virtual void SaveObject( nlohmann::json & aOutFile ) override
    {

    }

private:

    float myData = 21.f;

};
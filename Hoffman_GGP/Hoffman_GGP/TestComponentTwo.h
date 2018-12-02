#pragma once

#include "ECS/Component.h"

class TestComponentTwo : public ECS::Component<TestComponentTwo>
{
public:

    TestComponentTwo()
    {

    }

    ~TestComponentTwo()
    {

    }

    bool GetData() { return myData; }

    virtual const char* ComponentName() { return "TestComponentTwo"; }

    virtual void DrawEditorGUI() override;

    virtual void SaveObject( nlohmann::json& aOutFile ) override;

private:

    bool myData = true;

};
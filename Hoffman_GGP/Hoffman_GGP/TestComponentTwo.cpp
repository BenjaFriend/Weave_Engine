#include "TestComponentTwo.h"

void TestComponentTwo::DrawEditorGUI()
{
    // Draw anything you want to be in here with IMGUI
    ImGui::Checkbox( "Test Data", &myData );
}

void TestComponentTwo::SaveObject( nlohmann::json & aOutFile )
{
    /*aOutFile << "=== " << GetStaticComponentTypeID() << " ===\n";

    aOutFile << "    DATA " << myData << "\n";

    aOutFile << "=======\n";*/
}

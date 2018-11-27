#include "TestComponentTwo.h"

void TestComponentTwo::DrawEditorGUI()
{
    // Draw anything you want to be in here with IMGUI
    ImGui::Checkbox( "Test Data", &myData );
}

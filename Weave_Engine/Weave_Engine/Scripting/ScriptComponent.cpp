#include "stdafx.h"

#include "ScriptComponent.h"

#define SCRIPT_ASSET_PATH_SAVE_KEY  "ScriptPath"

COMPONENT_INIT( ScriptComponent )

ScriptComponent::ScriptComponent()
{
    ScriptMan = Scripting::ScriptManager::GetInstance();
    if ( ScriptFilePath.length() > 0 )
    {
        ScriptMan->RegisterScript( ScriptFilePath );
    }
}

ScriptComponent::ScriptComponent( const FileName & aFileName )
    : ScriptComponent()
{
    ScriptMan = Scripting::ScriptManager::GetInstance();

    // Attempt to load in the given script
    ScriptFilePath = std::string( aFileName.begin(), aFileName.end() );

    if ( ScriptFilePath.length() > 0 )
    {
        ScriptMan->RegisterScript( ScriptFilePath );
    }
}

ScriptComponent::ScriptComponent( nlohmann::json const & aInitData )
    : ScriptComponent()
{
    ScriptFilePath = aInitData.value( SCRIPT_ASSET_PATH_SAVE_KEY, "" );

	if (ScriptFilePath.length() > 0)
	{
		ScriptMan->RegisterScript(ScriptFilePath);
	}
}

ScriptComponent::~ScriptComponent()
{
    // Remove my callbacks from the script manager
    if ( ScriptFilePath.length() > 0 )
    {
        ScriptMan->ReleaseScript( ScriptFilePath );
    }

    ScriptMan = nullptr;
}

void ScriptComponent::SaveComponentData( nlohmann::json & aCompData )
{
    aCompData [ SCRIPT_ASSET_PATH_SAVE_KEY ] = ScriptFilePath;
}

void ScriptComponent::DrawEditorGUI()
{
    REMOVE_COMP_BTN( ScriptComponent );

    ImGui::LabelText( "Current Script: ", ScriptFilePath.c_str() );

    // Allow for editing of what script is loaded
    static char newScriptName [ 64 ] = "\0";
    ImGui::InputText( "Script Name: ", newScriptName, IM_ARRAYSIZE( newScriptName ) );

    if ( ImGui::Button( "Apply Script Change" ) )
    {
        // Remove the previous one if there is one
        if ( ScriptFilePath.length() > 0 )
        {
            ScriptMan->ReleaseScript( ScriptFilePath );
        }
        // Load this script to the manager
        ScriptFilePath = newScriptName;
        if ( ScriptFilePath.length() > 0 )
        {
            ScriptMan->RegisterScript( ScriptFilePath );
        }
    }
}
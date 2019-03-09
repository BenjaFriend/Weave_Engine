#pragma once

#include "../ECS/Component.h"
#include "ScriptUtils.h"

class ScriptComponent : public ECS::Component< ScriptComponent >
{
public:

    COMPONENT( ScriptComponent );

    ScriptComponent();

    /// <summary>
    /// Create this script component based on the given file name
    /// </summary>
    /// <param name="aFileName">The file name which the lua script is</param>
    ScriptComponent( const FileName & aFileName );
    
    /// <summary>
    /// Load in this component from the scene JSON data
    /// </summary>
    /// <param name="aInitData">Component init data</param>
    ScriptComponent( nlohmann::json const & aInitData );

    ~ScriptComponent();

    FORCE_INLINE const std::string & GetScriptFilePath() const { return ScriptFilePath; }

protected:

    virtual void SaveComponentData( nlohmann::json & aCompData ) override;

    // Inherited via Component
    virtual void DrawEditorGUI() override;

private:

    /** The file path to the asset boi */
    std::string ScriptFilePath;

    /** ref to the script manager */
    Scripting::ScriptManager* ScriptMan = nullptr;

};
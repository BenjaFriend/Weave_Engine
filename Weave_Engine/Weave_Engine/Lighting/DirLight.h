#pragma once

#include "../stdafx.h"

#include "LightShaderDefs.h"
#include "../ECS/Component.h"

class LightSystem;

/// <summary>
/// Component for controlling a directional light
/// </summary>
/// <author>Ben Hoffman</author>
class DirLight : public ECS::Component<DirLight>
{
public:

    DirLight( LightSystem* aRendSys, DirectionalLightData aLightData );

    ~DirLight();

    /// <summary>
    /// Get the light information about this 
    /// </summary>
    /// <returns></returns>
    const DirectionalLightData& GetLightData() const;

    /// <summary>
    /// Set this directional lights information
    /// </summary>
    /// <param name="aLightData">The lighting data for this dir light</param>
    void SetLightData( DirectionalLightData aLightData );

    COMP_NAME( "Dir Light" );

    virtual void SaveObject( nlohmann::json & aOutFile ) override;

    virtual void DrawEditorGUI() override;

private:

    /** The lighting information that is needed */
    DirectionalLightData LightingData;

};
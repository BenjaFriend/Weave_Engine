#pragma once

#include "../ECS/Component.h"
#include "ScriptUtils.h"

class ScriptComponent : public ECS::Component< ScriptComponent >
{
public:

    ScriptComponent( const FileName & aFileName );

    ~ScriptComponent();

};
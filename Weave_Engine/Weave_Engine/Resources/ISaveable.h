#pragma once

#include <iostream>
#include "json/json.hpp"

/// <summary>
/// An interface determining that this object is "saveable"
/// </summary>
/// <author>Ben Hoffman</author>
class ISaveable
{
public:

    /// <summary>
    /// Outputs this objects data to the given out file stream
    /// </summary>
    /// <param name="aOutFile">File to send this data to</param>
    virtual void SaveObject( nlohmann::json& aOutFile ) {}
    
};
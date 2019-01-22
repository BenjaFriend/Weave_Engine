#pragma once
#include "../stdafx.h"


namespace Input
{
    // #MakeCrossPlatform
    enum InputType
    {
        Horizontal,
        Vertical,

        Fire,
        FireReleased,

        Look,
        LookReleased,

        Use,

        Quit
    };

    struct InputBinding
    {
        UINT8 InputValue;
        InputType Type;
    };

    /// <summary>
    /// The Implementation of input binding/processing
    /// abstract class
    /// </summary>
    /// <author>Ben Hoffman</author>
    class IInput_Impl
    {
    public:

        /// <summary>
        /// Bind the input manager 
        /// </summary>
        virtual void InitBindings( std::vector<InputBinding> & aBindings ) = 0;

        virtual void ProcessOSCallback( UINT64 aMesg, UINT64 left, UINT64 right ) = 0;

    };

}   // namespace Input
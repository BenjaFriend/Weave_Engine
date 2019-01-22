#pragma once

#include "IInput_Impl.h"


namespace Input
{

    class Input_Win : public IInput_Impl
    {

        void InitBindings( std::vector<InputBinding> & aBindings ) override
        {
            aBindings.push_back( InputBinding{ VK_ESCAPE, InputType::Quit } );
        }

        void ProcessOSCallback( UINT64 aMesg, UINT64 left, UINT64 right ) override
        {

        }

    };

}   // Input

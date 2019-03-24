#pragma once

#include "IInput_Impl.h"


namespace Input
{

    class Input_Win : public IInput_Impl
    {

        void InitBindings( std::vector<InputBinding> & aBindings ) override
        {
            aBindings.push_back( InputBinding{ VK_ESCAPE, InputType::Quit } );
            aBindings.push_back( InputBinding{ VK_SPACE, InputType::Fire } );

            aBindings.push_back( InputBinding{ 'W', InputType::Move_Up } );
            aBindings.push_back( InputBinding{ 'A', InputType::Move_Left } );
            aBindings.push_back( InputBinding{ 'S', InputType::Move_Down } );
            aBindings.push_back( InputBinding{ 'D', InputType::Move_Right } );
        }

        void ProcessOSCallback( UINT64 aMesg, UINT64 left, UINT64 right ) override
        {

        }

    };

}   // Input

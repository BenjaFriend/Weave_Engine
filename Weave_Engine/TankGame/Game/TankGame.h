#pragma once

#include <Core/Game.h>
#include "Networking/NetworkManager.h"

namespace Tanks
{

    class TankGame : public Game
    {

    public:

        TankGame( HINSTANCE hInstance );
        ~TankGame();

        virtual void Init() override;

        virtual void Update( float deltaTime, float totalTime ) override;

        virtual void Quit() override;

    private:

        NetworkManager* NetMan = nullptr;

    };

}   // namespace Tanks
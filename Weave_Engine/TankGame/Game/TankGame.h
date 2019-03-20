#pragma once

#include <Core/Game.h>


class TankGame : public Game
{

public:

    TankGame( HINSTANCE hInstance );
    ~TankGame();

    virtual void Update( float deltaTime, float totalTime ) override;

};

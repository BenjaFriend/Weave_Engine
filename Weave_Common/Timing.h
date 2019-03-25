#pragma once

// Timing class taken from the multilayer  book as  well 

class Timing
{
public:

    Timing();

    void Update();

    float GetDeltaTime() const { return DeltaTime; }

    double GetTime() const;

    float GetTimef() const
    {
        return static_cast< float >( GetTime() );
    }

    float GetFrameStartTime() const { return FrameStartTimef; }

    double GetStartTime() const { return StartTime; }

    static Timing sInstance;

private:
    float DeltaTime;
    UINT64 DeltaTick;

    double LastFrameStartTime;
    float FrameStartTimef;
    double PerfCountDuration;

    /** The time that the program started */
    double StartTime;

};

#pragma once

namespace Utils
{
    /// <summary>
    /// Simple math helpers for any calculations that are 
    /// done a lot.
    /// </summary>
    /// <author>Ben Hoffman</author>
    class MathHelper
    {
    public:

        /// <summary>
        /// Basic linear interpolation function
        /// </summary>
        static float Lerp( float a, float b, float t );
        

    };  // class MathHelper

}   // namespace Utils

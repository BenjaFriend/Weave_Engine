#include "MathHelper.h"

float Utils::MathHelper::Lerp( float a, float b, float t )
{  
    return a + t * ( b - a );
}

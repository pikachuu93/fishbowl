#include "utility.hpp"

#include <ctime>
#include <cstdlib>

int randomInt()
{
    static bool seed = true;
    if (seed)
    {
        srand(time(NULL));
        seed = false;
    }

    return rand();
}

double random(double min, double max)
{
    double range = max - min;

    return range * (double)randomInt() / (double)RAND_MAX + min;
}

QVector3D randomPoint()
{
    return QVector3D(random(-1), random(-1), random(-1));
}

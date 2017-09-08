#ifndef WORLD_HPP
#define WORLD_HPP

#include <vector>

#include <QVector3D>

#include "worker.hpp"
#include "fish.hpp"

class World
{
private:
    int                    generation;
    int                    ticks;
    int                    generationLength;
    std::vector<Fish*>     population;
    std::vector<QVector3D> food;
    std::vector<Worker*>   workers;

public:
    World();
    ~World();

    static bool   compFunc(Fish *a, Fish *b);
    static double fitnessFunc(Fish *f);

    void   nextGeneration();

    std::vector<Fish*>     &getPopulation();
    std::vector<QVector3D> &getFood();

    int getGeneration();
    int getTicks();

    void setGeneration(int g);

    void update();
};

#endif

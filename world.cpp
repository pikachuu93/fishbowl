#include "world.hpp"
#include "fish.hpp"

#include <algorithm>

#include "utility.hpp"
#include "constants.hpp"

#include <iostream>

World::World() :
    generation(0),
    ticks(0),
    generationLength(GENERATION_LENGTH),
    population(),
    food(),
    workers()
{
    for (int i = 0; i < FISH_COUNT; ++i)
    {
        this->population.push_back(new Fish(randomPoint()));
    }

    for (int i = 0; i < FOOD_COUNT; ++i)
    {
        this->food.push_back(randomPoint());
    }
#ifdef USE_THREADED_UPDATE
    for (int i = 0; i < MAX_THREAD_COUNT; ++i)
    {
        this->workers.push_back(new Worker(this,
                                           &(this->population[16*i]),
                                           16));
    }
#endif
}

World::~World()
{
    std::vector<Fish*>::iterator a = this->population.begin();
    for (; a != this->population.end(); ++a)
    {
        delete *a;
    }
}


std::vector<Fish*> &World::getPopulation()
{
    return this->population;
}

std::vector<QVector3D> &World::getFood()
{
    return this->food;
}

void World::update()
{
    this->ticks += 1;

    if (this->ticks > this->generationLength)
    {
        this->ticks = 0;
        this->nextGeneration();
    }

#ifdef USE_THREADED_UPDATE
    std::vector<Worker*>::iterator w;
    for (w = this->workers.begin(); w != this->workers.end(); ++w)
    {
        (*w)->start();
    }

    for (w = this->workers.begin(); w != this->workers.end(); ++w)
    {
        (*w)->wait();
    }

    for (w = this->workers.begin(); w != this->workers.end(); ++w)
    {
        QVector3D **food = (*w)->getReturnData();
        for (int i = 0; i < (*w)->getCount(); ++i)
        {
            if (food[i] == NULL)
                continue;

            *(food[i]) = randomPoint();
        }
    }
#else
    std::vector<Fish*>::iterator f;
    for (f = this->population.begin(); f != this->population.end(); ++f)
    {
        (*f)->update(this);
    }
#endif
}

void World::nextGeneration()
{
    this->generation += 1;

    std::vector<Fish*> sorted = this->population;

    std::sort(sorted.begin(), sorted.end(), World::compFunc);

    std::vector<Fish*>::iterator a, c;

    for (a = sorted.begin(), c = this->population.begin();
         a != sorted.end();
         ++a, ++c)
    {
        (*c) = (*a)->reproduce(sorted[randomInt() % sorted.size()]);
    }

    for (; c != this->population.end(); ++c)
    {
        (*c) = new Fish(randomPoint());
    }

    for (a = sorted.begin(); a != sorted.end(); ++a)
    {
        delete (*a);
    }
}

int World::getGeneration()
{
    return this->generation;
}

int World::getTicks()
{
    return this->ticks;
}

bool World::compFunc(Fish *a, Fish *b)
{
    return World::fitnessFunc(a) > World::fitnessFunc(b);
}

double World::fitnessFunc(Fish *f)
{
    return f->getEnergy();
}

void World::setGeneration(int g)
{
    this->generation = g;
}

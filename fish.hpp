#ifndef FISH_HPP
#define FISH_HPP

// input layer 7*7 sight array
// 5 "memory cells", energy, speed, collision
#define INPUT_TOTAL (7*7 + 5 + 1 + 1 + 1)

// output layer accelaration, angleY change,
// angleZ change, 5 "memory cells".
#define OUTPUT_TOTAL (8)

#include "animal.hpp"

class World;

class Fish : public Animal
{
private:
    Matrix input;
    Matrix output;

    QVector3D  *setFoodInputs(std::vector<QVector3D> &food);
    void        setSightValue(QVector3D &pos);
    QVector3D  *setInput(World *w);
    void        processOutput();

    bool isColliding();

public:
    Fish(QVector3D position);
    virtual ~Fish();

    QVector3D *update(World *w);
    Fish      *reproduce(Fish *other);
};

#endif

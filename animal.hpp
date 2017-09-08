#ifndef ANIMAL_HPP
#define ANIMAL_HPP

#include <QVector3D>

#include "neural-net.hpp"

class World;

class Animal
{
protected:
    double    age;
    double    energy;
    double    speed;
    double    yaw;
    double    pitch;
    double    roll;
    QVector3D position;
    NeuralNet brain;

public:
    Animal();
    Animal(NeuralNet b);

    virtual QVector3D *update(World *w) = 0;

    void addEnergy();

    double    getEnergy();
    QVector3D getPositionP();
    QVector3D getPosition();
    QVector3D getYawPitchRoll();
};

#endif

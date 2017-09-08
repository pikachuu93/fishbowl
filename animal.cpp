#include "animal.hpp"

Animal::Animal() :
    age(0.0),
    energy(1.0),
    speed(0.0),
    yaw(0.0),
    pitch(0.0),
    roll(0.0),
    position()
{
}

Animal::Animal(NeuralNet b) :
    age(0.0),
    energy(100.0),
    speed(0.0),
    yaw(0.0),
    pitch(0.0),
    roll(0.0),
    position()
{
}

void Animal::addEnergy()
{
    this->energy += 0.05;

    if (this->energy > 1.0)
        this->energy = 1.0;
}

double Animal::getEnergy()
{
    return this->energy;
}

QVector3D Animal::getPositionP()
{
    return this->position;
}

QVector3D Animal::getPosition()
{
    return QVector3D(this->position.x(),
                     this->position.y(),
                     this->position.z());
}

QVector3D Animal::getYawPitchRoll()
{
    return QVector3D(this->yaw,
                     this->pitch,
                     this->roll);
}

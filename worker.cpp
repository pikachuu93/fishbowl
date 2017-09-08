#include "worker.hpp"
#include "fish.hpp"

Worker::Worker(World *w, Fish **a, int count) :
    QThread(),
    world(w),
    fish(a),
    retBuffer(),
    count(count)
{
    this->retBuffer = new QVector3D*[count];
}

Worker::~Worker()
{
    delete[] this->retBuffer;
}

void Worker::run()
{
    Fish **f = this->fish;
    for (int i = 0; i < count; ++i)
    {
        this->retBuffer[i] = (*f)->update(this->world);

        ++f;
    }
}

QVector3D **Worker::getReturnData()
{
    return this->retBuffer;
}

int Worker::getCount()
{
    return this->count;
}

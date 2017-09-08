#ifndef WORKER_HPP
#define WORKER_HPP

#include <QThread>

class QVector3D;
class World;
class Fish;

class Worker : public QThread
{
    World      *world;
    Fish      **fish;
    QVector3D **retBuffer;
    int         count;

public:
    Worker(World *w, Fish **a, int count);
    ~Worker();

    void run();

    QVector3D **getReturnData();
    int         getCount();
};

#endif

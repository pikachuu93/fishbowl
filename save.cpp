#include <fstream>

#include "save.hpp"
#include "world.hpp"

Save::Save(std::string name) :
    name(name)
{
}

template <class T>
void Save::readVal(T *v)
{
    this->file << *val;

    if (!this->file.good())
        throw 0;
}

bool Save::load(World *w)
{
    this->file = std::fstream(this->name);
    if (!this->file.good())
        throw 0;

    int i;
    this->readVal(i);
    w->setGeneration(i);

    this->readVal(i);
}

bool Save::save(World *w)
{
}

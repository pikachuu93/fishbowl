#ifndef SAVE_HPP
#define SAVE_HPP

#include <string>

class World;

class Save
{
private:
    std::string  name;
    std::fstream file;

    template <typename T>
    void readVal(T *v);

public:
    Save(std::string name);

    bool load(World *w);
    bool save(World *w);
};

#endif

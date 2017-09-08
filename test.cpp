#include <iostream>
#include <fstream>

int main()
{
    std::fstream f("in.txt");

    while (true)
    {
        int i;
        f >> i;

        if (!f.good())
            break;

        std::cout << "Read: " << i << std::endl;
    }

    return 0;
}

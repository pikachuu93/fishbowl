#include <iostream>
#include <QApplication>

#include "interface.hpp"

int main(int argc, char **argv)
{
    QApplication a(argc, argv);

    new Interface();

    return a.exec();
}

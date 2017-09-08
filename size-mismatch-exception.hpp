#ifndef SIZE_MISMATCH_EXCEPTION_HPP
#define SIZE_MISMATCH_EXCEPTION_HPP

#include "matrix.hpp"

class SizeMismatchException
{
public:
    Matrix a;
    Matrix b;

    SizeMismatchException(Matrix a, Matrix b);
};

#endif

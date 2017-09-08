#include "size-mismatch-exception.hpp"

SizeMismatchException::SizeMismatchException(Matrix a, Matrix b) :
    a(a),
    b(b)
{
}

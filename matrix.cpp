#include "matrix.hpp"

#include "size-mismatch-exception.hpp"

#include "utility.hpp"

#include <cmath>
#include <iostream>

double *Matrix::operator[](int i)
{
    return this->data + i * this->_width;
}

Matrix::Matrix() :
    _width(0),
    _height(0),
    data(NULL)
{
}

Matrix::Matrix(int width, int height, double v) :
    _width(width),
    _height(height),
    data(NULL)
{
    this->data = new double[width * height];

    for (int i = this->_width * this->_height; i != 0;)
    {
        --i;
        this->data[i] = v;
    }
}

Matrix::Matrix(const QVector3D &p) :
    _width(1),
    _height(3),
    data()
{
    this->data = new double[3];

    *(this->data    ) = p.x();
    *(this->data + 1) = p.y();
    *(this->data + 2) = p.z();
}

Matrix::Matrix(const Matrix &other)
{
    this->_width  = other._width;
    this->_height = other._height;
    this->data    = new double[this->_width * this->_height];

    for (int i = this->_width * this->_height; i != 0;)
    {
        --i;
        this->data[i] = other.data[i];
    }
}

Matrix& Matrix::operator=(const Matrix &other)
{
    this->_width  = other._width;
    this->_height = other._height;
    this->data    = new double[this->_width * this->_height];

    for (int i = this->_width * this->_height; i != 0;)
    {
        --i;
        this->data[i] = other.data[i];
    }

    return *this;
}

Matrix::~Matrix()
{
    if (this->data != NULL)
    {
        delete[] this->data;
        this->data = NULL;
    }
}

Matrix Matrix::rotationX(double theta)
{
    Matrix m(3, 3);

    m[0][0] = 1.0;

    double c = cos(theta);
    double s = sin(theta);

    m[1][1] = c;
    m[1][2] = -s;
    m[1][1] = s;
    m[1][2] = c;

    return m;
}

Matrix Matrix::rotationY(double theta)
{
    Matrix m(3, 3);

    m[1][1] = 1.0;

    double c = cos(theta);
    double s = sin(theta);

    m[0][0] = c;
    m[0][2] = s;
    m[2][0] = -s;
    m[2][2] = c;

    return m;
}

Matrix Matrix::rotationZ(double theta)
{
    Matrix m(3, 3);

    m[2][2] = 1.0;

    double c = cos(theta);
    double s = sin(theta);

    m[0][0] = c;
    m[0][1] = -s;
    m[1][0] = s;
    m[1][1] = c;

    return m;
}

Matrix Matrix::operator*(const double &other)
{
    Matrix ret = Matrix(this->_width, this->_height);

    for (int i = this->_width * this->_height; i != 0;)
    {
        --i;
        ret.data[i] = this->data[i] * other;
    }

    return ret;
}

Matrix Matrix::operator/(const double &other)
{
    Matrix ret = Matrix(this->_width, this->_height);

    for (int i = this->_width * this->_height; i != 0;)
    {
        --i;
        ret.data[i] = this->data[i] / other;
    }

    return ret;
}

Matrix Matrix::operator*(const Matrix &other)
{
    if (this->_width != other._height)
        throw SizeMismatchException(*this, other);

    Matrix ret = Matrix(other._width, this->_height);

    for (int row = 0; row < this->_height; row++)
    {
        for (int column = 0; column < other._width; column++)
        {
            for (int i = 0; i < this->_width; i++)
            {
                ret.data[ret._width * row + column]
                    += this->data[this->_width * row + i]
                     * other.data[other._width * i + column];
            }
        }
    }

    return ret;
}

Matrix Matrix::operator+(const Matrix &other)
{
    if (this->_width != other._width || this->_height != other._height)
        throw SizeMismatchException(*this, other);

    Matrix ret = Matrix(this->_width, this->_height);

    for (int i = this->_width * this->_height; i != 0;)
    {
        --i;
        ret.data[i] = this->data[i] + other.data[i];
    }

    return ret;
}

Matrix Matrix::operator-(const Matrix &other)
{
    if (this->_width != other._width || this->_height != other._height)
        throw SizeMismatchException(*this, other);

    Matrix ret = Matrix(this->_width, this->_height);

    for (int i = this->_width * this->_height; i != 0;)
    {
        --i;
        ret.data[i] = this->data[i] - other.data[i];
    }

    return ret;
}

Matrix Matrix::operator-()
{
    Matrix ret = Matrix(this->_width, this->_height);

    for (int i = this->_width * this->_height; i != 0;)
    {
        --i;
        ret.data[i] = -this->data[i];
    }

    return ret;
}

Matrix &Matrix::operator+=(const Matrix &other)
{
    if (this->_width != other._width || this->_height != other._height)
        throw SizeMismatchException(*this, other);

    for (int i = this->_width * this->_height; i != 0;)
    {
        --i;
        this->data[i] += other.data[i];
    }

    return *this;
}

Matrix Matrix::transpose()
{
    Matrix ret = Matrix(this->_height, this->_width);

    for (int row = 0; row < this->_height; row++)
    {
        for (int column = 0; column < this->_width; column++)
        {
            ret.data[this->_height * row + column] = this->data[this->_width * column + row];
        }
    }

    return ret;
}

Matrix Matrix::componentMult(const Matrix &other)
{
    if (this->_width != other._width || this->_height != other._height)
        throw SizeMismatchException(*this, other);

    Matrix ret(this->_width, this->_height);

    for (int i = this->_width * this->_height; i != 0;)
    {
        --i;
        ret.data[i] = this->data[i] * other.data[i];
    }

    return ret;
}

void Matrix::randomize(double min, double max)
{
    for (int i = this->_width * this->_height; i != 0;)
    {
        --i;
        this->data[i] = random(min, max);
    }
}

int Matrix::width()
{
    return this->_width;
}

int Matrix::height()
{
    return this->_height;
}

double Matrix::get(int row, int column)
{
    return this->data[this->_width * row + column];
}

void Matrix::set(int row, int column, double v)
{
    this->data[this->_width * row + column] = v;
}

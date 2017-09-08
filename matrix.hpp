#ifndef MATRIX_CPP
#define MATRIX_CPP

#include <vector>

#include <QVector3D>

#define ROW std::vector<double> 

class Matrix
{
private:
    int    _width;
    int    _height;
    double *data;

    double *operator[](int i);

public:
    Matrix();
    Matrix(const QVector3D &p);
    Matrix(int width, int height, double v = 0.0);
    Matrix(const Matrix &other);

    Matrix& operator=(const Matrix &other);

    ~Matrix();

    static Matrix rotationX(double theta);
    static Matrix rotationY(double theta);
    static Matrix rotationZ(double theta);

    Matrix operator*(const double &other);
    Matrix operator/(const double &other);
    Matrix operator*(const Matrix &other);
    Matrix operator+(const Matrix &other);
    Matrix operator-(const Matrix &other);
    Matrix operator-();

    Matrix &operator+=(const Matrix &other);

    Matrix transpose();

    Matrix componentMult(const Matrix &other);

    void randomize(double min = 0.0, double max = 1.0);

    double get(int row, int column);
    void   set(int row, int column, double v);

    int width();
    int height();
};

#endif

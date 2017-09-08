#include "fish.hpp"
#include "world.hpp"

#include <cmath>
#include <iostream>

#include "utility.hpp"
#include "constants.hpp"

Fish::Fish(QVector3D position) :
    Animal(),
    input(1, INPUT_TOTAL),
    output(1, OUTPUT_TOTAL)
{
    this->position = position;
    this->position = QVector3D(0.0, 0.0, 0.0);

    std::vector<int> sizes = std::vector<int>();

    sizes.push_back(INPUT_TOTAL);

    sizes.push_back(15);
    sizes.push_back(15);

    sizes.push_back(OUTPUT_TOTAL);

    this->brain = NeuralNet(sizes);
}

Fish::~Fish()
{
}

Fish *Fish::reproduce(Fish *other)
{
    //Mix DNA from this animal and the other.
    Fish *f = new Fish(randomPoint());

    std::vector<Matrix> &aWeights = this->brain.getWeights();
    std::vector<Matrix> &aBiases  = this->brain.getWeights();
    std::vector<Matrix> &bWeights = other->brain.getWeights();
    std::vector<Matrix> &bBiases  = other->brain.getWeights();
    std::vector<Matrix> &nWeights = f->brain.getWeights();
    std::vector<Matrix> &nBiases  = f->brain.getWeights();

    std::vector<Matrix>::iterator aW, aB, bW, bB, nW, nB;

    aW = aWeights.begin();
    aB = aBiases.begin();

    bW = bWeights.begin();
    bB = bBiases.begin();

    nW = nWeights.begin();
    nB = nBiases.begin();

    for (; aW != aWeights.end() && aB != aBiases.end();
         ++aW, ++aB, ++bW, ++bB, ++nW, ++nB)
    {
        if (randomInt() % 2)
        {
            (*nW) = (*aW);
            (*nB) = (*aB);
        }
        else
        {
            (*nW) = (*bW);
            (*nB) = (*bB);
        }

        int end = randomInt() % 100;
        for (int i = 0; i < end; ++i)
        {
            int row = randomInt() % nW->height();
            int col = randomInt() % nW->width();
            int b   = randomInt() % nB->height();

            nW->set(row, col, nW->get(row, col) + random(-0.1, 0.1));
            nB->set(b,   0,   nW->get(b,   0)   + random(-0.1, 0.1));
        }
    }

    return f;
}

QVector3D *Fish::update(World *w)
{
    QVector3D *r = this->setInput(w);
    this->output = this->brain.process(this->input);
    this->processOutput();

    return r;
}

QVector3D *Fish::setInput(World*w)
{
    this->input = Matrix(1, INPUT_TOTAL);

    QVector3D *r = this->setFoodInputs(w->getFood());

    for (int i = 0; i < 5; ++i)
    {
        // Memory outputs are values 3 to 7
        double v = this->output.get(i + 3, 0);
        this->input.set(i + 49, 0, v);
    }

    this->input.set(54, 0, this->energy);
    this->input.set(55, 0, this->speed);
    this->input.set(56, 0, this->isColliding());

    return r;
}

void Fish::processOutput()
{
    this->energy -= 0.0001;

    this->speed += ACCELERATION * (this->output.get(1, 0) - 0.5);
    this->pitch += ANGULAR_ACC  * (this->output.get(1, 0) - 0.5);
    this->yaw   += ANGULAR_ACC  * (this->output.get(2, 0) - 0.5);

    if (this->speed > MAX_SPEED)
        this->speed = MAX_SPEED;
    else if (this->speed < MIN_SPEED)
        this->speed = MIN_SPEED;

    if (this->pitch > PI / 4.0)
        this->pitch = PI / 4.0;
    else if (this->pitch < -PI / 4.0)
        this->pitch = -PI / 4.0;

    Matrix p = Matrix(this->position);

    Matrix pitch = Matrix::rotationY(this->pitch);
    Matrix yaw   = Matrix::rotationZ(this->yaw);

    Matrix x = Matrix(1, 3);
    x.set(0, 0, this->speed);

    // Must be this order to get the right value;
    // Roll won't affect this.
    p += yaw * pitch * x;

    this->position.setX(p.get(0, 0));
    this->position.setY(p.get(1, 0));
    this->position.setZ(p.get(2, 0));

    if (std::abs(this->position.x()) > 1.0)
    {
        this->position.setX(this->position.x() > 1 ? 1 : -1);
    }
    if (std::abs(this->position.y()) > 1.0)
    {
        this->position.setY(this->position.y() > 1 ? 1 : -1);
    }
    if (std::abs(this->position.z()) > 1.0)
    {
        this->position.setZ(this->position.z() > 1 ? 1 : -1);
    }
}

bool Fish::isColliding()
{
    return (std::abs(this->position.x()) > 0.99 ||
            std::abs(this->position.y()) > 0.99 ||
            std::abs(this->position.z()) > 0.99);
}

QVector3D *Fish::setFoodInputs(std::vector<QVector3D> &food)
{
    QVector3D *r = NULL;

    std::vector<QVector3D>::iterator f = food.begin();
    for (; f != food.end(); ++f)
    {
        this->setSightValue(*f);

        if ((this->position - *f).length() < EAT_DISTANCE)
        {
            r = &(*f);
        }
    }

    return r;
}

void Fish::setSightValue(QVector3D &pos)
{
    QVector3D p = pos - this->position;

    double range = 0.5;

    if (p.length() <= range)
    {
        // Angle around the y-axis, i.e. the x,z plane
        double thetaY = atan2(p.z(), p.x()) - this->pitch;
        // Angle around the z-axis, i.e. the y,z plane
        double thetaZ = atan2(p.y(), p.x()) - this->yaw;

        if (std::abs(thetaY) < PI / 6.0 && std::abs(thetaZ) < PI / 6.0)
        {
            int x = floor(7.0 * (thetaY + PI / 6.0) * 3.0 / PI);
            int y = floor(7.0 * (thetaZ + PI / 6.0) * 3.0 / PI);

            int i = x + 7 * y;
            if (this->input.get(i, 0) < p.length() / range)
                this->input.set(i, 0, p.length() / range);
        }
    }
}

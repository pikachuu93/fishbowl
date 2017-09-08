#include "neural-net.hpp"
#include "utility.hpp"

#include <cmath>
#include <iostream>

NeuralNet::NeuralNet() :
    learnRate(1.0),
    decayRate(0.000005),
    costHistory(),
    sizes(),
    weights(),
    biases()
{
}

NeuralNet::NeuralNet(std::vector<int> sizes) :
    learnRate(1.0),
    decayRate(0.00001),
    sizes(sizes),
    weights(),
    biases()
{
    std::vector<int>::iterator i, j;
    i = j = sizes.begin();
    for (i++; i != sizes.end(); i++, j++)
    {
        this->weights.push_back(Matrix(*j, *i));
        this->weights.back().randomize(-1.0, 1.0);

        this->biases.push_back(Matrix(1, *i));
        this->biases.back().randomize(-1.0, 1.0);
    }
}

GradVector NeuralNet::getGradients(Matrix input, Matrix target)
{
    std::vector<Matrix> inputs = std::vector<Matrix>();
    std::vector<Matrix> args   = std::vector<Matrix>();

    std::vector<Matrix>::iterator w, b;

    Matrix z;
    Matrix output;

    for (w = this->weights.begin(), b = this->biases.begin();
         w != this->weights.end() && b != this->biases.end();
         w++, b++)
    {
        z = *w * input + *b;

        inputs.push_back(input);
        args.push_back(z);

        output = this->sigma(z);

        input = output;
    }

    GradVector gradients = GradVector();

    std::vector<Matrix>::reverse_iterator i, a, rw;

    i = inputs.rbegin();
    a = args.rbegin();
    Matrix costP = this->costPrime(output, target);

    // dCost/dBias
    Matrix dCDb = output - target;
    //Matrix dCDb = this->sigmaPrime(*a).componentMult(costP);
    //dCost/dWeight
    Matrix dCDw = dCDb * (i->transpose());

    gradients.push_back(std::pair<Matrix, Matrix>(dCDw, dCDb));

    rw = this->weights.rbegin();
    for (++i, ++a, rw = this->weights.rbegin();
         i != inputs.rend() && a != args.rend();
         ++i, ++a, ++rw)
    {
        dCDb = this->sigmaPrime(*a).componentMult(rw->transpose() * dCDb);
        dCDw = dCDb * i->transpose();

        gradients.push_back(std::pair<Matrix, Matrix>(dCDw, dCDb));
    }

    return gradients;
}

void NeuralNet::update(GradVector gradients)
{
    GradVector::reverse_iterator  grad;
    std::vector<Matrix>::iterator w, b;

    for (w = this->weights.begin(),
         b = this->biases.begin(),
         grad = gradients.rbegin();
         w != this->weights.end() &&
         b != this->biases.end() &&
         grad != gradients.rend();
         w++, b++, grad++)
    {
        *w = *w - grad->first  * this->learnRate;
        *b = *b - grad->second * this->learnRate;
    }
}

double NeuralNet::sigma(double z)
{
    return 1 / (1 + exp(-z));
}

Matrix NeuralNet::sigma(Matrix z)
{
    Matrix ret(z.width(), z.height());

    for (int i = 0; i < z.height(); i++)
    {
        ret.set(i, 0, this->sigma(z.get(i, 0)));
    }

    return ret;
}

Matrix NeuralNet::sigmaPrime(Matrix z)
{
    Matrix ret(z.width(), z.height());

    for (int i = 0; i < z.height(); i++)
    {
        double v = z.get(i, 0);
        ret.set(i, 0, this->sigma(v) * this->sigma(-v));
    }

    return ret;
}

Matrix NeuralNet::costPrime(Matrix output, Matrix target)
{
    return output - target;
}

double NeuralNet::cost(Matrix output, Matrix target)
{
    double ret = 0;

    for (int i = 0; i < output.height(); ++i)
    {
        double a = output.get(i, 0);
        double y = target.get(i, 0);

        ret += - y * log(a) - (1.0 - y) * log(1.0 - a);
    }
    
    return ret;

    //Matrix diff = output - target;
    //return 0.5 * (diff * diff.transpose()).get(0, 0);
}

Matrix NeuralNet::process(Matrix input)
{
    std::vector<Matrix>::iterator w, b;
    for (w = this->weights.begin(), b = this->biases.begin();
         w != this->weights.end() && b != this->biases.end();
         w++, b++)
    {
        Matrix m = *w * input;

        input = this->sigma(m + *b);
    }

    return input;
}

void NeuralNet::learn(Matrix input, Matrix target)
{
    Matrix output = this->process(input);
    double cost   = this->cost(output, target);
    double diff   = 0;

    for (std::vector<double>::iterator i = this->costHistory.begin();
         i != this->costHistory.end();
         ++i)
    {
        diff += *i - cost;
    }

    if (diff < 0)
    {
        this->learnRate /= 2;
    }

    if (this->costHistory.size() > 5)
        this->costHistory.erase(this->costHistory.begin());


    this->costHistory.push_back(cost);

    GradVector grads = this->getGradients(input, target);

    this->update(grads);
}

void NeuralNet::learnBatch(std::vector<Matrix> inputs,
                           std::vector<Matrix> targets)
{
    std::vector<Matrix>::iterator i, t;
    i = inputs.begin();
    t = targets.begin();

    GradVector gs = this->getGradients(*i, *t);

    for (++i, ++t;
         i != inputs.end() && t != inputs.end();
         ++i, ++t)
    {
        GradVector temp = this->getGradients(*i, *t);

        GradVector::iterator next = temp.begin(), old = gs.begin();
        for (; next != temp.end(); ++next, ++old)
        {
            old->first  = old->first  + next->first;
            old->second = old->second + next->second;
        }
    }

    GradVector::iterator v;
    for (v = gs.begin(); v != gs.end(); ++v)
    {
        v->first  = v->first  / inputs.size();
        v->second = v->second / inputs.size();
    }

    this->update(gs);
}

void NeuralNet::setLearnRate(double r)
{
    this->learnRate = r;
}

std::vector<int> NeuralNet::getSizes()
{
    return this->sizes;
}

std::vector<Matrix> &NeuralNet::getWeights()
{
    return this->weights;
}

std::vector<Matrix> &NeuralNet::getBiases()
{
    return this->biases;
}

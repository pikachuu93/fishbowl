#ifndef NEURAL_HPP
#define NEURAL_HPP

#include <vector>
#include <utility>
#include <fstream>

#include "matrix.hpp"

#define GradVector std::vector<std::pair<Matrix, Matrix> > 

class NeuralNet
{
private:
    double              learnRate;
    double              decayRate;
    std::vector<double> costHistory;
    std::vector<int>    sizes;
    std::vector<Matrix> weights;
    std::vector<Matrix> biases;

    double     sigma(double z);
    Matrix     sigma(Matrix z);
    Matrix     sigmaPrime(Matrix z);
    Matrix     costPrime(Matrix output, Matrix target);
    GradVector getGradients(Matrix input, Matrix target);
    void       update(GradVector gradients);

public:
    NeuralNet();
    NeuralNet(std::vector<int> sizes);

    void   learn(Matrix input, Matrix target);
    void   learnBatch(std::vector<Matrix> input, std::vector<Matrix> target);

    Matrix process(Matrix input);
    double cost(Matrix output, Matrix target);

    void   setLearnRate(double r);

    std::vector<int>    getSizes();
    std::vector<Matrix> &getWeights();
    std::vector<Matrix> &getBiases();
};

#endif

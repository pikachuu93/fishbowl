import numpy

class Brain:
    def __init__(self, layers, isCopy = False):
        if (isCopy):
            return

        self.matrices = []
        self.sizes    = []

        size = layers[0]
        for val in layers[1:]:
            self.sizes.append((val, size))
            size = val

        def func(x):
            return (numpy.exp(x) - numpy.exp(-x))\
                 / (numpy.exp(x) + numpy.exp(-x))

        self.activation = numpy.vectorize(func)

        for size in self.sizes:
            m = self.initialiseMatrix(size[0], size[1])
            self.matrices.append(m)

    def run(self, vec):
        ret = numpy.matrix(vec).transpose()
        for mat in self.matrices:
            ret = self.activation(mat * ret)

        return ret

    def copy(self):
        b = Brain(True)

        b.matrices = []
        b.sizes = self.sizes

        for m in self.matrices:
            b.matrices.append(m.copy())

        return b

    def mutate(self):
        for m in self.matrices:
            m += self.initialiseMatrix(m.shape[0], m.shape[1], 0.1)

    def initialiseMatrix(self, x, y, stdDev = 1):
        return numpy.matrix(numpy.random.normal(0, stdDev, (x, y)))


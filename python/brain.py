import numpy, os

class Brain:
    def __init__(self, layers, isCopy = False, matrices = None):
        if (isCopy):
            return

        self.mutationRate = 0.1

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

        if matrices:
            self.matrices = matrices
        else:
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

    def breed(self, other):
        for i in range(len(self.matrices)):
            action = numpy.random.randint(0, 5)
            if action > 4:
                self.matrices[i] = other.matrices[i]
            elif action == 4:
                self.matrices[i] = (self.matrices[i] + other.matrices[i]) / 2

        self.mutationRate += numpy.random.normal(0, 0.1);

    def mutate(self):
        for m in self.matrices:
            m += self.initialiseMatrix(m.shape[0], m.shape[1], 0.1)

    def initialiseMatrix(self, x, y, stdDev = 1):
        return numpy.matrix(numpy.random.normal(0, stdDev, (x, y)))

    def save(self, id):
        dirname = "save/" + str(id)
        if os.path.isdir(dirname):
            for filename in os.listdir(dirname):
                os.remove(dirname + "/" + filename)
            os.rmdir(dirname)

        os.mkdir(dirname)
        
        i = 0
        for layer in self.matrices:
            numpy.save(dirname + "/" + str(i), layer)
            i += 1

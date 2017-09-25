from simulation import Simulation

import math, os, numpy

class Genetic:
    def __init__(self):
        self.simCount = 100

        if os.path.isdir("save"):
            self.sims = []
            for filename in os.listdir("save"):
                self.sims.append(Simulation("save/" + filename))
        else:
            self.sims  = [Simulation() for i in range(self.simCount)]

    def run(self):
        gen = 0
        try:
            while True:
                print("======= Running gen " + str(gen) + " =======")
                print()
                self.runGen()
                self.getNextGen()
                print()

                gen += 1
        except KeyboardInterrupt:
            print("======= Saving =======")
            i = 0
            if not os.path.isdir("save"):
                os.mkdir("save")

            def sort(sim):
                r = sim.rank()
                if math.isnan(r):
                    return -100000000

                return r

            self.sims.sort(key = sort)
            self.sims.reverse()

            for s in self.sims:
                print(s.rank())

                s.fish.brain.save(i)
                i += 1

            return

    def runGen(self):
        for s in self.sims:
            s.run(1500)

        def sort(sim):
            r = sim.rank()
            if math.isnan(r):
                return -100000000

            return r

        self.sims.sort(key = sort)
        self.sims.reverse()

        for s in self.sims:
            print(s.rank())

    def getNextGen(self):
        for i in range(self.simCount / 2):
            self.sims[i].fish.brain.breed(self.sims[int(round((self.simCount / 2) * (1 - numpy.random.power(3))))].fish.brain)
            self.sims[i + self.simCount / 2] = Simulation()


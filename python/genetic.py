from simulation import Simulation

import math

class Genetic:
    def __init__(self):
        self.simCount = 100

        self.sims  = [Simulation() for i in range(self.simCount)]

    def run(self):
        gen = 0
        while True:
            print("======= Running gen " + str(gen) + " =======")
            print()
            self.runGen()
            self.getNextGen()
            print()

            gen += 1

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
            self.sims[i].mutate()
            self.sims[i + self.simCount / 2] = Simulation()


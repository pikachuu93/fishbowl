from simulation import Simulation

class Genetic:
    def __init__(self):
        self.simCount = 100

        self.sims  = [Simulation() for i in range(self.simCount)]
        self.ranks = [0 for i in range(self.simCount)]

    def run(self):
        i = 0
        for s in self.sims:
            s.run(1500)
            self.ranks[i] = s.rank()
            print(self.ranks[i])

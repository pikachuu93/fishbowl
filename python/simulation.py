from fish import Fish
from food import Food

import os, numpy

class Simulation:
    def __init__(self, load = None):
        if load:
            temp = {}
            for filename in os.listdir(load):
                order = int(filename.split(".")[0])
                data  = numpy.load(load + "/" + filename)

                temp[order] = data

            brain = []
            for i in temp:
                brain.append(temp[i])

            self.fish   = Fish(brain)
        else:
            self.fish   = Fish()
        self.food   = Food(self.fish.x, self.fish.y, self.fish.z)

        self.numEaten = 0

        self.eatDistance = 0.01
    
    def run(self, iterations):
        for i in range(iterations):
            self.step()

    def step(self):
        self.fish.update(self.food)
        if self.eaten(self.fish, self.food):
            self.fish.energy += 10
            self.numEaten += 1
            self.food = Food(self.fish.x, self.fish.y, self.fish.z)

    def eaten(self, a, b):
        return self.disSqr(a, b) < self.eatDistance

    def disSqr(self, a, b):
        return (a.x - b.x) ** 2 + (a.y - b.y) ** 2 + (a.z - b.z) ** 2

    def rank(self):
        return self.numEaten - self.disSqr(self.fish, self.food)

    def mutate(self):
        self.fish.brain.mutate()

        self.fish.x = 0
        self.fish.y = 0
        self.fish.z = 0

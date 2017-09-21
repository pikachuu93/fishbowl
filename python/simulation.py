from fish import Fish
from food import Food

class Simulation:
    def __init__(self, render = None):
        self.fish   = Fish()
        self.food   = Food(self.fish.x, self.fish.y, self.fish.z)
        self.render = render

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

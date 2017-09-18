from fish import Fish
from food import Food

class Simulation:
    def __init__(self, render = None):
        self.fish   = Fish()
        self.food   = Food(self.fish.x, self.fish.y, self.fish.z)
        self.render = render

        self.eatDistance = 0.01
    
    def run(self, iterations):
        for i in range(iterations):
            self.step()

    def step(self):
        self.fish.update(self.food)
        if self.eaten(self.fish, self.food):
            self.fish.energy += 10
            self.food = Food(self.fish)

    def eaten(self, a, b):
        return (a.x - b.x) ** 2 + (a.y - b.y) ** 2 + (a.z - b.z) ** 2 < self.eatDistance

    def rank(self):
        return self.fish.energy

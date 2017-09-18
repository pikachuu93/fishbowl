from brain import Brain 
from math import cos, sin, pi

class Fish:
    def __init__(self):
        self.yaw    = 0
        self.pitch  = 0
        self.x      = 0
        self.y      = 0
        self.z      = 0
        self.energy = 100
        self.brain  = Brain([9, 5, 5, 3])

        # 15 is the target frame rate. Hence the fish can turn
        # half a circle in 1 second and move 10cm.
        self.agility = pi / 15
        self.speed   = 0.1 / 15

    def update(self, food):
        res = self.brain.run([self.yaw,
                              self.pitch,
                              self.x,
                              self.y,
                              self.z,
                              self.energy,
                              food.x,
                              food.y,
                              food.z])

        self.yaw   += self.agility * res[0, 0]
        self.pitch += self.agility * res[1, 0]

        self.x += res[2, 0] * self.speed * cos(self.yaw) * cos(self.pitch)
        self.y += res[2, 0] * self.speed * sin(self.yaw) * cos(self.pitch)
        self.z += res[2, 0] * self.speed * sin(self.pitch)

        self.energy -= 0.1

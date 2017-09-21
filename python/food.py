from numpy.random import normal

class Food:
    def __init__(self, x, y, z):
        self.x = x + normal(0.5, 0.3)
        self.y = y + normal(0.5, 0.3)
        self.z = z + normal(0.5, 0.3)

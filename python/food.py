from numpy.random import normal

class Food:
    def __init__(self, x, y, z):
        self.x = x + normal(1, 0.3)
        self.y = y + normal(1, 0.3)
        self.z = z + normal(1, 0.3)

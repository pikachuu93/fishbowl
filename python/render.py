from PyQt5 import QtGui
from PyQt5.QtWidgets import QApplication
from OpenGL.GL import *

from simulation import Simulation

class Gui(QtGui.QOpenGLWindow):
    def __init__(self):
        QtGui.QOpenGLWindow.__init__(self)

        self.sim = Simulation("save/0")

        self.rotationX = 0
        self.rotationY = 0
        self.time      = 0
        self.circle    = []
        self.real      = []
        self.imag      = []
        self.mouseStart = False

        self.resize(650, 650)
        self.show()

        self.startTimer(1000 / 15)

    def initializeGL(self):
        glClearColor(1.0, 1.0, 1.0, 1.0)
        glPointSize(5.0)
        glHint(GL_LINE_SMOOTH_HINT,GL_NICEST)
        glEnable(GL_DEPTH_TEST)

    def resizeGL(self, w, h):
        self.width  = w
        self.height = h

        glViewport(0, 0, self.width, self.height)

    def paintGL(self):
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT)

        s = [25]*3

        glMatrixMode(GL_PROJECTION)
        glLoadIdentity()
        glFrustum(-1, 1, -1, 1, 1, 200)

        glMatrixMode(GL_MODELVIEW)
        glLoadIdentity()

        glTranslatef(0.0, 0.0, -50.0)
        glScalef(*s)
        glRotatef(20, 1.0, 0.0, 0.0)
        glRotatef(self.rotationY, 1.0, 0.0, 0.0)
        glRotatef(self.rotationX, 0.0, 1.0, 0.0)

        fish = self.sim.fish
        food = self.sim.food

        glBegin(GL_LINE_LOOP)
        glColor3f(0, 0, 0)

        glVertex3f(-1, -1, -1)
        glVertex3f( 1, -1, -1)
        glVertex3f( 1,  1, -1)
        glVertex3f(-1,  1, -1)
        glEnd()

        glBegin(GL_LINE_LOOP)
        glVertex3f(-1, -1,  1)
        glVertex3f( 1, -1,  1)
        glVertex3f( 1,  1,  1)
        glVertex3f(-1,  1,  1)
        glEnd()

        glBegin(GL_LINES)
        glVertex3f(-1, -1, -1)
        glVertex3f(-1, -1,  1)

        glVertex3f( 1, -1, -1)
        glVertex3f( 1, -1,  1)

        glVertex3f( 1,  1, -1)
        glVertex3f( 1,  1,  1)

        glVertex3f(-1,  1, -1)
        glVertex3f(-1,  1,  1)
        glEnd()

        glBegin(GL_POINTS)
        glColor3f(1.0, 0.0, 0.0)
        glVertex3f(food.x, food.y, food.z)
        glColor3f(0.0, 0.0, 1.0)
        glVertex3f(fish.x, fish.y, fish.z)
        glEnd()

    def mousePressEvent(self, e):
        self.mouseStart = e.pos()

    def mouseMoveEvent(self, e):
        if not self.mouseStart:
            return

        self.rotationX += (e.x() - self.mouseStart.x()) / 2
        #self.rotationY += (e.y() - self.mouseStart.y()) / 2
        self.mouseStart = e.pos()
        self.update()

    def mouseReleaseEvent(self, e):
        self.mouseMoveEvent(e)
        self.update()
        self.mouseStart = False

    def timerEvent(self, e):
        self.sim.step()
        self.update()

a = QApplication([])

g = Gui()

a.exec_()


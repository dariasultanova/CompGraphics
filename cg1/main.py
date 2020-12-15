from OpenGL.GLUT import *
from OpenGL.GL import *
from OpenGL.GLU import *
import math
import pygame
from pygame.locals import *

lastPosX = 0
lastPosY = 0
zoomScale = 1.0
dataL = 0
xRot = 0
yRot = 0
zRot = 0


def mouseMove(event):
    global lastPosX, lastPosY, zoomScale, xRot, yRot, zRot

    if event.type == pygame.MOUSEBUTTONDOWN and event.button == 4:  # wheel rolled up
        glScaled(1.05, 1.05, 1.05)
    elif event.type == pygame.MOUSEBUTTONDOWN and event.button == 5:  # wheel rolled down
        glScaled(0.95, 0.95, 0.95)

    if event.type == pygame.MOUSEMOTION:
        x, y = event.pos
        dx = x - lastPosX
        dy = y - lastPosY

        mouseState = pygame.mouse.get_pressed()
        if mouseState[0]:
            modelView = (GLfloat * 16)()
            mvm = glGetFloatv(GL_MODELVIEW_MATRIX, modelView)

            # To combine x-axis and y-axis rotation
            temp = (GLfloat * 3)()
            temp[0] = modelView[0] * dy + modelView[1] * dx
            temp[1] = modelView[4] * dy + modelView[5] * dx
            temp[2] = modelView[8] * dy + modelView[9] * dx
            norm_xy = math.sqrt(temp[0] * temp[0] + temp[1] * temp[1] + temp[2] * temp[2])
            glRotatef(math.sqrt(dx * dx + dy * dy), temp[0] / norm_xy, temp[1] / norm_xy, temp[2] / norm_xy)

        lastPosX = x
        lastPosY = y


def cylinder():
    glTranslatef(0, 0, -0.76)
    quadratic = gluNewQuadric()
    gluQuadricDrawStyle(quadratic, GLU_LINE)
    gluCylinder(quadratic, 0.2, 0.2, 0.76, 15, 15)  # to draw the lateral parts of the cylinder
    gluDisk(quadratic, 0, 0.2, 15, 15)
    glTranslatef(0, 0, 0.76)
    gluDisk(quadratic, 0, 0.2, 15, 15)
    glFlush()


def sphere():
    quadratic = gluNewQuadric()
    gluQuadricDrawStyle(quadratic, GLU_LINE)
    gluSphere(quadratic, 0.18, 15, 15)
    glFlush()


def octahedron():
    glutWireOctahedron()


def torus():
    glutWireTorus(0.2, 0.5, 15, 15)


pygame.init()
display = (700, 500)
pygame.display.set_mode(display, DOUBLEBUF | OPENGL, RESIZABLE)
glColor3f(0.0, 0.0, 1.0)


def first():
    cylinder()
    glTranslatef(0, 0, -0.38)
    sphere()
    glTranslatef(0, 0, 0.38)


def second():
    cylinder()
    glTranslatef(0, 0, -0.76)
    sphere()
    glTranslatef(0, 0, 0.76)


def third():
    octahedron()
    glTranslatef(0, 0, -1)
    torus()
    glTranslatef(0, 0, 1)


def fourth():
    octahedron()
    glTranslatef(0, -0.5, -1)
    torus()
    glTranslatef(0, 0.5, 1)


counter = 0
while True:
    counter += 1
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            pygame.quit()
            quit()
        mouseMove(event)

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT)
    if counter < 500:
        first()
    if counter > 500 and counter < 1000:
        second()
    if counter > 1000 and counter < 1500:
        third()
    if counter > 1500 and counter < 2000:
        fourth()
    if counter > 2000:
        counter = 0

    pygame.display.flip()
    pygame.time.wait(20)

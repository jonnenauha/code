#!/usr/bin/python

from OpenGL.GLUT import *
from OpenGL.GLU import *
from OpenGL.GL import *
import sys
import math

def cross(a,b):
    (a1, a2, a3) = a
    (b1, b2, b3) = b
    return [a2*b3 - a3*b2, a3*b1 - a1*b3, a1*b2 - a2*b1]

def norm(vector):
    n = 0.0
    for v in vector: n += v*v
    return math.sqrt(n)

def normalize(vector):
    n = norm(vector)
    for i,v in enumerate(vector):
        vector[i] = v/n
    return vector

def move(vector, direction, m):
    vector = [m*d+v for v,d in zip(vector,direction)]
    return vector

def rotate(vector, axis, angle):
    (x, y, z) = axis # ||axis|| = 1
    (a, b, c) = vector

    s = math.sin(angle); c = math.cos(angle); C = 1-c
    xx = x*x; yy = y*y; zz = z*z
    xy = x*y; xz = x*z; yz = y*z
    xs = x*s; ys = y*s; zs = z*s

    vector[0] = (xx*C+c)*a + (xy*C-zs)*b + (xz*C+ys)*c
    vector[1] = (xy*C+zs)*a + (yy*C+c)*b + (yz*C-xs)*c
    vector[2] = (xz*C-ys)*a + (yz*C+xs)*b + (zz*C+c)*c
    return vector


class Camera(object):
    def __init__(self):
        self.fov = 45.0
        self.near = 1.0
        self.far = 40.0

        self.position = [0, 0, -10]
        self.forward = normalize([0, 0, -1])
        self.upward = normalize([0, 1, 0])
        self.sideways = normalize(cross(self.forward, self.upward))

    def move_fore(self, m):
        self.position = move(self.position, self.forward, m)

    def move_side(self, m):
        self.position = move(self.position, self.sideways, m)

    def move_up(self, m):
        self.position = move(self.position, self.upward, m)

    def pitch(self, a):
        self.forward = normalize(rotate(self.forward, self.sideways, a))
        self.upward = normalize(rotate(self.upward, self.sideways, a))

    def roll(self, a):
        self.upward = normalize(rotate(self.upward, self.forward, a))
        self.sideways = normalize(cross(self.forward, self.upward))

    def yaw(self, a):
        self.forward = normalize(rotate(self.forward, self.upward, a))
        self.sideways = normalize(cross(self.forward, self.upward))

    def get_matrix(self):
        (fx, fy, fz) = self.forward
        (sx, sy, sz) = self.sideways
        (ux, uy, uz) = self.upward
        return [sx, ux, -fx, 0.0,
                sy, uy, -fy, 0.0,
                sz, uz, -fz, 0.0,
                0.0, 0.0, 0.0, 1.0]


class Light(object):
    def __init__(self):
        self.position = [10.0, 10.0, 10.0, 1.0]
        self.diffuse_color = [1.0, 1.0, 1.0, 1.0]


class GLBaseState(object):
    def __init__(self):
        self.clear_color = [0.0, 0.0, 0.0 ,1.0]
        self.shade_model = GL_SMOOTH
        self.enable = [GL_CULL_FACE, GL_DEPTH_TEST]

    def reset(self):
        (r,g,b,a) = self.clear_color
        glClearColor(r, g, b, a)
        glShadeModel(self.shade_model)
        for s in self.enable:
            glEnable(s)

class GLLightingState(object):
    def __init__(self, lights):
        self.lights = {}
        self.light_enum = [GL_LIGHT0, GL_LIGHT1, 
                GL_LIGHT2, GL_LIGHT3, GL_LIGHT4, 
                GL_LIGHT5, GL_LIGHT6, GL_LIGHT7]

        for i, l in enumerate(lights):
            self.lights[self.light_enum[i]] = l 

    def reset(self):
        glEnable(GL_LIGHTING)
        for k,v in self.lights.iteritems():
            glLightfv(k, GL_POSITION, v.position)
            glLightfv(k, GL_DIFFUSE, v.diffuse_color)
            glEnable(k)

class GLState(GLBaseState, GLLightingState):
    def __init__(self):
        GLBaseState.__init__(self)
        GLLightingState.__init__(self, [Light()])
        
    def reset(self):
        GLBaseState.reset(self)
        #GLLightingState.reset(self)


class Window(object):
    def __init__(self, name, width, height):
        glutInitWindowSize(width, height)
        self.windowid = glutCreateWindow(name)
    
    def destroy(self):
        glutDestroyWindow(self.windowid)


class App(object):
    def __init__(self):
        glutInit(sys.argv)
        glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH)

        self.window = Window("App", 400, 400)
        self.camera = Camera()
        self.glstate = GLState()
        self.glstate.reset()
        
        glutDisplayFunc(self.display)
        glutIdleFunc(self.display)
        glutReshapeFunc(self.reshape)
        glutKeyboardFunc(self.keyboard)
        
        self.keymap = { 'x' : self.destroy, 
                'r' : self.up, 'f' : self.down,
                'a' : self.left, 'd' : self.right,
                'w' : self.forward, 's' : self.backward, 
                't' : self.pitchup, 'g' : self.pitchdown, 
                'q' : self.yawleft, 'e' : self.yawright, }
        
    def up(self): self.camera.move_up(0.1)
    def down(self): self.camera.move_up(-0.1)
    def left(self): self.camera.move_side(0.1)
    def right(self): self.camera.move_side(-0.1)
    def forward(self): self.camera.move_fore(-0.1)
    def backward(self): self.camera.move_fore(0.1)
    
    def pitchup(self): self.camera.pitch(-0.1)
    def pitchdown(self): self.camera.pitch(0.1)
    def yawleft(self): self.camera.yaw(-0.1)
    def yawright(self): self.camera.yaw(0.1)

    def run(self):
        glutMainLoop()

    def reshape(self, w, h):
        
        glViewport(0, 0, w, h)

        if h == 0: h = 1
        aspect = float(w)/float(h)

        glMatrixMode(GL_PROJECTION); 
        gluPerspective(self.camera.fov, aspect, self.camera.near, self.camera.far)
        glMatrixMode(GL_MODELVIEW);
        
    def display(self):
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT)
        glLoadIdentity()

        (px, py, pz) = self.camera.position
        glTranslatef(px, py, pz)
        glMultMatrixf(self.camera.get_matrix())

        glMaterialfv(GL_FRONT, GL_DIFFUSE, [0.0, 1.0, 0.0, 1.0])
        glutSolidCone(2.0, 5.0, 10, 10)

        #glMaterialfv(GL_FRONT, GL_DIFFUSE, [1.0, 0.0, 0.0, 1.0])
        #glutSolidSphere(2.0, 20, 20)
        
        glutSwapBuffers()

    def keyboard(self, key, x, y):
        if key in self.keymap:
            self.keymap[key]()

    def destroy(self):
        self.window.destroy()

def main():
    app = App()
    app.run()


if __name__ == '__main__': main()

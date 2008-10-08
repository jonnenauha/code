from OpenGL.GLUT import *
from OpenGL.GLU import *
from OpenGL.GL import *
import sys


class Camera(object):
    def __init__(self):
        self.fov = 45.0
        self.near = 1.0
        self.far = 40.0
        self.up = [0,1,0]
        self.eye = [0,0,10]
        self.center = [0,0,0]

class Light(object):
    def __init__(self):
        self.position = [10.0, 10.0, 10.0, 1.0]
        self.diffuse_color = [1.0, 1.0, 1.0, 1.0]
        self.const_attenuation = 0.1
        self.linear_attenuation = 0.05

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
            glLightf(k, GL_CONSTANT_ATTENUATION, v.const_attenuation)
            glLightf(k, GL_LINEAR_ATTENUATION, v.linear_attenuation)
            glEnable(k)

class GLState(GLBaseState, GLLightingState):
    def __init__(self):
        GLBaseState.__init__(self)
        GLLightingState.__init__(self, [Light()])

    def reset(self):
        GLBaseState.reset(self)
        GLLightingState.reset(self)
    
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
        self.keymap = { 'q' : self.destroy }
        self.camera = Camera()
        self.glstate = GLState()
        self.glstate.reset()
        
        glutDisplayFunc(self.display)
        glutReshapeFunc(self.reshape)
        glutKeyboardFunc(self.keyboard)
        
    def run(self):
        glutMainLoop()

    def reshape(self, w, h):
        glViewport(0, 0, w, h)
        
        glMatrixMode(GL_PROJECTION)
        gluPerspective(self.camera.fov, w/h, self.camera.near, self.camera.far)

        (ex,ey,ez) = self.camera.eye
        (cx,cy,cz) = self.camera.center
        (ux,uy,uz) = self.camera.up
        glMatrixMode(GL_MODELVIEW)
        gluLookAt(ex,ey,ez, cx,cy,cz, ux,uy,uz)

    def display(self):
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT)
        glPushMatrix()

        glMaterialfv(GL_FRONT, GL_DIFFUSE, [1.0, 0.0, 0.0, 1.0])
        glutSolidSphere(2,20,20)

        glPopMatrix()
        glutSwapBuffers()

    def keyboard(self, key, x, y):
        self.keymap[key]()

    def destroy(self):
        self.window.destroy()

def main():
    app = App()
    app.run()


if __name__ == '__main__': main()

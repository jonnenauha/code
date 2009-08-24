/* main.h -- exported function header
 *
 *			Ryan McDougall -- 2009
 */

#ifndef _MAIN_H_
#define _MAIN_H_

#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <cmath>

#include <QtGui>
#include <QtOpenGL>

#include <Eigen/Core>

USING_PART_OF_NAMESPACE_EIGEN
using namespace std;

typedef float real_t;
typedef Matrix <real_t,3,1> vector3;
typedef Matrix <real_t,4,1> vector4;

const real_t PI (3.1415926536);
const real_t DEG_TO_RAD (PI/180.0);
const real_t RAD_TO_DEG (180.0/PI);

template <typename T>
void orthonormalize (Matrix <T,3,1> &U, Matrix <T,3,1> &V, Matrix <T,3,1> &W)
{
    // If the input vectors are v0, v1, and v2, then the Gram-Schmidt
    // orthonormalization produces vectors u0, u1, and u2 as follows,
    //
    //   u0 = v0/|v0|
    //   u1 = (v1-(u0*v1)u0)/|v1-(u0*v1)u0|
    //   u2 = (v2-(u0*v2)u0-(u1*v2)u1)/|v2-(u0*v2)u0-(u1*v2)u1|
    //
    // where |A| indicates length of vector A and A*B indicates dot
    // product of vectors A and B.
    real_t dot0, dot1;

    // compute U'
    U.normalize();

    // compute V'
    dot0 = U.dot (V);
    V -= dot0 * U;
    V.normalize();

    // compute W'
    dot1 = V.dot (W);
    dot0 = U.dot (W);
    W -= (dot0 * U) + (dot1 * V);
    W.normalize();
}

struct Frustum
{
    Frustum () 
        : dir_min (0), dir_max (0), 
        up_min (0), up_max (0), 
        right_min (0), right_max (0)
    {}

    void direction_limit (real_t min, real_t max) { dir_min = min; dir_max = max; }
    pair <real_t,real_t> direction_limit () const { return make_pair (dir_min, dir_max); }

    void up_limit (real_t min, real_t max) { up_min = min; up_max = max; }
    pair <real_t,real_t> up_limit () const { return make_pair (up_min, up_max); }

    void right_limit (real_t min, real_t max) { right_min = min; right_max = max; }
    pair <real_t,real_t> right_limit () const { return make_pair (right_min, right_max); }

    void set_frustum (real_t fov_deg, real_t aspect, real_t dmin, real_t dmax)
    {
        real_t halfangle ((real_t)0.5 * fov_deg * DEG_TO_RAD);

        up_max = dmin * tan (halfangle);
        up_min = -up_max;

        right_max = aspect * up_max;
        right_min = -right_max;

        dir_max = dmax;
        dir_min = dmin;
    }

    real_t dir_min, dir_max;
    real_t up_min, up_max;
    real_t right_min, right_max;
    //real_t depth_min, depth_max;
};

struct Viewport
{
    Viewport (int w, int h) : max_width (w), max_height (h) {}

    void width (int l, int r) 
    { 
        assert(0 <= l && l <= max_width);
        assert(0 <= r && r <= max_width);
        left = l; right = r; 
    }

    void height (int t, int b) 
    { 
        assert(0 <= b && b <= max_height);
        assert(0 <= t && t <= max_height);
        top = t; bottom = b; 
    }

    pair <int,int> width () const { return make_pair (left, right); }
    pair <real_t,real_t> uwidth () const { return make_pair (left/max_width, right/max_width); }

    pair <int,int> height () const { return make_pair (top, bottom); }
    pair <real_t,real_t> uheight () const { return make_pair (top/max_height, bottom/max_height); }

    int left, right, top, bottom;
    int max_width, max_height;
};

struct Camera
{
    // always in world coordinates
    // always perspective
    public:
        // default position
        Camera () 
        {
            location << 0, 0, 0;
            direction << 0, 0, -1;
            up << 0, 1, 0;
            right << 1, 0, 0;
        }
        
        virtual ~Camera () {}

        void set_location (vector3 &P) { location = P; }

        void set_frame (vector3 &D, vector3 &U, vector3 &R)
        {
            direction = D;
            up = U;
            right = R;

            normalize_frame();
        }

        void normalize_frame ()
        {
            real_t det (std::abs (direction.dot (up.cross (right))));
            if (std::abs ((real_t)1.0 - det) > (real_t)0.01)
                orthonormalize (direction, up, right);
        }

        vector3 location, direction, up, right;
};

class GeometryPipeline
{
    public:
    private:
};

class GLWidget : public QGLWidget
{
    Q_OBJECT
    public:
        GLWidget(QWidget *parent = 0)
            : QGLWidget (parent)
        {
            xrot = yrot = zrot = 0;

            green = QColor::fromCmykF(0.40, 0.0, 1.0, 0.0);
            purple = QColor::fromCmykF(0.39, 0.39, 0.0, 0.0);
        }

        ~GLWidget () { makeCurrent(); }

        void initializeGL ()
        {
            qglClearColor(purple.dark());
            //object = makeObject();
            glShadeModel(GL_FLAT);
            glEnable(GL_DEPTH_TEST);
            glEnable(GL_CULL_FACE);
        }

        void paintGL ()
        {
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            glLoadIdentity();
            glTranslated(0.0, 0.0, -10.0);
            glRotated(xrot / 16.0, 1.0, 0.0, 0.0);
            glRotated(yrot / 16.0, 0.0, 1.0, 0.0);
            glRotated(zrot / 16.0, 0.0, 0.0, 1.0);
            //glCallList(object);
            draw_scene();
        }

        void resizeGL(int width, int height)
        {
            int side = qMin(width, height);
            glViewport((width - side) / 2, (height - side) / 2, side, side);

            glMatrixMode(GL_PROJECTION);
            glLoadIdentity();
            glOrtho(-0.5, +0.5, +0.5, -0.5, 4.0, 15.0);
            glMatrixMode(GL_MODELVIEW);
        }

        void mousePressEvent (QMouseEvent *event) 
        { 
            pos = event->pos(); 
        }

        void mouseMoveEvent (QMouseEvent *event)
        {
            int dx = event->x() - pos.x();
            int dy = event->y() - pos.y();

            if (event->buttons() & Qt::LeftButton) 
            {
                xrot += 8 * dy;
                yrot += 8 * dx;
            } 
            else if (event->buttons() & Qt::RightButton) 
            {
                xrot += 8 * dy;
                zrot += 8 * dx;
            }

            pos = event->pos();

            updateGL();
        }

        QSize minimumSizeHint () const { return QSize (50, 50); }
        QSize sizeHint () const { return QSize (400, 400); }

    private:

        void draw_scene ()
        {
            glBegin(GL_QUADS);

            GLdouble x1 = +0.06;
            GLdouble y1 = -0.14;
            GLdouble x2 = +0.14;
            GLdouble y2 = -0.06;
            GLdouble x3 = +0.08;
            GLdouble y3 = +0.00;
            GLdouble x4 = +0.30;
            GLdouble y4 = +0.22;

            quad(x1, y1, x2, y2, y2, x2, y1, x1);
            quad(x3, y3, x4, y4, y4, x4, y3, x3);

            extrude(x1, y1, x2, y2);
            extrude(x2, y2, y2, x2);
            extrude(y2, x2, y1, x1);
            extrude(y1, x1, x1, y1);
            extrude(x3, y3, x4, y4);
            extrude(x4, y4, y4, x4);
            extrude(y4, x4, y3, x3);

            const double Pi = 3.14159265358979323846;
            const int NumSectors = 200;

            for (int i = 0; i < NumSectors; ++i) {
                double angle1 = (i * 2 * Pi) / NumSectors;
                GLdouble x5 = 0.30 * sin(angle1);
                GLdouble y5 = 0.30 * cos(angle1);
                GLdouble x6 = 0.20 * sin(angle1);
                GLdouble y6 = 0.20 * cos(angle1);

                double angle2 = ((i + 1) * 2 * Pi) / NumSectors;
                GLdouble x7 = 0.20 * sin(angle2);
                GLdouble y7 = 0.20 * cos(angle2);
                GLdouble x8 = 0.30 * sin(angle2);
                GLdouble y8 = 0.30 * cos(angle2);

                quad(x5, y5, x6, y6, x7, y7, x8, y8);

                extrude(x6, y6, x7, y7);
                extrude(x8, y8, x5, y5);
            }

            glEnd();
        }

        void quad (GLdouble x1, GLdouble y1, GLdouble x2, GLdouble y2, GLdouble x3, GLdouble y3, GLdouble x4, GLdouble y4)
        {
            qglColor(green);

            glVertex3d(x1, y1, -0.05);
            glVertex3d(x2, y2, -0.05);
            glVertex3d(x3, y3, -0.05);
            glVertex3d(x4, y4, -0.05);

            glVertex3d(x4, y4, +0.05);
            glVertex3d(x3, y3, +0.05);
            glVertex3d(x2, y2, +0.05);
            glVertex3d(x1, y1, +0.05);
        }

        void extrude (GLdouble x1, GLdouble y1, GLdouble x2, GLdouble y2)
        {
            qglColor(green.dark(250 + int(100 * x1)));

            glVertex3d(x1, y1, +0.05);
            glVertex3d(x2, y2, +0.05);
            glVertex3d(x2, y2, -0.05);
            glVertex3d(x1, y1, -0.05);
        }

        GLuint object;
        int xrot, yrot, zrot;

        QPoint pos;
        QColor green, purple;
};

class Window : public QWidget
{
    Q_OBJECT

    public:
        Window ()
        {
            widget_ = new GLWidget;

            QHBoxLayout *layout = new QHBoxLayout;
            layout -> addWidget (widget_);
            setLayout (layout);

            setWindowTitle (tr("Qt Render"));
        }

    private:
        GLWidget *widget_;
};

#endif //_MAIN_H_

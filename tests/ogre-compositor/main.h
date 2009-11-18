#ifndef _MAIN_H_
#define _MAIN_H_

#include <QtCore>
#include <QtGui>
#include <QThread>

#ifndef Q_WS_WIN
#include <QX11Info>
#endif

#include <Ogre.h>
#include <OgreConfigFile.h>
#include <OIS/OIS.h>

using Ogre::Real;
using Ogre::Radian;
using Ogre::Vector3;
using Ogre::ColourValue;
using std::cout;
using std::endl;


class WorldModel
{
    public:
        WorldModel (Ogre::SceneManager *sm);
        virtual ~WorldModel ();

    private:
        Ogre::SceneManager      *scenemgr_;
        Ogre::Camera            *camera_;

        Ogre::SceneNode		    *planenode_;

        friend class WorldView;
        friend class WorldController;
};


class WorldController : public Ogre::FrameListener
{
    public:
        WorldController (WorldModel *model) 
            : model_ (model) {}
        
        // rotate the plane
        bool frameStarted (const Ogre::FrameEvent& evt)
        {
            model_-> planenode_-> yaw (Radian (evt.timeSinceLastFrame));

            return Ogre::FrameListener::frameStarted (evt);
        }

    private:
        WorldModel          *model_;
};


class WindowController : public Ogre::WindowEventListener, public Ogre::FrameListener
{
    public: 
        WindowController (Ogre::RenderWindow *win);
        virtual ~WindowController ();

        virtual void windowClosed (Ogre::RenderWindow* rw);

        bool frameStarted (const Ogre::FrameEvent& evt);
        bool frameRenderingQueued (const Ogre::FrameEvent& evt);

    private:
        Ogre::RenderWindow   *win_;

        OIS::InputManager   *inputman_;
        OIS::Keyboard       *keyboard_;
};


class WorldView : public QWidget //: public Ogre::RenderTargetListener
{
    Q_OBJECT

    public:
        WorldView (WorldModel *model, Ogre::RenderWindow *win);
        virtual ~WorldView ();
        
        void RenderOneFrame ();
        void OverlayUI (Ogre::PixelBox &ui);

        const char *GetRenderTargetName () { return "test/texture/UI"; }

    public:
        void paintEvent (QPaintEvent *e) { std::cout << "paintEvent" << std::endl; }
        void resizeEvent (QResizeEvent *e) { std::cout << "resizeEvent" << std::endl; }
		void closeEvent (QCloseEvent *event) { std::cout << "closeEvent" << std::endl; }
		void keyPressEvent (QKeyEvent* event) { std::cout << "keyPressEvent" << std::endl; }
		void keyReleaseEvent (QKeyEvent* event) { std::cout << "keyReleaseEvent" << std::endl; }
		void mousePressEvent (QMouseEvent* event) { std::cout << "mousePressEvent" << std::endl; }
		void mouseReleaseEvent (QMouseEvent* event) { std::cout << "mouseReleaseEvent" << std::endl; }
		void mouseDoubleClickEvent (QMouseEvent* event) { std::cout << "mouseDoubleClickEvent" << std::endl; }
		void mouseMoveEvent (QMouseEvent* event) { std::cout << "mouseMoveEvent" << std::endl; }
		void wheelEvent (QWheelEvent* event) { std::cout << "wheelEvent" << std::endl; }

    public:

        void create_render_texture_ (size_t width, size_t height);

        void adjust_render_texture_to_pixel_box_ (Ogre::PixelBox &dst);

        Ogre::Root              *root_;
        Ogre::TextureManager    *texmgr_;

        Ogre::TexturePtr        texture_;

        Ogre::RenderWindow      *win_;
        Ogre::Viewport          *view_;

        WorldModel              *model_;
};


class Ogre3DApplication
{
    public:
        Ogre3DApplication ();
        virtual ~Ogre3DApplication ();

        WorldModel *GetModel () { return model_; }
        WorldController *GetController () { return controller_; }
        WorldView *GetView () { return view_; }

        void Run () { root_-> startRendering(); }

    protected:
        void setup_resources ();

    private:

        Ogre::Root          *root_;
        Ogre::Camera        *camera_;
        Ogre::SceneManager  *scenemgr_;
        Ogre::RenderWindow  *win_;

        WorldModel          *model_;
        WorldController     *controller_;
        WorldView           *view_;

        WindowController    *winctrl_;
};

class QtApplication : public QApplication
{
    Q_OBJECT

    public:
        QtApplication (int &argc, char **argv);

        QGraphicsView *GetView () { return view_; }
        QGraphicsScene *GetScene () { return scene_; }

    private:
        QGraphicsView   *view_;
        QGraphicsScene   *scene_;
};

class RenderShim : public QObject
{
    Q_OBJECT

    public:
        RenderShim (QGraphicsView *uiview, WorldView *world);

        void Update ();

    protected:
        void timerEvent (QTimerEvent *e) { Update (); }

    private:
        QGraphicsView           *uiview_;
        WorldView               *worldview_;
};

#endif

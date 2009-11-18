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
        
        bool frameStarted (const Ogre::FrameEvent& evt);

    private:
        WorldModel          *model_;
};


class WorldView //: public Ogre::RenderTargetListener
{
    public:
        WorldView (WorldModel *model, Ogre::RenderWindow *win);
        virtual ~WorldView ();

        void RenderOneFrame ();
        void OverlayUI (Ogre::PixelBox &ui);

    public:
        void initialize_ ();

        Ogre::Root              *root_;

        Ogre::Viewport          *view_;
        Ogre::RenderWindow      *win_;

        Ogre::TexturePtr        ui_overlay_texture_;

        WorldModel              *model_;
};

class WorldWindow : public QWidget
{
    public:
        WorldWindow (QWidget *parent = 0);
        virtual ~WorldWindow ();

        Ogre::RenderWindow *GetRenderWindow () { return win_; }

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
        void create_render_window_ ();

        Ogre::RenderWindow      *win_;
};


class Ogre3DApplication
{
    public:
        Ogre3DApplication (QWidget *window = 0);
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

        WorldModel          *model_;
        WorldController     *controller_;
        WorldView           *view_;
        WorldWindow         *win_;
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

class RenderShim : public QWidget
{
    Q_OBJECT

    public:
        RenderShim (QGraphicsView *uiview, WorldView *world);

        void Update ();

    protected:
        void timerEvent (QTimerEvent *e) { Update (); }

    private:
        void create_render_window_ (QWidget *widget, WorldView *view);

        QGraphicsView           *uiview_;
        WorldView               *worldview_;
};

#endif

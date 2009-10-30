#ifndef _MAIN_H_
#define _MAIN_H_

#include <QtCore>
#include <QtGui>
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


class WorldView //: public Ogre::RenderTargetListener
{
    public:
        WorldView (WorldModel *model, Ogre::RenderWindow *win);
        virtual ~WorldView ();
        
        void RenderOneFrame (Ogre::PixelBox &dst);

        const char *GetRenderTargetName ();

    public:

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

        //WindowController    *winctrl_;
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
        RenderShim (QGraphicsView *uiview, WorldView *world) 
            : uiview_ (uiview), worldview_ (world)
        {
            startTimer (20);
            uiview_-> setUpdatesEnabled (false);
            uiview_-> viewport()-> setAttribute (Qt::WA_PaintOnScreen);
            uiview_-> viewport()-> setAttribute (Qt::WA_PaintOutsidePaintEvent);
            uiview_-> viewport()-> setAttribute (Qt::WA_NoSystemBackground);
        }

        void Update ()
        {
            QSize uisize (uiview_-> size());
            QRect uirect (QPoint (0, 0), uisize);

            QImage uibuffer (uisize, QImage::Format_ARGB32);
            QImage worldbuffer (uisize, QImage::Format_ARGB32);
            
            uibuffer.fill (Qt::transparent);
            worldbuffer.fill (Qt::transparent);

            Ogre::Box bounds (0, 0, uisize.width(), uisize.height());
            Ogre::PixelBox worldbufferbox (bounds, Ogre::PF_A8R8G8B8, (void *) worldbuffer.bits());

            // draw the world view
            worldview_-> RenderOneFrame (worldbufferbox);
            
            // draw the ui view
            QPainter compositor (&uibuffer);
            uiview_-> render (&compositor);

            // composit the two together
            compositor.setCompositionMode (QPainter::CompositionMode_DestinationOver);
            compositor.drawImage (uirect, worldbuffer);

            // draw the composit to the widget
            QPainter widgetpainter (uiview_-> viewport());
            widgetpainter.drawImage (uirect, uibuffer);
        }

    protected:
        void timerEvent (QTimerEvent *e) { Update (); }

    private:
        QGraphicsView           *uiview_;
        WorldView               *worldview_;
};

#endif

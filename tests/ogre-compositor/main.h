#ifndef _MAIN_H_
#define _MAIN_H_

#include <QtCore>
#include <QtGui>
#include <QThread>

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

        const char *GetRenderTargetName () { return "world-scene-off-screen-texture"; }

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


class RedirectedGraphicsView : public QGraphicsView
{
    Q_OBJECT

    public:

        void Render (QPainter &painter, QRect &target)
        {
            painter.drawImage (target, *backbuffer_); // should map 1-1??
        }

        void CleanUsedBuffers ()
        {
            std::list<QImage *>::iterator it (used_buffers_.begin());
            for (; it != used_buffers_.end(); ++it)
                delete *it;
        }

    protected:

        void resizeEvent (QResizeEvent *e)
        {
            QGraphicsView::resizeEvent (e);

            QSize maxsize = (backbuffer_)? 
                viewport()-> size().expandedTo (backbuffer_-> size()) : 
                viewport()-> size();

            if (backbuffer_ && (backbuffer_-> size() != maxsize))
            {
                used_buffers_.push_back (backbuffer_);
                backbuffer_ = NULL;
            }
                
            if (!backbuffer_)
                backbuffer_ = new QImage (maxsize, QImage::Format_ARGB32);
        }
        
        void paintEvent (QPaintEvent *e)
        {
            QRect exposed (e-> region().boundingRect());
            QRectF bufexposed (exposed); // buffer should map 1-1??

            // paint to back buffer
            QPainter painter (backbuffer_);
            
            // clean previously dirty rect
            //painter.fillRect (dirty_, Qt::transparent);
            backbuffer_-> fill (Qt::transparent);

            // draw new exposed rect
            QGraphicsView::render (&painter, bufexposed, exposed);
            
            // store this expose
            dirty_ = exposed;
        }

    private:

        QImage     *backbuffer_;
        QRect       dirty_;

        std::list <QImage *> used_buffers_;
};

class QtApplication : public QApplication
{
    Q_OBJECT

    public:
        QtApplication (int &argc, char **argv);

        //RedirectedGraphicsView *GetView () { return view_; }
        QGraphicsView *GetView () { return view_; }
        QGraphicsScene *GetScene () { return scene_; }

    private:

        //RedirectedGraphicsView  *view_;
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
        //RedirectedGraphicsView           *uiview_;
        QGraphicsView           *uiview_;
        WorldView               *worldview_;
};

#endif

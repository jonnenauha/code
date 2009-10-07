/* main.h -- exported function header
 *
 *			Ryan McDougall -- 2009
 */

#ifndef _MAIN_H_
#define _MAIN_H_

#include <QtGui>

#ifndef Q_WS_WIN
#include <QX11Info>
#endif

#include <OGRE/Ogre.h>

using std::cout;
using std::endl;

class OgreWidget : public QWidget
{
    Q_OBJECT

    public:
        OgreWidget (Ogre::Root *root, QWidget *parent = 0)
            : QWidget (parent), root_ (root), win_ (NULL)
        {
            setAttribute (Qt::WA_PaintOnScreen);
            setAttribute (Qt::WA_PaintOutsidePaintEvent);
            setAttribute (Qt::WA_NoSystemBackground);
            createRenderWindow ();
        }

        ~OgreWidget () {}

        // forces rendering of a frame
        void Update ()
        {
            if (win_) 
            {
                root_-> _fireFrameStarted();
                win_-> update();
                root_-> _fireFrameEnded();
            }
        }

    protected:

        // Qt overrides
        QSize minSizeHint () const { return QSize (100, 100); }
        
        void paintEvent (QPaintEvent *e) { Update (); }

        void resizeEvent(QResizeEvent *e) { if (win_) resizeRenderWindow (); }

        // ogre-specific render window handling
        void createRenderWindow ()
        {
            if (win_) return;

            Ogre::NameValuePairList params;

#ifndef Q_WS_WIN
            QX11Info info = x11Info();
            Ogre::String winhandle;

            winhandle = Ogre::StringConverter::toString 
                (reinterpret_cast<unsigned long> 
                 (info.display()));
            winhandle += ":";

            winhandle += Ogre::StringConverter::toString 
                (static_cast<unsigned int> 
                 (info.screen()));
            winhandle += ":";

            winhandle += Ogre::StringConverter::toString 
                (static_cast<unsigned long> 
                 (parentWidget()? 
                  parentWidget()->winId() : winId()));

            params["parentWindowHandle"] = winhandle;
#else
            params["externalWindowHandle"] = 
                Ogre::StringConverter::toString
                ((size_t)(HWND) winId());
#endif

            win_ = root_-> createRenderWindow ("View", width(), height(), false, &params);

            // take over ogre window
            // needed with parent windows
            if (parentWidget())
            {
                WId ogre_winid = 0x0;
                win_-> getCustomAttribute ("WINDOW", &ogre_winid);
                assert (ogre_winid);
                create (ogre_winid);
            }
        }

        void resizeRenderWindow ()
        {
            win_-> resize (width(), height());
            win_-> windowMovedOrResized ();
        }

        // implementation variables
        Ogre::RenderWindow  *win_;
        Ogre::Root          *root_;
};

class TestWidget : public OgreWidget
{
    Q_OBJECT

    public:

        TestWidget (Ogre::Root *root, QWidget *parent = 0)
            : OgreWidget (root, parent)
        {
            sceneman = root_->createSceneManager (Ogre::ST_GENERIC);

            // Create the camera
            cam = sceneman->createCamera ("PlayerCam");
            cam->setPosition (Ogre::Vector3(0, 0, 200));

            // Look back along -Z
            cam->lookAt (Ogre::Vector3(0, 0, -300));
            cam->setNearClipDistance (5);

            // Create one viewport, entire window
            viewport = win_->addViewport (cam);
            viewport->setBackgroundColour (Ogre::ColourValue(0,0,0));
            viewport->setClearEveryFrame (true);

            sceneman->setAmbientLight (Ogre::ColourValue(0.6, 0.6, 0.6));

            // Setup the actual scene
            Ogre::Light* l = sceneman->createLight ("MainLight");
            l->setPosition(0, 100, 500);

            Ogre::Entity* mesh = sceneman->createEntity ("mesh", "dwarf.mesh");
            mainnode = sceneman->getRootSceneNode()->createChildSceneNode();
            mainnode->attachObject (mesh);

            cam->setAutoTracking (true, mainnode);

            Ogre::MaterialManager::getSingleton().setDefaultTextureFiltering (Ogre::TFO_BILINEAR);
            Ogre::MaterialManager::getSingleton().setDefaultAnisotropy (1);

            // Alter the camera aspect ratio to match the viewport
            cam->setAspectRatio (Ogre::Real(viewport->getActualWidth()) / Ogre::Real(viewport->getActualHeight()));
        }

        void Update () { OgreWidget::Update (); }

    protected:
        
        void mousePressEvent (QMouseEvent *e) 
        {
            mousepos = e-> pos();
            if (mainnode) orientation = mainnode-> getOrientation();
            mousepressed = true;
        }
        
        void mouseReleaseEvent (QMouseEvent *) { mousepressed = false; }
        
        void mouseMoveEvent (QMouseEvent *e) 
        {
            if (mousepressed) 
            {
                QPoint curPos = e-> pos();

                double w = width();
                double h = height();

                double curX = (curPos.x() * 2. - w) / w;
                double curY = (h - curPos.y() * 2.) / h;
                double x0 = (mousepos.x() * 2. - w) / w;
                double y0 = (h - mousepos.y() * 2.) / h;

                Ogre::Vector3 v1(x0, y0, 0);
                Ogre::Vector3 v2(curX, curY, 0);

                double radiusSqr = 0.8 * 0.8;
                double cutoff = radiusSqr * 0.5;
                double Rho = v1[0] * v1[0] + v1[1] * v1[1];
                v1[2] = (Rho < cutoff) ? sqrt(radiusSqr - Rho) : (cutoff / sqrt(Rho));

                Rho = v2[0] * v2[0] + v2[1] * v2[1];
                v2[2] = (Rho < cutoff) ? sqrt(radiusSqr - Rho) : (cutoff / sqrt(Rho));

                // v_cross is the normal of rotating plane
                Ogre::Vector3 cross = v2.crossProduct(v1);
                cross.normalise();

                // compute the angle
                v1.normalise();
                v2.normalise();
                double cosAngle = v1.dotProduct(v2);
                if (cosAngle < -1.0)
                    cosAngle = -1.0;
                else if(cosAngle > 1.0)
                    cosAngle = 1.0;
                double angle = acos(cosAngle);

                mainnode->rotate(cross, Ogre::Radian(angle));

                mousepos = curPos;
                orientation = mainnode->getOrientation();
            }
        }
        
        QSize minSizeHint () const { return QSize (100, 100); }

        void paintEvent (QPaintEvent *e) { Update (); }

        void resizeEvent(QResizeEvent *e)
        {
            OgreWidget::resizeEvent (e);

            if (win_) 
            {
                cam-> setAspectRatio (Ogre::Real(width()) / Ogre::Real(height()));
                viewport-> update();
            }
        }

        bool mousepressed;
        QPoint mousepos;
        
        Ogre::Quaternion orientation;
        Ogre::SceneNode *mainnode;
        Ogre::SceneManager *sceneman;
        Ogre::Camera *cam;
        Ogre::Viewport *viewport;
};

class RedirectedQGraphicsView : public QGraphicsView
{
    public:
        RedirectedQGraphicsView (QPainter *r, QGraphicsScene *s, QWidget *p = 0)
            : QGraphicsView (s, p), redirpainter_ (r) {}

        void Update () { render (redirpainter_, dst_, viewport()-> rect()); }
    
        void Realize ()
        {
            show ();
            //hide ();
        }

    protected:
        bool event (QEvent *e)
        {
            cout << "uiview event : " << e-> type() << endl;
            QGraphicsView::event (e);
        }

        void mouseMoveEvent (QMouseEvent *e)
        {
            cout << "uiview mouseMoveEvent : " << e-> type() << endl;
            QGraphicsView::mouseMoveEvent (e);
        }

        void resizeEvent (QResizeEvent *e) 
        { 
            const QSize &s (e-> size());
            dst_ = QRect (0, 0, s.width (), s.height ());
        }

    private:
        QRect       dst_; 
        QPainter    *redirpainter_;
};

class SceneManager : public QObject
{
    Q_OBJECT

    public:
        SceneManager (Ogre::Root *scene_root);

        void Start () { mainwin-> show(); }

        void Update ()
        {
            worldscene-> Update ();
            uiview-> Update ();
        }

    protected:

        void timerEvent (QTimerEvent *e) { Update (); }

        bool eventFilter (QObject *o, QEvent *e);

        QWidget         *mainwin;
        QVBoxLayout     *mainlay;

        QPainter        *mainpainter;

        QLineEdit       *line;

        QGraphicsScene          *uiscene;
        RedirectedQGraphicsView *uiview;
        
        OgreWidget      *worldscene;
};

#endif //_MAIN_H_

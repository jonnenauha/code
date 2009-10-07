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
        OgreWidget (Ogre::Root *root, QWidget *parent = 0);
        ~OgreWidget () {}

        // forces rendering of a frame
        void Update ();

    protected:

        // Qt overrides
        QSize minSizeHint () const { return QSize (100, 100); }
        void paintEvent (QPaintEvent *e) { Update (); }
        void resizeEvent(QResizeEvent *e) { if (win_) resizeRenderWindow (); }

        // ogre-specific render window handling
        void createRenderWindow ();
        void resizeRenderWindow ();

        // implementation variables
        Ogre::RenderWindow  *win_;
        Ogre::Root          *root_;
};

class TestWidget : public OgreWidget
{
    Q_OBJECT

    public:

        TestWidget (Ogre::Root *root, QWidget *parent = 0);
        ~TestWidget () {}

        void Update () { OgreWidget::Update (); }

    protected:
        
        void mousePressEvent (QMouseEvent *e);
        void mouseReleaseEvent (QMouseEvent *e);
        void mouseMoveEvent (QMouseEvent *e);
        
        void paintEvent (QPaintEvent *e) { Update (); }
        void resizeEvent (QResizeEvent *e);

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
            //setAttribute (Qt::WA_DontShowOnScreen); // seems to work as hide()
        }

    protected:
        bool event (QEvent *e)
        {
            cout << " uiview event : " << e-> type() << endl;
            return QGraphicsView::event (e);
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
        SceneManager (Ogre::Root *scene_root)
        {
            mainwin = new QWidget;
            mainlay = new QVBoxLayout;

            worldscene = new TestWidget (scene_root, mainwin);

            mainpainter = new QPainter (worldscene);

            uiscene = new QGraphicsScene;
            uiview = new RedirectedQGraphicsView (mainpainter, uiscene);

            line = new QLineEdit;
            line-> setText ("wtf");

            mainlay-> addWidget (worldscene);
            mainwin-> setLayout (mainlay);
            mainwin-> setMinimumSize (100, 100);

            uiscene-> addWidget (line);

            worldscene-> installEventFilter (this);
            uiscene-> installEventFilter (this);
            uiview-> installEventFilter (this);

            uiview-> Realize ();

            startTimer (33);
        }

        void Start () { mainwin-> show(); }

        void Update ()
        {
            worldscene-> Update ();
            uiview-> Update ();
        }

    protected:

        void timerEvent (QTimerEvent *e) { Update (); }

        bool eventFilter (QObject *o, QEvent *e)
        {
            // paint manually in the timer to avoid flicker
            if (e-> type() == QEvent::Paint)
                return true;

            // forward to uiview
            if (o == worldscene)
            {
                cout << "forwarded: " << e-> type();
                if (e-> spontaneous()) cout << " (spontaneous) ";
                bool result = QApplication::sendEvent (uiview, e);
                if (!result) cout << " not"; cout << " accepted" << endl;
            }

            return false;
        }


        QWidget         *mainwin;
        QVBoxLayout     *mainlay;

        QPainter        *mainpainter;

        QLineEdit       *line;

        QGraphicsScene          *uiscene;
        RedirectedQGraphicsView *uiview;
        
        OgreWidget      *worldscene;
};

#endif //_MAIN_H_

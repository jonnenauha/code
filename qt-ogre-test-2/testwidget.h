/* testwidget.h -- 
 *
 *			Ryan McDougall -- 2009
 */

#ifndef _TESTWIDGET_H_
#define _TESTWIDGET_H_

#include "main.h"
#include "ogrewidget.h"

class TestWidget : public OgreWidget
{
    Q_OBJECT

    public:

        TestWidget (Ogre::Root *root, QWidget *parent = 0);

        void mousePressEvent (QMouseEvent *e);
        void mouseReleaseEvent (QMouseEvent *e);
        void mouseMoveEvent (QMouseEvent *e);
        
        void resizeEvent (QResizeEvent *e);

    protected:

        bool mousepressed;
        QPoint mousepos;
        
        Ogre::Quaternion orientation;
        Ogre::SceneNode *mainnode;
        Ogre::SceneManager *sceneman;
        Ogre::Camera *cam;
        Ogre::Viewport *viewport;
};

//        void timerEvent (QTimerEvent *e) { draw_to_render_window (); }

#endif //_TESTWIDGET_H_

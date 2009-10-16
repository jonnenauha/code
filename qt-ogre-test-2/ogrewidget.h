/* ogrewidget.h
 *
 *			Ryan McDougall -- 2009
 */

#ifndef _OGREWIDGET_H_
#define _OGREWIDGET_H_

#include "main.h"

#ifndef Q_WS_WIN
#include <QX11Info>
#endif

class OgreWidget : public QWidget
{
    Q_OBJECT

    public:
        OgreWidget (Ogre::Root *root, QWidget *parent = 0);
        virtual ~OgreWidget ();

        void draw_to_render_window ();
        void create_render_window ();
        void resize_render_window ();

        void paintEvent (QPaintEvent *e) { /*std::cout << "paintEvent" << std::endl;*/ draw_to_render_window (); }
        void resizeEvent (QResizeEvent *e) { /*std::cout << "resizeEvent" << std::endl;*/ resize_render_window (); }
		//void closeEvent (QCloseEvent *event) { std::cout << "closeEvent" << std::endl; }
		//void keyPressEvent (QKeyEvent* event) { std::cout << "keyPressEvent" << std::endl; }
		//void keyReleaseEvent (QKeyEvent* event) { std::cout << "keyReleaseEvent" << std::endl; }
		//void mousePressEvent (QMouseEvent* event) { std::cout << "mousePressEvent" << std::endl; }
		//void mouseReleaseEvent (QMouseEvent* event) { std::cout << "mouseReleaseEvent" << std::endl; }
		//void mouseDoubleClickEvent (QMouseEvent* event) { std::cout << "mouseDoubleClickEvent" << std::endl; }
		//void mouseMoveEvent (QMouseEvent* event) { std::cout << "mouseMoveEvent" << std::endl; }
		//void wheelEvent (QWheelEvent* event) { std::cout << "wheelEvent" << std::endl; }

    protected:

        Ogre::RenderWindow  *win_;
        Ogre::Root          *root_;

        QPixmap             *buffer_;
    
    private:
        OgreWidget (const OgreWidget&);
        void operator= (const OgreWidget&);
};

#endif

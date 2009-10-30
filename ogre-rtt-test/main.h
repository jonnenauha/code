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

class AppController : public QObject
{
    Q_OBJECT

    public:
        AppController (QGraphicsView *view) 
            : uiview_ (view), 
              worldtexmgr_ (Ogre::TextureManager::getSingleton()), 
              root_ (Ogre::Root::getSingleton())
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

            root_.renderOneFrame();
            
            QPainter compositor (&uibuffer);
            uiview_-> render (&compositor);

            Ogre::TexturePtr tex = worldtexmgr_.getByName ("RttTex");
            tex-> getBuffer()-> blitToMemory (worldbufferbox);

            compositor.setCompositionMode (QPainter::CompositionMode_DestinationOver);
            compositor.drawImage (uirect, worldbuffer);

            QPainter widgetpainter (uiview_-> viewport());
            widgetpainter.drawImage (uirect, uibuffer);
        }

    protected:
        void timerEvent (QTimerEvent *e) { Update (); }

    private:
        QGraphicsView           *uiview_;

        Ogre::TextureManager    &worldtexmgr_;
        Ogre::Root              &root_;
};

#endif

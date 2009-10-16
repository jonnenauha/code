/* ogrewidget.cpp
 *
 *			Ryan McDougall -- 2009
 */

#include "main.h"
#include "ogrewidget.h"

//=============================================================================

OgreWidget::OgreWidget (Ogre::Root *root, QWidget *parent) : 
    QWidget (parent), 
    root_ (root), 
    win_ (NULL), 
    buffer_ (NULL)
{
    setAttribute (Qt::WA_PaintOnScreen);
    setAttribute (Qt::WA_PaintOutsidePaintEvent);
    setAttribute (Qt::WA_NoSystemBackground);

    setFocusPolicy (Qt::StrongFocus);

    create_render_window ();
}

OgreWidget::~OgreWidget ()
{
    // TODO: what is proper Ogre clean-up?
}

void OgreWidget::draw_to_render_window ()
{
    if (win_) 
    {
        root_-> _fireFrameStarted();
        win_-> update();
        root_-> _fireFrameRenderingQueued ();
        root_-> _fireFrameEnded();
    }
}

void OgreWidget::create_render_window ()
{
    if (win_) return;

    bool stealparent (parentWidget() != NULL);
    QWidget *nativewin ((stealparent)? parentWidget () : this);

    Ogre::NameValuePairList params;
    Ogre::String winhandle;

#ifdef Q_WS_WIN
    // According to Ogre Docs
    // positive integer for W32 (HWND handle)
    winhandle = Ogre::StringConverter::toString 
        ((unsigned int) 
         (nativewin-> winId ()));

    //Add the external window handle parameters to the existing params set.
    params["externalWindowHandle"] = winhandle;
#endif

#ifdef Q_WS_X11
    // GLX - According to Ogre Docs:
    // poslong:posint:poslong:poslong (display*:screen:windowHandle:XVisualInfo*)
    QX11Info info = x11Info ();

    winhandle  = Ogre::StringConverter::toString 
        ((unsigned long)
         (info.display ()));
    winhandle += ":";

    winhandle += Ogre::StringConverter::toString 
        ((unsigned int)
         (info.screen ()));
    winhandle += ":";
    
    winhandle += Ogre::StringConverter::toString 
        ((unsigned long)
         nativewin-> winId());
         //(isHidden ()) ? 
         //nativewin-> effectiveWinId () : 
         //nativewin-> winId ());

    //Add the external window handle parameters to the existing params set.
    params["parentWindowHandle"] = winhandle;
#endif

    win_ = root_-> createRenderWindow ("View", width(), height(), false, &params);

    // take over ogre window
    // needed with parent windows
    if (stealparent)
    {
        WId ogre_winid = 0x0;
#ifndef Q_WS_WIN
        win_-> getCustomAttribute ("WINDOW", &ogre_winid);
#else
        win_-> getCustomAttribute ("HWND", &ogre_winid);
#endif
        assert (ogre_winid);
        create (ogre_winid);
    }
}

void OgreWidget::resize_render_window ()
{
    if (win_)
    {
        win_-> resize (width (), height ());
        win_-> windowMovedOrResized ();
    }
}


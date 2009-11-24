#include "main.h"

//=============================================================================
// WorldModel

WorldModel::WorldModel (Ogre::SceneManager *sm) : 
    scenemgr_ (sm)
{
    // Create the camera
    camera_ = scenemgr_->createCamera("PlayerCam");

    // Position it at 500 in Z direction
    camera_->setPosition(Vector3(0,0,500));

    // Look back along -Z
    camera_->lookAt(Vector3(0,0,-300));
    camera_->setNearClipDistance(5);

    // Set ambient light
    scenemgr_->setAmbientLight(ColourValue(0.2f, 0.2f, 0.2f));

    // Create a light
    scenemgr_->createLight("MainLight")->setPosition(20, 80, 50);

    // Position the camera
    camera_->setPosition(60, 200, 70);
    camera_->lookAt(0, 0, 0);

    // Create a simple plane
    Ogre::MovablePlane *plane = new Ogre::MovablePlane ("Plane");
    plane->d = 0;
    plane->normal = Vector3::UNIT_Y;

    Ogre::MeshManager::getSingleton().
        createPlane ("PlaneMesh", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, 
                *plane, 120, 120, 1, 1, true, 1, 1, 1, Vector3::UNIT_Z);

    Ogre::Entity *planeent = scenemgr_-> createEntity ("PlaneEntity", "PlaneMesh");
    planeent-> setMaterialName ("PlaneMat");

    // Attach the plane to a scene node
    planenode_ = scenemgr_-> getRootSceneNode()-> createChildSceneNode ();
    planenode_-> attachObject (planeent);
}

WorldModel::~WorldModel ()
{
}


//=============================================================================
// rotate the plane on every frame

bool WorldController::frameStarted (const Ogre::FrameEvent& evt)
{
    model_-> planenode_-> yaw (Radian (evt.timeSinceLastFrame));

    return Ogre::FrameListener::frameStarted (evt);
}


//=============================================================================
//

WorldView::WorldView (WorldModel *model, Ogre::RenderWindow *win) :
    model_ (model), 
    win_ (win)
{
    root_ = Ogre::Root::getSingletonPtr();
    initialize_ ();
}

void WorldView::initialize_ ()
{ 
    // Create one viewport, entire window
    view_ = win_-> addViewport (model_->camera_);
    view_-> setBackgroundColour (ColourValue (0,0,0));

    // Alter the camera aspect ratio to match the viewport
    model_-> camera_-> setAspectRatio 
        (Real (view_-> getActualWidth()) / 
         Real (view_-> getActualHeight()));

    // set up off-screen texture
    ui_overlay_texture_ =
        Ogre::TextureManager::getSingleton().createManual
        ("test/texture/UI",
         Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, 
         Ogre::TEX_TYPE_2D, win_-> getWidth(), win_-> getHeight(), 0, 
         Ogre::PF_A8R8G8B8, Ogre::TU_DYNAMIC_WRITE_ONLY_DISCARDABLE);

    Ogre::MaterialPtr material
        (Ogre::MaterialManager::getSingleton().create
         ("test/material/UI", 
          Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME));

    Ogre::TextureUnitState *state 
        (material->getTechnique(0)->getPass(0)->createTextureUnitState());

    state-> setTextureName ("test/texture/UI");

    material->getTechnique(0)->getPass(0)->setSceneBlending
        (Ogre::SBF_SOURCE_ALPHA, Ogre::SBF_ONE_MINUS_SOURCE_ALPHA);

    // set up overlays
    ui_overlay_ = 
        (Ogre::OverlayManager::getSingleton().create 
         ("test/overlay/UI"));

    ui_overlay_container_ =
        (Ogre::OverlayManager::getSingleton().createOverlayElement 
         ("Panel", "test/overlay/UIPanel"));

    ui_overlay_container_-> setMaterialName ("test/material/UI");

    ui_overlay_-> add2D (static_cast <Ogre::OverlayContainer *> (ui_overlay_container_));
    ui_overlay_-> show ();
}

WorldView::~WorldView ()
{
}

void WorldView::Resize (int width, int height)
{
    if (win_ && Ogre::TextureManager::getSingletonPtr())
    {
        ui_overlay_texture_-> freeInternalResources ();
        ui_overlay_texture_-> setWidth (width);
        ui_overlay_texture_-> setHeight (height);
        ui_overlay_texture_-> createInternalResources ();

        win_-> resize (width, height);
        win_-> windowMovedOrResized ();
    }
}

void WorldView::RenderOneFrame ()
{
    root_-> _fireFrameStarted ();
    win_-> update();
    root_-> _fireFrameRenderingQueued ();
    root_-> _fireFrameEnded ();
}

void WorldView::OverlayUI (Ogre::PixelBox &ui)
{
    ui_overlay_texture_-> getBuffer()-> blitFromMemory (ui);
}


//=============================================================================
//

QOgreUIView::QOgreUIView () :
    QGraphicsView (),
    view_ (0)
{
    initialize_ ();
}

QOgreUIView::QOgreUIView (QGraphicsScene *scene) : 
    QGraphicsView (scene),
    view_ (0)
{
    initialize_ ();
}

QOgreUIView::~QOgreUIView ()
{
}

void QOgreUIView::initialize_ ()
{
    //setUpdatesEnabled (false);
    //setAttribute (Qt::WA_PaintOnScreen);
    //setAttribute (Qt::WA_PaintOutsidePaintEvent);
    //setAttribute (Qt::WA_NoSystemBackground);

    setFocusPolicy (Qt::StrongFocus);
    setViewportUpdateMode (QGraphicsView::FullViewportUpdate);
}    

Ogre::RenderWindow *QOgreUIView::CreateRenderWindow ()
{
    bool stealparent 
        ((parentWidget())? true : false);

    QWidget *nativewin 
        ((stealparent)? parentWidget() : this);

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
    QX11Info info =  x11Info ();

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

    //Add the external window handle parameters to the existing params set.
    params["parentWindowHandle"] = winhandle;
#endif

    QSize size (nativewin-> size());

    QGraphicsView *view = static_cast <QGraphicsView *> (nativewin);
    if (view) size = view-> viewport()-> size();

    Ogre::RenderWindow *window = Ogre::Root::getSingletonPtr()-> 
        createRenderWindow ("View", size.width(), size.height(), false, &params);

    // take over ogre window
    // needed with parent windows
    if (stealparent)
    {
        WId ogre_winid = 0x0;
#ifndef Q_WS_WIN
        window-> getCustomAttribute ("WINDOW", &ogre_winid);
#else
        window-> getCustomAttribute ("HWND", &ogre_winid);
#endif
        assert (ogre_winid);
        create (ogre_winid);
    }

    return window;
}

void QOgreUIView::resizeEvent (QResizeEvent *e)
{
    QGraphicsView::resizeEvent (e);
    if (view_) view_-> Resize (viewport()-> width(), viewport()-> height());
}


//=============================================================================

Ogre3DApplication::Ogre3DApplication (QOgreUIView *uiview)
{
    try 
    {
        root_ = new Ogre::Root ();

        setup_resources ();
        root_-> restoreConfig();

        root_-> initialise (false);
        win_ = uiview-> CreateRenderWindow ();

        scenemgr_ = root_-> createSceneManager (Ogre::ST_GENERIC, "SceneManager");

        Ogre::ResourceGroupManager::getSingleton().  initialiseAllResourceGroups ();

        model_ = new WorldModel (scenemgr_);
        controller_ = new WorldController (model_);
        view_ = new WorldView (model_, win_);

        uiview-> SetWorldView (view_);
        root_-> addFrameListener (controller_);
    }
    catch (Ogre::Exception &e)
    {
        std::cerr << e.getFullDescription() << std::endl;
        throw;
    }
}

Ogre3DApplication::~Ogre3DApplication ()
{
    delete controller_;
    delete view_;
    delete model_;

    delete root_;
}

void Ogre3DApplication::setup_resources ()
{
    Ogre::String sec_name, type_name, arch_name;
    Ogre::ConfigFile cf;
    cf.load ("resources.cfg");

    Ogre::ConfigFile::SectionIterator seci = cf.getSectionIterator();
    while (seci.hasMoreElements())
    {
        sec_name = seci.peekNextKey();
        Ogre::ConfigFile::SettingsMultiMap *settings = seci.getNext();
        Ogre::ConfigFile::SettingsMultiMap::iterator i;
        for (i = settings->begin(); i != settings->end(); ++i)
        {
            type_name = i->first;
            arch_name = i->second;

            Ogre::ResourceGroupManager::getSingleton().
                addResourceLocation (arch_name, type_name, sec_name);
        }
    }
}


//=============================================================================
//

QtApplication::QtApplication (int &argc, char **argv) : 
    QApplication (argc, argv)
{
    view_ = new QOgreUIView ();
    scene_ = new QGraphicsScene ();

    QDialog *dialog1 = new QDialog ();
    QDialog *dialog2 = new QDialog ();

    dialog1-> setWindowOpacity (0.8);
    dialog1-> setWindowTitle ("testing baby");
    dialog1-> setLayout (new QVBoxLayout);
    dialog1-> layout()-> addWidget (new QLineEdit);

    dialog2-> setWindowOpacity (0.8);
    dialog2-> setWindowTitle ("you suck");
    dialog2-> setLayout (new QVBoxLayout);
    dialog2-> layout()-> addWidget (new QLineEdit);

    scene_-> addWidget (dialog1);
    scene_-> addWidget (dialog2);
    view_-> setScene (scene_);

    QGraphicsItem *item1 (view_-> items().takeAt (0));
    item1-> setFlag (QGraphicsItem::ItemIsMovable);
    item1-> setCacheMode (QGraphicsItem::DeviceCoordinateCache);
    item1-> setPos (10, 50);

    QGraphicsItem *item2 (view_-> items().takeAt (1));
    item2-> setFlag (QGraphicsItem::ItemIsMovable);
    item2-> setCacheMode (QGraphicsItem::DeviceCoordinateCache);
    item2-> setPos (50, 50);

    view_-> resize (1024, 768);
    view_-> show ();
}

//=============================================================================
//

QOgreRenderShim::QOgreRenderShim (QGraphicsView *uiview, WorldView *world) : 
    uiview_ (uiview), worldview_ (world)
{
    startTimer (20);
}

void QOgreRenderShim::Update ()
{
    QSize viewsize (uiview_-> viewport()-> size());
    QRect viewrect (QPoint (0, 0), viewsize);

    // compositing back buffer
    QImage buffer (viewsize, QImage::Format_ARGB32);
    QPainter painter (&buffer);
    
    // paint ui view into buffer
    uiview_-> render (&painter);

    // blit ogre view into buffer
    Ogre::Box bounds (0, 0, viewsize.width(), viewsize.height());
    Ogre::PixelBox bufbox (bounds, Ogre::PF_A8R8G8B8, (void *) buffer.bits());
    
    worldview_-> OverlayUI (bufbox);
    worldview_-> RenderOneFrame ();
}

//=============================================================================
//


#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
#define WIN32_LEAN_AND_MEAN
#include "windows.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

int main(int argc, char **argv)
{
    QtApplication qtapp (argc, argv);
    Ogre3DApplication ogreapp (qtapp.GetView());
    QOgreRenderShim shim (qtapp.GetView(), ogreapp.GetView());

    return qtapp.exec();
}

#ifdef __cplusplus
}
#endif



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

    // Create a material for the plane (just a simple texture, here grass.jpg)
    Ogre::MaterialPtr m = Ogre::MaterialManager::getSingleton().
        create ("PlaneMat", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);

    Ogre::TextureUnitState* t = m->getTechnique(0)->getPass(0)->
        createTextureUnitState ("test.jpg");

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
//

WorldView::WorldView (WorldModel *model, Ogre::RenderWindow *win) : 
    model_ (model), 
    win_ (win)
{ 
    root_ = Ogre::Root::getSingletonPtr();
    texmgr_ = Ogre::TextureManager::getSingletonPtr();

    if (false)
    {
        // Create one viewport, entire window
        view_ = win_-> addViewport (model_->camera_);
        view_-> setBackgroundColour (ColourValue (0,0,0));

        // Alter the camera aspect ratio to match the viewport
        model_-> camera_-> setAspectRatio 
            (Real (view_-> getActualWidth()) / 
             Real (view_-> getActualHeight()));
    }
    else
    {
        // window render target can be removed
        root_-> getRenderSystem()-> detachRenderTarget ("OGRE Render Window");

        // TODO: unmap the empty window, as it's still displayed
    }

    // Create the off-screen RTT texture
    texture_ = texmgr_-> 
        createManual (GetRenderTargetName(), 
                Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, Ogre::TEX_TYPE_2D, 
                800/*win_->getWidth()*/, 600/*win_->getHeight()*/, 0, Ogre::PF_R8G8B8, Ogre::TU_RENDERTARGET); 

    // Set up texture as RTT
    Ogre::RenderTexture *rendertex (texture_-> getBuffer()-> getRenderTarget());
    rendertex-> addViewport (model_-> camera_);
    rendertex-> getViewport (0)-> setClearEveryFrame (true);
    rendertex-> getViewport (0)-> setBackgroundColour (ColourValue::Black);
    rendertex-> getViewport (0)-> setOverlaysEnabled (false);
    //rendertex-> addListener (this); // this was used for the mini-view
}

WorldView::~WorldView ()
{
}

void WorldView::RenderOneFrame (Ogre::PixelBox &dst)
{
    root_-> renderOneFrame();
    texture_-> getBuffer()-> blitToMemory (dst);
}

const char *WorldView::GetRenderTargetName ()
{
    return "world-scene-off-screen-texture";
}


//=============================================================================
// WindowController

WindowController::WindowController (Ogre::RenderWindow *win) : 
    win_ (win)
{
    Ogre::LogManager::getSingletonPtr()-> logMessage ("Initializing OIS");

    OIS::ParamList pl;
    std::ostringstream winidstr; size_t winid = 0;

    win-> getCustomAttribute ("WINDOW", &winid); winidstr << winid;
    pl.insert (std::make_pair (std::string ("WINDOW"), winidstr.str()));

    inputman_ = OIS::InputManager::createInputSystem (pl);
    keyboard_ = static_cast <OIS::Keyboard*> (inputman_-> createInputObject (OIS::OISKeyboard, false));

    Ogre::WindowEventUtilities::addWindowEventListener (win_, this);
}

WindowController::~WindowController ()
{
    Ogre::WindowEventUtilities::removeWindowEventListener (win_, this);
    windowClosed (win_);
}

void WindowController::windowClosed (Ogre::RenderWindow* rw)
{
    if ((rw == win_) && (inputman_))
    {
        inputman_-> destroyInputObject (keyboard_);
        OIS::InputManager::destroyInputSystem (inputman_);
        inputman_ = 0;
    }
}

bool WindowController::frameStarted (const Ogre::FrameEvent& evt)
{
    static int count = 0; ++ count;
    return (count < 5000); // quit after 100 frames always
}

bool WindowController::frameRenderingQueued (const Ogre::FrameEvent& evt)
{
    if (win_-> isClosed()) return false;

    keyboard_-> capture();

    if (!keyboard_-> buffered())
        if (keyboard_-> isKeyDown (OIS::KC_ESCAPE) || 
                keyboard_-> isKeyDown (OIS::KC_Q))
            return false;

    return true;
}


//=============================================================================
//

Ogre3DApplication::Ogre3DApplication ()
{
    root_ = OGRE_NEW Ogre::Root ();

    setup_resources ();
    root_-> restoreConfig();

    win_ = root_-> initialise (true);

    scenemgr_ = root_-> createSceneManager (Ogre::ST_GENERIC, "SceneManager");

    Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();

    model_ = new WorldModel (scenemgr_);
    view_ = new WorldView (model_, win_);
    controller_ = new WorldController (model_);
    //winctrl_ = new WindowController (win_);

    root_-> addFrameListener (controller_);
    //root_-> addFrameListener (winctrl_);

    // show available render targets
    //Ogre::RenderSystem::RenderTargetIterator it (root_->getRenderSystem()->getRenderTargetIterator());
    //while (it.hasMoreElements())
    //	std::cout << "Render Target: " << it.getNext()-> getName() << std::endl;
}

Ogre3DApplication::~Ogre3DApplication ()
{
    //delete winctrl_;
    delete controller_;
    delete view_;
    delete model_;

    //OGRE_DELETE root_;
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
    view_ = new QGraphicsView ();
    scene_ = new QGraphicsScene ();

    QDialog *dialog = new QDialog ();

    dialog-> setWindowOpacity (0.8);
    dialog-> setWindowTitle ("testing baby");
    dialog-> setLayout (new QVBoxLayout);
    dialog-> layout()-> addWidget (new QLineEdit);

    scene_-> addWidget (dialog);
    view_-> setScene (scene_);
    view_-> setViewportUpdateMode (QGraphicsView::FullViewportUpdate);

    QGraphicsItem *item (view_-> items().front());
    item-> setFlag (QGraphicsItem::ItemIsMovable);
    item-> setCacheMode (QGraphicsItem::DeviceCoordinateCache);
    item-> setPos (10, 50);

    view_-> show();
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

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
    INT WINAPI WinMain(HINSTANCE hInst, HINSTANCE, LPSTR strCmdLine, INT)
#else
        int main(int argc, char **argv)
#endif
        {
            Ogre3DApplication ogreapp;
            QtApplication qtapp (argc, argv);

            RenderShim shim (qtapp.GetView(), ogreapp.GetView());

            return qtapp.exec();
        }

#ifdef __cplusplus
}
#endif



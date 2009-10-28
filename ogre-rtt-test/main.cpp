#include "main.h"

class WorldModel
{
    public:

        WorldModel (Ogre::SceneManager *sm)
            : scenemgr_ (sm)
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

    private:

        Ogre::SceneManager      *scenemgr_;
        Ogre::Camera            *camera_;

        Ogre::SceneNode		    *planenode_;

        friend class WorldRenderer;
        friend class WorldController;
};

class WorldRenderer : public Ogre::RenderTargetListener
{
    public:
        WorldRenderer (WorldModel *model, Ogre::RenderWindow *win) 
            : model_ (model), win_ (win)
        { 
            showwin_ = true;
            showrtt_ = true;
            
            if (showwin_)
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
                Ogre::Root::getSingleton().
                    getRenderSystem()-> detachRenderTarget ("OGRE Render Window");
            }

            // Create the textures
            Ogre::TexturePtr texture = Ogre::TextureManager::getSingleton().
                createManual ("RttTex", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, Ogre::TEX_TYPE_2D, 
                        800/*win_->getWidth()*/, 600/*win_->getHeight()*/, 0, Ogre::PF_R8G8B8, Ogre::TU_RENDERTARGET); 

            tex_ = texture->getBuffer()->getRenderTarget();
            tex_-> addViewport (model_-> camera_);
            tex_-> getViewport (0)-> setClearEveryFrame (true);
            tex_-> getViewport (0)-> setBackgroundColour (ColourValue::Black);
            tex_-> getViewport (0)-> setOverlaysEnabled (false);
            tex_-> addListener (this);

            if (showrtt_)
            {
                // Create the mini screen rectangle and attach it to a scene node
                miniscreen_ = new Ogre::Rectangle2D (true);
                miniscreen_->setCorners (0.5, -0.5, 1.0, -1.0);
                miniscreen_->setBoundingBox (Ogre::AxisAlignedBox (-100000.0*Vector3::UNIT_SCALE, 100000.0*Vector3::UNIT_SCALE)); 

                model_-> scenemgr_-> getRootSceneNode()-> 
                    createChildSceneNode ("MiniScreenNode")-> attachObject (miniscreen_);

                // Create the material for the mini screen
                Ogre::MaterialPtr material = Ogre::MaterialManager::getSingleton().
                    create ("RttMat", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);

                Ogre::Technique *technique = material->createTechnique();
                technique->createPass();
                material->getTechnique(0)->getPass(0)->setLightingEnabled(false);
                material->getTechnique(0)->getPass(0)->createTextureUnitState("RttTex");

                // Apply the material to the mini screen
                miniscreen_-> setMaterial ("RttMat");
            }
        }

        ~WorldRenderer ()
        {
            tex_-> writeContentsToFile ("rtt.png");
            delete miniscreen_;
        }
        
        void preRenderTargetUpdate(const Ogre::RenderTargetEvent &evt)
        {
            if (showrtt_) miniscreen_-> setVisible (false);
        }

        void postRenderTargetUpdate(const Ogre::RenderTargetEvent &evt)
        {
            if (showrtt_) miniscreen_-> setVisible (true);
        }

    public:

        bool                showwin_;
        bool                showrtt_;

        Ogre::RenderWindow  *win_;
        Ogre::RenderTexture *tex_;
        Ogre::Viewport      *view_;
        Ogre::Rectangle2D   *miniscreen_;

        WorldModel          *model_;
};

class WindowController : public Ogre::WindowEventListener, public Ogre::FrameListener
{
    public: 
        WindowController (Ogre::RenderWindow *win)
            : win_ (win)
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

        virtual ~WindowController ()
        {
            Ogre::WindowEventUtilities::removeWindowEventListener (win_, this);
            windowClosed (win_);
        }

        virtual void windowClosed (Ogre::RenderWindow* rw)
        {
            if ((rw == win_) && (inputman_))
            {
                inputman_-> destroyInputObject (keyboard_);
                OIS::InputManager::destroyInputSystem (inputman_);
                inputman_ = 0;
            }
        }

        bool frameStarted (const Ogre::FrameEvent& evt)
        {
            static int count = 0; ++ count;
            return (count < 5000); // quit after 100 frames always
        }

        bool frameRenderingQueued (const Ogre::FrameEvent& evt)
        {
            if (win_-> isClosed()) return false;

            keyboard_-> capture();

            if (!keyboard_-> buffered())
                if (keyboard_-> isKeyDown (OIS::KC_ESCAPE) || 
                        keyboard_-> isKeyDown (OIS::KC_Q))
                    return false;

            return true;
        }

    private:
        Ogre::RenderWindow   *win_;

        OIS::InputManager   *inputman_;
        OIS::Keyboard       *keyboard_;
};

class WorldController : public Ogre::FrameListener
{
    public:
        WorldController (WorldModel *model) 
            : model_ (model)
        { }
        
        // rotate the plane
        bool frameStarted (const Ogre::FrameEvent& evt)
        {
            model_-> planenode_-> yaw (Radian (evt.timeSinceLastFrame));

            return Ogre::FrameListener::frameStarted (evt);
        }

    private:
        WorldModel          *model_;
};


class Ogre3DApplication
{
    public:
        Ogre3DApplication (const Ogre::String &pluginpath, const Ogre::String &resourcepath)
        {
            root_ = OGRE_NEW Ogre::Root ();//(pluginpath, resourcepath + "ogre.cfg", resourcepath + "ogre.log");

            setup_resources ();
            root_-> restoreConfig();

            win_ = root_->initialise (true);

            scenemgr_ = root_-> createSceneManager (Ogre::ST_GENERIC, "SceneManager");

            Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();

            model_ = new WorldModel (scenemgr_);
            renderer_ = new WorldRenderer (model_, win_);
            controller_ = new WorldController (model_);
            //winctrl_ = new WindowController (win_);

            root_-> addFrameListener (controller_);
            //root_-> addFrameListener (winctrl_);

            Ogre::RenderSystem::RenderTargetIterator it (root_->getRenderSystem()->getRenderTargetIterator());
            while (it.hasMoreElements())
				std::cout << "Render Target: " << it.getNext()-> getName() << std::endl;
        }

        virtual ~Ogre3DApplication ()
        {
            //delete winctrl_;
            delete controller_;
            delete renderer_;
            delete model_;

            //OGRE_DELETE root_;
        }

        void Run () 
        { 
            root_-> startRendering (); 
        }

    protected:
        void setup_resources ()
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

    private:

        Ogre::Root          *root_;
        Ogre::Camera        *camera_;
        Ogre::SceneManager  *scenemgr_;
        Ogre::RenderWindow  *win_;

        WorldModel          *model_;
        WorldController     *controller_;
        WorldRenderer       *renderer_;

        WindowController    *winctrl_;
};


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
            // Create application object
            Ogre3DApplication app ("./", "./");
            QApplication qapp (argc, argv);

            QGraphicsView *view = new QGraphicsView ();
            QGraphicsScene *scene = new QGraphicsScene ();
            QDialog *dialog = new QDialog ();

            dialog-> setWindowOpacity (0.8);
            dialog-> setWindowTitle ("testing baby");
            dialog-> setLayout (new QVBoxLayout);
            dialog-> layout()-> addWidget (new QLineEdit);

            scene-> addWidget (dialog);
            view-> setScene (scene);
            view-> setViewportUpdateMode (QGraphicsView::FullViewportUpdate);

            QGraphicsItem *item (view-> items().front());
            item-> setFlag (QGraphicsItem::ItemIsMovable);
            item-> setCacheMode (QGraphicsItem::DeviceCoordinateCache);
            item-> setPos (10, 50);

            view-> show();

            try 
            {
                //app.Run ();
            } 
            catch (Ogre::Exception& e) 
            {
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
                MessageBoxA(NULL, e.getFullDescription().c_str(),
                        "An exception has occurred!", MB_OK | MB_ICONERROR | MB_TASKMODAL);
#else
                std::cerr << "An exception has occurred: " << e.getFullDescription();
#endif
            }

            return qapp.exec();
        }

#ifdef __cplusplus
}
#endif



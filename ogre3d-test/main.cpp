/* main.cpp -- main module
 *
 *			Ryan McDougall -- 2009
 */

#include <OgreCamera.h>
#include <OgreEntity.h>
#include <OgreLogManager.h>
#include <OgreOverlay.h>
#include <OgreOverlayElement.h>
#include <OgreOverlayManager.h>
#include <OgreRoot.h>
#include <OgreViewport.h>
#include <OgreSceneManager.h>
#include <OgreRenderWindow.h>
#include <OgreConfigFile.h>

#include <OISEvents.h>
#include <OISInputManager.h>
#include <OISKeyboard.h>
#include <OISMouse.h>

//#include <QtGui>

#include "main.h"

//=============================================================================
//
template<> OgreFramework* Ogre::Singleton<OgreFramework>::ms_Singleton = 0;

OgreFramework::OgreFramework() :
    move_speed_ (0.1),
    rot_speed_ (0.3),
    stop_ (false),
    nscreenshot_ (0)
{
}

OgreFramework::~OgreFramework ()
{
    delete keyboard_;
    delete mouse_;

    OIS::InputManager::destroyInputSystem (inputmgr_);

    delete root_;
}

void OgreFramework::initOgre (Ogre::String title, OIS::KeyListener *key_listener, OIS::MouseListener *mouse_listener)
{
    Ogre::LogManager* logMgr = new Ogre::LogManager();

    log_ = Ogre::LogManager::getSingleton().createLog ("OgreLogfile.log", true, true, false);
    log_->setDebugOutputEnabled (true);

    root_ = new Ogre::Root();
    root_->showConfigDialog();
    renderwin_ = root_->initialise (true, title);

    scenemgr_ = root_->createSceneManager (ST_GENERIC, "SceneManager");
    scenemgr_->setAmbientLight (Ogre::ColourValue (0.7, 0.7, 0.7));

    camera_ = scenemgr_->createCamera ("Camera");
    camera_->setPosition (Vector3 (0, 60, 60));
    camera_->lookAt (Vector3 (0,0,0));
    camera_->setNearClipDistance (1);

    viewport_ = renderwin_->addViewport (camera_);
    viewport_->setBackgroundColour (ColourValue (0.8, 0.7, 0.6, 1.0));

    camera_->setAspectRatio (Real(viewport_->getActualWidth()) / Real(viewport_->getActualHeight()));
    viewport_->setCamera (camera_);

    unsigned long window_handle = 0;
    OIS::ParamList param_list;
    renderwin_->getCustomAttribute ("WINDOW", &window_handle);

    param_list.insert (OIS::ParamList::value_type ("WINDOW", Ogre::StringConverter::toString (window_handle)));

    inputmgr_ = OIS::InputManager::createInputSystem (param_list);

    keyboard_ = static_cast<OIS::Keyboard*> (inputmgr_->createInputObject (OIS::OISKeyboard, true));
    mouse_ = static_cast<OIS::Mouse*> (inputmgr_->createInputObject (OIS::OISMouse, true));

    mouse_->getMouseState().height = renderwin_->getHeight();
    mouse_->getMouseState().width = renderwin_->getWidth();

    if (key_listener == 0)
        keyboard_->setEventCallback (this);
    else
        keyboard_->setEventCallback (key_listener);

    if (mouse_listener == 0)
        mouse_->setEventCallback (this);
    else
        mouse_->setEventCallback (mouse_listener);

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
            Ogre::ResourceGroupManager::getSingleton().addResourceLocation (arch_name, type_name, sec_name);
        }
    }
    Ogre::TextureManager::getSingleton().setDefaultNumMipmaps (5);
    Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();

    timer_ = new Ogre::Timer();
    timer_->reset();

    debugoverlay_ = OverlayManager::getSingleton().getByName ("Core/DebugOverlay");
    debugoverlay_->show();

    renderwin_->setActive (true);
}

bool OgreFramework::keyPressed (const OIS::KeyEvent &ev)
{
    log_->logMessage ("OgreFramework::keyPressed");

    if (keyboard_->isKeyDown (OIS::KC_ESCAPE))
    {
        stop_ = true;
        return true;
    }

    if (keyboard_->isKeyDown (OIS::KC_SYSRQ))
    {
        std::ostringstream ss;
        ss << "screenshot_" << ++nscreenshot_ << ".png";
        renderwin_->writeContentsToFile (ss.str());
        return true;
    }

    if (keyboard_->isKeyDown (OIS::KC_M))
    {
        static int mode = 0;

        if (mode == 2)
        {
            camera_->setPolygonMode (PM_SOLID);
            mode = 0;
        }
        else if (mode == 0)
        {
            camera_->setPolygonMode (PM_WIREFRAME);
            mode = 1;
        }
        else if (mode == 1)
        {
            camera_->setPolygonMode (PM_POINTS);
            mode = 2;
        }
    }

    if (keyboard_->isKeyDown (OIS::KC_O))
    {
        if (debugoverlay_)
        {
            if (!debugoverlay_->isVisible())
                debugoverlay_->show();
            else
                debugoverlay_->hide();
        }
    }

    return true;
}

bool OgreFramework::keyReleased (const OIS::KeyEvent &ev)
{
    return true;
}

bool OgreFramework::mouseMoved (const OIS::MouseEvent &ev)
{
    camera_->yaw (Degree (ev.state.X.rel * -0.1));
    camera_->pitch (Degree (ev.state.Y.rel * -0.1));

    return true;
}

bool OgreFramework::mousePressed (const OIS::MouseEvent &ev, OIS::MouseButtonID id)
{
    return true;
}

bool OgreFramework::mouseReleased (const OIS::MouseEvent &ev, OIS::MouseButtonID id)
{
    return true;
}

void OgreFramework::updateOgre (double timeSinceLastFrame)
{
    move_scale_ = move_speed_   * timeSinceLastFrame;
    rot_scale_  = rot_speed_ * timeSinceLastFrame;

    translate_ = Vector3::ZERO;

    getInput();
    moveCamera();

    updateStats();
}

void OgreFramework::updateStats() 
{ 
    static String currFps = "Current FPS: "; 
    static String avgFps = "Average FPS: "; 
    static String bestFps = "Best FPS: "; 
    static String worstFps = "Worst FPS: "; 
    static String tris = "Triangle Count: "; 
    static String batches = "Batch Count: "; 

    OverlayElement* guiAvg = OverlayManager::getSingleton().getOverlayElement ("Core/AverageFps"); 
    OverlayElement* guiCurr = OverlayManager::getSingleton().getOverlayElement ("Core/CurrFps"); 
    OverlayElement* guiBest = OverlayManager::getSingleton().getOverlayElement ("Core/BestFps"); 
    OverlayElement* guiWorst = OverlayManager::getSingleton().getOverlayElement ("Core/WorstFps"); 

    const RenderTarget::FrameStats& stats = renderwin_->getStatistics(); 
    guiAvg->setCaption (avgFps + StringConverter::toString (stats.avgFPS)); 
    guiCurr->setCaption (currFps + StringConverter::toString (stats.lastFPS)); 
    guiBest->setCaption (bestFps + StringConverter::toString (stats.bestFPS) 
            +" "+StringConverter::toString (stats.bestFrameTime)+" ms"); 
    guiWorst->setCaption (worstFps + StringConverter::toString (stats.worstFPS) 
            +" "+StringConverter::toString (stats.worstFrameTime)+" ms"); 

    OverlayElement* guiTris = OverlayManager::getSingleton().getOverlayElement ("Core/NumTris"); 
    guiTris->setCaption (tris + StringConverter::toString (stats.triangleCount)); 

    OverlayElement* guiBatches = OverlayManager::getSingleton().getOverlayElement ("Core/NumBatches"); 
    guiBatches->setCaption (batches + StringConverter::toString (stats.batchCount)); 

    OverlayElement* guiDbg = OverlayManager::getSingleton().getOverlayElement ("Core/DebugText"); 
    guiDbg->setCaption ("");
}

void OgreFramework::moveCamera()
{
    if (keyboard_->isKeyDown (OIS::KC_LSHIFT)) camera_->moveRelative (translate_);
    camera_->moveRelative (translate_ / 10);
}

void OgreFramework::getInput()
{
    if (keyboard_->isKeyDown (OIS::KC_A))
    {
        translate_.x = -move_scale_;
    }

    if (keyboard_->isKeyDown (OIS::KC_D))
    {
        translate_.x = move_scale_;
    }

    if (keyboard_->isKeyDown (OIS::KC_W))
    {
        translate_.z = -move_scale_;
    }

    if (keyboard_->isKeyDown (OIS::KC_S))
    {
        translate_.z = move_scale_;
    }

    if (keyboard_->isKeyDown (OIS::KC_LEFT))
    {
        camera_->yaw (rot_scale_);
    }

    if (keyboard_->isKeyDown (OIS::KC_RIGHT))
    {
        camera_->yaw (-rot_scale_);
    }

    if (keyboard_->isKeyDown (OIS::KC_UP))
    {
        camera_->pitch (rot_scale_);
    }

    if (keyboard_->isKeyDown (OIS::KC_DOWN))
    {
        camera_->pitch (-rot_scale_);
    }
}

//=============================================================================
//

//=============================================================================
// Main entry point
int
main (int argc, char** argv)
{
    //QApplication app (argc, argv); 
    //Window win;

    //win.show();

    //return app.exec();
    OgreApplication app ("test");
    app.Go();
    return 0;
}

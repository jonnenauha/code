/* main.h -- exported function header
 *
 * see: http://www.ogre3d.org/wiki/index.php/Basic_Ogre_Framework
 *
 *			Ryan McDougall -- 2009
 */

#ifndef _MAIN_H_
#define _MAIN_H_

using namespace Ogre;

class OgreFramework : public Ogre::Singleton<OgreFramework>, OIS::KeyListener, OIS::MouseListener
{
    public:
        OgreFramework ();
        ~OgreFramework ();

        void initOgre (Ogre::String wndTitle, OIS::KeyListener *key_listener = 0, OIS::MouseListener *mouse_listener = 0);
        void updateOgre (double timeSinceLastFrame);
        void updateStats ();
        void moveCamera ();
        void getInput ();

        bool isOgreToBeShutDown () const { return stop_; }

        bool keyPressed (const OIS::KeyEvent &ev);
        bool keyReleased (const OIS::KeyEvent &ev);

        bool mouseMoved (const OIS::MouseEvent &evt);
        bool mousePressed (const OIS::MouseEvent &evt, OIS::MouseButtonID id); 
        bool mouseReleased (const OIS::MouseEvent &evt, OIS::MouseButtonID id);

        Ogre::Root*             root_;
        Ogre::SceneManager*     scenemgr_;
        Ogre::RenderWindow*     renderwin_;
        Ogre::Camera*           camera_;
        Ogre::Viewport*         viewport_;
        Ogre::Log*              log_;
        Ogre::Timer*            timer_;

        OIS::InputManager*      inputmgr_;
        OIS::Keyboard*          keyboard_;
        OIS::Mouse*             mouse_;

    private:
        OgreFramework (const OgreFramework&);
        OgreFramework& operator= (const OgreFramework&);

        Ogre::Overlay*      debugoverlay_;
        Ogre::Overlay*      infooverlay_;
        int                 nscreenshot_;

        bool                stop_;

        Ogre::Vector3       translate_;
        Ogre::Real          move_speed_; 
        Ogre::Degree        rot_speed_; 
        float               move_scale_; 
        Ogre::Degree        rot_scale_;
};

class OgreApplication : public OIS::KeyListener
{
    public:
        OgreApplication (const std::string &name) :
            stop_ (false)
        {
            new OgreFramework();
            OgreFramework::getSingletonPtr()->initOgre(name, this, 0);
            OgreFramework::getSingletonPtr()->log_->logMessage(name + " initialized!");

            OgreFramework::getSingletonPtr()->scenemgr_->setSkyBox(true, "Examples/SpaceSkyBox");
            OgreFramework::getSingletonPtr()->scenemgr_->createLight("Light")->setPosition(75,75,75);

            cubeentity_ = OgreFramework::getSingletonPtr()->scenemgr_->createEntity("Cube", "ogrehead.mesh");
            cubenode_ = OgreFramework::getSingletonPtr()->scenemgr_->getRootSceneNode()->createChildSceneNode("CubeNode");
            cubenode_->attachObject(cubeentity_);
        }

        void Go ()
        {
            OgreFramework::getSingletonPtr()->log_->logMessage("Start main loop...");

            double timeSinceLastFrame = 0;
            double startTime = 0;

            OgreFramework::getSingletonPtr()->renderwin_->resetStatistics();

            while(!stop_ && !OgreFramework::getSingletonPtr()->isOgreToBeShutDown()) 
            {
                if(OgreFramework::getSingletonPtr()->renderwin_->isClosed()) 
                    stop_ = true;

                if(OgreFramework::getSingletonPtr()->renderwin_->isActive())
                {
                    startTime = OgreFramework::getSingletonPtr()->timer_->getMillisecondsCPU();

                    OgreFramework::getSingletonPtr()->keyboard_->capture();
                    OgreFramework::getSingletonPtr()->mouse_->capture();

                    OgreFramework::getSingletonPtr()->updateOgre(timeSinceLastFrame);
                    OgreFramework::getSingletonPtr()->root_->renderOneFrame();

                    timeSinceLastFrame = OgreFramework::getSingletonPtr()->timer_->getMillisecondsCPU() - startTime;
                }
                //else
                //{
                //    usleep (1000);
                //}
            }

            OgreFramework::getSingletonPtr()->log_->logMessage("Main loop quit");
            OgreFramework::getSingletonPtr()->log_->logMessage("Shutdown OGRE...");
        }

        bool keyPressed(const OIS::KeyEvent &ev)
        {
            OgreFramework::getSingletonPtr()->keyPressed(ev);

            if(OgreFramework::getSingletonPtr()->keyboard_->isKeyDown(OIS::KC_F))
            {
                //do something
            }

            return true;
        }

        bool keyReleased(const OIS::KeyEvent &ev)
        {
            OgreFramework::getSingletonPtr()->keyReleased(ev);

            return true;
        }

    private:
        Ogre::SceneNode*    cubenode_;
        Ogre::Entity*       cubeentity_;

        bool                stop_;
};

#endif //_MAIN_H_

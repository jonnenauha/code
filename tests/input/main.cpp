/* main.cpp -- main module
 *
 *			Ryan McDougall -- 2009
 */

#include <QtGui>
#include <OGRE/Ogre.h>
#include <OIS/OIS.h>

#include <iostream>
#include <cstring>
#include <cassert>

using namespace std;

//=============================================================================
//
//Ogre::Root *render_setup (int width, int height)
//{
//    try 
//    {
//        Ogre::Root *root = new Ogre::Root("Data/plugins.cfg", "Data/ogre.cfg", "Data/ogre.log");
//
//        if (!root->restoreConfig()) 
//        {
//            // setup a renderer
//            Ogre::RenderSystemList *renderers = root-> getAvailableRenderers();
//            assert (!renderers-> empty()); // we need at least one renderer to do anything useful
//
//            //Ogre::RenderSystem *renderSystem = chooseRenderer(renderers);
//            Ogre::RenderSystem *renderSystem = *renderers-> begin();
//            assert (renderSystem); // user might pass back a null renderer, which would be bad!
//
//            root-> setRenderSystem (renderSystem);
//
//            QString dimensions = QString ("%1x%2").arg (width).arg (height);
//            renderSystem-> setConfigOption ("Video Mode", dimensions.toStdString());
//
//            root-> getRenderSystem()-> setConfigOption ("Full Screen", "No");
//            root-> saveConfig();
//        }
//
//        // initialize without creating window
//        root-> initialise (false);
//
//        // Load resource paths from config file
//        Ogre::ConfigFile config;
//        config.load ("Data/resources.cfg");
//
//        // Go through all sections & settings in the file
//        Ogre::ConfigFile::SectionIterator it = config.getSectionIterator();
//
//        Ogre::String secName, typeName, archName;
//        while (it.hasMoreElements()) 
//        {
//            secName = it.peekNextKey();
//            Ogre::ConfigFile::SettingsMultiMap *settings = it.getNext();
//            Ogre::ConfigFile::SettingsMultiMap::iterator i;
//
//            for (i = settings->begin(); i != settings->end(); ++i) 
//            {
//                typeName = i->first;
//                archName = i->second;
//                Ogre::ResourceGroupManager::getSingleton().addResourceLocation (archName, typeName, secName);
//            }
//        }
//
//        Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
//
//        return root;
//    }
//    catch (std::exception &e)
//    {
//        cout << "exception: " << e.what() << endl;
//        throw;
//    }
//
//    return NULL;
//}
//
//class Test : public QObject
//{
//    public:
//        Test ()
//        {
//            root = render_setup (640, 480);
//            win = root-> createRenderWindow ("win", 640, 480, false);
//
//            OIS::ParamList params;
//            std::ostringstream winidstr; size_t winid = 0;
//
//            win-> getCustomAttribute ("WINDOW", &winid); winidstr << winid;
//            params.insert (std::make_pair (std::string ("WINDOW"), winidstr.str()));
//
//            inmgr = OIS::InputManager::createInputSystem (params);
//            keyboard = static_cast <OIS::Keyboard*> (inmgr-> createInputObject (OIS::OISKeyboard, false));
//
//            startTimer (1000);
//        }
//
//        void Destroy ()
//        {
//            inmgr-> destroyInputObject (keyboard);
//            OIS::InputManager::destroyInputSystem (inmgr);
//            inmgr = 0;
//        }
//
//        void timerEvent (QTimerEvent *e) 
//        {
//            cout << "begin" << endl;
//            keyboard-> capture();
//            
//            if (keyboard-> isKeyDown (OIS::KC_ESCAPE))
//            {
//                Destroy ();
//                exit (0);
//            }
//
//            if (keyboard-> isKeyDown (OIS::KC_Q))
//                cout << "KC_Q" << endl; 
//            if (keyboard-> isKeyDown (OIS::KC_W))
//                cout << "KC_W" << endl; 
//            if (keyboard-> isKeyDown (OIS::KC_E))
//                cout << "KC_E" << endl; 
//            if (keyboard-> isKeyDown (OIS::KC_R))
//                cout << "KC_R" << endl; 
//            if (keyboard-> isKeyDown (OIS::KC_T))
//                cout << "KC_T" << endl; 
//            if (keyboard-> isKeyDown (OIS::KC_Y))
//                cout << "KC_Y" << endl; 
//            cout << "end" << endl;
//        }
//
//    private:
//        Ogre::Root          *root;
//        Ogre::RenderWindow  *win;
//        OIS::InputManager   *inmgr;
//        OIS::Keyboard       *keyboard;
//};

struct KeyState
{
    KeyState () : 
        key (0), 
        modifiers (0) 
    {}

    KeyState (int k, unsigned int m) :
        key (k), modifiers (m)
    {}

    KeyState (const KeyState &c) : 
        key (c.key), 
        modifiers (c.modifiers) 
    {}

    KeyState (const QKeyEvent *e) : 
        key (e-> key()), 
        modifiers (e-> modifiers()) 
    {}

    KeyState &operator= (const KeyState &rhs) 
    { 
        key = rhs.key; 
        modifiers = rhs.modifiers; 
        return *this; 
    }

    bool operator== (const KeyState &rhs)
    {
        return (key == rhs.key);
    }

    bool operator== (int k)
    {
        return (key == k);
    }

    int key;
    unsigned int modifiers;
};


class KeyStateMachine : public QWidget
{
    public:
        KeyStateMachine ()
        {
            state.reserve (10); // we presume users only have 10 fingers

            startTimer (1000);
        }

        const std::vector<KeyState> &GetState () { return state; }

        bool IsKeyDown (int key) { return is_key_pressed_ (KeyState (key, 0)); }

    protected:
        void timerEvent (QTimerEvent *e)
        {
            cout << "---" << endl;
            std::vector <KeyState>::iterator i (state.begin());
            for (; i != state.end(); ++i)
            {
                int k = i-> key;

                cout << "* state: " << " key: "; 
                if (k >= 65 && k <= 90) // ASCII
                    cout << (char)(k);
                else
                    cout << k;
                cout << " modifiers: " << i-> modifiers << endl;
            }
        }

        void keyPressEvent (QKeyEvent *e)
        {
            KeyState key (e);

            assert (!is_key_pressed_ (key));
            press_key_ (key);
        }
        
        void keyReleaseEvent (QKeyEvent *e)
        {
            KeyState key (e);

            assert (is_key_pressed_ (key));
            release_key_ (key);
        }

    private:
        bool is_key_pressed_ (const KeyState &k)
        {
            return (std::find (state.begin(), state.end(), k) != state.end());
        }

        void press_key_ (const KeyState &k)
        { 
            state.push_back (k); 
        }

        void release_key_ (const KeyState &k)
        { 
            state.erase (std::find (state.begin(), state.end(), k)); 
        }
            
        std::vector <KeyState>   state; 
};


//=============================================================================
// Main entry point
int
main (int argc, char** argv)
{
    QApplication app (argc, argv);
    
    KeyStateMachine t; t.show();

    return app.exec ();
}

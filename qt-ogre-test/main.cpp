/* main.cpp -- main module
 *
 *			Ryan McDougall -- 2009
 */

#include <iostream>
#include <main.h>

using namespace std;


//=============================================================================

Ogre::Root *render_setup (int width, int height)
{
    Ogre::Root *root = new Ogre::Root("Data/plugins.cfg", "Data/ogre.cfg", "Data/ogre.log");

    if (!root->restoreConfig()) 
    {
        // setup a renderer
        Ogre::RenderSystemList *renderers = root-> getAvailableRenderers();
        assert (!renderers-> empty()); // we need at least one renderer to do anything useful

        //Ogre::RenderSystem *renderSystem = chooseRenderer(renderers);
        Ogre::RenderSystem *renderSystem = *renderers-> begin();
        assert (renderSystem); // user might pass back a null renderer, which would be bad!

        root-> setRenderSystem (renderSystem);

        QString dimensions = QString ("%1x%2").arg (width).arg (height);
        renderSystem-> setConfigOption ("Video Mode", dimensions.toStdString());

        // initialize without creating window
        root-> getRenderSystem()-> setConfigOption("Full Screen", "No");
        root-> saveConfig();
    }

    root-> initialise (false);

    // Load resource paths from config file
    Ogre::ConfigFile config;
    config.load ("Data/resources.cfg");

    // Go through all sections & settings in the file
    Ogre::ConfigFile::SectionIterator it = config.getSectionIterator();

    Ogre::String secName, typeName, archName;
    while (it.hasMoreElements()) 
    {
        secName = it.peekNextKey();
        Ogre::ConfigFile::SettingsMultiMap *settings = it.getNext();
        Ogre::ConfigFile::SettingsMultiMap::iterator i;

        for (i = settings->begin(); i != settings->end(); ++i) 
        {
            typeName = i->first;
            archName = i->second;
            Ogre::ResourceGroupManager::getSingleton().addResourceLocation (archName, typeName, secName);
        }
    }
            
    Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();

    return root;
}

//=============================================================================
//
SceneManager::SceneManager (Ogre::Root *scene_root)
{
    mainwin = new QWidget;
    mainlay = new QVBoxLayout;

    worldscene = new TestWidget (scene_root, mainwin);

    mainpainter = new QPainter (worldscene);

    uiscene = new QGraphicsScene;
    uiview = new RedirectedQGraphicsView (mainpainter, uiscene);

    line = new QLineEdit;
    line-> setText ("wtf");

    mainlay-> addWidget (worldscene);
    mainwin-> setLayout (mainlay);
    mainwin-> setMinimumSize (100, 100);

    uiscene-> addWidget (line);

    worldscene-> installEventFilter (this);
    uiscene-> installEventFilter (this);
    uiview-> installEventFilter (this);

    uiview-> show();

    startTimer (20);
}

bool SceneManager::eventFilter (QObject *o, QEvent *e)
{
    // paint manually in the timer to avoid flicker
    if (e-> type() == QEvent::Paint)
        return true;

    // forward to uiview
    if (o == worldscene)// && e-> spontaneous())
        QApplication::sendEvent (uiview, e);
    
    return false;
}

//=============================================================================
// Main entry point
int
main (int argc, char** argv)
{
    QApplication app (argc, argv);
    
    SceneManager scene (render_setup (100, 100));
    scene.Start ();

    return app.exec ();
}

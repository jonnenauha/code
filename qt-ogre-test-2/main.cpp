/* main.cpp -- main module
 *
 *			Ryan McDougall -- 2009
 */

#include "main.h"
#include "ogrewidget.h"
#include "testwidget.h"

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
// Main entry point
int
main (int argc, char** argv)
{
    QApplication app (argc, argv);
    
    const int width (800), height (800);

    //QWidget *mainwin = new QWidget;
    //QVBoxLayout *mainlay = new QVBoxLayout; // layout generates QResizeEvents
    //mainwin-> setLayout (mainlay);
    //mainwin-> setMinimumSize (width, height); // crash if zero

    TestWidget *widget = new TestWidget (render_setup (width, height));//, mainwin);

    widget-> show();

    //mainlay-> addWidget (widget);
    //mainwin-> show();

    return app.exec ();
}

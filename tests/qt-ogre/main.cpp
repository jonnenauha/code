/* main.cpp -- main module
 *
 *			Ryan McDougall -- 2009
 */

#include "main.h"
#include "ogrewidget.h"
#include "testwidget.h"
#include "viewmodel.h"

class Dialog : public QDialog
{
    public:
        Dialog (QWidget *parent = 0, Qt::WindowFlags f = 0)
            : QDialog (parent, f)
        { }

        void paintEvent (QPaintEvent *e) { QWidget::paintEvent (e); }
};

//=============================================================================

Ogre::Root *render_setup (int width, int height)
{
    try 
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

            root-> getRenderSystem()-> setConfigOption ("Full Screen", "No");
            root-> saveConfig();
        }

        // initialize without creating window
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
    catch (std::exception &e)
    {
        cout << "exception: " << e.what() << endl;
        throw;
    }

    return NULL;
}

//=============================================================================
// Main entry point
int
main (int argc, char** argv)
{
    QApplication app (argc, argv);
    
    const int width (800), height (800);
    Ogre::Root *ogre_scene_graph (render_setup (width, height)); 

    if ((argc > 1) && (!strcmp (argv[1], "graphicsscene")))
    {
        GraphicsView *view = new GraphicsView ();

        Dialog *dialog = new Dialog (0, Qt::CustomizeWindowHint | Qt::WindowTitleHint);

        dialog-> setWindowOpacity (0.8);
        dialog-> setWindowTitle ("testing baby");
        dialog-> setLayout (new QVBoxLayout);
        dialog-> layout()-> addWidget (new QLineEdit);
        
        TestWidget *scenewidget = new TestWidget (ogre_scene_graph, view);

        GraphicsScene *scene = new GraphicsScene (scenewidget);
        scene-> setView (view);
        scene-> addWidget (dialog);
        
        view-> setScene (scene);
        view-> setViewport (scenewidget);
        view-> installEventFilter (scenewidget);
        view-> setViewportUpdateMode (QGraphicsView::FullViewportUpdate);

        QGraphicsItem *item (view-> items().front());
        item-> setFlag (QGraphicsItem::ItemIsMovable);
        item-> setCacheMode (QGraphicsItem::DeviceCoordinateCache);
        item-> setPos (10, 50);
        
        view-> show();
    }
    else if ((argc > 1) && (!strcmp (argv[1], "layout")))
    {
        QWidget *mainwin = new QWidget;
        QVBoxLayout *mainlay = new QVBoxLayout; // layout generates QResizeEvents
        
        TestWidget *scenewidget = new TestWidget (ogre_scene_graph, mainwin);

        mainlay-> addWidget (scenewidget);
        mainwin-> setLayout (mainlay);
        mainwin-> setMinimumSize (width, height); // crash if zero

        mainwin-> show();
    }
    else
    {
        TestWidget *scenewidget = new TestWidget (ogre_scene_graph);
        scenewidget-> show();
    }

    return app.exec ();
}

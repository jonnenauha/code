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
OgreWidget::OgreWidget (Ogre::Root *root, QWidget *parent)
: QWidget (parent), root_ (root), win_ (NULL)
{
    setAttribute (Qt::WA_PaintOnScreen);
    setAttribute (Qt::WA_PaintOutsidePaintEvent);
    setAttribute (Qt::WA_NoSystemBackground);
    createRenderWindow ();
}

void OgreWidget::Update ()
{
    if (win_) 
    {
        root_-> _fireFrameStarted();
        win_-> update();
        root_-> _fireFrameEnded();
    }
}

void OgreWidget::createRenderWindow ()
{
    if (win_) return;

    Ogre::NameValuePairList params;

#ifndef Q_WS_WIN
    QX11Info info = x11Info();
    Ogre::String winhandle;

    winhandle = Ogre::StringConverter::toString 
        (reinterpret_cast<unsigned long> 
         (info.display()));
    winhandle += ":";

    winhandle += Ogre::StringConverter::toString 
        (static_cast<unsigned int> 
         (info.screen()));
    winhandle += ":";

    winhandle += Ogre::StringConverter::toString 
        (static_cast<unsigned long> 
         (parentWidget()? 
          parentWidget()->winId() : winId()));

    params["parentWindowHandle"] = winhandle;
#else
    params["externalWindowHandle"] = 
        Ogre::StringConverter::toString
        ((size_t)(HWND) 
         (parentWidget()? 
          parentWidget()->winId() : winId()));
#endif

    win_ = root_-> createRenderWindow ("View", width(), height(), false, &params);


#ifndef Q_WS_WIN
    // take over ogre window
    // needed with parent windows
    if (parentWidget())
    {
        WId ogre_winid = 0x0;
        win_-> getCustomAttribute ("WINDOW", &ogre_winid);
        assert (ogre_winid);
        create (ogre_winid);
    }
#endif
}

void OgreWidget::resizeRenderWindow ()
{
    win_-> resize (width(), height());
    win_-> windowMovedOrResized ();
}

void TestWidget::mousePressEvent (QMouseEvent *e) 
{
    mousepos = e-> pos();
    if (mainnode) orientation = mainnode-> getOrientation();
    mousepressed = true;
}

void TestWidget::mouseReleaseEvent (QMouseEvent *e) 
{ 
    mousepressed = false; 
}

void TestWidget::mouseMoveEvent (QMouseEvent *e) 
{
    if (mousepressed) 
    {
        QPoint curPos = e-> pos();

        double w = width();
        double h = height();

        double curX = (curPos.x() * 2. - w) / w;
        double curY = (h - curPos.y() * 2.) / h;
        double x0 = (mousepos.x() * 2. - w) / w;
        double y0 = (h - mousepos.y() * 2.) / h;

        Ogre::Vector3 v1(x0, y0, 0);
        Ogre::Vector3 v2(curX, curY, 0);

        double radiusSqr = 0.8 * 0.8;
        double cutoff = radiusSqr * 0.5;
        double Rho = v1[0] * v1[0] + v1[1] * v1[1];
        v1[2] = (Rho < cutoff) ? sqrt(radiusSqr - Rho) : (cutoff / sqrt(Rho));

        Rho = v2[0] * v2[0] + v2[1] * v2[1];
        v2[2] = (Rho < cutoff) ? sqrt(radiusSqr - Rho) : (cutoff / sqrt(Rho));

        // v_cross is the normal of rotating plane
        Ogre::Vector3 cross = v2.crossProduct(v1);
        cross.normalise();

        // compute the angle
        v1.normalise();
        v2.normalise();
        double cosAngle = v1.dotProduct(v2);
        if (cosAngle < -1.0)
            cosAngle = -1.0;
        else if(cosAngle > 1.0)
            cosAngle = 1.0;
        double angle = acos(cosAngle);

        mainnode->rotate(cross, Ogre::Radian(angle));

        mousepos = curPos;
        orientation = mainnode->getOrientation();
    }
}

void TestWidget::resizeEvent (QResizeEvent *e)
{
    OgreWidget::resizeEvent (e);

    if (win_) 
    {
        cam-> setAspectRatio (Ogre::Real(width()) / Ogre::Real(height()));
        viewport-> update();
    }
}

//=============================================================================
//
TestWidget::TestWidget (Ogre::Root *root, QWidget *parent)
: OgreWidget (root, parent)
{
    sceneman = root_->createSceneManager (Ogre::ST_GENERIC);

    // Create the camera
    cam = sceneman->createCamera ("PlayerCam");
    cam->setPosition (Ogre::Vector3(0, 0, 200));

    // Look back along -Z
    cam->lookAt (Ogre::Vector3(0, 0, -300));
    cam->setNearClipDistance (5);

    // Create one viewport, entire window
    viewport = win_->addViewport (cam);
    viewport->setBackgroundColour (Ogre::ColourValue(0,0,0));
    viewport->setClearEveryFrame (true);

    sceneman->setAmbientLight (Ogre::ColourValue(0.6, 0.6, 0.6));

    // Setup the actual scene
    Ogre::Light* l = sceneman->createLight ("MainLight");
    l->setPosition(0, 100, 500);

    Ogre::Entity* mesh = sceneman->createEntity ("mesh", "dwarf.mesh");
    mainnode = sceneman->getRootSceneNode()->createChildSceneNode();
    mainnode->attachObject (mesh);

    cam->setAutoTracking (true, mainnode);

    Ogre::MaterialManager::getSingleton().setDefaultTextureFiltering (Ogre::TFO_BILINEAR);
    Ogre::MaterialManager::getSingleton().setDefaultAnisotropy (1);

    // Alter the camera aspect ratio to match the viewport
    cam->setAspectRatio (Ogre::Real(viewport->getActualWidth()) / Ogre::Real(viewport->getActualHeight()));
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

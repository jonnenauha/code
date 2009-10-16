/* main.cpp -- main module
 *
 *			Ryan McDougall -- 2009
 */

#include "main.h"
#include "ogrewidget.h"
#include "testwidget.h"

//=============================================================================

TestWidget::TestWidget (Ogre::Root *root, QWidget *parent) :
    OgreWidget (root, parent)
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
    //std::cout << "TestWidget::resizeEvent" << std::endl;

    OgreWidget::resizeEvent (e);

    if (win_) 
    {
        cam-> setAspectRatio (Ogre::Real(width()) / Ogre::Real(height()));
        viewport-> update();
    }
}

bool TestWidget::eventFilter (QObject *dst, QEvent *evt)
{
    // watch for resizes
    if (evt-> type() == QEvent::Resize)
        resizeEvent (static_cast <QResizeEvent *> (evt));
    
    // don't steal the event
    return false;
}

/* viewmodel.cpp -- 
 *
 *			Ryan McDougall -- 2009
 */

#include "main.h"
#include "viewmodel.h"
#include "point3d.h"

void GraphicsScene::drawBackground (QPainter *painter, const QRectF &rect)
{
    worldscene_-> draw_to_render_window ();

    // delay scene draw to aleviate flicker
    QTimer::singleShot (20, this, SLOT (update ())); 
}

void GraphicsScene::mouseMoveEvent (QGraphicsSceneMouseEvent *event)
{
    QGraphicsScene::mouseMoveEvent (event);

    if (event-> isAccepted())
        return;

    if (event-> buttons() & Qt::LeftButton) 
    {
        worldscene_-> mouseMoveEvent (mouse_event_translate (event));

        event-> accept();
        update();
    }
}

void GraphicsScene::mousePressEvent (QGraphicsSceneMouseEvent *event)
{
    QGraphicsScene::mousePressEvent (event);

    if (event-> isAccepted())
        return;

    worldscene_-> mousePressEvent (mouse_event_translate (event));

    event-> accept();
}

void GraphicsScene::mouseReleaseEvent (QGraphicsSceneMouseEvent *event)
{
    QGraphicsScene::mouseReleaseEvent (event);

    if (event-> isAccepted())
        return;

    worldscene_-> mouseReleaseEvent (mouse_event_translate (event));

    event-> accept();
    update();
}

QMouseEvent *GraphicsScene::mouse_event_translate (QGraphicsSceneMouseEvent *from)
{
    // position is always relative to root scene
    return new QMouseEvent 
        (from-> type(), 
         from-> scenePos().toPoint(), from-> scenePos().toPoint(), 
         from-> button(), from-> buttons(), from-> modifiers());
}


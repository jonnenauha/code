/* viewmodel.cpp -- 
 *
 *			Ryan McDougall -- 2009
 */

#include "main.h"
#include "viewmodel.h"

void GraphicsScene::drawBackground (QPainter *painter, const QRectF &rect)
{
    cout << "." << flush;

    worldscene_-> draw_to_render_window ();

    //if (buffer_) 
    //{
    //    // composit buffer over background
    //    QPainter::CompositionMode mode (painter-> compositionMode ());

    //    painter-> setCompositionMode (QPainter::CompositionMode_SourceOver);
    //    painter-> drawImage (QPoint (0, 0), *buffer_);
    //    painter-> setCompositionMode (mode);
    //}
}


void GraphicsScene::drawItems 
(QPainter *painter, int numItems, QGraphicsItem *items[], 
 const QStyleOptionGraphicsItem options[], QWidget *widget)
{
    cout << "!" << flush;

    QGraphicsScene::drawItems (painter, numItems, items, options, widget);

    //int w (painter-> device()-> width()), 
    //    h (painter-> device()-> height());

    //if (!buffer_ || (w != buffer_-> width()) || (h != buffer_-> height()))
    //{
    //    // resize buffer to match view size
    //    delete buffer_;
    //    buffer_ = new QImage (w, h, QImage::Format_ARGB32_Premultiplied);
    //}

    //buffer_-> fill (0x00000000);

    //QPainter redirected_painter (buffer_); // what about non-default painter state?
    //QGraphicsScene::drawItems (&redirected_painter, numItems, items, options, widget);
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


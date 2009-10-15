/* viewmodel.h -- 
 *
 *			Ryan McDougall -- 2009
 */

#ifndef _VIEWMODEL_H_
#define _VIEWMODEL_H_

#include "main.h"
#include "testwidget.h"
#include "point3d.h"

class GraphicsView : public QGraphicsView
{
    Q_OBJECT

    public:
        GraphicsView ()
            : QGraphicsView ()
        { }

        GraphicsView (QGraphicsScene *scene)
            : QGraphicsView (scene)
        { }

    protected:
        void resizeEvent (QResizeEvent *event) 
        {
            std::cout << "GraphicsView::resizeEvent" << std::endl;

            if (scene())
                scene()-> setSceneRect (QRect (QPoint (0, 0), event-> size ()));

            QGraphicsView::resizeEvent (event);
        }

    private:
};

class GraphicsScene : public QGraphicsScene
{
    Q_OBJECT

    public:
        GraphicsScene (TestWidget *world) 
            : worldscene_ (world)
        { }

        void drawBackground (QPainter *painter, const QRectF &rect);

    protected:

        void mousePressEvent (QGraphicsSceneMouseEvent *event); // { std::cout << "mousePressEvent" << std::endl; }
        void mouseReleaseEvent (QGraphicsSceneMouseEvent *event); // { std::cout << "mouseReleaseEvent" << std::endl; }
        void mouseMoveEvent (QGraphicsSceneMouseEvent *event); // { std::cout << "mouseMoveEvent" << std::endl; }

    private:

        QMouseEvent *mouse_event_translate (QGraphicsSceneMouseEvent *from);

        TestWidget *worldscene_;
};

#endif

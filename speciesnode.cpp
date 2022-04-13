#include "speciesnode.h"


SpeciesNode::SpeciesNode()
{
    pressed = false;
    setFlag(ItemIsMovable);
    setFlag(QGraphicsItem::ItemSendsScenePositionChanges);
    commonName = "";
    scientificName =  "";
}

SpeciesNode::SpeciesNode(QString _commonName, QString _scientificName)
{
    pressed = false;
    setFlag(ItemIsMovable);
    setFlag(QGraphicsItem::ItemSendsScenePositionChanges);
    commonName = _commonName;
    scientificName = _scientificName;
}

void SpeciesNode::addEdge(QGraphicsLineItem *line,QGraphicsPolygonItem *arrowItem, bool _isParent)
{
        //this->line = line;
        lines.push_back(line);
        arrowItems.push_back(arrowItem);
        isParent.push_back(_isParent);
}

QVariant SpeciesNode::itemChange(GraphicsItemChange change, const QVariant &value)
   {
       if (change == ItemPositionChange && scene()) {
           // value is the new position.
           QPointF newPosition = value.toPointF();

           for(unsigned int i = 0; i < lines.size(); i++)
            moveLineToCenter(newPosition,lines[i],arrowItems[i],isParent[i]);
       }
       return QGraphicsItem::itemChange(change, value);
   }

void SpeciesNode::moveLineToCenter(QPointF newPosition,QGraphicsLineItem *line,QGraphicsPolygonItem *arrowItem,bool isParent) {
       // to its center position
       int dx = boundingRect().x() + boundingRect().width()/2;
       int dy = boundingRect().y() + boundingRect().width()/2;

       QPointF newEllipsePosition = QPointF(newPosition.x() + dx, newPosition.y() + boundingRect().y());

       // Move the required point of the line to the center of the elipse
       QPointF parent,child;

       //checks if the ellipse is the parent or the child and
       //changes the point accordingly
       if(isParent)
       {
           parent = newEllipsePosition;
       }
       else
       {
           parent = line->line().p1();
       }

       if(isParent)
       {
           child = line->line().p2();
       }
       else
       {
           child = QPointF(newPosition.x() + dx, newPosition.y() + boundingRect().y() + boundingRect().width());
           //if you want center position QPointF(newPosition.x() + dx, newPosition.y() + dy);
       }


       line->setLine(QLineF(parent,child));

       //from Qt docs: https://doc.qt.io/qt-5/qtwidgets-graphicsview-diagramscene-example.html#arrow-class-definition
       qreal arrowSize = 20; // size of head


       double angle = std::atan2(-line->line().dy(), line->line().dx());
       QPointF arrowP1 = line->line().p1() + QPointF(sin(angle + M_PI / 3) * arrowSize,
                                             cos(angle + M_PI / 3) * arrowSize);
       QPointF arrowP2 = line->line().p1() + QPointF(sin(angle + M_PI - M_PI / 3) * arrowSize,
                                             cos(angle + M_PI - M_PI / 3) * arrowSize);

        QPolygonF arrowHead;
         arrowHead.clear();
         arrowHead << line->line().p1() << arrowP1 << arrowP2;

         //generate arrowhead
        arrowItem->setPolygon(arrowHead);

        //fill arrowhead
        QBrush brush(Qt::lightGray);
        brush.setStyle(Qt::SolidPattern);
        arrowItem->setBrush(brush);
   }


QRectF SpeciesNode::boundingRect() const
{
    return QRectF (0,0,100,100);
}

void SpeciesNode::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);
    QGraphicsEllipseItem ellipse = QGraphicsEllipseItem(boundingRect());
    QBrush brush(Qt::lightGray);
    brush.setStyle(Qt::SolidPattern);
    painter->setBrush(brush);
    painter->drawEllipse(ellipse.rect());
    QString name = commonName + QString("\n") + scientificName;
    painter->drawText(ellipse.rect(),Qt::AlignCenter, name);

}


void SpeciesNode::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    pressed = true;
    update();
    QGraphicsItem::mousePressEvent(event);
}

void SpeciesNode::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
        pressed = true;
        update();
        QGraphicsItem::mouseReleaseEvent(event);
}

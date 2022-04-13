#ifndef SPECIESNODE_H
#define SPECIESNODE_H
#include <QPainter>
#include <QGraphicsItem>
#include <QDebug>
#include "TaxonomyGraph.h"


class SpeciesNode : public QGraphicsItem
{
public:
    SpeciesNode();
    SpeciesNode(QString _commonName, QString _scientificName);

    //used to draw lines between nodes
    void addEdge(QGraphicsLineItem *line,QGraphicsPolygonItem *arrowItem, bool isPoint1);
    QVariant itemChange(GraphicsItemChange change, const QVariant &value);
    void moveLineToCenter(QPointF newPos,QGraphicsLineItem *line,QGraphicsPolygonItem *arrowItem, bool isParent);


    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);



protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

private:
     bool pressed;
    QString commonName;
    QString scientificName;
    vector<QGraphicsLineItem*> lines;
    vector<QGraphicsPolygonItem*> arrowItems;
    //QGraphicsLineItem *line;
    //QGraphicsPolygonItem *arrowItem;


    //is parent
    vector<bool> isParent;
    //bool isParent;
};

#endif // SPECIESNODE_H

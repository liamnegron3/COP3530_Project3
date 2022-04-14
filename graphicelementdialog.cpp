#include "graphicelementdialog.h"
#include "ui_graphicelementdialog.h"

GraphicElementDialog::GraphicElementDialog(vector<pair<string,string>> &speciesNames,QString display,pair<string,string> _parentSpecies, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::GraphicElementDialog)
{
    ui->setupUi(this);

    scene = new QGraphicsScene(this);
    ui->graphicsView->setScene(scene);

    parentSpecies = _parentSpecies;

    if(display == "Ancestor Tree")
        NodalAncestorPath(scene,speciesNames);
    else
        RelatedSpecies(scene,speciesNames);

/*
    node = new SpeciesNode(QString("Parent"),QString("Panthera onca"));
    SpeciesNode *node2 = new SpeciesNode(QString("Child"),QString("Panthera onca"));

    QLineF sceneLine = QLineF(40, 40, 80, 80);
    //from Qt docs: https://doc.qt.io/qt-5/qtwidgets-graphicsview-diagramscene-example.html#arrow-class-definition
    qreal arrowSize = 20;
    double angle = std::atan2(-sceneLine.dy(), sceneLine.dx());
    QPointF arrowP1 = sceneLine.p1() + QPointF(sin(angle + M_PI / 3) * arrowSize,
                                           cos(angle + M_PI / 3) * arrowSize);
    QPointF arrowP2 = sceneLine.p1() + QPointF(sin(angle + M_PI - M_PI / 3) * arrowSize,
                                           cos(angle + M_PI - M_PI / 3) * arrowSize);
    QPolygonF sceneArrowHead;
    sceneArrowHead.clear();
    sceneArrowHead << sceneLine.p1() << arrowP1 << arrowP2;

    QGraphicsLineItem *line = scene->addLine(sceneLine);
    QGraphicsPolygonItem *arrowHead = scene->addPolygon(sceneArrowHead);

    scene->addItem(node);
    scene->addItem(node2);

    node->addEdge(line,arrowHead,false);
    node2->addEdge(line,arrowHead,true);*/


}

void GraphicElementDialog::NodalAncestorPath(QGraphicsScene *scene,vector<pair<string,string>> &ancestorPath)
{
    //set base position
    int x = 0;
    int y = 0;

    //add root
    SpeciesNode* root = new SpeciesNode(ancestorPath[0].first.c_str(),ancestorPath[0].second.c_str());
    root->setPos(QPointF(x,y));
    scene->addItem(root);


    //add rest of the species
    for(unsigned int i = 1; i < ancestorPath.size(); i++)
    {
        SpeciesNode *child = new SpeciesNode(ancestorPath[i].first.c_str(),ancestorPath[i].second.c_str());

        QLineF sceneLine = QLineF(40, 40, 80, 80);
        //from Qt docs: https://doc.qt.io/qt-5/qtwidgets-graphicsview-diagramscene-example.html#arrow-class-definition
        qreal arrowSize = 20;
        double angle = std::atan2(-sceneLine.dy(), sceneLine.dx());
        QPointF arrowP1 = sceneLine.p1() + QPointF(sin(angle + M_PI / 3) * arrowSize,
                                               cos(angle + M_PI / 3) * arrowSize);
        QPointF arrowP2 = sceneLine.p1() + QPointF(sin(angle + M_PI - M_PI / 3) * arrowSize,
                                               cos(angle + M_PI - M_PI / 3) * arrowSize);
        QPolygonF sceneArrowHead;
        sceneArrowHead.clear();
        sceneArrowHead << sceneLine.p1() << arrowP1 << arrowP2;

        QGraphicsLineItem *line = scene->addLine(sceneLine);
        QGraphicsPolygonItem *arrowHead = scene->addPolygon(sceneArrowHead);
        int oldx = x;
        int oldy = y;
        x+=100;
        y+=150;

        child->setPos(QPointF(x,y));
        root->addEdge(line,arrowHead,false);
        child->addEdge(line,arrowHead,true);
        scene->addItem(child);
        child->itemChange(QGraphicsItem::ItemPositionChange,QPointF(x+1,y+1));
        root->itemChange(QGraphicsItem::ItemPositionChange,QPointF(oldx+1,oldy+1));

        root = child;
    }

}

void GraphicElementDialog::RelatedSpecies(QGraphicsScene *scene,vector<pair<string,string>> &siblings)
{
    //set base position
    int x = 0;
    int y = 0;

    //add root
    SpeciesNode* root = new SpeciesNode(parentSpecies.first.c_str(),parentSpecies.second.c_str());
    root->setPos(QPointF(x,y));
    scene->addItem(root);

    //add rest of the species
    for(unsigned int i = 0; i < siblings.size(); i++)
    {
        SpeciesNode *child = new SpeciesNode(siblings[i].first.c_str(),siblings[i].second.c_str());

        QLineF sceneLine = QLineF(40, 40, 80, 80);
        //from Qt docs: https://doc.qt.io/qt-5/qtwidgets-graphicsview-diagramscene-example.html#arrow-class-definition
        qreal arrowSize = 20;
        double angle = std::atan2(-sceneLine.dy(), sceneLine.dx());
        QPointF arrowP1 = sceneLine.p1() + QPointF(sin(angle + M_PI / 3) * arrowSize,
                                               cos(angle + M_PI / 3) * arrowSize);
        QPointF arrowP2 = sceneLine.p1() + QPointF(sin(angle + M_PI - M_PI / 3) * arrowSize,
                                               cos(angle + M_PI - M_PI / 3) * arrowSize);
        QPolygonF sceneArrowHead;
        sceneArrowHead.clear();
        sceneArrowHead << sceneLine.p1() << arrowP1 << arrowP2;

        QGraphicsLineItem *line = scene->addLine(sceneLine);
        QGraphicsPolygonItem *arrowHead = scene->addPolygon(sceneArrowHead);
        x+=100;
        y+=100;
        child->setPos(QPointF(x,y));
        scene->addItem(child);
        root->addEdge(line,arrowHead,false);
        child->addEdge(line,arrowHead,true);
        child->itemChange(QGraphicsItem::ItemPositionChange,QPointF(x+1,y+1));
    }
}

GraphicElementDialog::~GraphicElementDialog()
{
    delete node;
    delete ui;
}


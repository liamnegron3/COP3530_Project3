#ifndef GRAPHICELEMENTDIALOG_H
#define GRAPHICELEMENTDIALOG_H

#include <QDialog>
#include <QtCore>
#include <QtWidgets>
#include <QGraphicsView>
#include "speciesnode.h"

QT_BEGIN_NAMESPACE
namespace Ui { class GraphicElementDialog; }
QT_END_NAMESPACE

class GraphicElementDialog : public QDialog
{
    Q_OBJECT

public:
    void DrawLineWithArrow(QPainter& painter, QPoint start, QPoint end);
    void NodalAncestorPath(QGraphicsScene *scene,vector<pair<string,string>> &ancestorPath);
    void RelatedSpecies(QGraphicsScene *scene,vector<pair<string,string>> &siblings);
    GraphicElementDialog(vector<pair<string,string>> &speciesNames,QString display,pair<string,string> parentSpecies, QWidget *parent = nullptr);
    ~GraphicElementDialog();

private:
    Ui::GraphicElementDialog *ui;
    QGraphicsScene *scene;
    SpeciesNode *node;
    pair<string,string> parentSpecies;
    int nodeCount;
};
#endif // GRAPHICELEMENTDIALOG_H

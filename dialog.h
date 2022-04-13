#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QtCore>
#include <QtWidgets>
#include <TaxonomyGraph.h>

QT_BEGIN_NAMESPACE
namespace Ui { class Dialog; }
QT_END_NAMESPACE

class Dialog1 : public QDialog
{
    Q_OBJECT
QTreeWidgetItem* AddRoot(QString name, QString Description);
QTreeWidgetItem* AddChild(QTreeWidgetItem *parent, QString name, QString Description);

public:
    Dialog1(vector<pair<string,string>> &ancestorPath,QWidget *parent = nullptr);
    ~Dialog1();

private:
    Ui::Dialog *ui;

//protected:
  //  void paintEvent(QPaintEvent *e);
};
#endif // DIALOG_H

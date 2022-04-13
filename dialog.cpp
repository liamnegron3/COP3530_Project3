#include "dialog.h"
#include "ui_dialog.h"
#include "TaxonomyGraph.h"
#include <QtCore>
#include <QtWidgets>

Dialog1::Dialog1(vector<pair<string,string>> &ancestorPath, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Dialog)
{
    ui->setupUi(this);

    //generate large tree list
    ui->treeWidget->setColumnCount(2);
    ui->treeWidget->setHeaderLabels(QStringList() << "Common Name" << "Scientific Name");


    //create root
    QTreeWidgetItem *root = AddRoot(ancestorPath[0].first.c_str(),ancestorPath[0].second.c_str());
    //generate rest of tree
    for(int i = 1; i < ancestorPath.size(); i++)
    {
        root = AddChild(root,ancestorPath[i].first.c_str(),ancestorPath[i].second.c_str());
    }

}

Dialog1::~Dialog1()
{
    delete ui;
}

QTreeWidgetItem* Dialog1::AddRoot(QString name, QString Description)
{
    QTreeWidgetItem *itm = new QTreeWidgetItem(ui->treeWidget);
    itm->setText(0,name);
    itm->setText(1,Description);
    itm->setExpanded(true);
    return itm;
}
QTreeWidgetItem* Dialog1::AddChild(QTreeWidgetItem *parent, QString name, QString Description)
{
    QTreeWidgetItem *itm = new QTreeWidgetItem(parent);
    itm->setText(0,name);
    itm->setText(1,Description);
    itm->setExpanded(true);
    return itm;
}



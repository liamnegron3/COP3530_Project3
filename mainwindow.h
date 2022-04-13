#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <dialog.h>
#include <graphicelementdialog.h>
#include <TaxonomyGraph.h>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_clicked();

private:
    Ui::MainWindow *ui;
    //graph to store the species
    TaxonomyGraph speciesGraph;
    Dialog1        *objDialog;
    GraphicElementDialog *objGraphicDialog;
    //store the names
    QString commonName1;
    QString commonName2;

};
#endif // MAINWINDOW_H

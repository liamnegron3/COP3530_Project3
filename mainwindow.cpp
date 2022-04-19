#include "mainwindow.h"
#include "ui_mainwindow.h"



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    QString windowTitle("Taxon Search");
    ui->setupUi(this);

    this->setWindowTitle(windowTitle);

    //combo box for search algorithm selection
    ui->comboBox->addItem("Quick Sort");
    ui->comboBox->addItem("Merge Sort");

    //get data into graph
    speciesGraph.ReadTaxonomyIDs("C:/Users/Liam/Documents/School/Class/Year 2/Spring 2022/COP 3530/Projects/Project3/Data1/NameUsage.tsv");
    speciesGraph.ReadCommonNames("C:/Users/Liam/Documents/School/Class/Year 2/Spring 2022/COP 3530/Projects/Project3/Data1/VernacularName.tsv");
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushButton_clicked()
{

    //get Names
    QcommonName1 = ui->lineEdit->text();
    QcommonName2 = ui->lineEdit_2->text();
    commonName1 = QcommonName1.toStdString();
    commonName2 = QcommonName2.toStdString();

    //verifyNames
    speciesGraph.verifyName(commonName1);
    speciesGraph.verifyName(commonName2);

    //Shortest Path button is pressed
    if(ui->radioButton->isChecked())
    {
        //validate Names throw error if not found in database or misspelled
        if(speciesGraph.NameExists(commonName1) == false || speciesGraph.NameExists(commonName2) == false)
        {
            //Error Dialog
            QMessageBox::warning(this, tr("Error"),tr("Species Name Not Found."));
        }
        else
        {
            vector<pair<string,string>> ancestorPath = speciesGraph.CommonAncestorPath(commonName1, commonName2);

            //check if size limit is exceeded
            if(ancestorPath.size() > 1000)
            {
                //find the common ancestor
                pair<string,string> commonAncestorPair = speciesGraph.CommonAncestor(commonName1,commonName2);
                QString commonAncestorCommon = QString::fromStdString(commonAncestorPair.first);
                QString commonAncestorScientific = QString::fromStdString(commonAncestorPair.second);

                //print the error message
                QString message = QString("Unable to display more than 1000 Species in shortest path between species.\n\nDo you want to find the common ancestor instead?");

                QMessageBox::StandardButton reply;
                reply = QMessageBox::warning(this,"Shortest Path Between Species",message,QMessageBox::Yes | QMessageBox::No);

                if(reply == QMessageBox::Yes)
                {
                    if(commonAncestorCommon.size() > 0)
                        message = commonAncestorCommon + QString(", ") + commonAncestorScientific;
                    else
                        message = commonAncestorScientific;

                    QMessageBox::information(this,"Common Ancestor",message);
                }
            }
            else
            {
                QString title = QString("Shortest Ancestor Path Between ") + QcommonName1 + QString(" and ") + QcommonName2;
                objDialog = new Dialog1(ancestorPath, this);
                objDialog->setWindowTitle(title);
                objDialog->show();
            }
            }
    }

    //Ancestor Path button is pressed
    if(ui->radioButton_2->isChecked())
    {

        if(speciesGraph.NameExists(commonName1) == false)
        {
            //Error Dialog
            QMessageBox::warning(this, tr("Error"),tr("Species Name Not Found."));
        }
        else
        {
            QString title = QcommonName1 + QString(" Ancestry Tree");

            vector<pair<string,string>> ancestorTree = speciesGraph.SpeciesAncestorTree(commonName1);
            objGraphicDialog = new GraphicElementDialog(ancestorTree,QString("Ancestor Tree"),ancestorTree[0],this);
            objGraphicDialog->setWindowTitle(title);
            objGraphicDialog->show();

            objDialog = new Dialog1(ancestorTree, this);
            objDialog->setWindowTitle(title);
            objDialog->show();
        }
    }

    //Common Ancestor Path button is pressed
    if(ui->radioButton_3->isChecked())
    {

        //validate Names throw error if not found in database or misspelled
        if(speciesGraph.NameExists(commonName1) == false || speciesGraph.NameExists(commonName2) == false)
        {
            //Error Dialog
            QMessageBox::warning(this, tr("Error"),tr("Species Name Not Found."));
        }
        else
        {

        //find the common ancestor
        pair<string,string> commonAncestorPair = speciesGraph.CommonAncestor(commonName1,commonName2);
        QString commonAncestorCommon = QString::fromStdString(commonAncestorPair.first);
        QString commonAncestorScientific = QString::fromStdString(commonAncestorPair.second);
        QString message;

        if(commonAncestorCommon.size() > 0)
            message = commonAncestorCommon + QString(", ") + commonAncestorScientific;
        else
            message = commonAncestorScientific;

        QMessageBox::information(this,"Common Ancestor",message);
        }
    }

    if(ui->radioButton_4->isChecked())
    {
        //Check if name exists
        if(speciesGraph.NameExists(commonName1) == false)
        {
            //Error Dialog
            QMessageBox::warning(this, tr("Error"),tr("Species Name Not Found."));
        }
        else
        {
             string sort = "";
            if(ui->comboBox->currentText() == "Quick Sort")
                sort = "Quick";
            else if(ui->comboBox->currentText() == "Merge Sort")
                sort = "Merge";

            auto start = chrono::high_resolution_clock::now();
            vector<pair<string,string>> siblings = speciesGraph.findSiblings(commonName1,sort);
            auto stop = chrono::high_resolution_clock::now();

            auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);

            string tempTime = to_string(duration.count());
            QString time = tempTime.c_str() + QString(" us");
            QString sortTitle = sort.c_str() + QString(" Sort Time");


            QMessageBox::information(this,sortTitle, time);
            //check sibling size to prevent the application from crashing
            if(siblings.size() == 0)
            {
                QMessageBox::information(this,"Related Species","No Related Species");
            }
            else if(siblings.size() > 5000)
            {
                QString message = QString("Unable to display more than 5000 related species.\n\nDo you want to display a reduced number instead?");

                QMessageBox::StandardButton reply;
                reply = QMessageBox::warning(this,"Related Species",message,QMessageBox::Yes | QMessageBox::No);

                if(reply == QMessageBox::Yes)
                {
                    siblings.resize(5000);

                    objDialog = new Dialog1(siblings,this);
                    QString title = QcommonName1 + QString(" Siblings");
                    objDialog->setWindowTitle(title);
                    objDialog->show();
                }

            }
            else if (siblings.size() > 1000)
            {
                QString message = QString("Unable to display more than 1000 related species graphically.\n\nDo you want to display a reduced number graphically?");

                QMessageBox::StandardButton reply;
                reply = QMessageBox::warning(this,"Related Species",message,QMessageBox::Yes | QMessageBox::No);

                if(reply == QMessageBox::Yes)
                {
                    siblings.resize(1000);
                    QString title = QcommonName1 + QString(" Siblings");

                    pair<string,string> parentSpecies = speciesGraph.getParentName(siblings[0].first);
                    objGraphicDialog = new GraphicElementDialog(siblings,QString("Related Siblings"),parentSpecies,this);
                    objGraphicDialog->setWindowTitle(title);
                    objGraphicDialog->show();

                    objDialog = new Dialog1(siblings,this);
                    objDialog->setWindowTitle(title);
                    objDialog->show();
                }
                if(reply == QMessageBox::No)
                {
                    QString message = QString("Do you want to display all related species in a text tree?");

                    QMessageBox::StandardButton reply2;
                    reply2 = QMessageBox::information(this,"Related Species",message,QMessageBox::Yes | QMessageBox::No);

                    if(reply2 == QMessageBox::Yes)
                    {
                        objDialog = new Dialog1(siblings,this);
                        QString title = QcommonName1 + QString(" Siblings");
                        objDialog->setWindowTitle(title);
                        objDialog->show();
                    }
                }
            }
            else
            {

                pair<string,string> parentSpecies = speciesGraph.getParentName(commonName1);

                QString title = QcommonName1 + QString(" Siblings");

                objGraphicDialog = new GraphicElementDialog(siblings,QString("Related Siblings"),parentSpecies,this);
                objGraphicDialog->setWindowTitle(title);
                objGraphicDialog->show();

                objDialog = new Dialog1(siblings,this);
                objDialog->setWindowTitle(title);
                objDialog->show();

            }
        }
    }

    if(ui->radioButton_5->isChecked())
    {

        string sort = "";
       if(ui->comboBox->currentText() == "Quick Sort")
           sort = "Quick";
       else if(ui->comboBox->currentText() == "Merge Sort")
           sort = "Merge";

       auto start = chrono::high_resolution_clock::now();
       vector<pair<string,string>> recommendedSpecies = speciesGraph.findRecommended(commonName1,commonName2,sort);
       auto stop = chrono::high_resolution_clock::now();

       auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);

       string tempTime = to_string(duration.count());
       QString time = tempTime.c_str() + QString(" ms");
       QString sortTitle = sort.c_str() + QString(" Sort Time");

       QMessageBox::information(this,sortTitle, time);

        QString title = QString("Top 10 Recommended Species");
        objDialog = new Dialog1(recommendedSpecies,this);
        objDialog->setWindowTitle(title);
        objDialog->show();
    }
}


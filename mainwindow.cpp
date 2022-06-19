#include "mainwindow.h"
#include "ui_mainwindow.h"



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{


    ui->setupUi(this);
    wind= new Window;



    pars=new parser(this);
    ui->treeView->setModel(pars);
    ui->treeView->setContextMenuPolicy(Qt::ContextMenuPolicy());


    setWindowTitle(tr("Работа с xml-файлами"));

    ui->treeView->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->treeView, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(slotContexMenu(QPoint)));
}



void MainWindow::slotContexMenu(QPoint point){
    if(pars->objByIndex(pars->parent(ui->treeView->indexAt(point)))==pars->isPointerNode()){
        QMenu* menu=new QMenu(this);
        QAction* activ=new QAction(tr("Сделать файл активным"),this);
        QAction* close=new QAction(tr("Закрыть"),this);
        newWindow = new QAction(tr("Открыть файл в новом окне"));
        connect(activ,SIGNAL(triggered()),pars,SLOT(slotMakeActive()));
        connect(close,SIGNAL(triggered()),this,SLOT(slotClosefile()));
        connect(newWindow, SIGNAL(triggered()), this, SLOT(slotClosefile()));
        connect(newWindow, SIGNAL(triggered()), this, SLOT(some_slot()));
        connect(this, SIGNAL(transfer(QStringList)), wind, SLOT(protectiontask(QStringList)));
        pars->setHelpingIndexObj(ui->treeView->indexAt(point));
        menu->addAction(activ);
        menu->addAction(close);
        menu->addAction(newWindow);
        menu->exec(QCursor::pos());

    }
}

void MainWindow::slotClosefile(){
    QObject* newPointerToTree=new QObject(this);
    bool isActive=false;


    for(QObject* a:pars->isPointerNode()->children()){
        if(a==pars->objByIndex(pars->isHelpingIndexObj())){
             str.append(a->property("absoluteFilePath").toString());
            a->setParent(newPointerToTree);}
        else if (a->property("font")==true) isActive=true;
    }
    if(!isActive) pars->isPointerNode()->children().at(0)->setProperty("font",true);
    ui->treeView->reset();
}

void MainWindow::on_op_triggered()
{
    QStringList FilesName=QFileDialog::getOpenFileNames(this,tr("Открыть XML-файл(-ы)"),"","XML-файл(*.xml)");
    pars->addFiles(&FilesName);
    ui->treeView->reset();

}


void MainWindow::on_cl_triggered()
{
    delete pars;
    pars=new parser(this);
    ui->treeView->setModel(pars);
    ui->treeView->reset();
}


void MainWindow::on_exit_triggered()
{
    close();
}

void MainWindow::some_slot(){
    emit transfer(str);
    wind->show();
    str.clear();



}



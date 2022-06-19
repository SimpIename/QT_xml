#include "window.h"
#include "ui_window.h"
#include <QWidget>

Window::Window(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Window)
{

    ui->setupUi(this);
    pars = new parser(this);
    ui->treeView->setModel(pars);




}

Window::~Window()
{
    delete ui;
}



void Window::protectiontask(QStringList FilesName){

    pars->addFiles(&FilesName);
    ui->treeView->reset();





}

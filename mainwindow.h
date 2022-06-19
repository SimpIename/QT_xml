#ifndef MYDIALOG_H
#define MYDIALOG_H

#include "window.h"


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);

private:
    Ui::MainWindow *ui;
    Window *wind;
    QStringList str;


    parser* pars;
    QAction* newWindow;

private slots:
    void slotContexMenu(QPoint);

    void slotClosefile();
    void on_op_triggered();
    void on_cl_triggered();
    void on_exit_triggered();
    void some_slot();
signals:
    void transfer(QStringList);
};

#endif

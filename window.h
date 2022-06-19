#ifndef WINDOW_H
#define WINDOW_H

#include <QWidget>
#include "parser.h"


namespace Ui {
class Window;
}

class Window : public QWidget
{
    Q_OBJECT

public:
    explicit Window(QWidget *parent = nullptr);
    ~Window();



private:
    Ui::Window *ui;
    parser* pars;


public slots:
    void protectiontask(QStringList);
};

#endif // WINDOW_H

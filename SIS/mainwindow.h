#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui>

#include <QtWidgets/QMainWindow>
#include <QtWidgets/QGraphicsScene>
#include <QtWidgets/QGraphicsPixmapItem>

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

    QGraphicsScene *m_pScene;
    QGraphicsPixmapItem *m_pGraphicsItem;
};

#endif // MAINWINDOW_H

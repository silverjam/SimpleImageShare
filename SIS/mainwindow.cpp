#include "mainwindow.h"
#include "ui_mainwindow.h"

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    m_pScene = new QGraphicsScene(this);

    QPixmap png;
    if ( ! png.load(":/baboon.png") )
        qDebug("load failed");

    m_pGraphicsItem = m_pScene->addPixmap(png);

    ui->graphicsView->setScene(m_pScene);
    ui->graphicsView->show();
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
MainWindow::~MainWindow()
{
    delete ui;
}

#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include <QScreen>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    _tdw = new Qt3DExtras::Qt3DWindow;
    QWidget *container = QWidget::createWindowContainer(_tdw);
    QSize screenSize = screen()->size();
    container->setMinimumSize(QSize(200, 100));
    container->setMaximumSize(screenSize);

    ui->container->layout()->addWidget(container);
}

MainWindow::~MainWindow()
{
    delete ui;
}

Qt3DExtras::Qt3DWindow *MainWindow::tdw() const
{
    return _tdw;
}

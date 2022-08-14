#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <Qt3DWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    Qt3DExtras::Qt3DWindow *tdw() const;

private:
    Ui::MainWindow *ui;
    Qt3DExtras::Qt3DWindow *_tdw;
};
#endif // MAINWINDOW_H

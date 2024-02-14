#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QgraphicsView>
#include <QGraphicsItem>
#include <vector>

#include "autorobot.h"
#include "rectangle.h"

#include <QTimer>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_btnCreateRobot_clicked();

    void on_btnTest_clicked();

    void on_btnTest_2_clicked();

    void on_btn_applyWorldConfigSize_clicked();


private:
    Ui::MainWindow *ui;
    QGraphicsScene *scene;

    std::vector<AutoRobot*> robots;
    std::vector<Rectangle*> obstacles;

    QTimer* timer;
};
#endif // MAINWINDOW_H

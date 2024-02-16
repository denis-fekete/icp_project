#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QgraphicsView>
#include <QGraphicsItem>

#include <vector>
#include <random>

#include "autorobot.h"
#include "rectangle.h"
#include "obstacle.h"
#include "simulator.h"


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


    void on_input_robot_randomizeColors_toggled(bool checked);

    void on_btnCreateObstacle_clicked();

    void on_input_obstacle_randomizeColors_toggled(bool checked);

    void on_btn_update_info_clicked();

    void on_input_robot_IDSelector_valueChanged(int arg1);

protected:
    Ui::MainWindow *ui;
    QGraphicsScene *scene;

    std::vector<AutoRobot*> robots;
    std::vector<Obstacle*> obstacles;
    AutoRobot* activeRobot;
    Obstacle* activeObstacle;
    Simulator* simulator;

    QTimer* timer;
    QTime*  clock;

private:
    void DrawGrid(unsigned density);
};
#endif // MAINWINDOW_H

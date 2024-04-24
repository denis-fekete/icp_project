#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QgraphicsView>
#include <QGraphicsItem>

#include <vector>
#include <random>

#include "QtSpecific/autorobot.h"
#include "QtSpecific/obstacle.h"
#include "2DSimulationLib/simulator.h"
#include "2DSimulationLib/randomgenerator.h"
#include "QtSpecific/savemanager.h"

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

    QColor getRandomColor();
private:
    // UI elements
    Ui::MainWindow *ui;
    QGraphicsScene *scene;
    // Objects in 2D space
    std::vector<std::unique_ptr<AutoRobot>> robots;
    std::vector<std::unique_ptr<Obstacle>> obstacles;
    AutoRobot* activeRobot;
    Obstacle* activeObstacle;
    // Simulation
    std::unique_ptr<Simulator> simulator;
    QTimer timer;
    // Generation of pseudo random numbers
    std::unique_ptr<RandomGenerator> randColor;
    std::unique_ptr<RandomGenerator> rand1000;

    void DrawGrid(unsigned density);
    void resizeEvent(QResizeEvent*);
    // Save/Load
    std::unique_ptr<SaveManager> saveManager;


// Analytics
public slots:
    void updateAnalytics();

private slots:
    void on_btnCreateRobot_clicked();

    void on_btnTest_clicked();

    void on_btnTest_2_clicked();

    void on_input_robot_randomizeColors_toggled(bool checked);

    void on_btnCreateObstacle_clicked();

    void on_input_obstacle_randomizeColors_toggled(bool checked);

    void on_input_robot_IDSelector_valueChanged(int arg1);

    void on_input_robot_onCollisionTurnLeft_clicked(bool checked);

    void on_input_robot_onCollisionTurnRight_clicked(bool checked);

    void on_btn_loadBenchmark_clicked();

    void on_btn_worldAddMoreRobots_clicked();

    void on_btn_worldApplySize_clicked();

    void closeEvent(QCloseEvent *event);

    void on_saveManger_btn_load_clicked();

    void on_saveManager_btn_save_clicked();

    void on_btnDeleteRobot_clicked();

    void on_program_btn_hide_clicked();

    void on_program_btn_pause_clicked();

    void on_program_btn_resume_clicked();
};
#endif // MAINWINDOW_H

/**
 * @file mainwindow.h
 * @brief Header file for MainWindow class that contains methods, slots 
 * (methods called from GUI) and internal attributes.
 *
 * @author Denis Fekete (xfeket01@fit.vutbr.cz)
 */

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsItem>
#include <QTimer>

#include <vector>
#include <random>

#include "QtSpecific/simulator.h"
#include "2DSimulationLib/randomgenerator.h"
#include "QtSpecific/customscene.h"
#include "QtSpecific/savemanager.h"


QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    /**
     * @brief MainWindow Constructor of main window
     * @param parent Parent of main window
     */
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    /**
     * @return Returns random color
     */
    QColor getRandomColor();
private:

    /**
     * @brief ui UI elements of application
     */
    Ui::MainWindow *ui;

    /**
     * @brief scene Pointer to the scene to which graphics items are added
     */
    std::unique_ptr<CustomScene> scene;

    /**
     * @brief simulator Pointer to the simulator object
     */
    std::unique_ptr<Simulator> simulator;

    /**
     * @brief randColor Random color generator
     */
    std::unique_ptr<RandomGenerator> randColor;

    /**
     * @brief resizeEvent Handling of window resizing
     */
    void resizeEvent(QResizeEvent*);

    /**
     * @brief saveManager Object for loading and saving into files
     */
    std::unique_ptr<SaveManager> saveManager;

    QTimer menuUpdate;

private slots:
    void updateMenuGUI();

    void on_btnCreateRobot_clicked();
    void on_btnCreateObstacle_clicked();
    void on_btn_worldApplySize_clicked();
    void closeEvent(QCloseEvent *event);
    void on_saveManger_btn_load_clicked();
    void on_saveManager_btn_save_clicked();
    void on_btnDeleteRobot_clicked();
    void on_btnDeleteObstacle_clicked();
    void on_program_btn_resumepause_clicked();
    void on_input_manualrobot_forward_clicked();
    void on_input_manualrobot_stay_clicked();
    void on_input_manualrobot_anticlockwise_clicked();
    void on_input_manualrobot_clockwise_clicked();
    void on_input_robot_randomizeColors_stateChanged(int arg1);
    void on_input_obstacle_randomizeColors_stateChanged(int arg1);
    void on_input_selectAutomatic_stateChanged(int arg1);
    void on_input_selectManual_stateChanged(int arg1);
    void on_world_applyThreadCount_clicked();
    void on_input_robot_updateValues_clicked();
    void on_input_obstacle_updateValues_clicked();
    void on_input_robot_anticlockwise_stateChanged(int arg1);
    void on_input_robot_clockwise_stateChanged(int arg1);
};
#endif // MAINWINDOW_H

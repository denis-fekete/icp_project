#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>
#include <QStyleFactory>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    // ------------------------------------------------------------------------
    // Setup internal objects/variables
    // value 235 means that values in RGB wont be too bright,
    // potentially resulting in white robot on white background
    randColor = std::make_unique<RandomGenerator>(0, 235);
    // ------------------------------------------------------------------------
    // Setup graphics view
    ui->setupUi(this);

    scene = std::make_unique<CustomScene> (this);
    ui->graphicsView->setScene(scene.get());
    ui->graphicsView->setRenderHint(QPainter::Antialiasing);

    ui->graphicsView->setViewportUpdateMode(QGraphicsView::BoundingRectViewportUpdate);
    ui->graphicsView->setDragMode(QGraphicsView::ScrollHandDrag);

    QApplication::setStyle(QStyleFactory::create("Fusion"));

    // ------------------------------------------------------------------------
    // Setup simulation
    simulator = std::make_unique<Simulator>(
        *scene,
        ui->sBox_worldc_sizeX->value(),
        ui->sBox_worldc_sizeY->value(),
        this->window()->size().width(),
        this->window()->size().height()
        );

    // set thread count

    simulator->setTimerPeriod(30);

    // add simulator to CustomScene
    scene->setSimulator(simulator.get());

    // ------------------------------------------------------------------------
    // Apply world configuration
    on_btn_worldApplySize_clicked();
}

MainWindow::~MainWindow()
{
    delete ui;
    scene.release();
}

void MainWindow::resizeEvent(QResizeEvent*)
{
    #define SPACING 5

    const auto graphicalSceneW = this->window()->size().width()
                                 - ui->mainMenu->geometry().width()
                                 - SPACING;
    const auto graphicalSceneH = this->window()->size().height() - SPACING;

    ui->graphicsView->setGeometry(ui->mainMenu->geometry().width() + SPACING,
                                  SPACING,
                                  graphicalSceneW,
                                  graphicalSceneH - SPACING);


    ui->mainMenu->setGeometry(SPACING,
                              ui->program_btn_resumepause->geometry().height(),
                              ui->mainMenu->geometry().width(),
                              graphicalSceneH - ui->program_btn_resumepause->geometry().height() - 5 * SPACING);

    simulator->setWindowSize(graphicalSceneW, graphicalSceneH);

    #undef SPACING
}

QColor MainWindow::getRandomColor()
{
    // RGB values from 0 - 235, not 255 in case three 255 were randomized
    return QColor(  randColor->getRandomValue(),
                    randColor->getRandomValue(),
                    randColor->getRandomValue());
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if(event){}
    simulator->stopSimulation();

    simulator.release();
    exit(0);
}

//----------------------------------------------------------------------------
// Main window
//----------------------------------------------------------------------------

void MainWindow::on_program_btn_resumepause_clicked()
{
    if(ui->groupBox_world->isEnabled())
    {
        ui->program_btn_resumepause->setText("Pause");
        simulator.get()->runSimulation();
        ui->groupBox_obstacles->setEnabled(false);
        ui->groupBox_robotcreator->setEnabled(false);
        ui->groupBox_world->setEnabled(false);
    }
    else
    {
        ui->program_btn_resumepause->setText("Resume");
        simulator.get()->stopSimulation();
        ui->groupBox_obstacles->setEnabled(true);
        ui->groupBox_robotcreator->setEnabled(true);
        ui->groupBox_world->setEnabled(true);
    }
}

//----------------------------------------------------------------------------
// World tab
//----------------------------------------------------------------------------

void MainWindow::on_saveManger_btn_load_clicked()
{
    // check if manager was initialized
    if(!saveManager)
    {
        saveManager = std::make_unique<SaveManager> (*simulator.get(), this);
    }

    saveManager.get()->loadFromFile();

    scene->update();
}

void MainWindow::on_saveManager_btn_save_clicked()
{
    // check if manager was initialized
    if(!saveManager)
    {
        saveManager = std::make_unique<SaveManager> (*simulator.get(), this);
    }

    saveManager.get()->saveToFile();
}


void MainWindow::on_btn_worldApplySize_clicked()
{
    simulator->setSimulationSize(
        ui->sBox_worldc_sizeX->value(),
        ui->sBox_worldc_sizeY->value());
}

void MainWindow::on_world_applyThreadCount_clicked()
{
    simulator->changeThreadCount(ui->sBox_world_threadCount->value());
    ui->world_applyThreadCount->setEnabled(false);
}


//----------------------------------------------------------------------------
// Create Robot tab
//----------------------------------------------------------------------------

void MainWindow::on_btnCreateRobot_clicked()
{

    QColor color;
    if(ui->input_obstacle_randomizeColors->isChecked())
    {
        color = MainWindow::getRandomColor();
    }
    else
    {
        color = QColor( ui->input_obstacle_color_r->value(),
                       ui->input_obstacle_color_g->value(),
                       ui->input_obstacle_color_b->value());
    }

    color = MainWindow::getRandomColor();

    if(ui->input_selectAutomatic->isChecked())

    {
        const short turnDirection = (ui->input_robot_onCollisionTurnRight->isChecked())? 1 : -1;

        simulator->addAutomaticRobot(
                ui->input_robot_xPos->value(),
                ui->input_robot_yPos->value(),
                ui->input_robot_radius->value(),
                ui->input_robot_rotation->value(),
                ui->input_robot_detRadius->value(),
                color,
                ui->input_robot_speed->value(),
                ui->input_robot_collisionDetectionAngle->value(),
                turnDirection
                );
    }
    else
    {
        simulator->addManualRobot(
            ui->input_robot_xPos->value(),
            ui->input_robot_yPos->value(),
            ui->input_robot_radius->value(),
            ui->input_robot_rotation->value(),
            ui->input_robot_detRadius->value(),
            color
            );
    }
}

void MainWindow::on_btnDeleteRobot_clicked()
{
    // erase robot
    simulator->deleteRobot();

    scene->update();
}

void MainWindow::on_input_robot_onCollisionTurnLeft_stateChanged(int arg1)
{
    ui->input_robot_onCollisionTurnLeft->setChecked(arg1);
    ui->input_robot_onCollisionTurnRight->setChecked(!arg1);

}


void MainWindow::on_input_robot_onCollisionTurnRight_stateChanged(int arg1)
{
    ui->input_robot_onCollisionTurnRight->setChecked(arg1);
    ui->input_robot_onCollisionTurnLeft->setChecked(!arg1);

}

void MainWindow::on_input_selectAutomatic_stateChanged(int arg1)
{
    ui->input_selectAutomatic->setChecked(arg1);
    ui->input_selectManual->setChecked(!arg1);

    ui->input_robot_speed->setEnabled(true);
    ui->input_robot_onCollisionTurnLeft->setEnabled(true);
    ui->input_robot_onCollisionTurnRight->setEnabled(true);
    ui->input_robot_collisionDetectionAngle->setEnabled(true);
}


void MainWindow::on_input_selectManual_stateChanged(int arg1)
{
    ui->input_selectManual->setChecked(arg1);
    ui->input_selectAutomatic->setChecked(!arg1);

    ui->input_robot_speed->setEnabled(false);
    ui->input_robot_onCollisionTurnLeft->setEnabled(false);
    ui->input_robot_onCollisionTurnRight->setEnabled(false);
    ui->input_robot_collisionDetectionAngle->setEnabled(false);
}

void MainWindow::on_input_robot_randomizeColors_stateChanged(int arg1)
{
    ui->input_robot_color_r->setEnabled(!arg1);
    ui->input_robot_color_g->setEnabled(!arg1);
    ui->input_robot_color_b->setEnabled(!arg1);
}

//----------------------------------------------------------------------------
// Obstacle tab
//----------------------------------------------------------------------------

void MainWindow::on_btnCreateObstacle_clicked()
{

    QColor color;
    if(ui->input_obstacle_randomizeColors->isChecked())
    {
        color = MainWindow::getRandomColor();
    }
    else
    {
        color = QColor( ui->input_obstacle_color_r->value(),
                       ui->input_obstacle_color_g->value(),
                       ui->input_obstacle_color_b->value());
    }

    simulator->addObstacle(
            ui->input_obstacle_xPos->value(),
            ui->input_obstacle_yPos->value(),
            ui->input_obstacle_width->value(),
            ui->input_obstacle_height->value(),
            ui->input_obstacle_rotation->value(),
            color);
}

void MainWindow::on_input_obstacle_randomizeColors_stateChanged(int arg1)
{
    ui->input_obstacle_color_r->setEnabled(!arg1);
    ui->input_obstacle_color_g->setEnabled(!arg1);
    ui->input_obstacle_color_b->setEnabled(!arg1);
}


void MainWindow::on_btnDeleteObstacle_clicked()
{
    // erase robot
    simulator->deleteObstacle();

    scene->update();
}

//----------------------------------------------------------------------------
// Control Robots tab
//----------------------------------------------------------------------------

void MainWindow::on_input_manualrobot_forward_clicked()
{
    ui->input_manualrobot_stay->setChecked(false);
    ui->input_manualrobot_anticlockwise->setChecked(false);
    ui->input_manualrobot_clockwise->setChecked(false);

    auto robot = simulator.get()->getActiveManualRobot();
    if(robot != nullptr)
    {
        robot->setCommand(Command::FORWARD);
        robot->setSpeed(ui->input_manualrobot_speed->value() / (1000 / simulator->getTimerPeriod()));
    }
    else
    {
        QMessageBox msgBox;
        msgBox.setText("Warning: No ManualRobot is selected.");
        msgBox.exec();
    }
}

void MainWindow::on_input_manualrobot_stay_clicked()
{
    ui->input_manualrobot_forward->setChecked(false);
    ui->input_manualrobot_anticlockwise->setChecked(false);
    ui->input_manualrobot_clockwise->setChecked(false);

    auto robot = simulator.get()->getActiveManualRobot();
    if(robot != nullptr)
    {
        robot->setCommand(Command::STAY);
    }
    else
    {
        QMessageBox msgBox;
        msgBox.setText("Warning: No ManualRobot is selected.");
        msgBox.exec();
    }
}

void MainWindow::on_input_manualrobot_anticlockwise_clicked()
{
    ui->input_manualrobot_forward->setChecked(false);
    ui->input_manualrobot_stay->setChecked(false);
    ui->input_manualrobot_clockwise->setChecked(false);

    auto robot = simulator.get()->getActiveManualRobot();
    if(robot != nullptr)
    {
        robot->setCommand(Command::ROTATE_ANTICLOCK);
        robot->setTurnAngle(-ui->input_manualrobot_turnAngle->value() / (1000 / simulator->getTimerPeriod()));
    }
    else
    {
        QMessageBox msgBox;
        msgBox.setText("Warning: No ManualRobot is selected.");
        msgBox.exec();
    }
}

void MainWindow::on_input_manualrobot_clockwise_clicked()
{

    ui->input_manualrobot_forward->setChecked(false);
    ui->input_manualrobot_stay->setChecked(false);
    ui->input_manualrobot_anticlockwise->setChecked(false);

    auto robot = simulator.get()->getActiveManualRobot();
    if(robot != nullptr)
    {
        robot->setCommand(Command::ROTATE_CLOCK);
        robot->setCommand(Command::ROTATE_ANTICLOCK);
        robot->setTurnAngle(ui->input_manualrobot_turnAngle->value() / (1000 / simulator->getTimerPeriod()));
    }
    else
    {
        QMessageBox msgBox;
        msgBox.setText("Warning: No ManualRobot is selected.");
        msgBox.exec();
    }
}

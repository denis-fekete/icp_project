/**
 * @file mainwindow.cpp
 * @brief Implementation of MainWindow class methods
 *
 * @author Denis Fekete (xfeket01@fit.vutbr.cz)
 */

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
    ui->graphicsView->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    ui->graphicsView->setResizeAnchor(QGraphicsView::AnchorViewCenter);


    QApplication::setStyle(QStyleFactory::create("Fusion"));

    // ------------------------------------------------------------------------
    // Setup simulation
    simulator = std::make_unique<Simulator>(
        *scene,
        ui->sBox_worldc_sizeX->value(),
        ui->sBox_worldc_sizeY->value(),
        this->window()->size().width(),
        this->window()->size().height(),
        std::bind(&MainWindow::updateMenuGUI, this),
        &menuUpdate,
        30, 30
        );

    // add simulator to CustomScene for unselecting objects
    scene->setSimulator(simulator.get());

    // ------------------------------------------------------------------------
    // Apply world configuration
    on_btn_worldApplySize_clicked();

    // ------------------------------------------------------------------------
    //

    menuUpdate.setInterval(500);
    connect(&menuUpdate, &QTimer::timeout, this, &MainWindow::updateMenuGUI);
}

MainWindow::~MainWindow()
{
    delete ui;
    scene.release();
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if(event){}
    simulator->stopSimulation();

    simulator.release();
    exit(0);
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

void MainWindow::updateMenuGUI()
{
    auto obstacle = simulator.get()->getActiveObstacle();
    auto robot = simulator.get()->getActiveRobot();

    if(obstacle != nullptr)
    {
        auto sim = obstacle->getSim();
        ui->input_obstacle_xPos->setValue(sim->getX());
        ui->input_obstacle_yPos->setValue(sim->getY());

        ui->input_obstacle_width->setValue(sim->getW());
        ui->input_obstacle_height->setValue(sim->getH());

        ui->input_obstacle_rotation->setValue(sim->getRotation());

        QColor& color = obstacle->getColor();
        ui->input_obstacle_color_r->setValue(color.red());
        ui->input_obstacle_color_g->setValue(color.green());
        ui->input_obstacle_color_b->setValue(color.blue());
    }

    if(robot != nullptr)
    {
        auto sim = robot->getSim();
        ui->input_robot_xPos->setValue(sim->getX());
        ui->input_robot_yPos->setValue(sim->getY());
        ui->input_robot_radius->setValue(sim->getRadius());
        ui->input_robot_rotation->setValue(sim->getRotation());
        ui->input_robot_speed->setValue(robot->getSpeed());
        QColor& color = robot->getColor();
        ui->input_robot_color_r->setValue(color.red());
        ui->input_robot_color_g->setValue(color.green());
        ui->input_robot_color_b->setValue(color.blue());
        ui->input_robot_turnSpeed->setValue(robot->getTurnSpeed());
        ui->input_robot_detRadius->setValue(sim->getDetRadius());
        if(robot->getTurnDirection() == 1)
        {
            ui->input_robot_clockwise->setChecked(true);
            ui->input_robot_anticlockwise->setChecked(false);
        }
        else
        {
            ui->input_robot_clockwise->setChecked(false);
            ui->input_robot_anticlockwise->setChecked(true);
        }
    }

    ui->sBox_worldc_sizeX->setValue(simulator->getSimulationWidth());
    ui->sBox_worldc_sizeY->setValue(simulator->getSimulationHeight());
}

QColor MainWindow::getRandomColor()
{
    // RGB values from 0 - 235, not 255 in case three 255 were randomized
    return QColor(  randColor->getRandomValue(),
                    randColor->getRandomValue(),
                    randColor->getRandomValue());
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

void MainWindow::on_world_input_graphicsPeriod_valueChanged(int arg1)
{
    simulator.get()->setGraphicsTimer(arg1);
}


void MainWindow::on_world_input_simulationPeriod_valueChanged(int arg1)
{
     simulator.get()->setSimulationTimer(arg1);
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
        const short turnDirection = (ui->input_robot_clockwise->isChecked())? 1 : -1;

        simulator->addAutomaticRobot(
                ui->input_robot_xPos->value(),
                ui->input_robot_yPos->value(),
                ui->input_robot_radius->value(),
                ui->input_robot_rotation->value(),
                ui->input_robot_detRadius->value(),
                color,
                ui->input_robot_speed->value(),
            ui->input_robot_turnSpeed->value(),
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

void MainWindow::on_input_robot_anticlockwise_stateChanged(int arg1)
{
    ui->input_robot_anticlockwise->setChecked(arg1);
    ui->input_robot_clockwise->setChecked(!arg1);
}

void MainWindow::on_input_robot_clockwise_stateChanged(int arg1)
{
    ui->input_robot_clockwise->setChecked(arg1);
    ui->input_robot_anticlockwise->setChecked(!arg1);
}

void MainWindow::on_input_selectAutomatic_stateChanged(int arg1)
{
    ui->input_selectAutomatic->setChecked(arg1);
    ui->input_selectManual->setChecked(!arg1);

    ui->input_robot_speed->setEnabled(true);
    ui->input_robot_anticlockwise->setEnabled(true);
    ui->input_robot_clockwise->setEnabled(true);
    ui->input_robot_turnSpeed->setEnabled(true);
}

void MainWindow::on_input_selectManual_stateChanged(int arg1)
{
    ui->input_selectManual->setChecked(arg1);
    ui->input_selectAutomatic->setChecked(!arg1);

    ui->input_robot_speed->setEnabled(false);
    ui->input_robot_anticlockwise->setEnabled(false);
    ui->input_robot_clockwise->setEnabled(false);
    ui->input_robot_turnSpeed->setEnabled(false);
}

void MainWindow::on_input_robot_randomizeColors_stateChanged(int arg1)
{
    ui->input_robot_color_r->setEnabled(!arg1);
    ui->input_robot_color_g->setEnabled(!arg1);
    ui->input_robot_color_b->setEnabled(!arg1);
}

void MainWindow::on_input_robot_updateValues_clicked()
{
    auto robot = simulator.get()->getActiveRobot();

    if(robot != nullptr)
    {
        QColor color;
        if(ui->input_obstacle_randomizeColors->isChecked())
        {
            color = robot->getColor();
        }
        else
        {
            color = QColor(ui->input_robot_color_r->text().toDouble(),
                           ui->input_robot_color_g->text().toDouble(),
                           ui->input_robot_color_b->text().toDouble());
        }

        robot->updateValues(
            ui->input_robot_xPos->text().toDouble(),
            ui->input_robot_yPos->text().toDouble(),
            ui->input_robot_radius->text().toDouble(),
            ui->input_robot_rotation->text().toDouble(),
            ui->input_robot_detRadius->text().toDouble(),
            color,
            ui->input_robot_speed->text().toDouble(),
            ui->input_robot_turnSpeed->text().toDouble(),
            (ui->input_robot_clockwise->isChecked()) ? 1 : -1
            );
    }
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
    simulator->deleteObstacle();

    scene->update();
}

void MainWindow::on_input_obstacle_updateValues_clicked()
{
    auto obstacle = simulator.get()->getActiveObstacle();

    if(obstacle != nullptr)
    {
        QColor color;
        if(ui->input_obstacle_randomizeColors->isChecked())
        {
            color = obstacle->getColor();
        }
        else
        {
            color = QColor(ui->input_obstacle_color_r->text().toDouble(),
                           ui->input_obstacle_color_g->text().toDouble(),
                           ui->input_obstacle_color_b->text().toDouble());
        }

        obstacle->updateValues(
            ui->input_obstacle_xPos->text().toDouble(),
            ui->input_obstacle_yPos->text().toDouble(),
            ui->input_obstacle_width->text().toDouble(),
            ui->input_obstacle_height->text().toDouble(),
            ui->input_obstacle_rotation->text().toDouble(),
            color
            );
    }
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
        robot->setSpeed(ui->input_manualrobot_speed->value());
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
        robot->setturnSpeed(-ui->input_manualrobot_turnSpeed->value());
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
        robot->setturnSpeed(ui->input_manualrobot_turnSpeed->value());
    }
    else
    {
        QMessageBox msgBox;
        msgBox.setText("Warning: No ManualRobot is selected.");
        msgBox.exec();
    }
}

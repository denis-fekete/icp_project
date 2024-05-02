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
    rand1000 = std::make_unique<RandomGenerator>(0, 1000);
    // ------------------------------------------------------------------------
    // Setup graphics view
    ui->setupUi(this);

    scene = new QGraphicsScene(this);
    ui->graphicsView->setScene(scene);
    ui->graphicsView->setRenderHint(QPainter::Antialiasing);

    view = new QGraphicsView(scene);
    view->setViewportUpdateMode(QGraphicsView::BoundingRectViewportUpdate);
    // view->setDragMode(QGraphicsView::ScrollHandDrag);

    QApplication::setStyle(QStyleFactory::create("Fusion"));
    // QApplication::setStyle(QStyleFactory::create("Windows"));

    // ------------------------------------------------------------------------
    // Setup simulation
    simulator = std::make_unique<Simulator> (*scene, 4, 700, 500, 1000, 600);
    simulator->initializeCores();
    simulator->setTimerPeriod(30);

    // ------------------------------------------------------------------------
    // Apply world configuration
    on_btn_worldApplySize_clicked();

    // ------------------------------------------------------------------------
    // Analyitics
    QTimer timer;
    timer.setInterval(1000);
    timer.start();
    connect(&timer, SIGNAL(timeout()), this, SLOT(updateAnalytics()));
}

MainWindow::~MainWindow()
{
    delete view;
    delete ui;
    delete scene;
}

void MainWindow::DrawGrid(unsigned density)
{
    // remove all QGraphics objects at level 0 (grid lines)
    auto objectList = scene->items();
    for(int i = 0; i < objectList.size(); i++)
    {
        if(objectList.at(i)->zValue() == 0)
        {
            scene->removeItem(objectList.at(i));
        }
    }

    QPen gridPen(QColor(245, 245, 245));
    const auto xMax = ui->sBox_worldc_sizeX->value();
    const auto yMax = ui->sBox_worldc_sizeY->value();
    for(int y = 0; y <yMax; y+= density)
    {
        scene->addLine(0, y , xMax , y, gridPen);
    }

    for(int x = 0; x < xMax; x+= density)
    {
        scene->addLine(x, 0 ,x , yMax, gridPen);
    }

    scene->update();
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
                                  graphicalSceneH);

    // DrawGrid(ui->sBox_world_gridSize->value());

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

void MainWindow::updateAnalytics()
{
    ui->analytics_label_simulationCycles->setText(QString::number(simulator.get()->getCycleTime()));
}

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

    simulator.get()->balanceCores();
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

void MainWindow::on_btn_loadBenchmark_clicked()
{
    unsigned int benchmarkWidth = ui->sBox_world_benchMarkSizeX->value();
    unsigned int benchmarkHeight = ui->sBox_world_benchMarkSizeY->value();
    if(benchmarkWidth > scene->sceneRect().width())
    {
        benchmarkWidth = scene->sceneRect().width();
    }
    if(benchmarkHeight > scene->sceneRect().height())
    {
        benchmarkHeight = scene->sceneRect().height();
    }


    const int sizeConst = 20;
    // fill upper top layer
    for(size_t x = 0; x < benchmarkWidth; x += sizeConst)
    {
        double xPosOffset = (rand1000->getRandomValue() % 10) - 5;
        double yPosOffset = (rand1000->getRandomValue() % 10) - 5;
        double sizeOffsetW = (rand1000->getRandomValue() % sizeConst) - 4;
        double sizeOffsetH = (rand1000->getRandomValue() % sizeConst) - 4;
        double rot= (rand1000->getRandomValue() % 360);
        QColor color = getRandomColor();

        simulator->addObstacle(x + xPosOffset, 10 + yPosOffset, 10 + sizeOffsetW, 10 + sizeOffsetH, rot, color);

    }
    // fil bottom layer
    for(size_t x = 0; x < benchmarkWidth; x += sizeConst)
    {
        double xPosOffset = (rand1000->getRandomValue() % 10) - 5;
        double yPosOffset = (rand1000->getRandomValue() % 10) - 5;
        double sizeOffsetW = (rand1000->getRandomValue() % sizeConst) - 4;
        double sizeOffsetH = (rand1000->getRandomValue() % sizeConst) - 4;
        double rot= (rand1000->getRandomValue() % 360);
        QColor color = getRandomColor();

        simulator->addObstacle(x + xPosOffset, benchmarkHeight - 10 + yPosOffset, 10 + sizeOffsetW, 10 + sizeOffsetH, rot, color);

    }

    for(size_t y = 0; y < benchmarkHeight; y += sizeConst)
    {
        double xPosOffset = (rand1000->getRandomValue() % 10) - 5;
        double yPosOffset = (rand1000->getRandomValue() % 10) - 5;
        double sizeOffsetW = (rand1000->getRandomValue() % sizeConst) - 4;
        double sizeOffsetH = (rand1000->getRandomValue() % sizeConst) - 4;
        double rot= (rand1000->getRandomValue() % 360);
        QColor color = getRandomColor();

        simulator->addObstacle(10 + xPosOffset, y + yPosOffset, 10 + sizeOffsetW, 10 + sizeOffsetH, rot, color);
    }

    for(size_t y = 0; y < benchmarkHeight; y += sizeConst)
    {
        double xPosOffset = (rand1000->getRandomValue() % 10) - 5;
        double yPosOffset = (rand1000->getRandomValue() % 10) - 5;
        double sizeOffsetW = (rand1000->getRandomValue() % sizeConst) - 4;
        double sizeOffsetH = (rand1000->getRandomValue() % sizeConst) - 4;
        double rot= (rand1000->getRandomValue() % 360);
        QColor color = getRandomColor();

        simulator->addObstacle(benchmarkWidth - 10 + xPosOffset, y + yPosOffset, 10 + sizeOffsetW, 10 + sizeOffsetH, rot, color);
    }
}

void MainWindow::on_btn_worldAddMoreRobots_clicked()
{
    unsigned int benchmarkWidth = ui->sBox_world_benchMarkSizeX->value();
    unsigned int benchmarkHeight = ui->sBox_world_benchMarkSizeY->value();

    if(benchmarkWidth > scene->sceneRect().width())
    {
        benchmarkWidth = scene->sceneRect().width();
    }
    if(benchmarkHeight > scene->sceneRect().height())
    {
        benchmarkHeight = scene->sceneRect().height();
    }

    for(int robs = 0; robs < ui->sBox_world_robotsCount->value(); robs++)
    {
        double rad = (rand1000->getRandomValue() % 10) + 15;
        double xPos = benchmarkWidth / 2 + (rand1000->getRandomValue() % (static_cast<int>(rad*3)));
        double yPos = benchmarkHeight / 2 + (rand1000->getRandomValue() % (static_cast<int>(rad*3)));
        double detRad = (rand1000->getRandomValue() % 20) + 40;
        double rot= (rand1000->getRandomValue() % 360);
        double speed = (rand1000->getRandomValue() % 15) + 10;
        double turnAngle = (rand1000->getRandomValue() % 50) + 25;
        double turnDirection = (rand1000->getRandomValue() % 2);
        turnDirection = (turnDirection == 0)? -1 : turnDirection;
        QColor color = getRandomColor();

        simulator->addAutomaticRobot(xPos, yPos, rad, rot, detRad, color, speed, turnAngle, turnDirection);
    }

    simulator.get()->balanceCores();
}

void MainWindow::on_btn_worldApplySize_clicked()
{
    // ui->graphicsView->setGeometry(ui->graphicsView->geometry().x(),
    //                               ui->graphicsView->geometry().y(),
    //                               ui->sBox_worldc_sizeX->value(),
    //                               ui->sBox_worldc_sizeY->value());

    simulator->setSimulationSize(
        ui->sBox_worldc_sizeX->value(),
        ui->sBox_worldc_sizeY->value());

    // MainWindow::DrawGrid(ui->sBox_world_gridSize->value());
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

void MainWindow::on_input_robot_randomizeColors_toggled(bool checked)
{
    ui->input_robot_color_r->setEnabled(!checked);
    ui->input_robot_color_g->setEnabled(!checked);
    ui->input_robot_color_b->setEnabled(!checked);
}

void MainWindow::on_input_robot_onCollisionTurnRight_clicked(bool checked)
{
    ui->input_robot_onCollisionTurnLeft->setChecked(!checked);
}

void MainWindow::on_input_robot_onCollisionTurnLeft_clicked(bool checked)
{
    ui->input_robot_onCollisionTurnLeft->setChecked(checked);
    ui->input_robot_onCollisionTurnRight->setChecked(!checked);
}

void MainWindow::on_input_selectAutomatic_clicked()
{
    ui->input_selectAutomatic->setChecked(true);
    ui->input_selectManual->setChecked(false);

    ui->input_robot_speed->setEnabled(true);
    ui->input_robot_onCollisionTurnLeft->setEnabled(true);
    ui->input_robot_onCollisionTurnRight->setEnabled(true);
    ui->input_robot_collisionDetectionAngle->setEnabled(true);
}

void MainWindow::on_input_selectManual_clicked()
{
    ui->input_selectManual->setChecked(true);
    ui->input_selectAutomatic->setChecked(false);

    ui->input_robot_speed->setEnabled(false);
    ui->input_robot_onCollisionTurnLeft->setEnabled(false);
    ui->input_robot_onCollisionTurnRight->setEnabled(false);
    ui->input_robot_collisionDetectionAngle->setEnabled(false);
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

void MainWindow::on_input_obstacle_randomizeColors_toggled(bool checked)
{
    ui->input_obstacle_color_r->setEnabled(!checked);
    ui->input_obstacle_color_g->setEnabled(!checked);
    ui->input_obstacle_color_b->setEnabled(!checked);
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


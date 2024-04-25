#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>
#include <QStyleFactory>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{

    // ------------------------------------------------------------------------
    // Setup internal objects/variables
    // value 235 means that values in RGB wont be too bright,
    // potentialy resulting in white robot on white background
    randColor = std::make_unique<RandomGenerator>(0, 235);
    rand1000 = std::make_unique<RandomGenerator>(0, 1000);
    // ------------------------------------------------------------------------
    // Setup graphics view
    ui->setupUi(this);
    scene = new QGraphicsScene(this);
    ui->graphicsView->setScene(scene);

    QGraphicsView view(scene);
    view.setViewportUpdateMode(QGraphicsView::BoundingRectViewportUpdate);

    QApplication::setStyle(QStyleFactory::create("Fusion"));
    // QApplication::setStyle(QStyleFactory::create("Windows"));

    ui->controlTab->setVisible(false);
    // ------------------------------------------------------------------------
    // Apply world configuration
    on_btn_worldApplySize_clicked();

    // ------------------------------------------------------------------------
    // Setup simulation
    simulator = std::make_unique<Simulator> (robots, *scene, 8, &timer);
    simulator->initializeCores();
    simulator->setTimerPeriod(30);

    // ------------------------------------------------------------------------
    // Analyitics
    connect(&timer, SIGNAL(timeout()), this, SLOT(updateAnalytics()));
}

MainWindow::~MainWindow()
{
    delete ui;
    delete scene;
}

void MainWindow::DrawGrid(unsigned density)
{
    QPen gridPen(Qt::lightGray);
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
    #define VERTICAL_SPACING 5
    #define HORIZONTAL_SPACING 5

    const auto windowWidth = this->window()->size().width();
    const auto windowHeight = this->window()->size().height();

    ui->graphicsView->setGeometry(VERTICAL_SPACING, HORIZONTAL_SPACING,
                                  windowWidth - VERTICAL_SPACING,
                                  windowHeight - HORIZONTAL_SPACING);
    DrawGrid(ui->sBox_world_gridSize->value());

    const auto resumepause = ui->program_btn_resumepause->geometry();
    ui->program_btn_resumepause->setGeometry(windowWidth / 2- resumepause.width() / 2,
                                        0,
                                        resumepause.width(),
                                        resumepause.height()
                                        );

    #undef VERTICAL_SPACING
    #undef HORIZONTAL_SPACING
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
    simulator->stopSimulation();
    simulator.release();
    exit(0);
}

//----------------------------------------------------------------------------
// Main window
//----------------------------------------------------------------------------

void MainWindow::on_program_btn_hide_clicked()
{
    ui->controlTab->setVisible(!ui->controlTab->isVisible());
    ui->program_btn_hide->setText((ui->controlTab->isVisible())? "Hide controls" : "Show controls");
}

void MainWindow::updateAnalytics()
{
    ui->analytics_label_simulationCycles->setText(QString::number(simulator.get()->getCycleTime()));
}

void MainWindow::on_program_btn_resumepause_clicked()
{
    if(ui->controlTab->isEnabled())
    {
        ui->program_btn_resumepause->setText("Pause");
        simulator.get()->runSimulation();
        ui->controlTab->setEnabled(false);
    }
    else
    {
        ui->program_btn_resumepause->setText("Resume");
        simulator.get()->stopSimulation();
        ui->controlTab->setEnabled(true);
    }
}

//----------------------------------------------------------------------------
// World tab
//----------------------------------------------------------------------------

void MainWindow::on_saveManger_btn_load_clicked()
{
    // check if manager was initialzied
    if(!saveManager)
    {
        saveManager = std::make_unique<SaveManager> (robots, obstacles, this, *scene, &activeRobot, &activeObstacle);
    }

    saveManager.get()->loadFromFile();

    simulator.get()->balanceCores();
    scene->update();
}

void MainWindow::on_saveManager_btn_save_clicked()
{
    // check if manager was initialzied
    if(!saveManager)
    {
        saveManager = std::make_unique<SaveManager> (robots, obstacles, this, *scene, &activeRobot, &activeObstacle);
    }

    saveManager.get()->saveToFile();
}

void MainWindow::on_btn_loadBenchmark_clicked()
{
    unsigned int benchmarkWidth = ui->sBox_world_benchMarkSizeX->value();
    unsigned int benchmarkHeigth = ui->sBox_world_benchMarkSizeY->value();
    if(benchmarkWidth > scene->sceneRect().width())
    {
        benchmarkWidth = scene->sceneRect().width();
    }
    if(benchmarkHeigth > scene->sceneRect().height())
    {
        benchmarkHeigth = scene->sceneRect().height();
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

        Obstacle::addObstacleToWorld(x + xPosOffset, 10 + yPosOffset, 10 + sizeOffsetW, 10 + sizeOffsetH, rot, color, obstacles, *scene, &activeObstacle);

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

        Obstacle::addObstacleToWorld(x + xPosOffset, benchmarkHeigth - 10 + yPosOffset, 10 + sizeOffsetW, 10 + sizeOffsetH, rot, color, obstacles, *scene, &activeObstacle);

    }

    for(size_t y = 0; y < benchmarkHeigth; y += sizeConst)
    {
        double xPosOffset = (rand1000->getRandomValue() % 10) - 5;
        double yPosOffset = (rand1000->getRandomValue() % 10) - 5;
        double sizeOffsetW = (rand1000->getRandomValue() % sizeConst) - 4;
        double sizeOffsetH = (rand1000->getRandomValue() % sizeConst) - 4;
        double rot= (rand1000->getRandomValue() % 360);
        QColor color = getRandomColor();

        Obstacle::addObstacleToWorld(10 + xPosOffset, y + yPosOffset, 10 + sizeOffsetW, 10 + sizeOffsetH, rot, color, obstacles, *scene, &activeObstacle);

    }

    for(size_t y = 0; y < benchmarkHeigth; y += sizeConst)
    {
        double xPosOffset = (rand1000->getRandomValue() % 10) - 5;
        double yPosOffset = (rand1000->getRandomValue() % 10) - 5;
        double sizeOffsetW = (rand1000->getRandomValue() % sizeConst) - 4;
        double sizeOffsetH = (rand1000->getRandomValue() % sizeConst) - 4;
        double rot= (rand1000->getRandomValue() % 360);
        QColor color = getRandomColor();

        Obstacle::addObstacleToWorld(benchmarkWidth - 10 + xPosOffset, y + yPosOffset, 10 + sizeOffsetW, 10 + sizeOffsetH, rot, color, obstacles, *scene, &activeObstacle);

    }
}

void MainWindow::on_btn_worldAddMoreRobots_clicked()
{
    unsigned int benchmarkWidth = ui->sBox_world_benchMarkSizeX->value();
    unsigned int benchmarkHeigth = ui->sBox_world_benchMarkSizeY->value();

    if(benchmarkWidth > scene->sceneRect().width())
    {
        benchmarkWidth = scene->sceneRect().width();
    }
    if(benchmarkHeigth > scene->sceneRect().height())
    {
        benchmarkHeigth = scene->sceneRect().height();
    }

    for(size_t robs = 0; robs < ui->sBox_world_robotsCount->value(); robs++)
    {
        double rad = (rand1000->getRandomValue() % 10) + 15;
        double xPos = benchmarkWidth / 2 + (rand1000->getRandomValue() % (static_cast<int>(rad*3)));
        double yPos = benchmarkHeigth / 2 + (rand1000->getRandomValue() % (static_cast<int>(rad*3)));
        double detRad = (rand1000->getRandomValue() % 20) + 40;
        double rot= (rand1000->getRandomValue() % 360);
        double speed = (rand1000->getRandomValue() % 3) + 1;
        double turnAngle = (rand1000->getRandomValue() % 7) - 14;
        QColor color = getRandomColor();

        AutoRobot::addRobotToWorld(xPos, yPos, rad, rot, detRad, color, speed, turnAngle, true, obstacles, robots, *scene, &activeRobot);

    }

    // If ID selector is not enabled enable it
    if(!ui->input_robot_IDSelector->isEnabled())
    {
        ui->input_robot_IDSelector->setEnabled(true);
        activeRobot = robots.back().get();
    }

    // Set maximum value of robot selector to a size vector that stores robots
    auto old = ui->input_robot_IDSelector->value();
    ui->input_robot_IDSelector->setMaximum((int) robots.size() - 1);
    ui->input_robot_IDSelector->setValue(old);
    on_input_robot_IDSelector_valueChanged(old);

    simulator.get()->balanceCores();
}

void MainWindow::on_btn_worldApplySize_clicked()
{
    scene->setSceneRect(0, 0, ui->sBox_worldc_sizeX->value(), ui->sBox_worldc_sizeY->value());

    MainWindow::DrawGrid(ui->sBox_world_gridSize->value());
}


//----------------------------------------------------------------------------
// Robot tab
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

    AutoRobot::addRobotToWorld(
            ui->input_robot_xPos->value(),
            ui->input_robot_yPos->value(),
            ui->input_robot_radius->value(),
            ui->input_robot_rotation->value(),
            ui->input_robot_detRadius->value(),
            color,
            ui->input_robot_speed->value(),
            ui->input_robot_collisionDetectionAngle->value(),
            ui->input_robot_onCollisionTurnRight->isChecked(),
            obstacles,
            robots,
            *scene,
            &activeRobot
            );


    // If ID selector is not enabled enable it
    if(!ui->input_robot_IDSelector->isEnabled())
    {
        ui->input_robot_IDSelector->setEnabled(true);
        activeRobot = robots.back().get();
    }

    // Set maximum value of robot selector to a size vector that stores robots
    auto old = ui->input_robot_IDSelector->value();
    ui->input_robot_IDSelector->setMaximum((int) robots.size() - 1);
    ui->input_robot_IDSelector->setValue(old);
    on_input_robot_IDSelector_valueChanged(old);

    simulator.get()->balanceCores();
}

void MainWindow::on_btnDeleteRobot_clicked()
{
    // get active robot id
    auto robotId = ui->input_robot_IDSelector->value();
    // erase robot
    robots.erase(robots.begin() + robotId);
    // set activeRobot pointer to null
    activeRobot = nullptr;
    // calculate new val
    auto val = (int) robots.size() - 1;

    if(val <= 0)
        val = 0;

    ui->input_robot_IDSelector->setMaximum(val);
    ui->input_robot_IDSelector->setValue(val);

    scene->update();
}

void MainWindow::on_btnTest_clicked()
{
    if(activeRobot != nullptr)
    {
        activeRobot->moveRobot(25);
    }
}

void MainWindow::on_btnTest_2_clicked()
{
    if(activeRobot != nullptr)
    {
        activeRobot->rotateRobot(45);
    }
}

void MainWindow::on_input_robot_randomizeColors_toggled(bool checked)
{
    ui->input_robot_color_r->setEnabled(!checked);
    ui->input_robot_color_g->setEnabled(!checked);
    ui->input_robot_color_b->setEnabled(!checked);
}

void MainWindow::on_input_robot_IDSelector_valueChanged(int arg1)
{
        if(robots.size() <= 0)
        {
            return;
        }
        // Unselect old active robot and set active the new
        if(activeRobot != nullptr)
        {
            activeRobot->setUnselected();
        }
        activeRobot = robots.at(arg1).get();
        activeRobot->setSelected();
}

void MainWindow::on_input_robot_onCollisionTurnLeft_clicked(bool checked)
{
    ui->input_robot_onCollisionTurnRight->setChecked(!checked);
}

void MainWindow::on_input_robot_onCollisionTurnRight_clicked(bool checked)
{
    ui->input_robot_onCollisionTurnLeft->setChecked(!checked);
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

    Obstacle::addObstacleToWorld(
            ui->input_obstacle_xPos->value(),
            ui->input_obstacle_yPos->value(),
            ui->input_obstacle_width->value(),
            ui->input_obstacle_heigth->value(),
            ui->input_obstacle_rotation->value(),
            color,
            obstacles,
            *scene,
            &activeObstacle);


    // If ID selector is not enabled enable it
    if(!ui->input_obstacle_IDSelector->isEnabled())
    {
        ui->input_obstacle_IDSelector->setEnabled(true);
        activeObstacle = obstacles.back().get();
    }

    // Set maximum value of robot selector to a size vector that stores robots
    auto old = ui->input_obstacle_IDSelector->value();
    ui->input_obstacle_IDSelector->setMaximum((int) obstacles.size() - 1);
    ui->input_obstacle_IDSelector->setValue(old);
    on_input_obstacle_IDSelector_valueChanged(old);

    scene->update();
}

void MainWindow::on_input_obstacle_randomizeColors_toggled(bool checked)
{
    ui->input_obstacle_color_r->setEnabled(!checked);
    ui->input_obstacle_color_g->setEnabled(!checked);
    ui->input_obstacle_color_b->setEnabled(!checked);
}

void MainWindow::on_btnDeleteObstacle_clicked()
{
    // get active robot id
    auto obstacleId = ui->input_obstacle_IDSelector->value();
    // erase robot
    obstacles.erase(obstacles.begin() + obstacleId);
    // set activeRobot pointer to null
    activeObstacle = nullptr;
    // calculate new val
    auto val = (int) obstacles.size() - 1;

    if(val <= 0)
        val = 0;

    ui->input_obstacle_IDSelector->setMaximum(val);
    ui->input_obstacle_IDSelector->setValue(val);

    scene->update();
}

void MainWindow::on_input_obstacle_IDSelector_valueChanged(int arg1)
{

    if(obstacles.size() <= 0)
    {
        return;
    }
    // Unselect old active robot and set active the new
    if(activeObstacle != nullptr)
    {
        activeObstacle->setUnselected();
    }
    activeObstacle = obstacles.at(arg1).get();
    activeObstacle->setSelected();

    scene->update();
}



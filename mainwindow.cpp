#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    scene = new QGraphicsScene(this);
    ui->graphicsView->setScene(scene);

    QGraphicsView view(scene);
    view.setViewportUpdateMode(QGraphicsView::BoundingRectViewportUpdate);

    // Apply world configuration
    on_btn_applyWorldConfigSize_clicked();

    DrawGrid(50);

    clock = new QTime();
    clock->start();

    activeRobot = nullptr;
\
    simulator = new Simulator(&robots, &obstacles, 50);
    simulator->SetTimerPeriod(30);
    simulator->RunSimulation();
}

MainWindow::~MainWindow()
{
    delete ui;
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
}

void MainWindow::on_btnCreateRobot_clicked()
{

    QColor* newColor;
    if(ui->input_robot_randomizeColors->isChecked())
    {
        std::random_device device;
        std::mt19937 rng(device());
        // RGB values from 0 - 225, not 255 in case three 255 were randomized
        std::uniform_int_distribution<std::mt19937::result_type> distColor(0, 225);

        newColor = new QColor(distColor(rng), distColor(rng), distColor(rng));
    }
    else
    {
        newColor = new QColor(ui->input_robot_color_r->value(),
                              ui->input_robot_color_g->value(),
                              ui->input_robot_color_b->value());
    }


    this->robots.push_back(
        new AutoRobot(
            ui->input_robot_xPos->value(),
            ui->input_robot_yPos->value(),
            ui->input_robot_radius->value(),
            ui->input_robot_rotation->value(),
            ui->input_robot_detRadius->value(),
            *newColor,
            ui->input_robot_speed->value(),
            &obstacles,
            this->clock
            ));

    robots.back()->Initialize(this->scene);

    // If ID selector is not enabled enable it
    if(!ui->input_robot_IDSelector->isEnabled())
    {
        ui->input_robot_IDSelector->setEnabled(true);
        activeRobot = robots.back();
    }

    // Set maximum value of robot selector to a size vector that stores robots
    ui->input_robot_IDSelector->setMaximum((int) robots.size() - 1);
}


void MainWindow::on_btnTest_clicked()
{
    if(activeRobot != nullptr)
    {
        activeRobot->MoveRobot(25);
    }
}


void MainWindow::on_btnTest_2_clicked()
{
    if(activeRobot != nullptr)
    {
        activeRobot->RotateRobot(45);
    }
}


void MainWindow::on_btn_applyWorldConfigSize_clicked()
{
    scene->setSceneRect(0, 0, ui->sBox_worldc_sizeX->value(), ui->sBox_worldc_sizeY->value());
}

void MainWindow::on_input_robot_randomizeColors_toggled(bool checked)
{
    ui->input_robot_color_r->setEnabled(!checked);
    ui->input_robot_color_g->setEnabled(!checked);
    ui->input_robot_color_b->setEnabled(!checked);
}

void MainWindow::on_btnCreateObstacle_clicked()
{
    QColor* newColor;
    if(ui->input_obstacle_randomizeColors->isChecked())
    {
        std::random_device device;
        std::mt19937 rng(device());
        // RGB values from 0 - 225, not 255 in case three 255 were randomized
        std::uniform_int_distribution<std::mt19937::result_type> distColor(0, 225);

        newColor = new QColor(distColor(rng), distColor(rng), distColor(rng));
    }
    else
    {
        newColor = new QColor(ui->input_obstacle_color_r->value(),
                              ui->input_obstacle_color_g->value(),
                              ui->input_obstacle_color_b->value());
    }


    this->obstacles.push_back(
        new Obstacle(
            ui->input_obstacle_xPos->value(),
            ui->input_obstacle_yPos->value(),
            ui->input_obstacle_width->value(),
            ui->input_obstacle_heigth->value(),
            ui->input_obstacle_rotation->value(),
            *newColor
            ));


    scene->addItem(obstacles.back());
}


void MainWindow::on_input_obstacle_randomizeColors_toggled(bool checked)
{
    ui->input_obstacle_color_r->setEnabled(!checked);
    ui->input_obstacle_color_g->setEnabled(!checked);
    ui->input_obstacle_color_b->setEnabled(!checked);
}


void MainWindow::on_btn_update_info_clicked()
{
    if(activeRobot == nullptr)
    {
        return;
    }

    Robot* currSim = activeRobot->GetSimatationInfo();
    ui->info_sim_main_posX->setNum(currSim->x);
    ui->info_sim_main_posY->setNum(currSim->y);
    ui->info_sim_main_rot->setNum(currSim->rot);

    ui->info_sim_coll_posX->setNum(currSim->colliderFwd.x);
    ui->info_sim_coll_posY->setNum(currSim->colliderFwd.y);
    ui->info_sim_coll_rot->setNum(currSim->colliderFwd.rot);

    ui->info_sim_coll_lbX->setNum(currSim->colliderFwd.LB.x);
    ui->info_sim_coll_lbY->setNum(currSim->colliderFwd.LB.y);

    ui->info_sim_coll_rbX->setNum(currSim->colliderFwd.RB.x);
    ui->info_sim_coll_rbY->setNum(currSim->colliderFwd.RB.y);

    ui->info_sim_coll_rtX->setNum(currSim->colliderFwd.RT.x);
    ui->info_sim_coll_rtY->setNum(currSim->colliderFwd.RT.y);

    ui->info_sim_coll_ltX->setNum(currSim->colliderFwd.LT.x);
    ui->info_sim_coll_ltY->setNum(currSim->colliderFwd.LT.y);

    //-------------------------------------------------------

    auto graphics = activeRobot->GetGraphics();
    auto collider = activeRobot->GetCollider();

    ui->info_gphx_main_posX->setNum(graphics->x());
    ui->info_gphx_main_posY->setNum(graphics->y());
    ui->info_gphx_main_rot->setNum(graphics->rotation());

    ui->info_gphx_coll_posX->setNum(collider->x());
    ui->info_gphx_coll_posY->setNum(collider->y());
    ui->info_gphx_coll_rot->setNum(collider->rotation());

    ui->info_gphx_coll_lbX->setNum(collider->rect().x());
    ui->info_gphx_coll_lbY->setNum(collider->rect().y());

    ui->info_gphx_coll_rbX->setNum(collider->rect().x() + collider->rect().width());
    ui->info_gphx_coll_rbY->setNum(collider->rect().y());

    ui->info_gphx_coll_rtX->setNum(collider->rect().x() + collider->rect().width());
    ui->info_gphx_coll_rtY->setNum(collider->rect().y() + collider->rect().height());

    ui->info_gphx_coll_ltX->setNum(collider->rect().x());
    ui->info_gphx_coll_ltY->setNum(collider->rect().y() + collider->rect().height());
}



void MainWindow::on_input_robot_IDSelector_valueChanged(int arg1)
{
    // Unselect old active robot and set active the new
    activeRobot->SetUnselected();
    activeRobot = robots.at(arg1);
    activeRobot->SetSelected();
}


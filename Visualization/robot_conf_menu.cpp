#include "../mainwindow.h"
#include "../ui_mainwindow.h"

#include "QDebug"

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
            *scene
            );


    // If ID selector is not enabled enable it
    if(!ui->input_robot_IDSelector->isEnabled())
    {
        ui->input_robot_IDSelector->setEnabled(true);
        activeRobot = &(robots.back());
    }

    // Set maximum value of robot selector to a size vector that stores robots
    ui->input_robot_IDSelector->setMaximum((int) robots.size() - 1);
}


void MainWindow::on_btnTest_clicked()
{
    if(activeRobot != nullptr)
    {
        activeRobot->get()->moveRobot(25);
    }
}


void MainWindow::on_btnTest_2_clicked()
{
    if(activeRobot != nullptr)
    {
        activeRobot->get()->rotateRobot(45);
    }
}


void MainWindow::on_input_robot_randomizeColors_toggled(bool checked)
{
    ui->input_robot_color_r->setEnabled(!checked);
    ui->input_robot_color_g->setEnabled(!checked);
    ui->input_robot_color_b->setEnabled(!checked);
}

void MainWindow::on_btn_update_info_clicked()
{
    if(activeRobot == nullptr)
    {
        return;
    }

    Robot currSim = activeRobot->get()->getSimulationInfo();
    // ui->info_sim_main_posX->setNum(currsim.x);
    // ui->info_sim_main_posY->setNum(currsim.y);
    // ui->info_sim_main_rot->setNum(currsim.getRotation());

    // ui->info_sim_coll_posX->setNum(currsim.colliderFwd.x);
    // ui->info_sim_coll_posY->setNum(currsim.colliderFwd.y);
    // ui->info_sim_coll_rot->setNum(currsim.colliderFwd.getRotation());

    // ui->info_sim_coll_lbX->setNum(currsim.colliderFwd.LB.x);
    // ui->info_sim_coll_lbY->setNum(currsim.colliderFwd.LB.y);

    // ui->info_sim_coll_rbX->setNum(currsim.colliderFwd.RB.x);
    // ui->info_sim_coll_rbY->setNum(currsim.colliderFwd.RB.y);

    // ui->info_sim_coll_rtX->setNum(currsim.colliderFwd.RT.x);
    // ui->info_sim_coll_rtY->setNum(currsim.colliderFwd.RT.y);

    // ui->info_sim_coll_ltX->setNum(currsim.colliderFwd.LT.x);
    // ui->info_sim_coll_ltY->setNum(currsim.colliderFwd.LT.y);
    // std::string s = std::to_string(simulator->getCycleTime());
    // ui->info_sim_main_cycleTime->setText(QString::fromStdString(s));
}




void MainWindow::on_input_robot_IDSelector_valueChanged(int arg1)
{
    // Unselect old active robot and set active the new
    activeRobot->get()->setUnselected();
    activeRobot = &(robots.at(arg1));
    activeRobot->get()->setSelected();
}

void MainWindow::on_input_robot_onCollisionTurnLeft_clicked(bool checked)
{
    ui->input_robot_onCollisionTurnRight->setChecked(!checked);
}


void MainWindow::on_input_robot_onCollisionTurnRight_clicked(bool checked)
{
    ui->input_robot_onCollisionTurnLeft->setChecked(!checked);
}


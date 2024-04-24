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
            *scene
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

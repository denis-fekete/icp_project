#include "../mainwindow.h"
#include "../ui_mainwindow.h"

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

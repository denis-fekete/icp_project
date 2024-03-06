#include "mainwindow.h"
#include "ui_mainwindow.h"

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

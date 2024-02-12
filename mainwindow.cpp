#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <iostream>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    scene = new QGraphicsScene(this);
    ui->graphicsView->setScene(scene);

    // Apply world configuration
    on_btn_applyWorldConfigSize_clicked();

    QPen pen(Qt::gray);
    pen.setWidth(1);
    scene->addRect(0, 0, 20, 20, pen);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_btnCreateRobot_clicked()
{
    this->robots.push_back(
        new AutoRobot(
            ui->input_robot_xPos->value(),
            ui->input_robot_yPos->value(),
            ui->input_robot_radius->value(),
            ui->input_robot_rotation->value(),
            ui->input_robot_detRadius->value(),
            QColor(
                ui->input_robot_color_r->value(),
                ui->input_robot_color_g->value(),
                ui->input_robot_color_b->value())
            ));

    robots.back()->Initialize(this->scene);
}



void MainWindow::on_btnTest_clicked()
{
    robots.back()->Move(10);
}


void MainWindow::on_btnTest_2_clicked()
{
    robots.back()->RotateAroundSelf(45);
}


void MainWindow::on_btn_applyWorldConfigSize_clicked()
{
    scene->setSceneRect(0, 0, ui->sBox_worldc_sizeX->value(), ui->sBox_worldc_sizeY->value());
}


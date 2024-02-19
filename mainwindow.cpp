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

void MainWindow::on_input_robot_onCollisionTurnLeft_clicked(bool checked)
{
    ui->input_robot_onCollisionTurnRight->setChecked(!checked);
}


void MainWindow::on_input_robot_onCollisionTurnRight_clicked(bool checked)
{
    ui->input_robot_onCollisionTurnLeft->setChecked(!checked);
}


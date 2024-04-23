#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>
#include <QStyleFactory>

#include <iostream>

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
    simulator = std::make_unique<Simulator> (robots, *scene, 0, &timer);
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

    const auto resumeBtn = ui->program_btn_resume->geometry();
    ui->program_btn_resume->setGeometry(windowWidth / 2- resumeBtn.width(),
                                        0,
                                        resumeBtn.width(),
                                        resumeBtn.height()
                                        );

    const auto pauseBtn = ui->program_btn_pause->geometry();
    ui->program_btn_pause->setGeometry(windowWidth / 2,
                                        0,
                                        pauseBtn.width(),
                                        pauseBtn.height()
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



void MainWindow::on_program_btn_hide_clicked()
{
    ui->controlTab->setVisible(!ui->controlTab->isVisible());
    ui->program_btn_hide->setText((ui->controlTab->isVisible())? "Hide controls" : "Show controls");
}


void MainWindow::on_program_btn_pause_clicked()
{
    simulator.get()->stopSimulation();
}


void MainWindow::on_program_btn_resume_clicked()
{
    simulator.get()->runSimulation();
}

void MainWindow::updateAnalytics()
{
    ui->analytics_label_simulationCycles->setText(QString::number(simulator.get()->getCycleTime()));
}


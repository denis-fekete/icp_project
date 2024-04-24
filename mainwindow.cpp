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
    simulator = std::make_unique<Simulator> (robots, *scene, 4, &timer);
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



void MainWindow::on_program_btn_hide_clicked()
{
    ui->controlTab->setVisible(!ui->controlTab->isVisible());
    ui->program_btn_hide->setText((ui->controlTab->isVisible())? "Hide controls" : "Show controls");
}

void MainWindow::updateAnalytics()
{
    ui->analytics_label_simulationCycles->setText(QString::number(simulator.get()->getCycleTime()));
}

void MainWindow::on_saveManger_btn_load_clicked()
{
    // check if manager was initialzied
    if(!saveManager)
    {
        saveManager = std::make_unique<SaveManager> (robots, obstacles, this, *scene);
    }

    saveManager.get()->loadFromFile();
}

void MainWindow::on_saveManager_btn_save_clicked()
{
    // check if manager was initialzied
    if(!saveManager)
    {
        saveManager = std::make_unique<SaveManager> (robots, obstacles, this, *scene);
    }

    saveManager.get()->saveToFile();
}

void MainWindow::on_program_btn_resumepause_clicked(bool checked)
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


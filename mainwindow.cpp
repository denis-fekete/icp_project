#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    // value 235 means that values in RGB wont be too bright,
    // potentialy resulting in white robot on white background
    randColor = std::make_unique<RandomGenerator>(0, 235);
    rand1000 = std::make_unique<RandomGenerator>(0, 1000);

    ui->setupUi(this);
    scene = new QGraphicsScene(this);
    ui->graphicsView->setScene(scene);

    QGraphicsView view(scene);
    view.setViewportUpdateMode(QGraphicsView::BoundingRectViewportUpdate);

    // Apply world configuration
    on_btn_worldApplySize_clicked();

    simulator = std::make_unique<Simulator> (robots, *scene, 1);
    simulator->initializeCores();
    simulator->setTimerPeriod(30);
    simulator->runSimulation();
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

    scene->update();
}



#define VERTICAL_SPACING 10
#define HORIZONTAL_SPACING 20
void MainWindow::resizeEvent(QResizeEvent*)
{
    auto windowWidth = this->window()->size().width();
    auto windowHeight = this->window()->size().height();

    auto toolBoxGeo = ui->toolBox->geometry();

    ui->graphicsView->setGeometry(toolBoxGeo.x() + toolBoxGeo.width() + VERTICAL_SPACING,
                                  HORIZONTAL_SPACING,
                                  windowWidth - (toolBoxGeo.width() + 2 * VERTICAL_SPACING),
                                  windowHeight - 2 * HORIZONTAL_SPACING
                                  );

    DrawGrid(ui->sBox_world_gridSize->value());
}
#undef VERTICAL_SPACING
#undef HORIZONTAL_SPACING

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

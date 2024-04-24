// #include "mainwindow.h"

// void MainWindow::on_btn_loadBenchmark_clicked()
// {
//     unsigned int benchmarkWidth = ui->sBox_world_benchMarkSizeX->value();
//     unsigned int benchmarkHeigth = ui->sBox_world_benchMarkSizeY->value();
//     if(benchmarkWidth > scene->sceneRect().width())
//     {
//         benchmarkWidth = scene->sceneRect().width();
//     }
//     if(benchmarkHeigth > scene->sceneRect().height())
//     {
//         benchmarkHeigth = scene->sceneRect().height();
//     }


//     const int sizeConst = 20;
//     // fill upper top layer
//     for(size_t x = 0; x < benchmarkWidth; x += sizeConst)
//     {
//         double xPosOffset = (rand1000->getRandomValue() % 10) - 5;
//         double yPosOffset = (rand1000->getRandomValue() % 10) - 5;
//         double sizeOffsetW = (rand1000->getRandomValue() % sizeConst) - 4;
//         double sizeOffsetH = (rand1000->getRandomValue() % sizeConst) - 4;
//         double rot= (rand1000->getRandomValue() % 360);
//         QColor color = getRandomColor();

//         Obstacle::addObstacleToWorld(x + xPosOffset, 10 + yPosOffset, 10 + sizeOffsetW, 10 + sizeOffsetH, rot, color, obstacles, *scene, &activeObstacle);

//     }
//     // fil bottom layer
//     for(size_t x = 0; x < benchmarkWidth; x += sizeConst)
//     {
//         double xPosOffset = (rand1000->getRandomValue() % 10) - 5;
//         double yPosOffset = (rand1000->getRandomValue() % 10) - 5;
//         double sizeOffsetW = (rand1000->getRandomValue() % sizeConst) - 4;
//         double sizeOffsetH = (rand1000->getRandomValue() % sizeConst) - 4;
//         double rot= (rand1000->getRandomValue() % 360);
//         QColor color = getRandomColor();

//         Obstacle::addObstacleToWorld(x + xPosOffset, benchmarkHeigth - 10 + yPosOffset, 10 + sizeOffsetW, 10 + sizeOffsetH, rot, color, obstacles, *scene, &activeObstacle);

//     }

//     for(size_t y = 0; y < benchmarkHeigth; y += sizeConst)
//     {
//         double xPosOffset = (rand1000->getRandomValue() % 10) - 5;
//         double yPosOffset = (rand1000->getRandomValue() % 10) - 5;
//         double sizeOffsetW = (rand1000->getRandomValue() % sizeConst) - 4;
//         double sizeOffsetH = (rand1000->getRandomValue() % sizeConst) - 4;
//         double rot= (rand1000->getRandomValue() % 360);
//         QColor color = getRandomColor();

//         Obstacle::addObstacleToWorld(10 + xPosOffset, y + yPosOffset, 10 + sizeOffsetW, 10 + sizeOffsetH, rot, color, obstacles, *scene, &activeObstacle);

//     }

//     for(size_t y = 0; y < benchmarkHeigth; y += sizeConst)
//     {
//         double xPosOffset = (rand1000->getRandomValue() % 10) - 5;
//         double yPosOffset = (rand1000->getRandomValue() % 10) - 5;
//         double sizeOffsetW = (rand1000->getRandomValue() % sizeConst) - 4;
//         double sizeOffsetH = (rand1000->getRandomValue() % sizeConst) - 4;
//         double rot= (rand1000->getRandomValue() % 360);
//         QColor color = getRandomColor();

//         Obstacle::addObstacleToWorld(benchmarkWidth - 10 + xPosOffset, y + yPosOffset, 10 + sizeOffsetW, 10 + sizeOffsetH, rot, color, obstacles, *scene, &activeObstacle);

//     }
// }

// void MainWindow::on_btn_worldAddMoreRobots_clicked()
// {
//     unsigned int benchmarkWidth = ui->sBox_world_benchMarkSizeX->value();
//     unsigned int benchmarkHeigth = ui->sBox_world_benchMarkSizeY->value();

//     if(benchmarkWidth > scene->sceneRect().width())
//     {
//         benchmarkWidth = scene->sceneRect().width();
//     }
//     if(benchmarkHeigth > scene->sceneRect().height())
//     {
//         benchmarkHeigth = scene->sceneRect().height();
//     }

//     for(size_t robs = 0; robs < ui->sBox_world_robotsCount->value(); robs++)
//     {
//         double rad = (rand1000->getRandomValue() % 10) + 15;
//         double xPos = benchmarkWidth / 2 + (rand1000->getRandomValue() % (static_cast<int>(rad*3)));
//         double yPos = benchmarkHeigth / 2 + (rand1000->getRandomValue() % (static_cast<int>(rad*3)));
//         double detRad = (rand1000->getRandomValue() % 20) + 40;
//         double rot= (rand1000->getRandomValue() % 360);
//         double speed = (rand1000->getRandomValue() % 3) + 1;
//         double turnAngle = (rand1000->getRandomValue() % 7) - 14;
//         QColor color = getRandomColor();

//         AutoRobot::addRobotToWorld(xPos, yPos, rad, rot, detRad, color, speed, turnAngle, true, obstacles, robots, *scene);

//     }

//     // If ID selector is not enabled enable it
//     if(!ui->input_robot_IDSelector->isEnabled())
//     {
//         ui->input_robot_IDSelector->setEnabled(true);
//         activeRobot = robots.back().get();
//     }

//     // Set maximum value of robot selector to a size vector that stores robots
//     auto old = ui->input_robot_IDSelector->value();
//     ui->input_robot_IDSelector->setMaximum((int) robots.size() - 1);
//     ui->input_robot_IDSelector->setValue(old);
//     on_input_robot_IDSelector_valueChanged(old);

//     simulator.get()->balanceCores();
// }

// void MainWindow::on_btn_worldApplySize_clicked()
// {
//     scene->setSceneRect(0, 0, ui->sBox_worldc_sizeX->value(), ui->sBox_worldc_sizeY->value());

//     MainWindow::DrawGrid(ui->sBox_world_gridSize->value());
// }

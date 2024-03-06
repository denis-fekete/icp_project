#include "mainwindow.h"
#include "ui_mainwindow.h"

void MainWindow::on_btn_applyWorldConfigSize_clicked()
{
    scene->setSceneRect(0, 0, ui->sBox_worldc_sizeX->value(), ui->sBox_worldc_sizeY->value());
}

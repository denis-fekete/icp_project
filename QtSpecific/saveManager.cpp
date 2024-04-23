#include "mainwindow.h"

#include "QIODevice"
#include "QFile"
#include "QXmlStreamReader"
#include "QXmlStreamWriter"
#include "ui_mainwindow.h"
#include <qcoreapplication.h>
#include "QDebug"
#include "QFileDialog"

#include <iostream>

void MainWindow::on_saveManager_btn_save_clicked()
{
    QString fileName =
        QFileDialog::getOpenFileName(this, tr("Open Save File"),
                                     QDir::currentPath() + "/debug/saves/",
                                     tr("XBEL Files (*.xbel *.xml)"));

    QFile file(fileName);

    if(!file.open(QIODevice::ReadWrite))
    {
        qDebug("Failed to open XML file for reading");
    }

    QXmlStreamWriter xmlWriter(&file);

    xmlWriter.setAutoFormatting(true);
    xmlWriter.setAutoFormattingIndent(4);

    xmlWriter.writeStartDocument("1.0.0");
    xmlWriter.writeStartElement("Robots");

    for(size_t i = 0; i < robots.size(); i++)
    {
        auto& robSim = robots.at(i).get()->getSim();
        auto rob = robots.at(i).get();

        xmlWriter.writeEmptyElement(QString::number(i));
        xmlWriter.writeAttribute("x", QString::number(robSim.getX()));
        xmlWriter.writeAttribute("y", QString::number(robSim.getY()));

        xmlWriter.writeAttribute("radius", QString::number(robSim.getRadius()));
        xmlWriter.writeAttribute("detection_radius", QString::number(robSim.getDetRadius()));

        xmlWriter.writeAttribute("rotation", QString::number(robSim.getRotation()));

        xmlWriter.writeAttribute("color_red", QString::number(rob->getColor().red()));
        xmlWriter.writeAttribute("color_green", QString::number(rob->getColor().green()));
        xmlWriter.writeAttribute("color_blue", QString::number(rob->getColor().blue()));

        xmlWriter.writeAttribute("speed", QString::number(rob->getSpeed()));
        xmlWriter.writeAttribute("turn_angle", QString::number(rob->getTurnAngle()));
        xmlWriter.writeAttribute("turn_direction", QString::number(rob->getTurnDirection()));
    }

    xmlWriter.writeEndElement(); /*end robots*/

    xmlWriter.writeStartElement("Obstacles");

    for(size_t i = 0; i < obstacles.size(); i++)
    {
        auto& obsSim = obstacles.at(i).get()->getSim();
        auto obstacle = obstacles.at(i).get();

        xmlWriter.writeEmptyElement(QString::number(i));
        xmlWriter.writeAttribute("x", QString::number(obsSim.getX()));
        xmlWriter.writeAttribute("y", QString::number(obsSim.getY()));
        xmlWriter.writeAttribute("width", QString::number(obsSim.getW()));
        xmlWriter.writeAttribute("heigth", QString::number(obsSim.getH()));

        xmlWriter.writeAttribute("rotation", QString::number(obsSim.getRotation()));

        xmlWriter.writeAttribute("color_red", QString::number(obstacle->getColor().red()));
        xmlWriter.writeAttribute("color_green", QString::number(obstacle->getColor().green()));
        xmlWriter.writeAttribute("color_blue", QString::number(obstacle->getColor().blue()));
    }

    xmlWriter.writeEndElement();

    xmlWriter.writeEndDocument();

    file.close();
}


void MainWindow::on_saveManger_btn_load_clicked()
{
    QString fileName =
        QFileDialog::getOpenFileName(this, tr("Open Save File"),
                                     QDir::currentPath() + "/debug/saves/",
                                     tr("XBEL Files (*.xbel *.xml)"));

    QFile file(fileName);

    if(!file.open(QIODevice::ReadWrite))
    {
        qDebug("Failed to open XML file for reading");
    }

    QXmlStreamReader xmlReader(&file);


}

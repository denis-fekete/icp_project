/**
 * @file savemanager.cpp
 * @brief Implementation of SaveManager class methods
 *
 * @author Denis Fekete (xfeket01@fit.vutbr.cz)
 */

#include "savemanager.h"

#include <QMessageBox>
#include <QCoreApplication>

SaveManager::SaveManager(Simulator& simulator, QWidget* widget) : simulator(simulator)
{
    this->widget = widget;
}

void SaveManager::saveToFile()
{
    QString fileName =
        QFileDialog::getOpenFileName(widget, QCoreApplication::tr("Open Save File"),
                                     QDir::currentPath() + "/debug/saves/",
                                     QCoreApplication::tr("XML Files (*.xml)"));

    QFile file(fileName);

    if(!file.open(QIODevice::ReadWrite | QIODevice::Truncate))
    {
        QMessageBox msgBox;
        msgBox.setText("Failed to open XML file for reading! No changes will be made to Simulator.");
        msgBox.exec();
        return;
    }

    xmlWriter.setDevice(&file);

    xmlWriter.setAutoFormatting(true);

    xmlWriter.writeStartDocument();
    // xmlWriter.writeDTD("<!DOCTYPE icpSave>");

    xmlWriter.writeStartElement("Simulation_space");
    xmlWriter.writeAttribute("width", QString::number(simulator.getSimulationWidth()));
    xmlWriter.writeAttribute("height", QString::number(simulator.getSimulationHeight()));

    xmlWriter.writeStartElement("Robots");

    Robot* robSim;
    BaseRobot* rob;
    AutoRobot* autoRob = nullptr;

    const double simTimePeriod = 1000 / simulator.getTimerPeriod();
    for(size_t i = 0; i < simulator.getRobotsCount(); i++)
    {
        robSim = simulator.getRobot(i)->getSim();
        rob = simulator.getRobot(i);

        if(typeid(*rob) == typeid(ManualRobot))
        {
            xmlWriter.writeStartElement("Robot");
            xmlWriter.writeAttribute("type", "manual");
        }
        else
        {
            xmlWriter.writeStartElement("Robot");
            xmlWriter.writeAttribute("type", "automatic");
            autoRob = dynamic_cast<AutoRobot*> (rob);
        }

        xmlWriter.writeAttribute("id", QString::number(i));

        xmlWriter.writeTextElement("x", QString::number(robSim->getX()));
        xmlWriter.writeTextElement("y", QString::number(robSim->getY()));

        xmlWriter.writeTextElement("radius", QString::number(robSim->getRadius()));
        xmlWriter.writeTextElement("detection_radius", QString::number(robSim->getDetRadius()));

        xmlWriter.writeTextElement("rotation", QString::number(robSim->getRotation()));

        xmlWriter.writeTextElement("color_red", QString::number(rob->getColor().red()));
        xmlWriter.writeTextElement("color_green", QString::number(rob->getColor().green()));
        xmlWriter.writeTextElement("color_blue", QString::number(rob->getColor().blue()));

        if(autoRob != nullptr)
        {

            xmlWriter.writeTextElement("speed", QString::number(autoRob->getSpeed() * simTimePeriod));
            xmlWriter.writeTextElement("turn_angle", QString::number(autoRob->getTurnAngle() * simTimePeriod));
            xmlWriter.writeTextElement("turn_direction", QString::number(autoRob->getTurnDirection()));
        }
        xmlWriter.writeEndElement(); // end Robot
    }

    xmlWriter.writeEndElement(); // end Robots

    xmlWriter.writeStartElement("Obstacles");

    for(size_t i = 0; i < simulator.getObstaclesCount(); i++)
    {
        Rectangle* obsSim = simulator.getObstacle(i)->getSim();
        Obstacle* obstacle = simulator.getObstacle(i);

        xmlWriter.writeStartElement("Obstacle");
        xmlWriter.writeAttribute("id", QString::number(i));

        xmlWriter.writeTextElement("x", QString::number(obsSim->getX()));
        xmlWriter.writeTextElement("y", QString::number(obsSim->getY()));
        xmlWriter.writeTextElement("width", QString::number(obsSim->getW()));
        xmlWriter.writeTextElement("height", QString::number(obsSim->getH()));

        xmlWriter.writeTextElement("rotation", QString::number(obsSim->getRotation()));

        xmlWriter.writeTextElement("color_red", QString::number(obstacle->getColor().red()));
        xmlWriter.writeTextElement("color_green", QString::number(obstacle->getColor().green()));
        xmlWriter.writeTextElement("color_blue", QString::number(obstacle->getColor().blue()));

        xmlWriter.writeEndElement(); // end Obstacle
    }

    xmlWriter.writeEndElement(); // end Obstacles

    xmlWriter.writeEndDocument();

    file.close();
}


void SaveManager::loadFromFile()
{
    QString fileName =
        QFileDialog::getOpenFileName(widget, QCoreApplication::tr("Open Save File"),
                                     QDir::currentPath() + "/debug/saves/",
                                     QCoreApplication::tr("XML Files (*.xml)"));

    QFile file(fileName);

    if(!file.open(QIODevice::ReadOnly))
    {
        QMessageBox msgBox;
        msgBox.setText("Failed to open XML file for reading. No changes will be done to Simulator");
        msgBox.exec();
        return;
    }

    xmlReader.setDevice(&file);

    xmlReader.readNextStartElement();
    if(xmlReader.name() != "Simulation_space")
    {
        file.close();

        QMessageBox msgBox;
        msgBox.setText("Bad XML format. First element is not <Simulation_space>");
        msgBox.exec();
        return;
    }


    auto attrs = xmlReader.attributes();

    double width = -1;
    double height = -1;

    if(attrs.hasAttribute("width"))
    {
        width = attrs.value("width").toDouble();
    }

    if(attrs.hasAttribute("height"))
    {
        height = attrs.value("width").toDouble();
    }

    if(width == -1 || height == -1)
    {
        file.close();

        QMessageBox msgBox;
        msgBox.setText("Bad XML format. Width or Height attributes not set in <Simulation_space>");
        msgBox.exec();
        return;
    }

    simulator.setSimulationSize(width, height);

    returnType result = readNext;
    while(!xmlReader.atEnd())
    {
        if(result == readNext)
            xmlReader.readNextStartElement();

        if(xmlReader.tokenType() == QXmlStreamReader::StartElement)
        {
            if(xmlReader.name() == "Robots")
            {
                result = readRobots();
            }
            else if(xmlReader.name() == "Obstacles")
            {
                result = readObstacles();
            }
            else
            {
                result = err;
                break;
            }
        }
    }

    if(result == err)
    {
        file.close();

        QMessageBox msgBox;
        msgBox.setText("Unknown XML format. Unknown eleemt, please check source file and correct it.");
        msgBox.exec();
        return;
    }

    // balance core load
    simulator.balanceCores();

    file.close();
}

returnType SaveManager::readRobots()
{
    returnType result = readNext;
    while (xmlReader.readNextStartElement() && result == readNext)
    {
        if(xmlReader.name() == "Robot")
        {
            result = readRobot();
        }
    }

    return result;
}

returnType SaveManager::readRobot()
{
    bool automatic = false;
    if(xmlReader.attributes().value("type") == "automatic")
    {
        automatic = true;
    }
    else if (xmlReader.attributes().value("type") == "manual")
    {}
    else
    {
        return err;
    }

    double x = -1;
    double y = -1;
    double radius = -1;
    double detRad = -1;
    bool rotRead = false;
    double rot = 0;
    bool colorRedRead = false;
    bool colorGreenRead = false;
    bool colorBlueRead = false;
    QColor color = Qt::black;
    double speed = -1;
    bool turnAngleRead = false;
    double turnAngle = 0;
    bool turnDirectionRead = false;
    int turnDirection = 0;

    while (xmlReader.readNextStartElement())
    {
        // found end of new section
        if(xmlReader.name() == "Obstacles")
        {
            break;
        }

        if (xmlReader.tokenType() == QXmlStreamReader::StartElement) {
            QStringRef elementName = xmlReader.name();

            if (elementName == "x")
                x = xmlReader.readElementText().toDouble();
            else if (elementName == "y")
                y = xmlReader.readElementText().toDouble();
            else if (elementName == "radius")
                radius = xmlReader.readElementText().toInt();
            else if(elementName == "detection_radius")
                detRad = xmlReader.readElementText().toDouble();
            else if(elementName == "speed")
                speed = xmlReader.readElementText().toDouble();
            else if(elementName == "turn_angle")
            {
                turnAngle = xmlReader.readElementText().toDouble();
                // check if not set, if set return false as fail
                if(!turnAngleRead)
                    turnAngleRead = true;
                else
                    return err;
            }
            else if(elementName == "turn_direction")
            {
                turnDirection = xmlReader.readElementText().toInt();
                // check if not set, if set return false as fail
                if(!turnDirectionRead)
                    turnDirectionRead = true;
                else
                    return err;
            }
            else if(elementName == "rotation")
            {
                rot = xmlReader.readElementText().toDouble();
                // check if not set, if set return false as fail
                if(!rotRead)
                    rotRead = true;
                else
                    return err;
            }
            else if(elementName == "color_red")
            {
                color.setRed(xmlReader.readElementText().toDouble());
                // check if not set, if set return false as fail
                if(!colorRedRead)
                    colorRedRead = true;
                else
                    return err;
            }
            else if(elementName == "color_green")
            {
                color.setGreen(xmlReader.readElementText().toDouble());
                // check if not set, if set return false as fail
                if(!colorGreenRead)
                    colorGreenRead = true;
                else
                    return err;
            }
            else if(elementName == "color_blue")
            {
                color.setBlue(xmlReader.readElementText().toDouble());
                // check if not set, if set return false as fail
                if(!colorBlueRead)
                    colorBlueRead = true;
                else
                    return err;
            }
            else
            {
                return err; // unknown element
            }
        }
    }

    if(automatic)
    {
        // check if all parameters were read
        if(x > -1 && y > -1 && radius > -1 && detRad > -1 && speed > -1 &&
            rotRead && colorRedRead && colorGreenRead &&
            colorBlueRead && turnAngleRead && turnDirectionRead)
        {
            simulator.addAutomaticRobot(
                x, y, radius, rot, detRad,
                color,
                speed,
                turnAngle,
                turnDirection);
        }
        else
            return err;
    }
    else
    {
        // check if all parameters were read
        if(x > -1 && y > -1 && radius > -1 && detRad > -1 && speed > -1 &&
            rotRead && colorRedRead && colorGreenRead &&
            colorBlueRead)
        {
            simulator.addManualRobot(
                x, y, radius, rot, detRad,
                color);
        }
        else
            return err;
    }


    if(xmlReader.name() == "Obstacles")
        return dontReadNext;
    else
        return readNext;

}

returnType SaveManager::readObstacles()
{
    returnType result = readNext;
    while (xmlReader.readNextStartElement())
    {
        if(xmlReader.name() == "Obstacle")
        {
            result = readObstacle();
        }
        else
        {
            return err;
        }
    }

    return result;
}

returnType SaveManager::readObstacle()
{
    double x = -1;
    double y = -1;
    double w = -1;
    double h = -1;
    bool rotRead = false;
    double rot = 0;
    bool colorRedRead = false;
    bool colorGreenRead = false;
    bool colorBlueRead = false;
    QColor color = Qt::black;

    while (xmlReader.readNextStartElement())
    {
        if (xmlReader.tokenType() == QXmlStreamReader::StartElement)
        {
            QStringRef elementName = xmlReader.name();

            if (elementName == "x")
                x = xmlReader.readElementText().toDouble();
            else if (elementName == "y")
                y = xmlReader.readElementText().toDouble();
            else if(elementName == "width")
                w = xmlReader.readElementText().toDouble();
            else if(elementName == "height")
                h = xmlReader.readElementText().toDouble();
            else if(elementName == "rotation")
            {
                rot = xmlReader.readElementText().toDouble();
                // check if not set, if set return false as fail
                if(!rotRead)
                    rotRead = true;
                else
                    return err;
            }
            else if(elementName == "color_red")
            {
                color.setRed(xmlReader.readElementText().toDouble());
                // check if not set, if set return false as fail
                if(!colorRedRead)
                    colorRedRead = true;
                else
                    return err;
            }
            else if(elementName == "color_green")
            {
                color.setGreen(xmlReader.readElementText().toDouble());
                // check if not set, if set return false as fail
                if(!colorGreenRead)
                    colorGreenRead = true;
                else
                    return err;
            }
            else if(elementName == "color_blue")
            {
                color.setBlue(xmlReader.readElementText().toDouble());
                // check if not set, if set return false as fail
                if(!colorBlueRead)
                    colorBlueRead = true;
                else
                    return err;
            }
            else
            {
                return err; // unknown element
            }
        }
    }

    // check if all parameters were read
    if(x > -1 && y > -1 && w > -1 && h > -1 &&
        rotRead && colorRedRead && colorGreenRead &&
        colorBlueRead)
    {
        simulator.addObstacle(x, y, w, h, rot, color);
    }

    return readNext;
}


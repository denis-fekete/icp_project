/**
 * @file savemanager.h
 * @brief Header file for SaveManager class that implements methods for 
 * saving/loading simulations into/from XML files in human readable form.
 * 
 * SaveManger is method for managing simulations, either by loading them 
 * from a file, or by saving them into a file for later use. XML format was 
 * chosen because it is easily readable for human is is quite robust to 
 * errors like indentation errors that could be cause by user editing files 
 * manually.
 * 
 * @author Denis Fekete (xfeket01@fit.vutbr.cz)
 */

#ifndef SAVEMANAGER_H
#define SAVEMANAGER_H

#include "simulator.h"
#include "QIODevice"
#include "QFile"
#include "QXmlStreamReader"
#include "QXmlStreamWriter"
#include <qcoreapplication.h>
#include "QDebug"
#include "QFileDialog"

#include "QtSpecific/autorobot.h"
#include "QtSpecific/obstacle.h"

/**
 * @brief Enumeration for internal communication between methods, to know 
 * if an error occurred, or next value should or shouldn't be read
 */
using returnType = enum {err, dontReadNext, readNext};

class SaveManager
{
public:

    /**
     * @brief Construct of a SaveManager object
     * 
     * @param simulator Reference to the Simulator object where objects will 
     * be stored into/will get saved from
     * @param widget Pointer to the QWidget of main window for making popup 
     * windows for user feedback
     */
    SaveManager(Simulator& simulator, QWidget* widget);

    /**
     * @brief Loads Simulation from choosen file
     */
    void loadFromFile();

    /**
     * @brief Stores Simulation into a chosen file 
     */
    void saveToFile();
private:
    /**
     * @brief Object for reading from XML
     */
    QXmlStreamReader xmlReader;

    /**
     * @brief Object for writing into a XML
     */
    QXmlStreamWriter xmlWriter;

    /**
     * @brief Reference to the Simulator object for storing/loading data 
     */
    Simulator& simulator;

    /**
     * @brief Pointer to the main window QWidget for popup windows
     */
    QWidget* widget;

    /**
     * @brief Reads <Robots> section of XML file and stores it into a Simulator
     * 
     * @return returnType Enum value for caller to know next step
     */
    returnType readRobots();

    /**
     * @brief Reads <Robot> section of XML file and stores it into a Simulator
     * 
     * @return returnType Enum value for caller to know next step
     */
    returnType readRobot();

    /**
     * @brief Reads <Obstacles> section of XML file and stores it into a Simulator
     * 
     * @return returnType Enum value for caller to know next step
     */
    returnType readObstacles();

    /**
     * @brief Reads <Obstacle> section of XML file and stores it into a Simulator
     * 
     * @return returnType Enum value for caller to know next step
     */
    returnType readObstacle();
};

#endif // SAVEMANAGER_H

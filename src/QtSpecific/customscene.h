#ifndef CUSTOMSCENE_H
#define CUSTOMSCENE_H

#include <QGraphicsSceneMouseEvent>
#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QDebug>

#include "2DSimulationLib/simulator.h"

class CustomScene : public QGraphicsScene
{
public:
    CustomScene(QObject *parent = nullptr) : QGraphicsScene(parent) {}

    void setSimulator(Simulator* simulator) { this->simulator = simulator; }
protected:
    Simulator* simulator;

    void mousePressEvent(QGraphicsSceneMouseEvent *event) override
    {
        QGraphicsScene::mousePressEvent(event);

        // Check if an item was selected
        QGraphicsItem *item = itemAt(event->scenePos(), QTransform());
        if (!item)
        {
            simulator->unselectActive();
        }
    }
};



#endif // CUSTOMSCENE_H

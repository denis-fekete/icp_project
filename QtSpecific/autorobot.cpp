#include "autorobot.h"
#include "QPen"

AutoRobot::AutoRobot(double x, double y, double radius, double rot,
                     double detRadius, QColor color, double speed,
                     double turnAngle, bool turnRight,
                     std::vector<std::unique_ptr<Obstacle>>& obstacles) : sim(x, y, radius, rot, detRadius), obstacles(obstacles)
{
    this->speed = speed;
    this->color = color;
    this->turnAngle = turnAngle;
    this->turnDirection = (turnRight) ? 1 : -1;

    initialized = false;
}

void AutoRobot::initialize(QGraphicsScene& scene)
{
    this->setFlag(QGraphicsItem::ItemIsMovable);
    this->setFlag(QGraphicsItem::ItemSendsGeometryChanges);
    this->setTransformOriginPoint(0, 0);
    scene.addItem(this);
    initialized = true;
}

QRectF AutoRobot::boundingRect() const
{
    const double adj = 1;
    // const auto halfRadius = sim.getRadius() / 2;
    return QRectF(-sim.getRadius() - adj, -sim.getRadius() - adj, sim.getRadius() + adj, sim.getRadius() + adj);
}

QPainterPath AutoRobot::shape() const
{
    QPainterPath path;
    // const auto halfRadius = sim.getRadius() / 2;
    // path.addEllipse(-halfRadius, -halfRadius, sim.getRadius(), sim.getRadius());
    // path.addRect(-halfRadius, -halfRadius, sim.getRadius(), sim.getRadius());
    path.addEllipse(-sim.getRadius(), -sim.getRadius(), sim.getRadius(), sim.getRadius());
    path.addRect(-sim.getDetRadius(), -sim.getDetRadius(), sim.getRadius(), sim.getRadius());
    return path;
}

void AutoRobot::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    // Correctly rotate
    this->setPos(0, 0);
    this->setRotation(sim.getRotation());
    this->setPos(sim.getX(), sim.getY());

    // Division is "costly", store it
    // const auto halfRadius = sim.getRadius() / 2;

    // Body
    painter->setBrush(color);
    // painter->drawEllipse(-halfRadius, -halfRadius, sim.getRadius(), sim.getRadius());
    painter->drawEllipse(-sim.getRadius(), -sim.getRadius(), 2 * sim.getRadius(), 2 * sim.getRadius());

    // Collider
    painter->setBrush(QBrush(this->color, Qt::BrushStyle::NoBrush));

    // painter->drawRect(-halfRadius, -halfRadius, sim.colliderFwd.getW(), sim.colliderFwd.getH());
    // painter->drawRect(-sim.getRadius(), -sim.getRadius(), sim.colliderFwd.getW(), sim.colliderFwd.getH());
    painter->drawRect(-sim.getRadius(), -sim.getRadius(),  sim.getRadius() + sim.getDetRadius(), 2 * sim.getRadius());

    painter->drawEllipse(-sim.getDetRadius(), -sim.getDetRadius(), 2 * sim.getDetRadius(), 2 * sim.getDetRadius()); //DEBUG:
}

void AutoRobot::simulate()
{
#ifdef USE_QT_COLLISION
    if(!initialized) return;

    bool collision = false;
    for(size_t i = 0; i < obstacles.size(); i++)
    {
        Obstacle* obs = obstacles.at(i).get();
        if(this->collidesWithItem(obs))
        {
            collision = true;
            break;
        }
    }

    if(collision)
    {
        this->rotateRobot(turnAngle * turnDirection);
    }
    else
    {
        this->moveRobot(speed);
    }

#else
    bool collision = this->sim.obstacleDetection(obstacles);
    if(collision)
    {
        this->rotateRobot(turnAngle * turnDirection);
    }
    else
    {
        this->moveRobot(speed);
    }
#endif
}


void AutoRobot::setSelected()
{
    // QColor highlightedColor = color.lighter(30);
    // this->setPen(QPen(highlightedColor, 5));
}

void AutoRobot::setUnselected()
{
    // this->setPen(QPen(color));
}

void AutoRobot::addRobotToWorld( double x, double y, double radius, double rot,
                                double detRadius, QColor color, double speed,
                                double turnAngle, bool turnRight,
                                std::vector<std::unique_ptr<Obstacle>>& obstaclesPointer,
                                std::vector<std::unique_ptr<AutoRobot>>& robots, QGraphicsScene& scene)
{
    robots.push_back(std::make_unique<AutoRobot> ( x, y, radius, rot, detRadius, color, speed, turnAngle, turnRight, obstaclesPointer));

    robots.back()->initialize(scene);
}

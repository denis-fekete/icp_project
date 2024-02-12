#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QgraphicsView>
#include <QGraphicsItem>
#include <vector>

#include <robot.h>
#include <rectangle.h>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QGraphicsScene *scene;

    std::vector<Robot*> robots;
    std::vector<Rectangle*> obstacles;
};
#endif // MAINWINDOW_H

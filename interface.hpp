#ifndef INTERFACE_HPP
#define INTERFACE_HPP

#include <QtOpenGL>
#include <QTimerEvent>

#include "object.hpp"
#include "world.hpp"

#include "shaders.hpp"

class Interface : public QGLWidget
{
private:
    int              timerId;
    double           time;
    QGLShaderProgram modelShader;
    QGLShaderProgram primativeShader;
    Object           model;
    QVector2D        size;
    double           rotation;

    World            world;

    void paintModels();
    void paintFood();
    void paintCube();

public:
    Interface();

    void initializeGL();
    void paintGL();
    void resizeGL(int width, int height);
    void hideEvent(QHideEvent *e);
    void showEvent(QShowEvent *e);
    void timerEvent(QTimerEvent *e);
};

#endif

#include "interface.hpp"

#include <ctime>
#include <iostream>

#include "constants.hpp"

Interface::Interface() :
    QGLWidget(),
    timerId(0),
    time(0.0),
    modelShader(),
    primativeShader(),
    model("fish"),
    size(),
    rotation(0.0),
    world()
{
    this->showMaximized();
    this->timerId = this->startTimer(MAX_FRAMERATE);
}

void Interface::initializeGL()
{
    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glEnable(GL_TEXTURE_2D);

    glFrontFace(GL_CCW);
    glCullFace(GL_BACK);

    glPointSize(2.0);
    glLineWidth(2.0);

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glClearColor(0.0, 0.0, 0.4, 0.0);

    this->modelShader.addShaderFromSourceCode(QGLShader::Vertex,
                                              MODEL_VERT);
    this->modelShader.addShaderFromSourceCode(QGLShader::Fragment,
                                              MODEL_FRAG);

    this->modelShader.link();

    this->primativeShader.addShaderFromSourceCode(QGLShader::Vertex,
                                                  PRIMATIVE_VERT);
    this->primativeShader.addShaderFromSourceCode(QGLShader::Fragment,
                                                  PRIMATIVE_FRAG);

    this->primativeShader.link();
}

void Interface::paintGL()
{
    this->paintModels();
    this->paintFood();
    this->paintCube();
}

void Interface::paintModels()
{
    this->modelShader.bind();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    this->bindTexture(this->model.getTexture());

    this->modelShader.setUniformValue("time",     (GLfloat)this->time);
    this->modelShader.setUniformValue("rotation", (GLfloat)this->rotation);
    this->modelShader.setUniformValue("size",     this->size);
    this->modelShader.setUniformValue("tex",      0);

    this->modelShader.enableAttributeArray("vertex");
    this->modelShader.enableAttributeArray("texCoord");
    this->modelShader.enableAttributeArray("normal");

    this->modelShader.setAttributeArray("vertex",   this->model.getVertexs());
    this->modelShader.setAttributeArray("texCoord", this->model.getTexCoords());
    this->modelShader.setAttributeArray("normal",   this->model.getNormals());

    std::vector<Fish*> &population = this->world.getPopulation();
    std::vector<Fish*>::iterator a;

    for (a = population.begin(); a != population.end(); ++a)
    {
        if ((*a)->getEnergy() < 0.0)
            continue;

        this->modelShader.setUniformValue("position",     (*a)->getPosition());
        this->modelShader.setUniformValue("yawPitchRoll", (*a)->getYawPitchRoll());

        glDrawArrays(GL_TRIANGLES, 0, this->model.vertexCount());
    }

    this->modelShader.release();
}

void Interface::paintFood()
{
    this->primativeShader.bind();

    std::vector<QVector3D> &food = this->world.getFood();

    this->primativeShader.enableAttributeArray("vertex");

    this->primativeShader.setUniformValue("rotation", (GLfloat)this->rotation);
    this->primativeShader.setUniformValue("size",     this->size);
    this->primativeShader.setUniformValue("color",    QVector3D(1.0, 1.0, 1.0));

    this->primativeShader.setAttributeArray("vertex", &(food[0]));

    glDrawArrays(GL_POINTS, 0, food.size());

    this->primativeShader.release();
}

void Interface::paintCube()
{
    this->primativeShader.bind();

    this->primativeShader.setUniformValue("rotation", (GLfloat)this->rotation);
    this->primativeShader.setUniformValue("size",     this->size);
    this->primativeShader.setUniformValue("color",    QVector3D(0.0, 0.0, 0.0));

    glBegin(GL_LINES);

    // Top face.
    glVertex3f(-1.0, -1.0, 1.0);
    glVertex3f( 1.0, -1.0, 1.0);
    glVertex3f( 1.0, -1.0, 1.0);
    glVertex3f( 1.0,  1.0, 1.0);
    glVertex3f( 1.0,  1.0, 1.0);
    glVertex3f(-1.0,  1.0, 1.0);
    glVertex3f(-1.0,  1.0, 1.0);
    glVertex3f(-1.0, -1.0, 1.0);

    // Bottom face.
    glVertex3f(-1.0, -1.0, -1.0);
    glVertex3f( 1.0, -1.0, -1.0);
    glVertex3f( 1.0, -1.0, -1.0);
    glVertex3f( 1.0,  1.0, -1.0);
    glVertex3f( 1.0,  1.0, -1.0);
    glVertex3f(-1.0,  1.0, -1.0);
    glVertex3f(-1.0,  1.0, -1.0);
    glVertex3f(-1.0, -1.0, -1.0);

    // Verticles.
    glVertex3f(-1.0, -1.0, -1.0);
    glVertex3f(-1.0, -1.0,  1.0);
    glVertex3f( 1.0, -1.0, -1.0);
    glVertex3f( 1.0, -1.0,  1.0);
    glVertex3f(-1.0,  1.0, -1.0);
    glVertex3f(-1.0,  1.0,  1.0);
    glVertex3f( 1.0,  1.0, -1.0);
    glVertex3f( 1.0,  1.0,  1.0);

    glEnd();

    this->primativeShader.release();
}

void Interface::resizeGL(int width, int height)
{
    this->size = QVector2D(width, height);
    glViewport(0, 0, width, height);
}

void Interface::hideEvent(QHideEvent *e)
{
    if (!this->timerId)
        return;

    this->killTimer(this->timerId);
    this->timerId = this->startTimer(MIN_FRAMERATE);
}

void Interface::showEvent(QShowEvent *e)
{
    if (!this->timerId)
        return;

    this->killTimer(this->timerId);
    this->timerId = this->startTimer(MAX_FRAMERATE);
}

void Interface::timerEvent(QTimerEvent *e)
{
    this->rotation += GUI_ROTATION;
    this->time += 0.1;

    this->world.update();

    this->setWindowTitle(QString("Generation: ")
                       + QString::number(this->world.getGeneration())
                       + " Ticks: "
                       + QString::number(this->world.getTicks()));

    if (!this->isMinimized())
        this->update();
}

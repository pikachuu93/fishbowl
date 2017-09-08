#ifndef OBJECT_HPP
#define OBJECT_HPP

#include <string>
#include <vector>

#include <QImage>
#include <QVector3D>
#include <QStringList>

class Object
{
private:
    std::string            dataFile;
    std::string            textureFile;
    QImage                 texture;
    std::vector<QVector3D> vertexs;
    std::vector<QVector3D> texCoords;
    std::vector<QVector3D> normals;

    QVector3D parseVertex(QStringList           parts);
    void      parseFace(QStringList             parts,
                        std::vector<QVector3D> &tempVertexs,
                        std::vector<QVector3D> &tempTexCoords,
                        std::vector<QVector3D> &tempNormals);

public:
    Object(std::string name);

    void load();

    QImage    &getTexture();
    QVector3D *getVertexs();
    QVector3D *getTexCoords();
    QVector3D *getNormals();
    int        vertexCount();
};

#endif

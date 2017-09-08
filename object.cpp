#include <fstream>
#include <iostream>

#include "object.hpp"

Object::Object(std::string name) :
    dataFile(),
    textureFile(),
    vertexs(),
    texCoords(),
    normals()
{
    this->dataFile = name + ".obj";
    this->textureFile = name + ".jpeg";

    this->texture = QImage(QString(this->textureFile.data()))
                   .convertToFormat(QImage::Format_ARGB32);

    this->load();
}

QVector3D Object::parseVertex(QStringList parts)
{
    double x, y, z;

    x = parts[1].toDouble();
    y = parts[2].toDouble();
    z = parts[3].toDouble();

    return QVector3D(x, y, z);
}

void Object::parseFace(QStringList             parts,
                       std::vector<QVector3D> &tempVertexs,
                       std::vector<QVector3D> &tempTexCoords,
                       std::vector<QVector3D> &tempNormals)
{
    QStringList::iterator v1Data = parts.begin() + 1;
    QStringList::iterator v2Data = v1Data + 1;
    QStringList::iterator v3Data = v2Data + 1;
    QStringList           v1     = v1Data->split("/");

    for (; v3Data != parts.end(); ++v2Data, ++v3Data)
    {
        QStringList v2 = v2Data->split("/");
        QStringList v3 = v3Data->split("/");

        this->vertexs.push_back(tempVertexs.at(v1[0].toInt() - 1));
        this->vertexs.push_back(tempVertexs.at(v2[0].toInt() - 1));
        this->vertexs.push_back(tempVertexs.at(v3[0].toInt() - 1));

        this->texCoords.push_back(tempTexCoords[v1[1].toInt() - 1]);
        this->texCoords.push_back(tempTexCoords[v2[1].toInt() - 1]);
        this->texCoords.push_back(tempTexCoords[v3[1].toInt() - 1]);

        this->normals.push_back(tempNormals[v1[2].toInt() - 1]);
        this->normals.push_back(tempNormals[v2[2].toInt() - 1]);
        this->normals.push_back(tempNormals[v3[2].toInt() - 1]);
    }
}

void Object::load()
{
    std::ifstream data(this->dataFile.data());

    std::vector<QVector3D> tempVertexs   = std::vector<QVector3D>();
    std::vector<QVector3D> tempTexCoords = std::vector<QVector3D>();
    std::vector<QVector3D> tempNormals   = std::vector<QVector3D>();

    while (data.good())
    {
        std::string rawLine;
        getline(data, rawLine);
        if (!rawLine.size())
            continue;

        QString line(rawLine.data());
        QStringList parts = line.split(" ", QString::SkipEmptyParts);
        if (parts[0][0] == 'v')
        {
            if (parts[0] == "v")
            {
                QVector3D v = this->parseVertex(parts);
                tempVertexs.push_back(v);
            }
            else if (parts[0] == "vt")
            {
                QVector3D v = this->parseVertex(parts);
                tempTexCoords.push_back(v);
            }
            else if (parts[0] == "vn")
            {
                QVector3D v = this->parseVertex(parts);
                tempNormals.push_back(v);
            }
        }
        else if (parts[0] == "f")
        {
            this->parseFace(parts, tempVertexs, tempTexCoords, tempNormals);
        }
    }

    data.close();
}

QImage &Object::getTexture()
{
    return this->texture;
}

QVector3D *Object::getVertexs()
{
    return &(this->vertexs[0]);
}

QVector3D *Object::getTexCoords()
{
    return &(this->texCoords[0]);
}

QVector3D *Object::getNormals()
{
    return &(this->normals[0]);
}

int Object::vertexCount()
{
    return this->vertexs.size();
}

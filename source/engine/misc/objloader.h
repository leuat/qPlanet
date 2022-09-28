#ifndef OBJLOADER_H
#define OBJLOADER_H

#include <QString>
#include <QStringList>
#include <QFile>
#include <QVector>
#include <QVector3D>
#include <QDebug>

#include <QtEndian>
class Face {
public:
    unsigned int v1=-1, v2=-1, v3=-1;
    unsigned int n1=-1,n2=-1,n3=-1;
    unsigned int vt1=-1, vt2=-1, vt3=-1;
};


class ObjGroup {
public:
    QVector<Face> m_faces;
    QVector<QVector3D> m_vertices, m_normals;
    QVector<QVector2D> m_tx;
    bool hasNormals = false;
    bool hasUV = false;
    bool singleFacelist = false;
};


class ObjLoader
{
public:

    QString m_filename;
    QString defaultGroup = "default";
    QStringList m_data;
    QMap<QString, QSharedPointer<ObjGroup>> groups;


    ObjLoader();
    ObjLoader(QString f) {
        Load(f);
    }

    void Load(QString fn);

    void Parse();

};

#endif // OBJLOADER_H

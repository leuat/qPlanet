#ifndef LTRIANGLES_H
#define LTRIANGLES_H

#include <QVector3D>
#include <QVector>

class LTriangle {
public:
    QVector3D m_p0, m_p1, m_p2;
    LTriangle() {}
    LTriangle(QVector3D p0, QVector3D p1, QVector3D p2) :m_p0(p0), m_p1(p1), m_p2(p2) {
    }
};


class LTriangles
{
public:
    LTriangles();
    QVector<LTriangle> m_triangles;
};

#endif // LTRIANGLES_H

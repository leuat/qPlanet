#ifndef MESHMAZE_H
#define MESHMAZE_H


#include "meshinstance.h".h"
#include <QVector>
#include <QVector3D>

class MeshWalker {
public:
    bool dead = false;
    QPoint pos;
    QPoint dir = QPoint(0,-1);
    int steps;

    MeshWalker(QPoint p,int ms): pos(p), steps(ms) {

    }
    void Turn()
    {
        int d=rand()%4;
        if (d==0) dir=QPoint(1,0);
        if (d==1) dir=QPoint(-1,0);
        if (d==2) dir=QPoint(0,1);
        if (d==3) dir=QPoint(0,-1);

    }
};


class MeshMaze : public MeshInstance
{
public:
    QPointF size;
    int width=0, height=0;
    MeshMaze(int w, int h);

    static const int FILLED = 0;
    static const int ROOM = 1;


    QVector<int> data;
    QVector<MeshWalker> walkers;

    void GenerateMaze();
    void GenerateMesh();




};

#endif // MESHMAZE_H

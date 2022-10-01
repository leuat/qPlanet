#include "meshmaze.h"


MeshMaze::MeshMaze(int w, int h) : width(w), height(h) {
    GenerateMaze();
    GenerateMesh();
}

void MeshMaze::GenerateMaze()
{
    data.resize(width*height);
    data.fill(FILLED);
    walkers.append(MeshWalker(QPoint(width/2,height/2),rand()%10+50));

    bool done = false;
    srand(2);
    while (!done) {
        done = true;
        for (auto& w:walkers) {
            if (w.steps==0)
                w.dead = true;
            if (w.dead)
                continue;

            done = false;
            int pos = w.pos.x()+w.pos.y()*width;
            if (w.pos.x()==0 || w.pos.x()==width || w.pos.y()==0 || w.pos.y()==height) w.dead=true;
            data[pos] = ROOM;
            if (rand()%4==0) w.Turn();

            QPoint nd = w.pos+w.dir;
            QPoint p2 = nd+w.dir;

            int pos2 = nd.x()+nd.y()*width;
            int pos3 = p2.x()+p2.y()*width;
            if (data[pos2]==FILLED && data[pos3]==FILLED) {
                w.pos = nd;
            }
            w.steps--;
        }

    }

    for (int y=0;y<height;y++) {
        QString s = "";
        for (int x=0;x<width;x++) {
            int pos = y*width + x;
            if (data[pos]==FILLED) s+="#";
            if (data[pos]==ROOM) s+=".";

        }
        qDebug() << s;
    }

}

void MeshMaze::GenerateMesh()
{

}


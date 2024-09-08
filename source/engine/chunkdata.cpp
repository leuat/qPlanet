#include "chunkdata.h"
#include <QThreadPool>
#include <QDebug>
#include <QThread>

WorldGen WorldGen::s;
ChunkData ChunkData::s;
float Chunk::scale = 1.0;
int Chunk::m_noChunks = 0;
QVector3D ChunkData::m_chunkSizes = QVector3D(16,4,0);

int WorldGen::generate(QVector3D pos, bool init)
{
/*
    if (pos.y()>-1.3)
        return 0;

    return 1;
*/
    pos.setY(pos.y()+hShift);
    float s = 0.05*2;
    float amp = 0.80*0.9;
    float h = sn.noise(pos.z()*s, pos.x()*s)*3*amp;
    h += sn.noise(pos.z()*s*5.21, pos.x()*s*6.42)*0.5*amp;
    //    float gh = sn.noise(pos.z()*s/4, pos.x()*s/4)*8;
    float gh = sn.getMultiFractal(pos*s/6.1,1.0, 4,2.0,1.0,2.0,0.0)*8*amp;
    float ls1 = sn.getMultiFractal(pos*s/20.3,1.0, 4,2.0,1.0,2.0,0.0)*8*amp;
    float ls2 = sn.noise(pos.z()*s/15.0, pos.x()*s/16.23)*1.9*amp;
    float trees = sn.noise(pos.z()*s*14.31, pos.x()*s*14.41);
    h+=gh-12+ls1;
    h*=ls2;
    int v = 1;
    // height

    float ground = pos.y()+20;

    if (pos.y()+25<0)
        v = 2; // water

    if (pos.y()+12+ls1>0)
        v = 3;


    if (pos.y()-5+gh>0)
        v = 4;

    float curHeight = ground-h;
    if (init) {
        if (v==1 && trees>0.99 && ground>h && ground <h+0.1) {
            ChunkData::s.mut.lock();
            auto np = QVector3D(pos - QVector3D(0,hShift,0));
//            np = QVector3D(round((np.x()/4)*4.0),np.y(),round((np.z()/4)*4.0));

            float s= 1.0;
//d            np = QVector3D((int)(np.x()*s), (int)(np.y()*s),(int)(np.z()*s));
//            qDebug() << np;

            if (!ChunkData::s.m_newEntitiesPos.contains(np)) {
                ChunkData::s.m_newEntities.append(QSharedPointer<ChunkEntity>(new ChunkTree(np)));
                ChunkData::s.m_newEntitiesPos.append(np);
            }

            ChunkData::s.mut.unlock();
   //         return 3;
            /*
        int curHeight = ground-h;

        float stemHeight = sn.noise(pos.z()*s*16.31, pos.y()*s, pos.x()*s*19.41)*2+2;
        // stem
        if(std::pow(trees+0.2,4)>1.9)
        if (curHeight<stemHeight)
            return 3;

//        if (trees>0.50)
            if (curHeight<stemHeight+4 && curHeight>=stemHeight)
                return 1;

        */

        }
    }



    if (ground>h && v!=2)
        return 0;




    return v;
}

int ChunkData::idxtoInt(QVector3D idx)
{
    idx += QVector3D(1,1,1)*1024;
    return ((int)idx.x())*Chunk::size*Chunk::size + ((int)idx.y())*Chunk::size + ((int)idx.z());
}

void ChunkData::remove(const QVector3D pos)
{
    m_data.remove(idxtoInt(pos/Chunk::scale/Chunk::size));
}

QSharedPointer<Chunk> ChunkData::get(QVector3D pos)
{
    QVector3D idx = pos  / Chunk::scale/ Chunk::size;
    if (m_data.keys().contains(idxtoInt(idx)))
        return m_data[idxtoInt(idx)];

    auto c = QSharedPointer<Chunk>(new Chunk(idx, pos));
    m_data[idxtoInt(idx)] = c;
    return c;
}

void ChunkData::set(QVector3D pos, unsigned short val)
{
    auto ch = get(pos);
    ch->m_changed = true;
    QVector3D idx = (pos - ch->m_pos)/Chunk::scale;/*/Chunk::size*/;// + 0.5*QVector3D(1,1.00,1)*Chunk::size;// - aidx*Chunk::size*Chunk::scale;/*/Chunk::size*/;// / Chunk::size / Chunk::scale;
    ch->set(round(idx.x()), round(idx.y()), round(idx.z()), val);
//    if (ch->m_ignore==true)
     ch->m_isGenerated = true;
    ch->m_ignore = false;

}

void ChunkData::Update()
{

    if (m_first) {
    }

    while (m_newEntities.count()!=0 && QThreadPool::globalInstance()->activeThreadCount()==0)
    {
//        qDebug() << m_newEntities.count();
        m_newEntities.first()->Grow();
        mut.lock();
        m_newEntities.removeFirst();
        mut.unlock();
    }



}

ChunkData::ChunkData() {
    srand(0);

}

Chunk::Chunk(QVector3D index, QVector3D pos)
{
    m_index = index;
    m_pos = pos;
    m_data.resize(size*size*size);
    m_data.fill(empty);
}


unsigned short Chunk::get(int i, int j, int k)
{
    /*
    auto p = (m_pos) + (QVector3D((i-size/2.0),(j-size/2.0),(k-size/2.0)))*scale;
    return WorldGen::s.generate(p,false);

*/
    if (m_ignore)
        return 0;
    if (i<0 || i>=size) return 0;
    if (j<0 || j>=size) return 0;
    if (k<0 || k>=size) return 0;
    return m_data[i*size*size + j*size + k];
}

unsigned short Chunk::getReal(int i, int j, int k)
{
    if (m_ignore)
        return 0 ;

    if (i>=0 && j>=0 && k>=0 && i<size && j<size && k<size && m_isGenerated)
        return get(i,j,k);

    auto p = (m_pos) + (QVector3D(i,j,k))*scale;
    //    if (rand()%1000>=999)
    //      qDebug() << p << WorldGen::s.generate(p);

    return WorldGen::s.generate(p, false);
}

void Chunk::set(int i, int j, int k, unsigned short d)
{
    const int idx = i*size*size + j*size + k;
    if (idx<0 || idx>=m_data.count())
        return;
    m_data[idx] = d;

}

void ChunkTree::Grow() {
    int height = rand()%4 +4;
//    qDebug() << "newtree "<<index;
    for (int i=0;i<height;i++) {
        ChunkData::s.set(index + QVector3D(0,(i)*Chunk::scale,0),1);
    }
    int bushHeight = height + rand()%4;
    for (int i=0;i<bushHeight;i++) {
        for (int j=0;j<bushHeight;j++)
            for (int k=0;k<bushHeight;k++) {
                QVector3D d = QVector3D(i-bushHeight/2,j-bushHeight/2,k-bushHeight/2)/(double)bushHeight;
                if (d.length()<0.45)
                    ChunkData::s.set(index + QVector3D(i-bushHeight/2,(j + height-1),k-bushHeight/2)*Chunk::scale,5);

            }
    }

}

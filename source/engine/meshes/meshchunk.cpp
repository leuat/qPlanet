#include "meshchunk.h"
#include "source/engine/sdata.h"
#include <QThreadPool>
#include <QDebug>

SimplexNoise MeshChunk::sn;

MeshChunk::MeshChunk(QVector3D pos, float scale, int type)
{
    isBuilt = false;
    m_pos = pos;
    m_scale = scale;
    m_type = type;
    size = worldSize;
    m_orgScale = m_scale;
    connect(this, &MeshChunk::meshReady, this, &MeshChunk::finishThread);
    //start();
//    run();
    m_shadowTick = rand()%8;

}



void MeshChunk::run()
{
//    qDebug() << "Hello from thread " << QThread::currentThread();
    Calculate();
    emit meshReady();
}

void MeshChunk::Calculate()
{
    if (!m_isGenerated) {

        m_currentLod = getEstimatedLod();
        /*
        for (int i=0;i<m_currentLod;i++) {
            size/=2;
            m_scale*=2;
        }
*/

        m_data.resize(worldSize*worldSize*worldSize);
        double sum = 0;
        for (int i=0;i<size;i++)
            for (int j=0;j<size;j++)
                for (int k=0;k<size;k++) {
                    auto p = m_pos + QVector3D((i-size/2.0)*m_scale,(j-size/2.0)*m_scale,(k-size/2.0)*m_scale);
                    auto val = WorldGen(p+QVector3D(0,hShift,0));
                    set(i,j,k,val);
                    sum+=val;
                }
        sum/=(double)(size*size*size);
        if (sum-(int)sum!=0.0)
            GenerateMesh();
        else m_ignore = true;

        m_isGenerated = true;
    }
    //    else qDebug() << "ZERO";
    calculateAmbientOcclusion();
    calculateShadow();

}

void MeshChunk::calculateAmbientOcclusion()
{

    for (auto& d : data) {
        //auto p = m_pos + QVector3D((i-size/2.0)*m_scale,(j-size/2.0)*m_scale,(k-size/2.0)*m_scale);
        QVector3D p2 = (((d.position*0.5-m_pos))/m_scale) + 0.5*size*QVector3D(1,1,1) + QVector3D(0.25,0,0.25);
        p2 = QVector3D((int)p2.x(),(int)p2.y(),(int)p2.z());
        //                if (rand()%1000>998)
        //                qDebug() << p2;
        float l = 1.0 -(
                            getReal(p2.x(),p2.y()+1,p2.z()-1)!=0 +
                                                                           getReal(p2.x(),p2.y()+1,p2.z()+1)!=0 +
                                   getReal(p2.x()-1,p2.y()+1,p2.z())!=0 +
                                   getReal(p2.x()+1,p2.y()+1,p2.z())!=0 +
                                   getReal(p2.x()-1,p2.y()+1,p2.z()-1)!=0 +
                                   getReal(p2.x()-1,p2.y()+1,p2.z()+1)!=0 +
                                   getReal(p2.x()-1,p2.y()+1,p2.z()-1)!=0 +
                                   getReal(p2.x()+1,p2.y()+1,p2.z()+1)!=0

                            )*0.25;

        //                        float dist = ((p*2 - d.position-QVector3D(0,-0.5,0)).length()*+0.5)*0.5;
        d.light = QVector3D(1.0, 1.0, 1.0)*(l);
    }

}

void MeshChunk::calculateShadow()
{
    m_lightDir = SData::sdata.s_directionalLight.normalized();
    if (m_currentLod!=0)
        return;
    if (m_ignore)
        return;


    for (auto& d : data) {
        //auto p = m_pos + QVector3D((i-size/2.0)*m_scale,(j-size/2.0)*m_scale,(k-size/2.0)*m_scale);
        QVector3D p = (((d.position*0.5-m_pos))/m_scale) + 0.5*size*QVector3D(1,1,1);
        QVector3D dir = SData::sdata.s_directionalLight.normalized()*m_scale;
        float l = 2.0;
        p+=dir;
        for (int i=0;i<100;i+=2) {
            p+=dir;
            if (getReal(p.x(),p.y(),p.z())!=0) {
                l-= 0.2;
                if (l<0.3)
                    break;
            }
        }

        //                        float dist = ((p*2 - d.position-QVector3D(0,-0.5,0)).length()*+0.5)*0.5;
        d.light = d.light*(l);
    }
}

void MeshChunk::UpdateShadow()
{
    m_shadowTick=(m_shadowTick+1)%1;
    if (m_currentLod!=0)
        return;

    if (m_shadowTick == 0 && ((m_lightDir.normalized()-SData::sdata.s_directionalLight.normalized()).length()>0.05))
    {
//        start();

        QThreadPool::globalInstance()->start(this);
    }
}

void MeshChunk::reGenerateAll()
{
    return;
    size = worldSize;
    m_scale = m_orgScale;
    m_isGenerated = false;
    data.clear();
    indices.clear();
    qDebug() << "Regenerating "<<rand()%100;
    m_isDone = false;
    //start();

}

int MeshChunk::getEstimatedLod()
{
    int lod = 0;
//    if ((m_pos*2-*SData::sdata.camera).length()>160)
  //      lod+=1;
    if ((m_pos*2-*SData::sdata.camera).length()>300)
        lod+=1;
//    if ((m_pos*2-*SData::sdata.camera).length()>200)
  //      lod+=1;
    return lod;
}

int MeshChunk::getChunkIndex(const int scale)
{
    int i = m_pos.x()/m_scale;
    int j = m_pos.y()/m_scale;
    int k = m_pos.z()/m_scale;
    return i*scale*scale + j*scale + k;
}


int MeshChunk::WorldGen(const QVector3D pos)
{
    float s = 0.05;
    float h = sn.noise(pos.z()*s, pos.x()*s)*3;
    h += sn.noise(pos.z()*s*5.21, pos.x()*s*6.42)*0.5;
//    float gh = sn.noise(pos.z()*s/4, pos.x()*s/4)*8;
    float gh = sn.getMultiFractal(pos*s/6.1,1.0, 4,2.0,1.0,2.0,0.0)*8;
    float ls1 = sn.getMultiFractal(pos*s/20.3,1.0, 4,2.0,1.0,2.0,0.0)*8;
    float ls2 = sn.noise(pos.z()*s/15.0, pos.x()*s/16.23)*1.5;
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

    /*
    if (v==1 && trees>0.96) {
        int curHeight = ground-h;

        float stemHeight = sn.noise(pos.z()*s*16.31, pos.y()*s, pos.x()*s*19.41)*2+2;
        // stem
        if(std::pow(trees+0.2,4)>1.9)
        if (curHeight<stemHeight)
            return 3;

//        if (trees>0.50)
            if (curHeight<stemHeight+4 && curHeight>=stemHeight)
                return 1;

    }
    */


    if (ground>h && v!=2)
        return 0;




    return v;
}

void MeshChunk::GenerateMesh()
{
    const int sz = size;
    const float s = m_scale;
    for (int i=0;i<sz;i++)
        for (int j=0;j<sz;j++)
            for (int k=0;k<sz;k++) {

                auto val = get(i,j,k);
                if (val-1==m_type) {
                    QVector3D p = QVector3D((i-size/2.0)*s,(j-size/2.0)*s,(k-size/2.0)*s) + m_pos;

                    bool f1 = get(i,j,k-1)!=val;
                    bool f2 = get(i,j,k+1)!=val;
                    bool f3 = get(i,j+1,k)!=val;
                    bool f4 = get(i,j-1,k)!=val;
                    bool f5 = get(i-1,j,k)!=val;
                    bool f6 = get(i+1,j,k)!=val;

                    if (i==0)
                        f5 = getReal(i-1,j,k)!=val;
                    if (i==sz-1)
                        f6 = getReal(i+1,j,k)!=val;
                    if (j==0)
                        f4 = getReal(i,j-1,k)!=val;
                    if (j==sz-1)
                        f3 = getReal(i,j+1,k)!=val;
                    if (k==0)
                        f1 = getReal(i,j,k-1)!=val;
                    if (k==sz-1)
                        f2 = getReal(i,j,k+1)!=val;

                    if (f1||f2||f3||f4||f5||f6) {

                        auto b = QSharedPointer<MeshBox>(new MeshBox(s,2, f1,f2,f3,f4,f5,f6,p*2));
                        const auto cnt = data.size();
                        for (int i=0;i<b->indices.size();i++)
                            b->indices[i] += cnt;

                        data.append(b->data);
                        indices.append(b->indices);
                    }

                }

            }
}

unsigned char MeshChunk::get(int i, int j, int k)
{
    if (i<0 || i>=size) return 0;
    if (j<0 || j>=size) return 0;
    if (k<0 || k>=size) return 0;
    return m_data[i*size*size + j*size + k];
}

unsigned char MeshChunk::getReal(int i, int j, int k)
{
    auto p = m_pos + QVector3D((i-size/2.0)*m_scale,(j-size/2.0)*m_scale,(k-size/2.0)*m_scale);

    return WorldGen(p+QVector3D(0,hShift,0));
}

void MeshChunk::set(int i, int j, int k, unsigned char d)
{
    const int idx = i*size*size + j*size + k;
    if (idx<0 || idx>m_data.count())
        return;
    m_data[idx] = d;

}

void MeshChunk::finishThread()
{

//    qDebug() << "Building";
    Build();
    m_isDone = true;
    isBuilt = true;
}

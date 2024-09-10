#include "meshchunk.h"
#include "source/engine/sdata.h"
#include <QThreadPool>
#include <QDebug>
#include "source/engine/settings.h"


MeshChunk::MeshChunk(QVector3D pos, int type)
{
    m_isDone = false;
    isBuilt = false;
    m_pos = pos;
    m_type = type;
    //start();
    //    m);
    m_shadowTick = rand()%8;

    m_chunk = ChunkData::s.get(m_pos);

    m_lightDir = SData::sdata.s_directionalLight.normalized();
}



void MeshChunk::Calculate()
{
    GenerateMesh();

    if (m_ignore)
        return;
    //    else qDebug() << "ZERO";
    calculateAmbientOcclusion();


    calculateShadow();

}

void MeshChunk::calculateAmbientOcclusion()
{
    for (auto& d : workData) {
        //auto p = m_pos + QVector3D((i-size/2.0)*m_scale,(j-size/2.0)*m_scale,(k-size/2.0)*m_scale);
        QVector3D p2 = (((d.position*0.5-m_pos))/Chunk::scale)/* + 0.5*Chunk::size*QVector3D(1,1,1)*/ + QVector3D(0.25,0.25,0.25);
        float s = Chunk::scale*Settings::s.occlusionDistanceScale;
        float a = 1.0;
        float s2 = s;
        float h = d.position.y()-Settings::s.waterHeight;
        if (h>0) h=0;
        float l = 1.5 + h*0.5;


        l -= (
                                 a*(m_chunk->getReal(p2.x(),p2.y()+s2,p2.z()-s)!=0) +
                                 a*(m_chunk->getReal(p2.x(),p2.y()+s2,p2.z()+s)!=0) +
                                 a*(m_chunk->getReal(p2.x()-s,p2.y()+s2,p2.z())!=0) +
                                 a*(m_chunk->getReal(p2.x()+s,p2.y()+s2,p2.z())!=0)
/*
                                   m_chunk->getReal(p2.x()-1,p2.y()+1,p2.z()-1)!=0 +
                                   m_chunk->getReal(p2.x()-1,p2.y()+1,p2.z()+1)!=0 +
                                   m_chunk->getReal(p2.x()-1,p2.y()+1,p2.z()-1)!=0 +
                                   m_chunk->getReal(p2.x()+1,p2.y()+1,p2.z()+1)!=0
*/
                         )*Settings::s.occlusionStrengthScale;

        l = std::min(std::max(l, 0.0f), 1.0f);

        d.light = QVector3D(1.0, 1.0, 1.0)*(l);
    }

}

void MeshChunk::calculateShadow()
{
    m_lightDir = SData::sdata.s_directionalLight.normalized();
    //    if (m_currentLod!=0)
    //      return;

    for (auto& d : workData) {
        //auto p = m_pos + QVector3D((i-size/2.0)*m_scale,(j-size/2.0)*m_scale,(k-size/2.0)*m_scale);
        QVector3D p = (((d.position*0.5-m_pos))/Chunk::scale);// + 0.5*QVector3D(1,1.00,1)*size;
        QVector3D dir = m_lightDir*Chunk::scale*2;
        float l = 2.0;
        p+=dir*2;
        for (int i=0;i<Settings::s.shadowSteps;i+=1) {
            p+=dir;
            if (m_chunk->getReal(p.x(),p.y(),p.z())!=0) {
                //                l-= 0.2;
                l*=Settings::s.shadowMultiplier;
                if (l<Settings::s.shadowThreshold)
                    break;
            }
            if (i>8) i+=3;
            if (i>40) i+=8;
            if (i>80) i+=8;
        }
        //                        float dist = ((p*2 - d.position-QVector3D(0,-0.5,0)).length()*+0.5)*0.5;
        d.light = d.light*(l);
    }
}

bool MeshChunkAll::UpdateShadow()
{
   //m_currentLod = getEstimatedLod();
//    return;
    if (isBuildingShadows())
       return false;
    if (m_ignore)
        return false;
    if (!m_isDone)
        return false;
    if (m_meshChunks.count()==0)
        return false;
    if (m_meshChunks[0]->m_shadowsOnly)
        return false;

    for (auto& m : m_meshChunks)
        if (!m->m_isDone)
            return false;
    /*
    m_shadowTick=(m_shadowTick+1)%1;
    if (m_currentLod!=0)
        return;
*/

    if (/*m_shadowTick == 0 && */((m_meshChunks[0]->m_lightDir.normalized()-SData::sdata.s_directionalLight.normalized()).length()>0.10 + (rand()%50)/100.0))
    {

//        qDebug() << (m_meshChunks[0]->m_lightDir)<<SData::sdata.s_directionalLight.normalized() << m_meshChunks[0]->m_isDone;
        for (auto m:m_meshChunks) {
            m->m_shadowsOnly = true;
        }
        return true;


    }
    return false;
}


void MeshChunkAll::finishThread()
{
    if (!m_ignore) {
        for (auto& m: m_meshChunks) {
            m->indices = m->workIndices;
            m->data = m->workData;
            if (!m->m_ignore) {
                m->Build();
                //               qDebug() << m->workData[0].position;
            }
            m->isBuilt = true;
            m->m_shadowsOnly = false;
            m->m_isDone = true;
            m->m_hasVBO = true;
        }
    }

    m_isDone = true;
}

void MeshChunkAll::regenerate()
{

 //    if (ignore())
   //     return;

    m_isDone = false;
     m_ignore = false;
    m_isGenerated = false;
    m_chunk->m_changed = false;
    m_chunk->m_isGenerated = false;
    QThreadPool::globalInstance()->start(this,0);

}

int MeshChunk::getEstimatedLod()
{
    int lod = 0;
    if ((m_pos*2-*SData::sdata.camera).length()>100)
        lod+=1;
    if ((m_pos*2-*SData::sdata.camera).length()>200)
        lod+=1;
    if ((m_pos*2-*SData::sdata.camera).length()>300)
        lod+=1;
    //    if ((m_pos*2-*SData::sdata.camera).length()>200)
    //      lod+=1;
    return lod;
}

int MeshChunk::getChunkIndex(const int scale)
{
    int i = m_pos.x()/Chunk::scale;
    int j = m_pos.y()/Chunk::scale;
    int k = m_pos.z()/Chunk::scale;
    return i*scale*scale + j*scale + k;
}



void MeshChunk::GenerateMesh()
{
    workData.clear();
    workIndices.clear();
    const int sz = Chunk::size;
    const int size = Chunk::size;
    const float s = Chunk::scale;
    m_ignore = false;
    for (int i=0;i<sz;i++)
        for (int j=0;j<sz;j++)
            for (int k=0;k<sz;k++) {

                auto val = m_chunk->get(i,j,k);
                if (val==m_type) {
//                    QVector3D p = QVector3D((i-size/2.0)*s,(j-size/2.0)*s,(k-size/2.0)*s) + m_pos;
                    QVector3D p = QVector3D(i,j,k)*s + m_pos;


                    bool f1 = m_chunk->get(i,j,k-1)!=val;
                    bool f2 = m_chunk->get(i,j,k+1)!=val;
                    bool f3 = m_chunk->get(i,j+1,k)!=val;
                    bool f4 = m_chunk->get(i,j-1,k)!=val;
                    bool f5 = m_chunk->get(i-1,j,k)!=val;
                    bool f6 = m_chunk->get(i+1,j,k)!=val;

                    if (i==0)
                        f5 = m_chunk->getReal(i-1,j,k)!=val;
                    if (i==sz-1)
                        f6 = m_chunk->getReal(i+1,j,k)!=val;
                    if (j==0)
                        f4 = m_chunk->getReal(i,j-1,k)!=val;
                    if (j==sz-1)
                        f3 = m_chunk->getReal(i,j+1,k)!=val;
                    if (k==0)
                        f1 = m_chunk->getReal(i,j,k-1)!=val;
                    if (k==sz-1)
                        f2 = m_chunk->getReal(i,j,k+1)!=val;

                    if (f1||f2||f3||f4||f5||f6) {

                        auto b = QSharedPointer<MeshBox>(new MeshBox(s,2, f1,f2,f3,f4,f5,f6,p*2));
                        const auto cnt = workData.size();
                        for (int i=0;i<b->workIndices.size();i++)
                            b->workIndices[i] += cnt;

                        workData.append(b->workData);
                        workIndices.append(b->workIndices);
                    }

                }

            }
    if (workData.count()==0)
        m_ignore = true;
}
/*
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
*/

MeshChunkAll::MeshChunkAll(QVector3D pos) {
    connect(this, &MeshChunkAll::meshReady, this, &MeshChunkAll::finishThread);
    m_pos = pos;
    m_isDone = false;
}

void MeshChunkAll::Generate()
{
    if (!m_isGenerated) {

        //        m_currentLod = getEstimatedLod();
        /*
        for (int i=0;i<m_currentLod;i++) {
            size/=2;
            m_scale*=2;
        }
*/

        m_chunk = ChunkData::s.get(m_pos);

        if (m_chunk->m_ignore) {
            m_isGenerated = true;
            m_ignore = true;
            return;
        }
        int size = Chunk::size;
        if (!m_chunk->m_isGenerated)  {
            for (int i=0;i<size;i++)
                for (int j=0;j<size;j++)
                    for (int k=0;k<size;k++) {
                        auto p = m_pos + QVector3D(i,j,k)*Chunk::scale;
                        auto val = WorldGen::s.generate(p, true);
                        // if above block is air, set grass
                        if (val==Settings::s.blocks["dirt"]->m_id && m_chunk->getReal(i,j+1,k)==0)
                            val = Settings::s.blocks["grass"]->m_id;
                        if (m_chunk->get(i,j,k)==Chunk::empty)
//                            ChunkData::s.set(p,val);
                            m_chunk->set(i,j,k,val);
                        sum+=val;
                    }
            sum/=(double)(size*size*size);
//            if (sum-(int)sum==0.0) {
            if (sum==0.0) {
//                m_chunk->m_ignore = true;
            }
            m_chunk->m_isGenerated = true;
        }
        m_isGenerated = true;
        m_ignore = m_chunk->m_ignore;
    }

}

bool MeshChunkAll::isBuildingShadows()
{
    for (auto& m : m_meshChunks)
        if (m!=nullptr)
            if (m->m_shadowsOnly)
                return true;

    return false;
}

void MeshChunkAll::run() {


    if (m_isDone) {
        bool regen = false;
        for (auto& m : m_meshChunks) {
            if (m->m_shadowsOnly) {
                m->calculateAmbientOcclusion();
                m->calculateShadow();
                regen = true;

            }
        }
        if (regen)
           emit meshReady();


        return;

    }

    m_isDone = false;
    Generate();

    if (m_ignore) {
        emit meshReady();
        return;
    }

    for (int i=1;i<m_meshChunks.count();i++)
        m_meshChunks[i]->Calculate();
//    qDebug() << "HERRE" <<m_chunk->m_pos;
    emit meshReady();

}

void MeshChunkAll::Setup()
{
    if (m_meshChunks.count()==0)
        m_meshChunks.resize(Settings::s.noMaterials);

    for (int i=0;i<Settings::s.noMaterials;i++) {
         m_meshChunks[i] = QSharedPointer<MeshChunk>(new MeshChunk(m_pos,i));
    }

}

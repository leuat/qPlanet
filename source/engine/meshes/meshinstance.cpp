#include "meshinstance.h"
#include "source/engine/misc/util.h"
#include "source/engine/sdata.h"
MeshInstance::MeshInstance()
{

}

void MeshInstance::Render(QMatrix4x4 projection)
{

    // Set modelview-projection matrix
    if (m_material==nullptr)
        return;
    if (m_mesh==nullptr)
        return;

    m_material->bind(projection * getMV(),m_rotation.normalized().toRotationMatrix());
    //    qDebug() << m_rotation;
    m_mesh->Render(m_material->program);
    //    qDebug() <<m_mesh->children.count();
    for (auto& c:m_mesh->children) {
        if (extraMats.contains(c->name))
            m_material->setDefaults(extraMats[c->name]);

        c->Render(m_material->program);
        if (extraMats.contains(c->name))
            m_material->setDefaults(m_material->mData);

    }

    m_material->release();

    Entity::Render(getMV());

}

MeshChunks::MeshChunks(int size, int sy, float scale, QVector<QSharedPointer<Material>> mat) {
//    connect(this, &MeshChunks::allDone, this, &MeshChunks::finishThread2);
    m_materials.append(mat);
    m_size = size;
    m_scale = scale;
    m_chunks.resize(0);
    m_sizeY = sy;
    //   m_chunks.resize(size*size*size);
    /*    for (int i=0;i<size;i++)
        for (int j=0;j<size;j++)
            for (int k=0;k<size;k++)  {
                QVector3D pos = QVector3D(i-size/2.0, j-size/2.0, k-size/2.0)*scale*MeshChunk::size;

                m_chunks[i*size*size + j*size + k] = QSharedPointer<MeshChunk>(new MeshChunk(pos,scale));
            }
*/
}

void MeshChunks::run()
{
//    SData::sdata.noThreads++;
    ManageChunks();
//    emit allDone();
    m_isDone = true;
//    SData::sdata.noThreads--;
    //  }

}




void MeshChunks::Update()
{
    //   if (!isRunning())
    time+=1;
    if (SData::sdata.noThreads<30)
        RemoveDistantChunks();
    if (m_isDone)
        start();
//    ManageChunks();
   // qDebug() <<SData::sdata.noThreads;

    //    ManageChunks();
}

void MeshChunks::ManageChunks()
{

    qDebug() << SData::sdata.noThreads;
    if (SData::sdata.noThreads>500)
        return;

    const auto size = m_size;
    if (m_cameraPointer==nullptr)
        return;


    //    qDebug() <<m_chunks.size() << (size*size*size);
    //    while (m_chunks.count()<size*size*size) {
    // Find Chunk
    QVector3D cp = *m_cameraPointer + (*m_targetPointer-*m_cameraPointer).normalized()*removalScaleAdd;
    const int step = 1;
    time2 = (time2+1)%step;
//    if (rand()%100>97)
  //      qDebug() << time;
    m_tmpChunks = m_chunks;
    int sx = m_cameraPointer->x()/((float)m_scale*MeshChunk::worldSize*2);
    int sz = m_cameraPointer->z()/((float)m_scale*MeshChunk::worldSize*2);

   {
  //    for (int i=0;i<size;i++)
        int i = time % size;
        for (int j=0;j<m_sizeY;j++)
            for (int k=0;k<size;k+=1)  {
                bool exists = false;
                const QVector3D curPos = QVector3D(i+sx-size/2.0, j-m_sizeY/2.0, k+sz-size/2.0)*m_scale*MeshChunk::worldSize;
                QVector3D pos = curPos*2 -  cp;

                   for (auto& v:m_tmpChunks) {
                    if (v.count()>=1 && v[0]!=nullptr) {
                        if ((v[0]->m_pos-curPos).length()<0.2) {
                            exists = true;
                            if (((v[0]->m_pos-curPos).length() <size*MeshChunk::worldSize*m_scale*0.4))
                                for (int i=0;i<v.count();i++) {
                                    v[i]->UpdateShadow();
          //                          if (v[i]->getEstimatedLod()!=v[i]->m_currentLod)
            //                            m_flaggedForRegen.append(v);
                                }
                            break;
                        }
                    }
                }

                if (m_chunks.size()<size*m_sizeY*size)
                    if (!exists &&  pos.length()<size*MeshChunk::worldSize*m_scale*1.2 && !m_queue.contains(curPos)) {
                    m_queue.append(curPos);

                }
            }
    }




//    qDebug() << m_chunks.count() << add <<removal.count()<< rand()%100 << *m_cameraPointer ;
    //  }
    /*
        for (int i=0;i<size;i++)
        for (int j=0;j<size;j++)
            for (int k=0;k<size;k++)  {

                QVector3D pos = QVector3D(i-size/2.0, j-size/2.0, k-size/2.0)*m_scale*MeshChunk::size;

                m_chunks[i*size*size + j*size + k] = QSharedPointer<MeshChunk>(new MeshChunk(pos,m_scale));
            }

*/

}

void MeshChunks::RemoveDistantChunks()
{
    // Remove far-away objects
    QVector3D cp = *m_cameraPointer + (*m_targetPointer-*m_cameraPointer).normalized()*removalScaleAdd;
    QList<QVector<QSharedPointer<MeshChunk>>> removal;
    for (auto& c : m_chunks) {
        QVector3D pos = c[0]->m_pos*2 -  cp;
        pos.setY(0);

        if (pos.length()>m_size*MeshChunk::worldSize*m_scale*1.2) {
            removal.append(c);
        }
    }

    //    m_chunks.removeAll(removal);

    for (auto& c: removal) {
        if (c[0]->m_isDone)
            m_chunks.removeAll(c);
    }
//    if (removal.count()!=0)
  //      qDebug() << "Removed "<<removal.size();


}

void MeshChunks::Render(QMatrix4x4 projection)
{
    // Set modelview-projection matrix
    finishThread();
    for (int type = 0; type <4; type++ ){
        auto material = m_materials[type];

        if (material==nullptr)
            return;

        material->bind(projection * getMV(),m_rotation.normalized().toRotationMatrix());
        //    qDebug() << m_rotation;
        for (auto& v : m_chunks) {
            if (v.count()!=0)
                if (v[type]!=nullptr)
                if (v[type]->m_isDone)
                    v[type]->Render(material->program);

        }

        material->release();

        Entity::Render(getMV());
    }
}

void MeshChunks::finishThread()
{
    for (auto& v : m_queue) {
        QVector<QSharedPointer<MeshChunk>> chunks;
        for (int type = 0;type<4; type++)
            chunks.append(QSharedPointer<MeshChunk>(new MeshChunk(v,m_scale, type)));
        m_chunks.append(chunks);
    }
    m_queue.clear();


    for (auto& v: m_flaggedForRegen)
        for (int i=0;i<v.count();i++)
            v[i]->reGenerateAll();

    m_flaggedForRegen.clear();
}


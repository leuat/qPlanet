#include "meshinstance.h"
#include "source/engine/misc/util.h"
#include "source/engine/sdata.h"
#include <QThreadPool>
#include <QMutexLocker>

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
    m_mesh->Render(m_material->program);
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

MeshChunks::MeshChunks(int size, int sy, QVector<QSharedPointer<Material>> mat) {
//    connect(this, &MeshChunks::allDone, this, &MeshChunks::finishThread2);
    m_materials.append(mat);
    m_size = size;
    m_chunks.resize(0);
    m_sizeY = sy;
    ChunkData::s.m_chunkSizes = QVector3D(size,sy,size);

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
    return;
//    SData::sdata.noThreads++;
    while (true) {
        if (m_isReady) {
            ManageChunks();
            time+=1;
            m_isReady = false;
        }
    }
//    emit allDone();
    //m_isDone = true;
//    SData::sdata.noThreads--;
    //  }

}




void MeshChunks::Update()
{

}


void MeshChunks::ManageChunks()
{

//    qDebug() << QThreadPool::globalInstance()->activeThreadCount();
//    if (SData::sdata.noThreads>32)
  //      return;

    const auto size = m_size;
    if (m_cameraPointer==nullptr)
        return;


    // Find Chunk
    QVector3D cp = *m_cameraPointer;// + (*m_targetPointer-*m_cameraPointer).normalized()*removalScaleAdd;
    const int step = 1;
    time2 = (time2+1)%step;

//    if (rand()%100>97)
  //      qDebug() << time;
    //m_tmpChunks = m_chunks;
    int sx = m_cameraPointer->x()/((float)Chunk::scale*Chunk::size*2);
    int sz = m_cameraPointer->z()/((float)Chunk::scale*Chunk::size*2);

    if (time%size==0) curY=(curY+1)%m_sizeY;
   {
  //    for (int i=0;i<size;i++)
        int i = time % size;
//      int j = (m_sizeY-curY-1)%m_sizeY;
//  for (int i=time&(size/2);i<time%size;i+=size/2)
        for (int j=0;j<m_sizeY;j++)
            for (int k=0;k<size;k+=1)  {
                bool exists = false;
                const QVector3D curPos = QVector3D(i+sx-size/2.0, j-m_sizeY/2.0, k+sz-size/2.0)*Chunk::scale*Chunk::size;
                QVector3D pos = curPos*2 -  cp;
                if (m_ignoreList.contains(curPos))
                    continue;

                   for (auto& v:m_chunks) {
                       {
                        if ((v->m_pos-curPos).length()<0.2) {
                            exists = true;
                            if (((v->m_pos-curPos).length() <size*Chunk::size*Chunk::scale*0.4))
                                if (v->UpdateShadow() && !m_updateQueue.contains(v))
                                    m_updateQueue.append(v);

                            if (v->m_chunk!=nullptr && v->m_isDone)
                            if (v->m_chunk->m_changed) {
                                v->regenerate();
                            }
                            break;
                        }
                    }
                }
                if (m_chunks.size()<size*m_sizeY*size)
                    if (!exists &&  pos.length()<size*Chunk::size*Chunk::scale*1.2 && !m_queue.contains(curPos)) {
                        m_queue.append(curPos);

                }
            }
    }

//   qDebug() << m_queue.count() << m_chunks.count() << m_ignoreList.count() << ChunkData::s.m_data.count();


}

void MeshChunks::RemoveDistantChunks()
{
    // Remove far-away objects
    QVector3D cp = *m_cameraPointer;// + (*m_targetPointer-*m_cameraPointer).normalized()*removalScaleAdd;
    QList<QSharedPointer<MeshChunkAll>> removal;
    for (auto& c : m_chunks) {
        QVector3D pos = c->m_pos*2 - cp;//*2 -  cp;
        pos.setY(0);


        if (c->m_ignore) {
            if (!m_ignoreList.contains(c->m_pos))
               m_ignoreList.append(c->m_pos);
        }
        if (!c->m_ignore && m_ignoreList.contains(c->m_pos)) {
            // Been added back to the fold
            m_ignoreList.removeAll(c->m_pos);
        }
        if (pos.length()>m_size*Chunk::size*Chunk::scale*1.2 || m_ignoreList.contains(c->m_pos)) {
            removal.append(c);
        }
    }
    //    m_chunks.removeAll(removal);
    for (auto& c: removal)
        if (c->m_isDone) {
            m_chunks.removeAll(c);
            ChunkData::s.remove(c->m_pos);
        }
    //qDebug() << m_chunks.size() << ChunkData::s.m_data.count() << m_queue.size() << m_renderChunks.count() << m_ignoreList.count() << removal.size() << QThreadPool::globalInstance()->activeThreadCount();


}


void MeshChunks::Render(QMatrix4x4 projection)
{
    // Set modelview-projection matrix
//    if (m_isReady == false) {
  //  }
//    finishThread();
    UpdateAll();

    m_renderChunks = m_chunks;

    for (int type = 1; type <Settings::s.noMaterials; type++ ) {
        auto material = m_materials[type];

        if (material==nullptr)
            return;

        material->bind(projection * getMV(),m_rotation.normalized().toRotationMatrix());
        //    qDebug() << m_rotation;
        int ign = 0;

        for (auto& v : m_renderChunks) {
            if (v->m_ignore)
                continue;

//            if (v->m_meshChunks.count()!=4)
  //              continue;

            auto ch = v->m_meshChunks[type];
            if (ch->m_ignore || !ch->m_hasVBO)
                continue;
//            if (ch->m_isDone==false)
  //              continue;
//            if (ch->ignore())
  //              ign++;

            if (ch!=nullptr)
                    ch->Render(material->program);


        }

        material->release();
//        if (rand()%100>90)
  //          qDebug() << ign/(float)m_chunks.count() << m_chunks.count() << QThreadPool::globalInstance()->activeThreadCount();

//        Entity::Render(getMV());
//        qDebug() << ign/(float)m_chunks.count() << m_chunks.count();
    }

  //  qDebug() << m_ignoreList.count() <<m_chunks.count() << m_tmpChunks.count() << m_renderChunks.count() << m_queue.count();
    if (m_isReady == false)
        m_isReady = true;





}

void MeshChunks::finishThread()
{
    ManageQueue();



}


void MeshChunks::ManagedFlaggedForRegenChunks()
{
    /*
    for (auto& v: m_flaggedForRegen)
        for (int i=0;i<v.count();i++) {
            v[i]->reGenerateAll();

        }

    m_flaggedForRegen.clear();
*/


}

void MeshChunks::ManageQueue()
{

    for (auto& v : m_queue) {
        m_chunks.append(QSharedPointer<MeshChunkAll>(new MeshChunkAll(v)));
        m_chunks.last()->setAutoDelete(false);
        m_chunks.last()->Setup();
        QThreadPool::globalInstance()->start(m_chunks.last().get(),1);
//        m_chunks.last()->run();
    }
    m_queue.clear();

    for (auto& v : m_updateQueue) {
        v->setAutoDelete(false);
        QThreadPool::globalInstance()->start(v.get(),1);
        //        m_chunks.last()->run();
    }
    m_updateQueue.clear();


}

void MeshChunks::UpdateAll()
{

    ChunkData::s.Update();

//    qDebug() << QThreadPool::globalInstance()->activeThreadCount();
    if (QThreadPool::globalInstance()->activeThreadCount()==0)
       RemoveDistantChunks();

    m_isReady = false;
   // RemoveIgnoredChunks();
    ManageChunks();
    ManageQueue();
    //    finishThread();

    time+=1;
    /*
    if (!m_isRunning) {
    //    run();
        m_isRunning = true;
        start();
    }
*/

}

#ifndef MESHINSTANCE_H
#define MESHINSTANCE_H

#include "mesh.h"
#include "meshchunk.h"
#include "source/engine/entity.h"
#include <QVector>
#include <QString>
#include <QMap>
#include <QThread>

class MeshInstance : public Entity
{
public:
    QMap<QString, MaterialData> extraMats;
    MeshInstance();
    Mesh* m_mesh = nullptr;
    Material* m_material = nullptr;


    void Render(QMatrix4x4 proj) override;

};


class MeshChunks :  public QThread,public Entity {

    Q_OBJECT
public:

    QVector<QSharedPointer<MeshChunkAll>> m_chunks;
    QVector<QSharedPointer<MeshChunkAll>> m_tmpChunks;
    QVector<QSharedPointer<MeshChunkAll>> m_renderChunks;
    QVector<QSharedPointer<MeshChunkAll>> m_flaggedForRegen;
    QVector<QVector3D> m_queue;
    QVector<QSharedPointer<MeshChunkAll>> m_updateQueue;
    QVector<QVector3D> m_ignoreList;
    int m_size, m_sizeY;
    bool m_isRunning = false;
    float m_scale;
    int curY = 0;
    std::atomic<bool> m_isReady = true;
    QVector<QSharedPointer<Material>> m_materials;
    int time = 0;
    int time2 = 0;
    bool m_isDone = true;
    const int removalScaleAdd = 25;
    MeshChunks(int size, int sizey, float scale, QVector<QSharedPointer<Material>> mat);
    void run() override;

    void Update() override;
    void ManageChunks();
    void RemoveDistantChunks();
    void RemoveIgnoredChunks();
    void ManagedFlaggedForRegenChunks();
    void ManageQueue();
    void UpdateAll();
    void Render(QMatrix4x4 proj) override;
public slots:
    void finishThread();
signals:
    void allDone();
};

#endif // MESHINSTANCE_H

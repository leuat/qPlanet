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


class MeshChunks :  public QThread, public Entity {

    Q_OBJECT
public:

    QVector<QVector<QSharedPointer<MeshChunk>>> m_chunks;
    QVector<QVector<QSharedPointer<MeshChunk>>> m_tmpChunks;
    QVector<QVector<QSharedPointer<MeshChunk>>> m_flaggedForRegen;
    QVector<QVector3D> m_queue;
    int m_size, m_sizeY;
    float m_scale;
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
    void Render(QMatrix4x4 proj) override;
public slots:
    void finishThread();
signals:
    void allDone();
};

#endif // MESHINSTANCE_H

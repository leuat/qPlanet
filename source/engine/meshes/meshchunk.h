#ifndef MESHCHUNK_H
#define MESHCHUNK_H

#include "mesh.h"
#include <QVector>
#include <QVector3D>
#include "source/engine/misc/SimplexNoise.h"
#include <QThread>
#include <QRunnable>
#include <QMutex>
#include "source/engine/chunkdata.h"



class MeshChunk : public Mesh
{
public:
    MeshChunk(QVector3D pos, int type);
    QVector3D m_pos;
    QSharedPointer<Chunk> m_chunk;
    bool m_ignore = false;
    bool m_shadowsOnly = false;

    int m_type = 0;
    int m_noChunks = 0;
    float m_localScale = 1;
    bool m_isDone = false;
    bool m_isGenerated = false;
    int m_shadowTick = 0;
    int m_currentLod = 1;
    QVector3D m_lightDir;
    int getChunkIndex(const int scale);
    void Calculate();
    void calculateAmbientOcclusion();
    void calculateShadow();
    void reGenerateAll();
    int getEstimatedLod();


    void GenerateMesh();
    /*
    unsigned char get(int i,int j, int k);
    unsigned char getReal(int i,int j, int k);
    void set(int i,int j, int k, unsigned char d);
    */

};

class MeshChunkAll : public QObject, public QRunnable {
    Q_OBJECT
public:

    QVector<QSharedPointer<MeshChunk>> m_meshChunks;
    QSharedPointer<Chunk> m_chunk;
    QVector3D m_pos;
    double sum = 0;
    MeshChunkAll(QVector3D pos);

    void Generate();
    bool m_isDone = false;
    bool m_ignore = false;
    bool m_isGenerated = false;
    bool isBuildingShadows();
    void run() override;
    void Setup();

    bool UpdateShadow();

public slots:
    void finishThread();

signals:
    void meshReady();

};



#endif // MESHCHUNK_H

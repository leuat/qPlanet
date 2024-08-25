#ifndef MESHCHUNK_H
#define MESHCHUNK_H

#include "mesh.h"
#include <QVector>
#include <QVector3D>
#include "source/engine/misc/SimplexNoise.h"
#include <QThread>
#include <QMutex>

class MeshChunk : public QThread, public Mesh
{
    Q_OBJECT
public:
    MeshChunk(QVector3D pos, float scale, int type);
    int size = 16;
    static const int worldSize = 12;
    static SimplexNoise sn;
    QVector3D m_pos;
    int m_type = 0;
    float m_orgScale;
    float m_scale = 1;
    float m_localScale = 1;
    bool m_isDone = false;
    bool m_isGenerated = false;
    int m_shadowTick = 0;
    int m_currentLod = 1;
    QVector3D m_lightDir;
    int getChunkIndex(const int scale);
    QVector<unsigned char> m_data;
    void run() override;
    void calculateAmbientOcclusion();
    void calculateShadow();
    void UpdateShadow();
    void reGenerateAll();
    int getEstimatedLod();


    int WorldGen(const QVector3D pos);

    void Generate();
    unsigned char get(int i,int j, int k);
    unsigned char getReal(int i,int j, int k);
    void set(int i,int j, int k, unsigned char d);

public slots:
    void finishThread();

signals:
    void meshReady();
};



#endif // MESHCHUNK_H

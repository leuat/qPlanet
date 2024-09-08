#ifndef CHUNKDATA_H
#define CHUNKDATA_H

#include <QMap>
#include <QVector>
#include "source/engine/misc/SimplexNoise.h"
#include <QVector3D>
#include <QSharedPointer>
#include <QMutex>
#include "settings.h"

class ChunkEntity {
public:
    QVector3D index;
    ChunkEntity(QVector3D idx):index(idx) {}
    virtual void Grow() {}
};


class ChunkTree : public ChunkEntity {
public:
    ChunkTree(QVector3D idx):ChunkEntity(idx) {}
    void Grow() override;
};


class WorldGen {
public:
    SimplexNoise sn;

    static WorldGen s;

//    static const int hShift = -12;
    static const int hShift = -12;

    int generate(const QVector3D pos, bool init);

};


class Chunk {
public:
    Chunk(QVector3D index, QVector3D pos);
    Chunk();

    static const int size = Settings::chunkSize;
    static float scale;
    static const int empty = 65535;

    bool m_isGenerated = false;
    bool m_ignore = false;
    bool m_changed = false;
    static int m_noChunks;

    QVector3D m_index, m_pos;
    QVector<unsigned short> m_data;

    unsigned short get(int i,int j, int k);
    unsigned short getReal(int i,int j, int k);
    void set(int i,int j, int k, unsigned short d);


};


class ChunkData
{
public:
    static ChunkData s;

    QMap<int,QSharedPointer<Chunk>> m_data;
    static QVector3D m_chunkSizes;
    QVector<QSharedPointer<ChunkEntity>> m_newEntities;
    QVector<QVector3D> m_newEntitiesPos;
    QMutex mut;
    static int idxtoInt(QVector3D idx);
    void remove(const QVector3D pos);
    QSharedPointer<Chunk> get(QVector3D pos);
    void set(QVector3D pos, unsigned short val);
    void Update();
    bool m_first = true;

    ChunkData();
};

#endif // CHUNKDATA_H

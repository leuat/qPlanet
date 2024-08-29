#ifndef CHUNKDATA_H
#define CHUNKDATA_H

#include <QMap>
#include <QVector>
#include "source/engine/misc/SimplexNoise.h"
#include <QVector3D>
#include <QSharedPointer>


class WorldGen {
public:
    SimplexNoise sn;

    static WorldGen s;

    static const int hShift = -12;

    int generate(const QVector3D pos);

};


class Chunk {
public:
    Chunk(QVector3D index, QVector3D pos);
    Chunk();

    static const int size = 32;
    static float scale;

    bool m_isGenerated = false;
    bool m_ignore = false;

    static int m_noChunks;

    QVector3D m_index, m_pos;
    QVector<unsigned short> m_data;

    unsigned short get(int i,int j, int k);
    unsigned short getReal(int i,int j, int k);
    void set(int i,int j, int k, unsigned char d);


};


class ChunkData
{
public:
    static ChunkData s;
    QMap<int,QSharedPointer<Chunk>> m_data;
    static QVector3D m_chunkSizes;
    static int idxtoInt(QVector3D idx);
    void remove(const QVector3D pos);
    QSharedPointer<Chunk> get(QVector3D pos);


    ChunkData();
};

#endif // CHUNKDATA_H

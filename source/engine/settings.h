#ifndef SETTINGS_H
#define SETTINGS_H

#include <QJsonObject>
#include <QString>
#include <QJsonDocument>
#include <QVector3D>

class BlockType {
public:
    BlockType() {}
    BlockType(QString material, QString name, QString texture, int id, QVector3D color):m_material(material),m_name(name),m_texture(texture),m_id(id), m_color(color) {

    }
    QString m_name, m_material, m_texture;
    int m_id;
    QVector3D m_color;
};

class Settings
{
public:
    Settings();

    static Settings s;

    QMap<QString,QSharedPointer<BlockType>> blocks;

    QVector3D toQVector3D(QJsonValue jv);

    // Lighting settings
    float occlusionDistanceScale = 2.0;
    float occlusionStrengthScale = 0.55;


    int shadowSteps = 200;
    float shadowMultiplier = 0.9;
    float shadowThreshold = 0.10;


    int noMaterials = 5;
    float waterHeight = -25.9;
    int waterSize = 512;
    int waterGridSize = 256;

    float cloudHeight = 100.0;

    bool hasWater = true;
    bool hasClouds = false;

    // Landscape chunk settings
    static const int chunkSize = 32;
    int worldSizeXZ = 16;
    int worldSizeY = 6;
    float blockScale = 0.2;


    double getDouble(QString a, QString b, QString c);
    void Load(QString file);


private:

    void loadBlocks();
    QJsonDocument m_json;


};

#endif // SETTINGS_H

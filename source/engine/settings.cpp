#include "settings.h"
#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>
Settings Settings::s;

Settings::Settings() {

}

QVector3D Settings::toQVector3D(QJsonValue jv)
{
    auto v = jv.toArray();
    return QVector3D(v[0].toDouble(),v[1].toDouble(), v[2].toDouble());
}

double Settings::getDouble(QString a,QString b="",QString c="")
{
    if (b=="" && c =="") {
        if (const QJsonValue v = m_json[a]; v.isDouble())  {
            return v.toDouble();
        }
    }
    else
        if (c =="") {
            if (const QJsonValue v = m_json[a][b]; v.isDouble())  {
                return v.toDouble();
            }
        }
        else
            if (const QJsonValue v = m_json[a][b][c]; v.isDouble())  {
                return v.toDouble();
            }

    qDebug() << "Incorrect settings value: "<<a<<b<<c;
    exit(1);

}

void Settings::Load(QString file)
{
    QFile jsonFile(file);
    jsonFile.open(QIODevice::ReadOnly);
    QByteArray data = jsonFile.readAll();
    m_json = QJsonDocument::fromJson(data);

    const QString ws = "worldSettings";
    const QString ls = "light";
    const QString w = "world";

    shadowMultiplier = getDouble(ws,ls,"shadowMultiplier");
    shadowSteps = getDouble(ws,ls,"shadowSteps");
    shadowThreshold = getDouble(ws,ls, "shadowThreshold");
    occlusionDistanceScale = getDouble(ws,ls,"occlusionDistanceScale");
    occlusionStrengthScale = getDouble(ws,ls, "occlusionStrengthScale");

    worldSizeXZ = getDouble(ws,w,"worldSizeXZ");
    worldSizeY = getDouble(ws,w,"worldSizeY");
    blockScale = getDouble(ws,w,"blockScale");
    loadBlocks();

}

void Settings::loadBlocks()
{
    auto bts = m_json["blocks"].toArray();
    noMaterials = bts.count(); // add air
    for (const QJsonValue &bt : bts) {
        blocks[bt["name"].toString()] = QSharedPointer<BlockType>(new BlockType(
            bt["material"].toString(),
            bt["name"].toString(),
            bt["texture"].toString(),
            bt["id"].toInt(),
            toQVector3D(bt["color"])
            ));
    }

}

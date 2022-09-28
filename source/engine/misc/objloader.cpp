#include "objloader.h"

ObjLoader::ObjLoader()
{

}

void ObjLoader::Load(QString fn) {
    m_filename = fn;
    QFile f(fn);
    f.open(QFile::ReadOnly| QFile::Text);
    QString s = f.readAll();

    m_data = s.split("\n");
    f.close();
}

void ObjLoader::Parse()
{
    QString curGrp = defaultGroup;
    groups.clear();
    groups[curGrp] = QSharedPointer<ObjGroup>(new ObjGroup());
    int curVidx = 0;
    int curUVidx = 0;
    int curNidx = 0;
    int curVAdd = 0;
    int curUVAdd = 0;
    int curNAdd = 0;
    try {
        for (QString l: m_data) {
            QStringList lst = l.trimmed().simplified().toLower().split(" ");

            auto g = groups[curGrp];

            if (lst.count()==0)
                continue;
            if (lst[0]=="g") {
 /*               curVidx += curVAdd;
                curVAdd = 0;
                curUVidx += curUVAdd;
                curUVAdd=0;
                curNidx += curNAdd;
                curNAdd=0;
*/
                QString n = lst[1];
                if (!groups.contains(n))
                    groups[n] = QSharedPointer<ObjGroup>(new ObjGroup());;
                curGrp = n;
            }
            if (lst[0]=="v") {
                g->m_vertices.append(QVector3D(lst[1].toFloat(),lst[2].toFloat(),lst[3].toFloat()));
                curVAdd+=1;
            }
            if (lst[0]=="vt") {
                g->m_tx.append(QVector2D(lst[1].toFloat(),lst[2].toFloat()));
                g->hasUV = true;
                curUVAdd+=1;
            }
            if (lst[0]=="vn") {
                g->m_normals.append(QVector3D(lst[1].toFloat(),lst[2].toFloat(),lst[3].toFloat()));
                g->hasNormals = true;
                curNAdd+=1;
            }

            if (lst[0]=="f") {
                QVector<int> ilst;
                QVector<int> nlst;
                QVector<int> vtlst;
                for (int i=1;i<lst.count();i++) {
                    if (lst[i].contains("/")) {
                        auto l = lst[i].split("/");
                        int v = l[0].toInt()-curVidx;
                        if (v<0) {
                            qDebug() << "ERROR Vidx<0 on" <<curGrp << " " <<l[0].toInt() <<curVidx;
                            exit(1);
                        }
                        ilst.append(v);

                        if (g->hasUV || l.count()>1) {
                            vtlst.append(l[1].toInt()-curUVidx);
                            g->hasUV = true;

                        }
                        if (g->hasNormals || l.count()>2) {
                            nlst.append(l[2].toInt()-curNidx);
                            g->hasNormals = true;
                        }
                    }
                    else {
                        ilst.append(lst[i].toInt()-curVidx);
                    }
                }
                if (ilst.count()==3) {
                    Face f;
                    f.v1 = ilst[0]-1;
                    f.v2 = ilst[1]-1;
                    f.v3 = ilst[2]-1;
                    if (g->hasNormals) {
//                        qDebug() << nlst;
                        f.n1 = nlst[0]-1;
                        f.n2 = nlst[1]-1;
                        f.n3 = nlst[2]-1;
                    }
                    if (g->hasUV) {
                        f.vt1 = vtlst[0]-1;
                        f.vt2 = vtlst[1]-1;
                        f.vt3 = vtlst[2]-1;
                    }
                    g->m_faces.append(f);

                }


                if (ilst.count()==4) {
                    Face f;
                    f.v1 = ilst[0]-1;
                    f.v2 = ilst[1]-1;
                    f.v3 = ilst[2]-1;

                    if (g->hasNormals) {
                        f.n1 = nlst[0]-1;
                        f.n2 = nlst[1]-1;
                        f.n3 = nlst[2]-1;
                    }
                    if (g->hasUV) {
                        f.vt1 = vtlst[0]-1;
                        f.vt2 = vtlst[1]-1;
                        f.vt3 = vtlst[2]-1;
                    }

                    g->m_faces.append(f);


                    Face f2;
                    f2.v1 = ilst[0]-1;
                    f2.v2 = ilst[2]-1;
                    f2.v3 = ilst[3]-1;
                    if (g->hasNormals) {
                        f2.n1 = nlst[0]-1;
                        f2.n2 = nlst[2]-1;
                        f2.n3 = nlst[3]-1;
                    }
                    if (g->hasUV) {

                        f2.vt1 = vtlst[0]-1;
                        f2.vt2 = vtlst[2]-1;
                        f2.vt3 = vtlst[3]-1;
                    }
                    g->m_faces.append(f2);
                }


                //            qDebug() << f.v1 << f.v2 << f.v3;
                //qDebug() << "Appending";
                //            m_faces.append(QVector3D(lst[1].toFloat(),lst[2].toFloat(),lst[3].toFloat()));
            }
        }
    } catch (const std::exception& e) {
        throw QString("Error during parsing : " + QString(e.what()));
    }
}


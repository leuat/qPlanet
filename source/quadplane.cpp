#include "quadplane.h"
//#include <omp.h>

void QuadPlane::generatePlane(QVector3D p1, QVector3D p2, QVector3D p3, QVector3D p4, int n, PlanetData &pd) {
    data.clear();
    indices.clear();
    QVector3D dx = (p2-p1)/(n-1);
    QVector3D dy = (p4-p1)/(n-1);
    //var vloc = PoolVector3Array()
 //   int nt = omp_get_num_threads();
    data.resize(n*n);
//#pragma omp parallel for
    for (int i=0;i<n;i++) {
        for (int j=0;j<n;j++) {
            auto v = p1 + dx*i + dy*j;
            v = v.normalized();
            v = pd.get_surface_vector(v);
            VertexData vd;
            vd.normal = pd.get_surface_normal(v);
//            vd.normal = v.normalized();

            vd.position = v;
            vd.texCoord = QVector2D(i,j);
//            data.append(vd);
            data[i*n + j] = vd;
        }
    }
    int vcount = 0;
    for (int i=0;i<n-0;i++) {

        for (int j=0;j<n-0;j++) {
            indices.append(vcount+i*n+j);
            indices.append(vcount+i*n+j+1);
            indices.append(vcount+(i+1)*n+j);

            indices.append(vcount+(i+1)*n+j);
            indices.append(vcount+i*n+j+1);
            indices.append(vcount+(i+1)*n+(j+1));
        }
    }
    Build();
}



QuadPlane::QuadPlane()
{
    //initializeOpenGLFunctions();
    // Generate 2 VBOs
    arrayBuf.create();
    indexBuf.create();

}

QuadPlane::~QuadPlane() {
    arrayBuf.destroy();
    indexBuf.destroy();

}

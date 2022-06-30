#include "planetdata.h"
#include "source/qnoise.h"
#include <math.h>
PlanetData::PlanetData()
{

}

double PlanetData::get_surface(QVector3D v) {
    double h = sn.getMultiFractal(v,scale,5,lacunarity,offset,gain,0)- 0.02;
    h = fmax(0.0,h);
    return h;
//    return sn.noise(v.x()*scale,v.y()*scale,v.z()*scale);
//    return QNoise::noise(QVector3D(v.x()*scale,v.y()*scale,v.z()*scale));
}

QVector3D PlanetData::get_surface_vector(QVector3D v) {
    v = v.normalized();
    auto s = get_surface(v);
    return v*(radius+s*heightScale);
}

QVector3D PlanetData::get_surface_normal(QVector3D v) {

    int N = 6;
    double s = 0.01;
    double h = 1;
    QVector3D norm = v*0;
    v = get_surface_vector(v);
    QVector3D t = QVector3D::crossProduct(QVector3D(0,0,1),v).normalized();
    auto bn = QVector3D::crossProduct(t,v).normalized();
    auto prev = QVector3D(0,0,0);
    for (int i=0;i<N;i++) {
            double x = cos(i/float(N-1)*3.14159265*2);
            double y = sin(i/float(N-1)*3.14159265*2);
            auto np = (v + (x * t + y * bn) * s).normalized();

            auto newPos = get_surface_vector(np);

            if (i>0)
                norm+=QVector3D::crossProduct(v-newPos,v-prev).normalized();

            prev = newPos;
    }
    return norm.normalized();
}

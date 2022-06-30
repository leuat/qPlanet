#ifndef QNOISE_H
#define QNOISE_H

#include <math.h>
#include <QVector3D>

class QNoise
{
public:
    QNoise();

    static float lerp(float a, float b, float w) {
        //return Mathf.Lerp(a,b,c);
          return a + w*(b-a);

    }

    static QVector3D frac(QVector3D a) {
              return QVector3D(a.x() - floor(a.x()),a.y() - floor(a.y()), a.z() - floor(a.z()));
          }

    static float frac(float a) {
        return a - floor(a);
    }

    static float iqhash(float n)
         {
 //			return (float)frac((double)System.Math.Sin(n)*753.5453123);
             return frac(sin(n) * (double)720.7535453123f);
         }
    static float noise(QVector3D x)
    {
    // The noise function returns a value in the range -1.0f -> 1.0f
        QVector3D p = QVector3D(floor(x.x()),floor(x.y()),floor(x.z()));
        QVector3D f = frac(x);
        f.setX(f.x()*f.x()*(3.0f - 2.0f*f.x()));
        f.setY(f.y()*f.y()*(3.0f - 2.0f*f.y()));
        f.setZ(f.z()*f.z()*(3.0f - 2.0f*f.z()));


        float n = (p.x() + p.y() * 57.0f + 113.0f * p.z());

        return lerp(lerp(lerp( iqhash(n+  0.0f), iqhash(n+  1.0f),f.x()),
                   lerp( iqhash(n+57.0f), iqhash(n+58.0f),f.x()),f.y()),
               lerp(lerp( iqhash(n+113.0f), iqhash(n+114.0f),f.x()),
                   lerp( iqhash(n+170.0f), iqhash(n+171.0f),f.x()),f.y()),f.z());


    }



    static float getMultiFractal(QVector3D p, float frequency, int octaves, float lacunarity, float offs, float gain, float initialO ) {

                float value = 0.0f;
                float weight = 1.0f;
                float w = -0.5;
                QVector3D vt = p * frequency;
                float f = 1;
                for (float octave = 0; octave < octaves; octave++)
                {
                     float signal = initialO + noise(vt);//perlinNoise2dSeamlessRaw(frequency, vt.x, vt.z,0,0,0,0);//   Mathf.PerlinNoise(vt.x, vt.z);

                    // Make the ridges.
                    signal = abs(signal);
                    signal = offs - signal;


                    signal *= signal;

                    signal *= weight;
                    weight = signal * gain;
                    weight = fmax(fmin(weight, 1),0);

                    value += (signal * pow(f, w));
                    vt = vt * lacunarity;
                    //frequency *= lacunarity;
                    f *= lacunarity;
                }
                return value;
            }

};

#endif // QNOISE_H

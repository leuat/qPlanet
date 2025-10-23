#ifndef SHAPE_H
#define SHAPE_H

#include <QVector2D>
#include <QVector>
#include <QColor>
#include <QMap>
#include <QHash>
#include <QSharedPointer>

class Shape
{
public:
    Shape();
    double RandomGaussian (double mu, double sigma);
    QVector<QVector2D> m_points;
    QVector<float> m_dirWeights;
    QVector2D m_center = QVector2D(0,0);
    int m_thickness = 1;
    int m_drawMax = 0;
    float m_timeDelta = 0;
    float m_resultDelta = 0;


    static QVector<QSharedPointer<Shape>> s_tempResultList;


    float getShapeBoundingSize();

    virtual QString type() { return "Shape";}

    QHash<QPoint,int> m_directionGroups;

    QColor m_col = QColor(0,255,255,255);
    QVector<float> m_parameters;
    virtual void Generate() {}
    virtual void Render();
    virtual float chiSquare(Shape& other);
    void Transform(float scale, float aspect, float angle);
    void calculateDirectionGroups(float scale);
    int calculateDirectionGroups2(float scale);
    void cleanup();
    QVector2D getPoint(float t);
//    virtual float getMinimumChisqBrute(Shape& other, QVector<float>& results) {}
    virtual float getMinimumChisqGD(Shape& other, QVector<float>& results);
    QVector2D getSmoothedPoint(int pos, int dist);
    void centerOrient();
    void CreateSphere(int N);
    void enforceEqualDistance(float dist);
    void generateAccelerationGraph(Shape& result);
    float calculateRoundness();

};

class ShapeRectangle: public Shape {
public:
    ShapeRectangle() {}
    QString type() override { return "Rectangle";}

    void Generate() override;
    float getMinimumChisqGD(Shape &other, QVector<float> &results) override;



};

class ShapeCircle: public Shape {
public:
    ShapeCircle() {}
    QString type() override { return "Circle";}
    void Generate() override;
};

class ShapeTriangle: public Shape {
public:
    ShapeTriangle() {}
    QString type() override { return "Triangle";}

    void Generate() override;
};

class ShapeMoon: public Shape {
public:
    ShapeMoon() {}
    QString type() override { return "Moon";}

    void Generate() override;
};

class ShapeHeart: public Shape {
public:
    ShapeHeart() {}
    QString type() override { return "Heart";}
    void Generate() override;
};


class FitShape {
public:
    static bool find(Shape& other,Shape& result, float& delta);
    static bool smooth(Shape& other,Shape& result, Shape& graph, float& delta);
    static bool smooth2(Shape& other,Shape& result, Shape& graph, float& delta);
};


#endif // SHAPE_H

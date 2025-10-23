#include "shape.h"
#include "imgui.h"
#include <QDebug>
#include "source/spline.h"

Shape::Shape() {}


QVector<QSharedPointer<Shape>> Shape::s_tempResultList;


double Shape::RandomGaussian(double mu, double sigma)
{
    double U1, U2, W, mult;
    static double X1, X2;
    static int call = 0;

    if (call == 1)
    {
        call = !call;
        return (mu + sigma * (double) X2);
    }

    do
    {
        U1 = -1 + ((double) rand () / RAND_MAX) * 2;
        U2 = -1 + ((double) rand () / RAND_MAX) * 2;
        W = pow (U1, 2) + pow (U2, 2);
    }
    while (W >= 1 || W == 0);

    mult = sqrt ((-2 * log (W)) / W);
    X1 = U1 * mult;
    X2 = U2 * mult;

    call = !call;

    return (mu + sigma * (double) X1);
}

float Shape::getShapeBoundingSize()
{
    QVector2D mi = QVector2D(1E30, 1E30);
    QVector2D ma = QVector2D(-1E30, -1E30);
    for (auto& p: m_points) {
        mi.setX( std::min(mi.x(),p.x()));
        mi.setY( std::min(mi.y(),p.y()));
        ma.setX( std::max(ma.x(),p.x()));
        ma.setY( std::max(ma.y(),p.y()));
    }
    return (mi-ma).length();
}

void Shape::Render() {

    auto l = ImGui::GetWindowDrawList();
    for (int i=0;i<m_points.size()-m_drawMax;i++) {

        QVector2D p1 = m_points[i]+m_center;
        QVector2D p2 = m_points[(i+1)%m_points.size()]+m_center;

        l->AddLine(ImVec2(p1.x(),p1.y()),ImVec2(p2.x(),p2.y()),m_col.rgb(), m_thickness);

    }

}

float Shape::chiSquare(Shape &other)
{
    float chisqA = 0;
    float chisqB = 0;
    int N = 100;
    float closest = 0;
    other.enforceEqualDistance(5);
    float minDistance = 1E15;
    for (int i=0;i<N;i++) {
        float t = i/(float)N;
        float d = (getPoint(t)-other.getPoint(0)).lengthSquared();
        if (d<minDistance) {
            minDistance = d;
            closest = t;
        }
    }
    m_timeDelta = closest;
    for (int i=0;i<N;i++) {
        float t = i/(float)N;
        float t2 = 1.0-t-1.0/(float)N;
        float ct = fmod(t+closest,1.0);
        float ct2 = fmod(t2+closest+1.0,1.0);
        chisqA += (getPoint(ct)-other.getPoint(t)).lengthSquared();
        chisqB += (getPoint(ct2)-other.getPoint(t)).lengthSquared();
    }
    chisqA/=(float)N;
    chisqB/=(float)N;
    return std::min(chisqA, chisqB);
    //    return chisq / (float)N;


}

void Shape::Transform(float scale, float aspect, float theta)
{
    for (int i=0;i<m_points.size();i++) {
        m_points[i] = m_points[i]*QVector2D(scale*aspect,scale);
        // rotate
        float x = cos(theta)*m_points[i].x() - sin(theta)*m_points[i].y();
        float y = sin(theta)*m_points[i].x() + cos(theta)*m_points[i].y();
        m_points[i] = QVector2D(x,y);
    }

}



QVector2D Shape::getPoint(float t)
{
    if (m_points.size()==0)
        return m_center;
    float pos = m_points.size()*t;
    QVector2D p1 = m_points[((int)pos)%m_points.size()];
    QVector2D p2 = m_points[(int)((pos+1))%m_points.size()];


    float r = (pos - (int)pos);


    return p2*r + p1*(1.0-r);
}

void Shape::centerOrient()
{
    if (m_points.size()==0)
        return;

    QVector2D c(0,0);
    for (auto& p:m_points)
        c+=p;

    c/=(float)m_points.size();
    //    c+=m_center;
    for (auto& p:m_points)
        p-=c;
    m_center+= c;


}

void Shape::CreateSphere(int N)
{
    m_points.clear();
    for (int i=0;i<N;i++) {
        float t = i/(float)N*(3.14159*2);
        auto p = QVector2D(cos(t), sin(t));
        m_points.append(p);
    }
}

void Shape::enforceEqualDistance(float dist)
{
    auto org = m_points;
    if (m_points.count()==0)
        return;
    m_points.clear();
    m_points.append(org.first());
    for (int i=1;i<org.size();i++) {
        if ((m_points.last()-org[i]).length()>dist)
            m_points.append(org[i]);

    }
}

void Shape::generateAccelerationGraph(Shape &result)
{
    result.m_points.clear();
    float max = 1;
    if (m_points.count()<1) return;
    for (int i=0;i<m_points.size()-1;i++) {
        float h = (m_points[i+1]-m_points[i]).length();
        result.m_points.append(QVector2D(i*3, h));
        if (h>max)
            max = h;
    }
    /*
    for (int i=0;i<result.m_points.size();i++) {
        float y = result.m_points[i].y();
        result.m_points[i].setY(y*100.0/max);
    }
*/

}

float Shape::calculateRoundness()
{
    QVector2D center(0,0);
    for (auto& p: m_points)
        center+=p;

    center/=(float)m_points.size();

    float lineLength = 0;
    float area = 0;

    for (int i=0;i<m_points.size()-1;i++) {
        auto cur = m_points[i];
        auto nxt = m_points[(i+1)%m_points.size()];
        lineLength+=(nxt-cur).length();
        area+=(nxt-cur).length()*(center-cur).length()/2.0;

    }
    return lineLength*lineLength/(4*3.14159*area);

}


void Shape::calculateDirectionGroups(float scale)
{
    m_directionGroups.clear();
    QPoint curDir;
    for (int i=0;i<m_points.size()-1;i++) {
        auto cur = m_points[i];
        auto nxt = m_points[(i+1)%m_points.size()];

        QVector2D dir = ((nxt-cur)).normalized()*scale;

        QPoint pDir = QPoint(dir.x(),dir.y());

        //        if (curDir.dotProduct())

        if (m_directionGroups.contains(pDir))
            m_directionGroups[pDir]+=1;
        else
            //        qDebug() << pDir;
            m_directionGroups[pDir] = 1;

    }


}

int Shape::calculateDirectionGroups2(float scale)
{
    int cnt = 0;
    QPoint curDir;
    QVector2D prev = QVector2D(0,0);
    for (int i=0;i<m_points.size();i++) {
        auto cur = m_points[i];
        auto nxt = m_points[(i+1)%m_points.size()];



        QVector2D dir = ((nxt-cur)).normalized();
        if (abs(QVector2D::dotProduct(dir, prev))<0.5) {
            prev = dir;
            cnt++;
        }

    }
    return cnt;

}

void Shape::cleanup()
{
    int cnt = 0;
    QPoint curDir;
    QVector2D prev = QVector2D(0,0);
    QVector<QVector2D> newPoints;
    newPoints.append(m_points.first());
    for (int i=0;i<m_points.size();i++) {
        auto cur = m_points[i];
        auto nxt = m_points[(i+1)%m_points.size()];



        QVector2D dir = ((nxt-cur)).normalized();
        if (abs(QVector2D::dotProduct(dir, prev))<0.9) {
            prev = dir;
            newPoints.append(cur);
            //            cnt++;
        }

    }
    newPoints.append(m_points.first());
    m_points = newPoints;
    enforceEqualDistance(10);
    //    centerOrient();
    //    return cnt;

}


void ShapeRectangle::Generate()
{
    if (m_parameters.size()!=3) {
        return;
    }
    float scale = m_parameters[0];
    float aspect = m_parameters[1];
    float theta = m_parameters[2];
    m_points.clear();

    m_points.append(QVector2D(-1,-1));
    m_points.append(QVector2D(1,-1));
    m_points.append(QVector2D(1,1));
    m_points.append(QVector2D(-1,1));
    Transform(scale,aspect,theta);
}

float ShapeRectangle::getMinimumChisqGD(Shape &other, QVector<float> &results)
{
    return Shape::getMinimumChisqGD(other,results);
}

void ShapeTriangle::Generate()
{
    if (m_parameters.size()!=3) {
        return;
    }
    float scale = m_parameters[0];
    float aspect = m_parameters[1];
    float theta = m_parameters[2];
    CreateSphere(3);
    Transform(scale,aspect,theta);
}

/*float ShapeRectangle::getMinimumChisqBrute(Shape &other, QVector<float>& results)
{

    float winner = 1E20;
//#pragma omp parallel for
    for (float scale = 10; scale <200; scale+=5) {
        for (float theta = 0; theta<3.14159*2; theta+=0.04) {
            for (float aspect = 0; aspect<2.0; aspect+=0.04) {
                m_parameters = {scale,aspect,theta};
                Generate();
                float chisq = chiSquare(other);
                if (chisq<winner) {
                    winner = chisq;
                    results = m_parameters;
                }
            }
        }
    }
    return winner;
}
*/
float Shape::getMinimumChisqGD(Shape &other, QVector<float> &results)
{
    QVector<float> current = {500,1,0};

    bool done = false;
    float winner = 1E20;
    int steps = 10000;

    current[0] = other.getShapeBoundingSize()*0.5;

    qDebug() << "START scale: " <<current[0];

    while (steps!=0) {
        m_parameters = current;
        float scale = steps/1000.0*1.0;
        //        scale = 1.0;
        m_parameters[0] +=RandomGaussian(0,15*scale);
        m_parameters[1] +=RandomGaussian(0,0.1*scale);
        m_parameters[2] +=RandomGaussian(0,0.1*scale);
        if (m_parameters[0]<0)
            m_parameters[0] = 0;
        /*        for (int i=0;i<3;i++) {
            if (m_parameters[i]<=0)
                m_parameters[i]=0;
        }*/
        Generate();
        float chisq = chiSquare(other);
        if (chisq<winner) {
//            qDebug() << chisq << m_parameters;
            current = m_parameters;
            winner = chisq;
            m_resultDelta = m_timeDelta;
        }
        steps--;
    }
    results = current;
    qDebug() << "winner : " <<winner << current;
    return winner;
}

QVector2D Shape::getSmoothedPoint(int pos, int dist)
{
    QVector2D p(0,0);

    if (pos==m_points.size()-1 || pos == 0)
        return m_points[pos];

    float sum = 0.0;
    float sigma = dist/2.0;
    for (int i=std::max(pos-dist,(int)0);i<std::min(pos+dist,(int)m_points.size());i++)
    {
        float dist = (i-pos);
        float scale = 1.0/sqrt(2*3.14159265*sigma*sigma)*exp(-dist*dist/(2*sigma*sigma));
        p+=m_points[i]*scale;
        sum+=scale;
//        cnt++;
    }
//    qDebug() << sum << dist/2.0;
    return p/sum;///(float)cnt;
}

void ShapeCircle::Generate()
{
    if (m_parameters.size()!=3) {
        return;
    }
    float scale = m_parameters[0];
    float aspect = m_parameters[1];
    float theta = m_parameters[2];

    CreateSphere(100);

    Transform(scale,aspect,theta);

}

bool FitShape::find(Shape &other, Shape& result, float& delta)
{
    QVector<QSharedPointer<Shape>> testShapes;
    float roundness = other.calculateRoundness();
    int shapeType = other.calculateDirectionGroups2(1);
    //    roundNess = (roundNess-1.0)*5.0;
    testShapes.append(QSharedPointer<ShapeHeart>(new ShapeHeart()));
    testShapes.append(QSharedPointer<ShapeCircle>(new ShapeCircle()));
    testShapes.append(QSharedPointer<ShapeTriangle>(new ShapeTriangle()));
    testShapes.append(QSharedPointer<ShapeRectangle>(new ShapeRectangle()));
    float threshold = 3000;
    float winnerChisq = 1E10;
    other.centerOrient();
    Shape::s_tempResultList.clear();
    qDebug() << "\n\nORG roundness : "<< roundness;
    for (auto& p:testShapes) {
        //        p.m_center = other.m_center;
        //        qDebug() << "center:" <<other.m_center;
        QVector<float> resultParams;
        float chisq = p->getMinimumChisqGD(other,resultParams);
        float mul = std::abs(roundness - p->calculateRoundness());
 //       Shape::s_tempResultList.append(p);


        chisq+=mul*100.0;


//        1.0 - 1.0

        qDebug() <<p->type() << " with best chisq "<< chisq << " and roundness "<< p->calculateRoundness() << "scaling with "<<mul;

        //chisq*=(roundness - p->calculateRoundness());
        //        qDebug() << "roundness :; "<<p->calculateRoundness();
        if (chisq < winnerChisq) {
            winnerChisq = chisq;
            result = *p;
            result.m_center = other.m_center;
            result.m_parameters = resultParams;
            //result.m_parameters = QVector<float>() << 100<<1<<0;
            //          qDebug() << resultParams;
            result.Generate();
            delta = p->m_resultDelta;
            //            qDebug() << "winner center: " <<winnerChisq << result.m_center;
        }
    }

    //    return true;

    return winnerChisq<threshold;
}


bool FitShape::smooth2(Shape &other, Shape &result, Shape &graph, float &delta)
{
    // split into several shapes
    QVector<QSharedPointer<Shape>> subShapes;
    QSharedPointer<Shape> cs = QSharedPointer<Shape>(new Shape());
    int i = 0;
    cs->m_points.append(other.m_points.first());
    while (i<other.m_points.size()-1) {
        auto prev = other.m_points[i];
        auto cur = other.m_points[i+1];
        auto nxt = other.m_points[(i+2)%other.m_points.size()];


        float scale = pow((QVector2D::dotProduct((cur-prev).normalized(), (nxt-cur).normalized())),1.0);
        if (scale<0.5) {
            subShapes.append(cs);
            cs->m_points.append(cur);
            cs = QSharedPointer<Shape>(new Shape());
        }
        cs->m_points.append(cur);
        i++;
    }

    cs->m_points.append(other.m_points.last());
    subShapes.append(cs);

    result.m_points.clear();
 //   qDebug() << subShapes.count();
    for (auto c : subShapes) {
        if (c->m_points.size()<=3)
            continue;

        Spline2D sp(4);
        std::vector<QVector3D> lst;


        for (int i=0;i<c->m_points.size();i++) {
            QVector2D p = c->getSmoothedPoint(i,8);
            lst.push_back(QVector3D(p.x(),p.y(),0));
            result.m_points.append(p);
        }
        sp.set_ctrl_points(lst);

        for (float t = 0;t<1.0; t+=0.01) {
            auto p = sp.eval_f(t);
//            result.m_points.append(QVector2D(p.x(),p.y()));
        }
        result.m_points.append(c->m_points.last());
    }
    result.m_center = other.m_center;
    result.m_drawMax = 1;

    return true;
}


bool FitShape::smooth(Shape &other, Shape& result, Shape& graph, float& delta)
{
    result.m_points.clear();
    result.m_points.append(other.m_points.first());
    result.m_dirWeights.resize(other.m_points.size());
    other.m_dirWeights.resize(other.m_points.size());
    graph.m_points.clear();
    for (int i=1;i<other.m_points.size()-1;i++) {
        auto prev = other.m_points[i];
        auto cur = other.m_points[i+1];
        auto nxt = other.m_points[(i+2)%other.m_points.size()];


        float scale = 1.0-std::min(pow((QVector2D::dotProduct((cur-prev).normalized(), (nxt-cur).normalized())),1.0),1.0);
        if (scale<0.5)
            scale = 0;

        other.m_dirWeights[i] = scale;
        //        auto sp = other.getSmoothedPoint(i,10);
        //x      scale = 1.0;
        //      result.m_points.append(cur*scale + (1-scale)*sp);
        //        result.m_points.append(sp);

    }

    // Smooth the weights
    int smoothDist = 4;
    for (int j=0;j<1;j++) {
        for (int i=0;i<other.m_dirWeights.size();i++) {
            float cnt = 0;
            float w = 0;
            for (int k=std::max(i-smoothDist,(int)0);k<std::min(i+smoothDist,(int)other.m_dirWeights.size());k++) {
                w+=other.m_dirWeights[k];
                cnt++;
            }
            result.m_dirWeights[i] = w/cnt;
        }

        other.m_dirWeights = result.m_dirWeights;
    }

    for (int i=1;i<other.m_points.size()-1;i++) {
        auto sp = other.getSmoothedPoint(i,6);
        float scale = std::min(other.m_dirWeights[i]*10.0,1.0);
//        scale = 0.0;

        result.m_points.append(other.m_points[i]*scale + (1.0-scale)*sp);

        graph.m_points.append(QVector2D(i*3,-scale*100.0));
        if (other.m_dirWeights[i]!=0)
            qDebug() <<scale << i/(float)other.m_points.size();

    }
    graph.m_drawMax = 1;
    result.m_points.append(other.m_points.last());
    result.m_center = other.m_center;
    result.m_drawMax = other.m_drawMax;

    return true;
}


void ShapeHeart::Generate() {
    if (m_parameters.size()!=3) {
        return;
    }
    float scale = m_parameters[0];
    float aspect = m_parameters[1];
    float theta = m_parameters[2];

    int N = 50;
    m_points.clear();
    for (int i=0;i<N;i++) {
        float t = N-1-i/(float)N*3.14159*2;
        float x = 16*pow(sin(t),3);
        float y = 13*cos(t) - 5*cos(2*t)-2*cos(3*t)-cos(4*t);
        m_points.append(QVector2D(x,y));
    }



    Transform(scale,aspect,theta);

}


void ShapeMoon::Generate() {
    if (m_parameters.size()!=3) {
        return;
    }
    float scale = m_parameters[0];
    float aspect = m_parameters[1];
    float theta = m_parameters[2];

    int N = 50;
    m_points.clear();

    float wide = 0.5;

    float angle=-wide;
    float fullness = 1.0;
    float step = 0.2;
    while (angle<3.14159+wide) {
        m_points.append(QVector2D(sin(angle),cos(angle)));
        angle+=step;
    }
    //    angle = 3.14159;
    while (angle<(2.0f*3.14159-wide)) {
        m_points.append(QVector2D(-
                                      sin(angle)-fullness,cos(angle)));
        angle+=step;
    }


    Transform(scale,aspect,theta);

}

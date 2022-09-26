#include "mainwidget.h"

#include "source/mymeshes/rotboxtest.h"
#include <QMouseEvent>


MainWidget::MainWidget():ErisWidget()
{

}

void MainWidget::Init()
{

    for (int i=0;i<3000; i++) {
        float s = 200.0;
//        s = 0;
     //   int i = 0;
        QVector3D pos = QVector3D(((rand()%1000)/1000.0-0.5)*s,((rand()%1000)/1000.0-0.5)*s,((rand()%1000)/1000.0-0.5)*s);

        QVector3D col = QVector3D(((rand()%1000)/1000.0),((rand()%1000)/1000.0),((rand()%1000)/1000.0));

        auto* mi = world->AddMeshInstance(new RotBoxTest(), "myBox"+QString::number(i),"root",pos,"box",new MaterialFlat());
//        mi->m_material->shininess = 25
        mi->m_material->color = col;
        mi->m_rotation = QQuaternion::fromEulerAngles(pos*1000.0);

    }

  /*  auto* mi = world->AddMeshInstance(new MeshInstance(), "alle","root",QVector3D(0,0,0),"alien",new MaterialFlat());
    mi->m_material->color = QVector3D(1,1,1);
    QQuaternion rot = QQuaternion::fromEulerAngles(rand()%10000,rand()%1000, rand()%1000);
    mi->m_rotation = rot;

*/
    auto* mi = world->AddMeshInstance(new MeshInstance(), "alle","root",
                                      QVector3D(0,0,0),"alien",
                                      new MaterialTexture("/Users/leuat/pic/yo-grl.png", QVector2D(1,1)));
    mi->m_material->color = QVector3D(1,1,1);



//        mi = world->AddMeshInstance(new RotBoxTest(), "myBox"+QString::number(i),"root",pos,"box",new MaterialFlat());
  //      mi->m_material->color = QVector3D(0,1,0);

   // }

}

void MainWidget::Update()
{
    SData::sdata.time+=1;

    float r = 1000;
    double time = SData::sdata.time;
    SData::sdata.s_directionalLight = QVector3D(r*cos(time/130.0),r*cos(time/152.3),r*sin(time/130.0)).normalized();

    world->Update();

}

void MainWidget::initMeshes()
{
    ErisWidget::initMeshes();
    world->m_meshes["teapot"] = QSharedPointer<MeshObject>(new MeshObject("/Users/leuat/code/qPlanet/objects/teapot.obj",0.4, QVector3D(0,-2,0)));
    world->m_meshes["fox"] = QSharedPointer<MeshObject>(new MeshObject("/Users/leuat/code/qPlanet/objects/fox.obj",0.2, QVector3D(0,-2,0)));
    world->m_meshes["alien"] = QSharedPointer<MeshObject>(new MeshObject("/Users/leuat/code/qPlanet/objects/alien.obj",1.0, QVector3D(0,0,0)));

}


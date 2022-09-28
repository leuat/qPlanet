#include "mainwidget.h"

#include <QMouseEvent>
#include "imgui/QtImGui.h"
#include "imgui/ImGui.h"


MainWidget::MainWidget():ErisWidget()
{
}

void MainWidget::Init()
{
    QtImGui::initialize(this);

    ImGuiStyle* style = &ImGui::GetStyle();
    style->Colors[ImGuiCol_Text] = ImVec4(1.0f, 1.0f, 1.0f, 1.00f);

    for (int i=0;i<3000; i++) {
        float s = 200.0;
//        s = 0;
     //   int i = 0;
        QVector3D pos = QVector3D(((rand()%1000)/1000.0-0.5)*s,((rand()%1000)/1000.0-0.5)*s,((rand()%1000)/1000.0-0.5)*s);

        QVector3D col = QVector3D(((rand()%1000)/1000.0),((rand()%1000)/1000.0),((rand()%1000)/1000.0));

        auto* mi = world->AddMeshInstance(new MeshInstance(), "myBox"+QString::number(i),"root",pos,"box",new MaterialFlat());



        mi->UpdateFn = [](Entity* entity) -> void {
            QQuaternion rot = QQuaternion::fromEulerAngles(entity->time*0.3,
                                                           entity->time*0.4,
                                                           entity->time*0.5);
           entity->m_rotation = rot;
           entity->time+=1;
        };

        mi->InitFn = [](Entity* e) ->void {
            e->time = rand()/100000.0;
        };

//        mi->m_material->shininess = 25
        mi->m_material->mData.color = col;
        mi->m_rotation = QQuaternion::fromEulerAngles(pos*1000.0);

    }

    auto* mi = world->AddMeshInstance(new MeshInstance(), "alle","root",
                                      QVector3D(0,0,0),"woman",

                                      new MaterialNormal(
  //                                    "/Users/leuat/Dropbox/code/code/TangyMinecraft/Assets/TangyTextures/Assets/ExportedTextures/Gnarled1/Gnarled1_Color.png", QVector2D(13,13),
//                                      "/Users/leuat/Dropbox/code/code/TangyMinecraft/Assets/TangyTextures/Assets/ExportedTextures/Gnarled1/Gnarled1_Normal.png", QVector2D(13,13)
                                          "/Users/leuat/Dropbox/code/code/TangyMinecraft/Assets/TangyTextures/Assets/MinecraftTextures/stone.png", QVector2D(2,2),
                                          "/Users/leuat/Dropbox/code/code/TangyMinecraft/Assets/TangyTextures/Assets/MinecraftTextures/stone_n.png", QVector2D(5,5)
                                                         )
//                                      new MaterialFlat()
                                  );


    mi->m_material->mData.color = QVector3D(1.0,0.9,0.6);
    mi->m_material->mData.normalIntensity = 0.5;
    mi->m_material->mData.shininess = 60;

    mi->UpdateFn = [](Entity* entity) -> void {
        QQuaternion rot = QQuaternion::fromEulerAngles(entity->time*0.3,
                                                       entity->time*0.4,
                                                       entity->time*0.5);
//       entity->m_rotation = rot;
       entity->m_rotation = QQuaternion::fromEulerAngles(0,180,0); entity->time+=1;
    };
    mi->m_material->mData.normalIntensity=0.2;
    auto d = mi->m_material->mData;

    d.color = QVector3D(0.3,0.8,0.35);
    d.normalIntensity = 0.8;
    d.specular = 0;
    mi->extraMats["kg5_top_862_shape"] = d;

    d.color = QVector3D(1.0,0.0,0.9);
    d.normalIntensity = 1.0;
    d.specular = 1.5;
    d.shininess = 30;
    mi->extraMats["nini_hair"] = d;

    d.color = QVector3D(0.3,0.5,1.0);
    d.normalIntensity = 1.3;
    d.specular = 0.2;
    d.shininess = 10;
    mi->extraMats["pants001"] = d;

    d.color = QVector3D(1.0,0.3,0.2);
    d.normalIntensity = 1.3;
    d.specular = 1.2;
    d.shininess = 60;
    mi->extraMats["base_eye"] = d;

//        mi = world->AddMeshInstance(new RotBoxTest(), "myBox"+QString::number(i),"root",pos,"box",new MaterialFlat());
  //      mi->m_material->color = QVector3D(0,1,0);

   // }



    world->Init();

}

void MainWidget::Update()
{
    ErisWidget::Update();
    SData::sdata.time+=1;

    float r = 1000;
    double time = SData::sdata.time;
//    SData::sdata.s_directionalLight = QVector3D(r*cos(time/130.0),r*cos(time/152.3),r*sin(time/130.0)).normalized();

//    world->Update();


}

void MainWidget::initMeshes()
{
    ErisWidget::initMeshes();
    world->m_meshes["woman"] = QSharedPointer<MeshObject>(new MeshObject("/Users/leuat/code/qPlanet/objects/female.obj",0.5, QVector3D(0,0,0),true));
    world->m_meshes["car"] = QSharedPointer<MeshObject>(new MeshObject("/Users/leuat/code/qPlanet/objects/car.obj",1.0, QVector3D(0,0,0),true));
  //  world->m_meshes["alien"] = QSharedPointer<MeshObject>(new MeshObject("/Users/leuat/code/qPlanet/objects/alien.obj",1.0, QVector3D(0,0,0),true));
/*    world->m_meshes["teapot"] = QSharedPointer<MeshObject>(new MeshObject("/Users/leuat/code/qPlanet/objects/teapot.obj",0.4, QVector3D(0,-2,0),true));
    world->m_meshes["fox"] = QSharedPointer<MeshObject>(new MeshObject("/Users/leuat/code/qPlanet/objects/fox.obj",0.2, QVector3D(0,-2,0),false));
//    world->m_meshes["brain"] = QSharedPointer<MeshObject>(new MeshObject("/Users/leuat/code/qPlanet/objects/brain.obj",25.0, QVector3D(0,0,0),true));
*/
}

void MainWidget::PaintGUI()
{

/*    QtImGui::newFrame();
    bool show;
    // Any application code here
//    ImGui::Begin("Start");
    ImGui::Text("Hello, world!");
  //  ImGui::End();
    ImGui::SetNextWindowSize(ImVec2(200,100), ImGuiCond_FirstUseEver);
               ImGui::Begin("Another Window", &show);
               ImGui::Text("Hello");
               ImGui::End();
               // End of frame: render Dear ImGui
    ImGui::Render();
    QtImGui::render();
    */
}


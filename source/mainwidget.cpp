#include "mainwidget.h"

#include <QMouseEvent>
#include "imgui/QtImGui.h"
#include "imgui/ImGui.h"
#include "source/engine/meshes/meshmaze.h"

#include "source/ltriangles.h"
#include "source/spline.h"

MainWidget::MainWidget():ErisWidget()
{
    QThread::currentThread()->setPriority(QThread::HighestPriority);
//    SimplexNoise::seed(2);

    Settings::s.Load("data/world.json");
}


void MainWidget::AddWalls() {
    /*
    world->m_meshes["room1"] = QSharedPointer<Mesh>(new MeshMaze(25,25));

    auto* mi = world->AddMeshInstance(new MeshInstance(), "r1","root",
                                      QVector3D(0,0,0),"room1",
                                      world->m_materi'als["wall"].get()
//                                      new MaterialFlat()
                                  );


    mi->m_material->mData.shininess = 100;
    mi->m_material->mData.color = QVector3D(0.5,0.1,0.2);
*/
    //    AddGirl();

}

void MainWidget::AddChunk()
{

    Chunk::scale = Settings::s.blockScale;
    QSharedPointer<MeshChunks> mc  = QSharedPointer<MeshChunks>(new MeshChunks(Settings::s.worldSizeXZ,Settings::s.worldSizeY,getMaterialList()));
    world->m_entityList["root"]->m_children.append(mc);
    world->m_entityList["chunk"] = mc.get();

    mc->m_cameraPointer = &world->m_camera.m_position;
    mc->m_targetPointer = &world->m_camera.m_target;
    SData::sdata.camera = &world->m_camera.m_position;

    if (Settings::s.hasWater) {

        water = world->AddMeshInstance(new MeshInstance(), "water","root",
                                         QVector3D(0,Settings::s.waterHeight,0),"watermesh",
                                         new MaterialWater(&world->m_camera,
                                             //                                    "/Users/leuat/Dropbox/code/code/TangyMinecraft/Assets/TangyTextures/Assets/ExportedTextures/Gnarled1/Gnarled1_Color.png", QVector2D(13,13),
                                             //                                      "/Users/leuat/Dropbox/code/code/TangyMinecraft/Assets/TangyTextures/Assets/ExportedTextures/Gnarled1/Gnarled1_Normal.png", QVector2D(13,13)
                                             "", QVector2D(2,2),
                                             "", QVector2D(2,2)
                                             )
                                          );
    }
    if (Settings::s.hasWater) {

        water = world->AddMeshInstance(new MeshInstance(), "clouds","root",
                                       QVector3D(0,Settings::s.cloudHeight,0),"cloudmesh",
                                       new MaterialClouds(&world->m_camera,
                                                         "data/textures/clouds.png", QVector2D(1,1),
                                                         "", QVector2D(2,2)
                                                         )
                                       );
    }
    //

    //    calculateWaterShadow();
}

void MainWidget::InitShapes()
{
    float s = 1.2;

    sUser.m_points.clear();
    sUser.m_drawMax = 1;
/*
    sUser.m_points.append(QVector2D(-100*s,-100));
    sUser.m_points.append(QVector2D(100*s,-120));
    sUser.m_points.append(QVector2D(100*s,100));
    sUser.m_points.append(QVector2D(-100*s,100));*/

    const int N = 10;
    for (int i=0;i<N;i++)
        sUser.m_points.append(QVector2D(-100+ 200/N*i,-100));
    for (int i=0;i<N;i++)
        sUser.m_points.append(QVector2D(100,-100+ 200/N*i ));
    for (int i=0;i<N;i++)
        sUser.m_points.append(QVector2D(100- 200/N*i,100));
    for (int i=0;i<N;i++)
        sUser.m_points.append(QVector2D(-100, 100- 200/N*i ));

    sUser.Transform(1,1,1.0);
    sUser.m_col = QColor(255,255,0,255);
    sOrg = sUser;
}


void MainWidget::AddGirl()
{
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

}

void MainWidget::AddCubes()
{
    for (int i=0;i<3000; i++) {
        float s = 400.0;
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

}

void MainWidget::Init()
{
    QtImGui::initialize(this);
    SetThemeEnemyMouse();
    ImGuiIO& io = ImGui::GetIO();
    //    qDebug() << QFile::exists(":/resources/fonts/OpenSans-Regular.ttf");
    ImFont* font1 = io.Fonts->AddFontFromFileTTF("/Users/leuat/code/qPlanet/resources/fonts/OpenSans-Regular.ttf", 24);
    //    ImGui::PushFont(font1);


    ImGuiStyle* style = &ImGui::GetStyle();
    style->Colors[ImGuiCol_Text] = ImVec4(1.0f, 1.0f, 1.0f, 1.00f);



    //        mi = world->AddMeshInstance(new RotBoxTest(), "myBox"+QString::number(i),"root",pos,"box",new MaterialFlat());
    //      mi->m_material->color = QVector3D(0,1,0);

    // }
    InitMaterials();
//    InitShapes();
    //    AddWalls();
    AddChunk();
    world->Init();

}

void MainWidget::Update()
{
//    return;
    ErisWidget::Update();
    SData::sdata.time+=1;

    float r = 100.0;
    double time = SData::sdata.time;
//    time = 1;
    time*=0.1;
 //   SData::sdata.s_directionalLight = QVector3D(r*cos(time/130.0),r*cos(time/171.0)+r*1.3,r*sin(time/130.0));
  //  SData::sdata.s_directionalLight = QVector3D(0.7,0.7,0.7).normalized();

    world->Update();
/*    if (water) {
        water->m_position = world->m_camera.m_position;
        water->m_position.setY(Settings::s.waterHeight);
    }*/

}

void MainWidget::initMeshes()
{
    ErisWidget::initMeshes();
    world->m_meshes["woman"] = QSharedPointer<MeshObject>(new MeshObject("/Users/leuat/code/qPlanet/objects/female.obj",0.1, QVector3D(0,-5,0),true));
//    world->m_meshes["car"] = QSharedPointer<MeshObject>(new MeshObject("/Users/leuat/code/qPlanet/objects/car.obj",1.0, QVector3D(0,0,0),true));
    //  world->m_meshes["alien"] = QSharedPointer<MeshObject>(new MeshObject("/Users/leuat/code/qPlanet/objects/alien.obj",1.0, QVector3D(0,0,0),true));
    /*    world->m_meshes["teapot"] = QSharedPointer<MeshObject>(new MeshObject("/Users/leuat/code/qPlanet/objects/teapot.obj",0.4, QVector3D(0,-2,0),true));
    world->m_meshes["fox"] = QSharedPointer<MeshObject>(new MeshObject("/Users/leuat/code/qPlanet/objects/fox.obj",0.2, QVector3D(0,-2,0),false));
//    world->m_meshes["brain"] = QSharedPointer<MeshObject>(new MeshObject("/Users/leuat/code/qPlanet/objects/brain.obj",25.0, QVector3D(0,0,0),true));
*/
}

void MainWidget::mousePressEvent(QMouseEvent *e)
{
    // Save mouse press position
    mousePressPosition = QVector2D(e->position());

//    qDebug() << world->m_camera.coord2ray(mousePressPosition.x(), mousePressPosition.y(),width(),height());
//    QVector3D ray = world->m_camera.coord2ray(width()/2, height()/2,width(),height());
    QVector3D ray = (world->m_camera.m_target - world->m_camera.m_position).normalized()*0.2;
    QVector3D org = world->m_camera.m_position;
//    qDebug() << org;
    auto p = QVector3D(org.x()*0.5, org.y(), org.z()*0.5) + ray*4;

    ChunkData::s.set(p,1);
    auto oc = ChunkData::s.get(p);
//    qDebug() << oc->m_index << ChunkData::idxtoInt(oc->m_index);
     MeshChunks* mc = (MeshChunks*)world->m_entityList["chunk"];
    if (mc==nullptr)
         return;
    for (auto& c: mc->m_chunks)
        if (oc == c->m_chunk) {
            c->regenerate();
        }

}

void MainWidget::PaintGUI()
{
    if (!m_isInMenu)
        return;
    QtImGui::newFrame();
    ImGui::SetNextWindowPos(ImVec2(10,10));
    ImGui::SetNextWindowSize(ImVec2(1400,800),ImGuiCond_Appearing);
    ImGui::Begin("Main menu");
    ImGui::SetWindowFontScale(1.0);

    bool show;
    // Any application code here
    //    ImGui::Begin("Start");
    ImGui::Text("Hello, world!");
    if (ImGui::Button("Jente"))
        AddGirl();
    if (ImGui::Button("Cubes"))
        AddCubes();
    if (ImGui::Button("Chunk"))
        AddChunk();

    if (ImGui::Button("Quit"))
        exit(0);


//    RenderSplines();
    RenderShapes();

    ImGui::End();

    //  ImGui::End();
    /*    ImGui::SetNextWindowSize(ImVec2(250,300), ImGuiCond_FirstUseEver);

    ImGui::Begin("Another Window", &show);
    ImGui::SetWindowFontScale(1.8);
        ImGui::Text("Hello");
        ImGui::End();
               // End of frame: render Dear ImGu
*/



    ImGui::Render();
    QtImGui::render();

}

void MainWidget::InitMaterials()
{
    world->m_materials["wall"] = QSharedPointer<Material>(new MaterialNormal(
        //                                    "/Users/leuat/Dropbox/code/code/TangyMinecraft/Assets/TangyTextures/Assets/ExportedTextures/Gnarled1/Gnarled1_Color.png", QVector2D(13,13),
        //                                      "/Users/leuat/Dropbox/code/code/TangyMinecraft/Assets/TangyTextures/Assets/ExportedTextures/Gnarled1/Gnarled1_Normal.png", QVector2D(13,13)
        "/Users/leuat/Dropbox/code/code/TangyMinecraft/Assets/TangyTextures/Assets/MinecraftTextures/stone.png", QVector2D(2,2),
        "/Users/leuat/Dropbox/code/code/TangyMinecraft/Assets/TangyTextures/Assets/MinecraftTextures/stone_n.png", QVector2D(5,5)
        ));

}

void MainWidget::calculateWaterShadow()
{
    auto m_lightDir = SData::sdata.s_directionalLight.normalized();
    //    if (m_currentLod!=0)
    //      return;

    for (auto& d : water->m_mesh->data) {
        //auto p = m_pos + QVector3D((i-size/2.0)*m_scale,(j-size/2.0)*m_scale,(k-size/2.0)*m_scale);
        QVector3D p = (d.position + water->m_position)*0.5;//*Chunk::scale;// + 0.5*QVector3D(1,1.00,1)*size;
        QVector3D dir = m_lightDir*Chunk::scale*2;
        float l = 1.0;
        p+=dir*2;
        for (int i=0;i<Settings::s.shadowSteps;i+=1) {
            p+=dir;
            auto val = WorldGen::s.generate(p, false);

            if (val!=0) {
                //                l-= 0.2;
                l*=Settings::s.shadowMultiplier;
                if (l<Settings::s.shadowThreshold)
                    break;
            }
            if (i>8) i+=3;
            if (i>40) i+=8;
            if (i>80) i+=8;
        }
        //                        float dist = ((p*2 - d.position-QVector3D(0,-0.5,0)).length()*+0.5)*0.5;
        d.light = d.light*(l);
    }
    water->m_mesh->Build();
}

void MainWidget::RenderSplines()
{
    LTriangles list;

    // Add some points to a list
    std::vector<QVector3D> lst;
    lst.push_back(QVector3D(100,100,0));
    lst.push_back(QVector3D(300,150,0));
    lst.push_back(QVector3D(700,250,0));
    lst.push_back(QVector3D(500,550,0));
    lst.push_back(QVector3D(200,350,0));
    lst.push_back(QVector3D(150,400,0));
    lst.push_back(QVector3D(200,600,0));
    lst.push_back(QVector3D(300,700,0));
    lst.push_back(QVector3D(500,600,0));
    lst.push_back(QVector3D(800,500,0));
    lst.push_back(QVector3D(900,200,0));
    // Don't remember what the 4 here in the constructor does
    const float w = 60;
    SData::sdata.time++;
    for (int i=0;i<lst.size();i++) {
        float t = SData::sdata.time*0.05+i*1.8;

        lst[i] += QVector3D(sin(t)*w, cos(t)*w,0);
    }


    Spline2D sp(4);
    sp.set_ctrl_points(lst);

    // Number of points to evaluate
    int count = 80;
    // With of the polygons from center line
    int lineWidth = 40.0;
    QVector<QVector3D> points;

    // Generate the points by evaluating the spline point (p) + cross(vec3(0,0,1), tagent of point)
    for (int i=0;i<count;i++) {
        float t = i/(float)count;
        QVector3D p = sp.eval_f(t);
        QVector3D n = QVector3D::crossProduct(QVector3D(0,0,1), sp.eval_df(t).normalized());
        // add center point - normal*width
        points.append(p-n*lineWidth);
        points.append(p+n*lineWidth);
   }
    // Generate the two triangles
    for (int i=0;i<count-1;i++) {
       // Index loopup
        int idx = i*2;
        list.m_triangles.append(LTriangle(points[idx+0], points[idx+1], points[idx+3]));
        list.m_triangles.append(LTriangle(points[idx+3], points[idx+2], points[idx+0]));

    }

    QColor c = Qt::white;
    int width = 1;
    // Render the triangles

    for (auto& l : list.m_triangles) {
        drawLine(l.m_p0.x(),l.m_p0.y(),l.m_p1.x(),l.m_p1.y(),c, width);
        drawLine(l.m_p1.x(),l.m_p1.y(),l.m_p2.x(),l.m_p2.y(),c, width);
        drawLine(l.m_p2.x(),l.m_p2.y(),l.m_p0.x(),l.m_p0.y(),c, width);
    }
}

void MainWidget::RenderShapes()
{
    SData::sdata.time+=1;
    //qDebug() << "hurra" <<ImGui::GetMouseCursor();
    //sRect.m_parameters = QVector<float>() << 100 << 0.5 << SData::sdata.time*0.02;
    //sRect.Generate();

//    sUser.

    ImGuiIO& io = ImGui::GetIO();
    auto mp = QVector2D(io.MousePos.x,io.MousePos.y);

    if (io.MouseClicked[0]==1 &&!mouseDown) {
        mouseDown=true;
        center = mp;
        sUser.m_points.clear();
        sUser.m_center = QVector2D(0,0);
    }
    if (io.MouseDown[0]==0 && mouseDown) {
        sUser.centerOrient();
        sGraph.m_col = QColor(255,0,255,255);
        sGraph.m_center = QVector2D(500,500);
        sUser.enforceEqualDistance(2);
        sUser.generateAccelerationGraph(sGraph);
//        sUser.cleanup();


        sUser.calculateDirectionGroups(4);
  //      for (auto& p: sUser.m_directionGroups.keys())
    //        qDebug() << " group : "<< p << sUser.m_directionGroups[p];

//        qDebug() << "Direction groups : " <<sUser.m_directionGroups.count();
      //  qDebug() << "Direction groups : " <<sUser.calculateDirectionGroups2(4);

//        qDebug() << "******** Roundness : "<<sUser.calculateRoundness();

//        InitShapes();
        mouseDown = false;

        displayResult = FitShape::find(sUser, sResult,delta);
//        displayResult = FitShape::smooth2(sUser, sResult, sGraph,delta);

    }
    else if (mouseDown){
        if (sUser.m_points.size()==0)
            sUser.m_points.append(mp);
        else
        if (sUser.m_points.last()!=mp) {
            sUser.m_points.append(mp);
        }
    }


/*
    for (int i=0;i<sUser.m_points.size();i++) {
        float t = SData::sdata.time*0.1 + i;
        float rad = 40;
        QVector2D delta(cos(t)*rad, sin(t)*rad);
        sUser.m_points[i] = sOrg.m_points[i] + delta;
    }
  */
    sGraph.Render();
    sResult.m_col = QColor(255,0,255,255);
    sUser.Render();
    sResult.m_thickness = 4;
    if (displayResult)
        sResult.Render();

    int i = 0;
    for (auto& p: Shape::s_tempResultList) {
        p->m_center = sResult.m_center;
        p->m_col = QColor(0,255-i,i,255);
        i+=63;
        p->Render();
    }



//    auto center = QVector2D(500,500);
/*
    QVector<float> result;
    auto winner = sRect.getMinimumChisqGD(sUser, result);
    sRect.m_parameters = result;
    qDebug() << sRect.m_parameters << winner;
    sRect.Generate();
//    auto l = ImGui::GetWindowDrawList();
//    auto p = sRect.getPoint(std::fmod(SData::sdata.time*0.01,1.0)) + center;
    sRect.Render(center);
    sUser.Render(center);
*/
    auto l = ImGui::GetWindowDrawList();
    auto p = sUser.getPoint(std::fmod(SData::sdata.time*0.01-delta,1.0)) + sUser.m_center;
    l->AddCircleFilled(ImVec2(p.x(),p.y()),10,QColor(0,0,255,255).rgb());
    if (displayResult) {
        auto p2 = sResult.getPoint(std::fmod(SData::sdata.time*0.01,1.0)) + sResult.m_center;
        l->AddCircleFilled(ImVec2(p2.x(),p2.y()),10,QColor(255,0,255,255).rgb());
    }
}

void MainWidget::drawLine(int x0, int y0, int x1, int y1, QColor col, int t)
{
    auto l = ImGui::GetWindowDrawList();
    l->AddLine(ImVec2(x0,y0),ImVec2(x1,y1),col.rgb(), t);

}


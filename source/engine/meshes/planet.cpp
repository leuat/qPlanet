/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the QtCore module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** BSD License Usage
** Alternatively, you may use this file under the terms of the BSD license
** as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "planet.h"

#include <QVector2D>
#include <QVector3D>


//! [0]
Planet::Planet()
{
}

Planet::~Planet()
{
}

void Planet::Render(QMatrix4x4 projection, QOpenGLShaderProgram *program)
{

    // Calculate model view transformation
    QMatrix4x4 matrix;
    matrix.translate(0.0, 0.0, -3.0);
    matrix.rotate(rotation);

    // Set modelview-projection matrix
    program->setUniformValue("mvp_matrix", projection * matrix);
//! [6]

    // Use texture unit 0 which contains cube.png
    program->setUniformValue("texture", 0);



    for (auto& pl:planes)
        pl->Render(program);
}
//! [0]

void Planet::initPlanetGeometry(double radius, double heightscale, double scale)
{
    double r = radius;
    double n = 250;
    pd.radius = r;
    pd.heightScale = heightscale*2;

    pd.scale = scale;
    pd.scale = 1.5;
    pd.lacunarity = 2.0;
    pd.offset = 0.7;
    pd.gain = 1.8;
    auto p1 = QVector3D(-r,-r,-r);
    auto p2 = QVector3D(r,-r,-r);
    auto p3 = QVector3D(r,r,-r);
    auto p4 = QVector3D(-r,r,-r);
    auto p5 = QVector3D(-r,-r,r);
    auto p6 = QVector3D(r,-r,r);
    auto p7 = QVector3D(r,r,r);
    auto p8 = QVector3D(-r,r,r);

    auto pl1 = QSharedPointer<QuadPlane>(new QuadPlane);
    auto pl2 = QSharedPointer<QuadPlane>(new QuadPlane);
    auto pl3 = QSharedPointer<QuadPlane>(new QuadPlane);
    auto pl4 = QSharedPointer<QuadPlane>(new QuadPlane);
    auto pl5 = QSharedPointer<QuadPlane>(new QuadPlane);
    auto pl6 = QSharedPointer<QuadPlane>(new QuadPlane);
    pl1->generatePlane(p4,p3,p2,p1,n,pd);
    pl2->generatePlane(p5,p6,p7,p8,n,pd);
    pl3->generatePlane(p8,p7,p3,p4,n,pd);
    pl4->generatePlane(p6,p5,p1,p2,n,pd);
    pl5->generatePlane(p4,p1,p5,p8,n,pd);
    pl6->generatePlane(p2,p3,p7,p6,n,pd);
    planes.append(pl1);
    planes.append(pl2);
    planes.append(pl3);
    planes.append(pl4);
    planes.append(pl5);
    planes.append(pl6);

}




void Planet::initPlanet()
{

}

//! [2]

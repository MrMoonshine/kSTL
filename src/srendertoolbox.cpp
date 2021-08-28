
/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the demonstration applications of the Qt Toolkit.
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

#include <srendertoolbox.hpp>

#include <QQuickWindow>
#include <QOpenGLShaderProgram>
#include <QOpenGLContext>
#include <QRunnable>

//! [4]
void SRenderToolbox::init()
{
    if (!mBgShdr || !mMeshShdr) {
        initializeOpenGLFunctions();
        mMeshShdr = new QOpenGLShaderProgram();
        mMeshShdr->addCacheableShaderFromSourceFile(QOpenGLShader::Vertex, "/home/david/Programmieren/C++/QT/kSTL/src/shaders/model.vert");
        mMeshShdr->addCacheableShaderFromSourceFile(QOpenGLShader::Fragment, "/home/david/Programmieren/C++/QT/kSTL/src/shaders/model.frag");
        mMeshShdr->link();
        mMesh = new SMeshSTL(mMeshShdr);
        mMesh->setViewportSize(&mViewportSize);
        mMesh->setColor(&mFilamentColor);

        mBgShdr = new QOpenGLShaderProgram();
        mBgShdr->addCacheableShaderFromSourceFile(QOpenGLShader::Vertex, "/home/david/Programmieren/C++/QT/kSTL/src/shaders/background.vert");
        mBgShdr->addCacheableShaderFromSourceFile(QOpenGLShader::Fragment, "/home/david/Programmieren/C++/QT/kSTL/src/shaders/background.frag");
        mBgShdr->bindAttributeLocation("bgvtx", 0);
        mBgShdr->link();
        mBackground = new SBackground(mBgShdr);
        mBackground->setViewportSize(&mViewportSize);

    }
}

SRenderToolbox::~SRenderToolbox()
{
    delete mMesh;
    delete mBackground;
    delete mMeshShdr;
    delete mBgShdr;
}

void SRenderToolbox::setViewportSize(const QSize &size){
    if(!size.isValid())
        return;
    validViewport = true;
    mViewportSize = size;
}

void SRenderToolbox::setFilamentColor(const QColor &color){
    mFilamentColor = color;
}

SMeshSTL* SRenderToolbox::model(){
    return mMesh;
}
//! [4] //! [5]
void SRenderToolbox::paint()
{
    // Play nice with the RHI. Not strictly needed when the scenegraph uses
    // OpenGL directly.
    m_window->beginExternalCommands();
    glClearColor(0.2f, 0.2f, 0.2f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    mBackground->draw();
    mMesh->draw();

    // Not strictly needed for this example, but generally useful for when
    // mixing with raw OpenGL.
    m_window->resetOpenGLState();

    m_window->endExternalCommands();
}
//! [5]


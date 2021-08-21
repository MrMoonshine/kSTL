
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
    if (!m_program) {
        QSGRendererInterface *rif = m_window->rendererInterface();
        Q_ASSERT(rif->graphicsApi() == QSGRendererInterface::OpenGL || rif->graphicsApi() == QSGRendererInterface::OpenGLRhi);

        initializeOpenGLFunctions();

        m_program = new QOpenGLShaderProgram();
        m_program->addCacheableShaderFromSourceCode(QOpenGLShader::Vertex,
                                                    "attribute highp vec4 vertices;"
                                                    "varying highp vec2 coords;"
                                                    "void main() {"
                                                    "    gl_Position = vertices;"
                                                    "    coords = vertices.xy;"
                                                    "}");
        m_program->addCacheableShaderFromSourceCode(QOpenGLShader::Fragment,
                                                    "uniform lowp float t;"
                                                    "varying highp vec2 coords;"
                                                    "void main() {"
                                                    "    lowp float i = 1. - (pow(abs(coords.x), 4.) + pow(abs(coords.y), 4.));"
                                                    "    i = smoothstep(t - 0.8, t + 0.8, i);"
                                                    "    i = floor(i * 20.) / 20.;"
                                                    "    gl_FragColor = vec4(coords * .5 + .5, i, i);"
                                                    "}");

        m_program->bindAttributeLocation("vertices", 0);
        m_program->link();

        mVaoShdr = new QOpenGLShaderProgram();
        mVaoShdr->addCacheableShaderFromSourceFile(QOpenGLShader::Vertex, "/home/david/Programmieren/C++/QT/kSTL/src/shaders/test.vert");
        mVaoShdr->addCacheableShaderFromSourceFile(QOpenGLShader::Fragment, "/home/david/Programmieren/C++/QT/kSTL/src/shaders/test.frag");
        mVaoShdr->link();
        mVao = new SVao(mVaoShdr);
        mVao->setViewportSize(&mViewportSize);

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
    delete mVao;
    delete mBackground;
    delete m_program;
    delete mVaoShdr;
    delete mBgShdr;
}

void SRenderToolbox::setViewportSize(const QSize &size){
    if(!size.isValid())
        return;
    validViewport = true;
    mViewportSize = size;
}

//! [4] //! [5]
void SRenderToolbox::paint()
{
    // Play nice with the RHI. Not strictly needed when the scenegraph uses
    // OpenGL directly.
    m_window->beginExternalCommands();
    glClearColor(0.2f, 0.2f, 0.2f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    /*m_program->bind();

    m_program->enableAttributeArray(0);

    float values[] = {
        -1, -1,
        1, -1,
        -1, 1,
        1, 1
    };

    // This example relies on (deprecated) client-side pointers for the vertex
    // input. Therefore, we have to make sure no vertex buffer is bound.
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    m_program->setAttributeArray(0, GL_FLOAT, values, 2);
    m_program->setUniformValue("t", (float) m_t);

    glViewport(0, 0, mViewportSize.width(), mViewportSize.height());

    glDisable(GL_DEPTH_TEST);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);

    glDrawArrays(GL_TRIANGLES, 0, 4);

    m_program->disableAttributeArray(0);
    m_program->release();*/

    mBackground->draw();
    mVao->draw();

    // Not strictly needed for this example, but generally useful for when
    // mixing with raw OpenGL.
    m_window->resetOpenGLState();

    m_window->endExternalCommands();
}
//! [5]


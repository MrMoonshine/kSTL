#include <sbackground.hpp>
#include <QApplication>
#include <QPalette>

QVector4D SBackground::color2vec4(const QColor &color){
    return QVector4D(
                color.redF(),
                color.greenF(),
                color.blueF(),
                color.alphaF()
           );
}

SBackground::SBackground(QOpenGLShaderProgram *program) : SVao(program)
{
    //Set Default Color
    qDebug() << QApplication::palette().color(
        QPalette::Normal,
        QPalette::Highlight
        ).name();
    mProgram->bind();
    mProgram->setUniformValue(
                "secColor",
                color2vec4(
                    QApplication::palette().color(
                        QPalette::Normal,
                        QPalette::Window
                        )
                    )
                );
    mProgram->setUniformValue(
                "mainColor",
                color2vec4(
                    QApplication::palette().color(
                        QPalette::Inactive,
                        QPalette::Highlight
                        )
                    )
                );
    mProgram->release();
}

SBackground::~SBackground()
{

}

void SBackground::draw(){
    //mVao.bind();
    mProgram->bind();
    mProgram->enableAttributeArray(0);

    float values[] = {
        -1, -1,
        1, -1,
        -1, 1,
        1, 1
    };

    // This example relies on (deprecated) client-side pointers for the vertex
    // input. Therefore, we have to make sure no vertex buffer is bound.
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    mProgram->setAttributeArray(0, GL_FLOAT, values, 2);
    //mProgram->setUniformValue("mainColor", QVector4D(0.0f, 0.0f, 1.0f, 1.0f));
    //mProgram->setUniformValue("secColor", QVector4D(1.0f, 1.0f, 0.0f, 1.0f));
    //mProgram->setUniformValue("t", (float)1);

    if(mViewportSize)
    glViewport(0, 0, mViewportSize->width(), mViewportSize->height());

    glDisable(GL_DEPTH_TEST);

    //glEnable(GL_BLEND);
    //glBlendFunc(GL_SRC_ALPHA, GL_ONE);

    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    mProgram->disableAttributeArray(0);
    mProgram->release();
    //mVao.release();
}

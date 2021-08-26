#ifndef SMESHSTL_HPP
#define SMESHSTL_HPP

#include <QObject>
#include <svao.hpp>
#include <QColor>
#include <QObject>
#include <QOpenGLShaderProgram>
#include <QOpenGLFunctions>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>
#include <QMatrix4x4>
#include <QVector3D>
#include <QVector2D>
#include <QtMath>
#include <QUrl>
#include <errorhandler.hpp>

#include <stl.h>

#define STL_HEADER_SIZE 80
#define STL_VERTEX_SIZE 50
#define STL_VERTEX_FLOAT_COUNT 9

#define STL_GL_10_BITS 10
#define STL_GL_10_MAX ((2 << (STL_GL_10_BITS - 1))-1)

class SMeshSTL : public SVao
{
public:
    SMeshSTL(QOpenGLShaderProgram *program, QObject *parent = nullptr);
    ~SMeshSTL();

    void draw() override;
    //test:
    int loadModel(const QUrl &model);
    void setColor(QColor *color);
    void setDeltaMouse(QVector2D deltaMouse);
private:
    void updateUniformBuffer();
    //Spherical Camera Radius
    float mRadius = 1000.0f;
    float mPhi = 0.0f;
    float mTheta = 0.0f;

    uint32_t mVertexCount = 0;
    QColor *mFilamentColor = nullptr;
    QOpenGLBuffer mVertexBO, mNormalsBO;
    QMatrix4x4 model, view, proj;
    QVector3D mModelUp;
};

#endif // SMESHSTL_HPP

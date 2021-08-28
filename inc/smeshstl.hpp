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
#define MOUSE_WHEEL_ROTATION_STEP_SIZE 120

class SMeshSTL : public SVao
{
public:
    SMeshSTL(QOpenGLShaderProgram *program, QObject *parent = nullptr);
    ~SMeshSTL();

    void draw() override;
    //test:
    int loadModel(const QUrl &model);
    void setColor(QColor *color);
    void setDeltaRotation(QVector2D deltaMouse);
    void setDeltaTransform(QVector2D deltaMouse);
    void setDeltaZoom(int deltaWheel);
private:
    void handleTransformation(const QVector3D &eye);
    void updateUniformBuffer();
    //Spherical Camera Radius
    float mRadius = 100.0f;
    float mPhi = 0.0f;
    float mTheta = 0.0f;
    float mZoom = 1.0f;
    //Transformation Vector for moving the model around
    QVector3D mTransform;
    QVector2D mDeltaMove;

    uint32_t mVertexCount = 0;
    QColor *mFilamentColor = nullptr;
    QOpenGLBuffer mVertexBO, mNormalsBO;
    QMatrix4x4 model, view, proj;
    QVector3D mModelUp;
};

#endif // SMESHSTL_HPP

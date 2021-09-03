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
#define MODEL_RADIUS_INITIAL_FACTOR 1.2f

class SMeshSTL : public SVao
{
public:
    typedef enum{
        UPPER,
        LOWER
    }Hemisphere;

    SMeshSTL(QOpenGLShaderProgram *program, QObject *parent = nullptr);
    ~SMeshSTL();

    void draw() override;
    void queueModelLoad(const QUrl &model);
    void setView(const QVector3D &view);

    void setColor(QColor *color);
    void setDeltaRotation(QVector2D deltaMouse);
    void setDeltaTransform(QVector2D deltaMouse);
    void setDeltaZoom(int deltaWheel);
private:
    int loadModel(const QUrl &model);
    void createBuffers();
    void deleteBuffers();
    void handleTransformation(const QVector3D &eye);
    void updateUniformBuffer();
    //Spherical Camera Radius
    float mRadius = 100.0f;
    float mPhi = 0.0f;
    float mTheta = 0.0f;
    float mZoom = 1.0f;
    Hemisphere mHemisphere;
    //Transformation Vector for moving the model around
    QVector3D mTransform;
    QVector2D mDeltaMove;
    //MetaData
    struct MetaSTL mMeta;
    bool mLoadNewFile = false;
    QUrl mURLNewFile;

    uint32_t mVertexCount = 0;
    QColor *mFilamentColor = nullptr;
    GLuint mVertexBuff, mNormalsBuff;
    QMatrix4x4 model, view, proj;
    QVector3D mModelUp;
};

#endif // SMESHSTL_HPP

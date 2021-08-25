#ifndef SVAO_HPP
#define SVAO_HPP

#include <QObject>
#include <QOpenGLShaderProgram>
#include <QOpenGLFunctions>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>
#include <QMatrix4x4>
#include <QVector3D>
#include <QtMath>

#include <QColor>


class SVao : public QObject, protected QOpenGLFunctions
{
    Q_OBJECT
public:
    explicit SVao(QOpenGLShaderProgram *program, QObject *parent = nullptr);
    ~SVao();
    virtual void draw();
    void setViewportSize(QSize *size);
    void setColor(QColor *color);
protected:
    QOpenGLVertexArrayObject mVao;
    QOpenGLBuffer mVbo;
    QOpenGLShaderProgram *mProgram;
    void updateUniformBuffer();
    QMatrix4x4 model, view, proj;
    QSize *mViewportSize = nullptr;
private:
    QColor *mFilamentColor = nullptr;

signals:

};

#endif // SVAO_HPP

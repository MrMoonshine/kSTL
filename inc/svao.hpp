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
protected:
    QOpenGLVertexArrayObject mVao;
    QOpenGLShaderProgram *mProgram;
    QSize *mViewportSize = nullptr;
private:

signals:

};

#endif // SVAO_HPP

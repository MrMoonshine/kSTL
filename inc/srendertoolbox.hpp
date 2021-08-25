#ifndef SQUIRCLERENDERER_HPP
#define SQUIRCLERENDERER_HPP

#include <QtQuick/QQuickItem>
#include <QtGui/QOpenGLShaderProgram>
#include <QtGui/QOpenGLFunctions>

#include <svao.hpp>
#include <sbackground.hpp>

//! [1]
class SRenderToolbox : public QObject, protected QOpenGLFunctions
{
    Q_OBJECT
public:
    SRenderToolbox() : m_t(0), m_program(0), mViewportSize(DEFAULT_APP_WIDTH, DEFAULT_APP_HEIGHT) { }
    ~SRenderToolbox();

    void setT(qreal t) { m_t = t; }
    void setViewportSize(const QSize &size);
    void setWindow(QQuickWindow *window) { m_window = window; }

    void setFilamentColor(const QColor &color);
public slots:
    void init();
    void paint();

private:
    bool validViewport = false;
    QSize mViewportSize;
    qreal m_t;
    QColor mFilamentColor = Qt::black;

    QOpenGLShaderProgram *m_program;
    QOpenGLShaderProgram *mBgShdr;
    QOpenGLShaderProgram *mVaoShdr;

    SBackground *mBackground;
    SVao *mVao;
    QQuickWindow *m_window;
};

#endif // SRenderToolbox_HPP

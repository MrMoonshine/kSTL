#ifndef SBACKGROUND_HPP
#define SBACKGROUND_HPP

#include <svao.hpp>
#include <QVector4D>
#include <PlatformTheme>
#include <QColor>

class SBackground : public SVao
{
public:
    SBackground(QOpenGLShaderProgram *program);
    ~SBackground();
    void draw() override;
private:
    static QVector4D color2vec4(const QColor &color);
};

#endif // SBACKGROUND_HPP

#ifndef RENDERAREA_H
#define RENDERAREA_H

#include <QObject>
#include <QQuickImageProvider>
#include <QImage>

class RenderArea : public QQuickImageProvider
{
public:
    explicit RenderArea();
    ~RenderArea();
    QImage requestImage(const QString & id, QSize * size, const QSize & requestedSize) override;
};

#endif // RENDERAREA_H

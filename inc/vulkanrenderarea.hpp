#ifndef VULKANRENDERAREA_HPP
#define VULKANRENDERAREA_HPP

#include <QQuickItem>
#include <squirclerenderer.hpp>

class VulkanRenderArea : public QQuickItem
{
    Q_OBJECT
    Q_PROPERTY(qreal t READ t WRITE setT NOTIFY tChanged)
    QML_ELEMENT
public:
    VulkanRenderArea();

    qreal t();
    void setT(qreal t);
signals:
    void tChanged();

public slots:
    void sync();
    void cleanup();

private slots:
    void handleWindowChanged(QQuickWindow *win);

private:
    void releaseResources() override;

    qreal mT = 0;
    SquircleRenderer *mRenderer = nullptr;
};

#endif // VULKANRENDERAREA_HPP

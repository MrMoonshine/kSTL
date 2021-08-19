#ifndef VULKANRENDERAREA_HPP
#define VULKANRENDERAREA_HPP

#include <QQuickItem>
#include <squirclerenderer.hpp>
#include <model.hpp>
#include <svulkanmeta.hpp>

class VulkanRenderArea : public QQuickItem
{
    Q_OBJECT
    Q_PROPERTY(qreal t READ t WRITE setT NOTIFY tChanged)
    QML_ELEMENT
public:
    VulkanRenderArea();
    ~VulkanRenderArea();

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
    SVulkanMeta meta;
    //SquircleRenderer *mRenderer = nullptr;
    Model *mModelRenderer = nullptr;
};

#endif // VULKANRENDERAREA_HPP

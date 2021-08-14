#ifndef VULKANWIDGET_HPP
#define VULKANWIDGET_HPP

#include <QVulkanWindow>
#include <QVulkanDeviceFunctions>

class SVulkanRenderer : public QVulkanWindowRenderer
{
public:
    SVulkanRenderer(QVulkanWindow *w);

    void initResources() override;
    void initSwapChainResources() override;
    void releaseSwapChainResources() override;
    void releaseResources() override;

    void startNextFrame() override;

private:
    QVulkanWindow *mWindow;
    QVulkanDeviceFunctions *mDevFuncs;
    float mGreen = 0;
};

class VulkanWidget : public QVulkanWindow
{
public:
    QVulkanWindowRenderer *createRenderer() override;
};

#endif // VULKANWIDGET_HPP

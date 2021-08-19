#ifndef SVULKANMETA_HPP
#define SVULKANMETA_HPP
#include <QtCore/QRunnable>
#include <QtQuick/QQuickWindow>

#include <QVulkanInstance>
#include <QVulkanFunctions>
#include <QUrl>

#include <errorhandler.hpp>

class SVulkanMeta final
{
public:
    SVulkanMeta(QQuickWindow *window);
    ~SVulkanMeta();
    //Init Handling
    void init(int framesInFlight_i);
    bool isInit();
    //Syncing functions
    void setWindow(QQuickWindow *window);
    //Return Metadata
    int framesInFlight();
    QQuickWindow *window();
    VkDevice device();
    VkPhysicalDevice physicalDevice();
    QVulkanDeviceFunctions *deviceFunctions();
    QVulkanFunctions *functions();
    VkRenderPass renderPass();
    VkPipelineCache pipelineCache();
private:
    QQuickWindow *mWindow;
    bool mInitialized = false;
    int mFramesInFlight;
    VkPhysicalDevice mPhysDev = VK_NULL_HANDLE;
    VkDevice mDev = VK_NULL_HANDLE;
    QVulkanDeviceFunctions *mDevFuncs = nullptr;
    QVulkanFunctions *mFuncs = nullptr;
    VkRenderPass mRenderPass;
    VkPipelineCache mPipelineCache;
};

#endif // SVULKANMETA_HPP

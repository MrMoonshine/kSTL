#ifndef SVULKANMETA_HPP
#define SVULKANMETA_HPP
#include <QtCore/QRunnable>
#include <QtQuick/QQuickWindow>

#include <QVulkanInstance>
#include <QVulkanFunctions>
#include <QUrl>
#include <QMatrix4x4>

#include <errorhandler.hpp>

#define S_MAT4X4_SIZE 16
#define S_MAT4X4_SIZE_RAW (16*sizeof(float))
//Helper Struct for Vulkan description Sets
struct SUniformBufferObject {
    float model[S_MAT4X4_SIZE];
    float view[S_MAT4X4_SIZE];
    float proj[S_MAT4X4_SIZE];
};

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

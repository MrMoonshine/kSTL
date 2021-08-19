#include <svulkanmeta.hpp>
static const char* TAG = "VulkanMeta";

SVulkanMeta::SVulkanMeta(QQuickWindow *window_i){
    mWindow = window_i;
}

SVulkanMeta::~SVulkanMeta(){

}

bool SVulkanMeta::isInit(){
    return mInitialized;
}

void SVulkanMeta::setWindow(QQuickWindow *window_i){
    mWindow = window_i;
}

void SVulkanMeta::init(int framesInFlight_i){
    ErrorHandle::logI(TAG,"initializing...");

    Q_ASSERT(framesInFlight_i <= 3);
    mFramesInFlight = framesInFlight_i;
    mInitialized = true;

    QSGRendererInterface *rif = mWindow->rendererInterface();
    QVulkanInstance *inst = reinterpret_cast<QVulkanInstance *>(rif->getResource(mWindow, QSGRendererInterface::VulkanInstanceResource));
    Q_ASSERT(inst && inst->isValid());

    mPhysDev = *reinterpret_cast<VkPhysicalDevice *>(rif->getResource(mWindow, QSGRendererInterface::PhysicalDeviceResource));
    mDev = *reinterpret_cast<VkDevice *>(rif->getResource(mWindow, QSGRendererInterface::DeviceResource));
    Q_ASSERT(mPhysDev && mDev);

    mDevFuncs = inst->deviceFunctions(mDev);
    mFuncs = inst->functions();
    Q_ASSERT(mDevFuncs && mFuncs);

    mRenderPass = *reinterpret_cast<VkRenderPass *>(rif->getResource(mWindow, QSGRendererInterface::RenderPassResource));
    Q_ASSERT(mRenderPass);

    VkPhysicalDeviceProperties physDevProps;
    mFuncs->vkGetPhysicalDeviceProperties(mPhysDev, &physDevProps);

    VkPhysicalDeviceMemoryProperties physDevMemProps;
    mFuncs->vkGetPhysicalDeviceMemoryProperties(mPhysDev, &physDevMemProps);
}

int SVulkanMeta::framesInFlight(){
    return mFramesInFlight;
}

QQuickWindow *SVulkanMeta::window(){
    return mWindow;
}

VkDevice SVulkanMeta::device(){
    return mDev;
}

VkPhysicalDevice SVulkanMeta::physicalDevice(){
    return mPhysDev;
}

QVulkanDeviceFunctions *SVulkanMeta::deviceFunctions(){
    return mDevFuncs;
}

QVulkanFunctions *SVulkanMeta::functions(){
    return mFuncs;
}

VkRenderPass SVulkanMeta::renderPass(){
    return mRenderPass;
}

VkPipelineCache SVulkanMeta::pipelineCache(){
    return mPipelineCache;
}

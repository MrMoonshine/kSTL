#include <vulkanwidget.hpp>

SVulkanRenderer::SVulkanRenderer(QVulkanWindow *w)
    : mWindow(w)
{
}

void SVulkanRenderer::initResources(){
    qDebug() << "Init resources";
    mDevFuncs = mWindow->vulkanInstance()->deviceFunctions(mWindow->device());
}

void SVulkanRenderer::initSwapChainResources(){
    qDebug("initSwapChainResources");
}

void SVulkanRenderer::releaseSwapChainResources(){
    qDebug("releaseSwapChainResources");
}

void SVulkanRenderer::releaseResources(){
    qDebug("releaseResources");
}

void SVulkanRenderer::startNextFrame(){
    mGreen += 0.005f;
    if(mGreen > 1.0f)
        mGreen = 0.0f;

    VkClearColorValue clearColor = {{0.8f, mGreen, 0.0f, 1.0f}};
    VkClearDepthStencilValue clearDS = { 1.0f, 0 };
    VkClearValue clearValues[2];
    memset(clearValues, 0, sizeof(clearValues));
    clearValues[0].color = clearColor;
    clearValues[1].depthStencil = clearDS;

    VkRenderPassBeginInfo rpBeginInfo;
    memset(&rpBeginInfo, 0, sizeof(rpBeginInfo));
    rpBeginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    rpBeginInfo.renderPass = mWindow->defaultRenderPass();
    rpBeginInfo.framebuffer = mWindow->currentFramebuffer();
    const QSize sz = mWindow->swapChainImageSize();
    rpBeginInfo.renderArea.extent.width = sz.width();
    rpBeginInfo.renderArea.extent.height = sz.height();
    rpBeginInfo.clearValueCount = 2;
    rpBeginInfo.pClearValues = clearValues;
    VkCommandBuffer cmdBuf = mWindow->currentCommandBuffer();
    mDevFuncs->vkCmdBeginRenderPass(cmdBuf, &rpBeginInfo, VK_SUBPASS_CONTENTS_INLINE);

    //Do Nothing...

    mDevFuncs->vkCmdEndRenderPass(cmdBuf);
    mWindow->frameReady();
    mWindow->requestUpdate();
}

QVulkanWindowRenderer *VulkanWidget::createRenderer()
{
    return new SVulkanRenderer(this);
}

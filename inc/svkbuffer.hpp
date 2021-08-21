#ifndef SVKBUFFER_HPP
#define SVKBUFFER_HPP
#include <QVulkanFunctions>
#include <QVulkanInstance>
#include <svulkanmeta.hpp>
#include <errorhandler.hpp>

class SVkBuffer
{
public:
    SVkBuffer(SVulkanMeta *meta);
    ~SVkBuffer();
    void setSize(VkDeviceSize size);
    void writeData(const float *data);
    void writeData(SUniformBufferObject *data);
    VkBuffer *buffer();
    VkDeviceMemory *bufferMemory();
    VkResult create(VkDeviceSize size = 0);
protected:
    uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);

    SVulkanMeta *mMeta;
    VkDeviceSize mSize = 0;

    VkBuffer mBuffer = VK_NULL_HANDLE;
    VkDeviceMemory mBufferMemory = VK_NULL_HANDLE;
};

#endif // SVKBUFFER_HPP

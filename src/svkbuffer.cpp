#include <svkbuffer.hpp>
static const char* TAG = "Vulkan Buffer";

SVkBuffer::SVkBuffer(SVulkanMeta *meta){
    mMeta = meta;
}

SVkBuffer::~SVkBuffer(){
    mMeta->deviceFunctions()->vkDestroyBuffer(mMeta->device(), mBuffer, NULL);
    mMeta->deviceFunctions()->vkFreeMemory(mMeta->device(), mBufferMemory, NULL);
}

void SVkBuffer::setSize(VkDeviceSize size){
    mSize = size;
}

void SVkBuffer::writeData(const float *data){
    void* vdata;
    mMeta->deviceFunctions()->vkMapMemory(mMeta->device(), mBufferMemory, 0, mSize, 0, &vdata);
    memcpy(vdata, data, (size_t)mSize);
    mMeta->deviceFunctions()->vkUnmapMemory(mMeta->device(), mBufferMemory);
}

void SVkBuffer::writeData(SUniformBufferObject *data){
    void* vdata;
    mMeta->deviceFunctions()->vkMapMemory(mMeta->device(), mBufferMemory, 0, mSize, 0, &vdata);
    memcpy(vdata, data, (size_t)mSize);
    mMeta->deviceFunctions()->vkUnmapMemory(mMeta->device(), mBufferMemory);
}

VkBuffer* SVkBuffer::buffer(){
    return &mBuffer;
}

VkDeviceMemory* SVkBuffer::bufferMemory(){
    return &mBufferMemory;
}

VkResult SVkBuffer::create(VkDeviceSize size){
    if(size > 0)
        mSize = size;
    if(mSize == 0){
        ErrorHandle::logW(TAG, "No Size Specified!");
        return VK_ERROR_INITIALIZATION_FAILED;
    }

    VkBufferCreateInfo bufferInfo{};
    bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferInfo.size = mSize;
    bufferInfo.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
    bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

    if (mMeta->deviceFunctions()->vkCreateBuffer(mMeta->device(), &bufferInfo, nullptr, &mBuffer) != VK_SUCCESS) {
        ErrorHandle::logE(TAG, "failed to create vertex buffer!");
        return VK_ERROR_INITIALIZATION_FAILED;
    }

    VkMemoryRequirements memRequirements;
    mMeta->deviceFunctions()->vkGetBufferMemoryRequirements(mMeta->device(), mBuffer, &memRequirements);

    VkMemoryAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = memRequirements.size;
    allocInfo.memoryTypeIndex = findMemoryType(memRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

    if(mMeta->deviceFunctions()->vkAllocateMemory(mMeta->device(), &allocInfo, nullptr, &mBufferMemory) != VK_SUCCESS){
        ErrorHandle::logE(TAG, "Buffers setup sucessful!");
        return VK_ERROR_UNKNOWN;
    }
    mMeta->deviceFunctions()->vkBindBufferMemory(mMeta->device(), mBuffer, mBufferMemory, 0);
    return VK_SUCCESS;
}

uint32_t SVkBuffer::findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties) {
    VkPhysicalDeviceMemoryProperties memProperties;
    mMeta->functions()->vkGetPhysicalDeviceMemoryProperties(mMeta->physicalDevice(), &memProperties);

    for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
        if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties) {
            return i;
        }
    }

    ErrorHandle::logE(TAG, "failed to find suitable memory type!");
    return 0;
}

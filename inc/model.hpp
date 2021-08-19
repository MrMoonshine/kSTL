#ifndef MODEL_HPP
#define MODEL_HPP

#include <QObject>
#include <QVector3D>
#include <QVector>
#include <QQuickWindow>

#include <QVulkanFunctions>
#include <QVulkanInstance>
#include <QObject>

#include <svulkanmeta.hpp>
#include <errorhandler.hpp>

class Model : public QObject
{
    Q_OBJECT
public:
    Model(SVulkanMeta *meta);
    ~Model();
    VkResult load();
    void setViewportSize(const QSize &size);

    static VkVertexInputBindingDescription bindingDescription();
    //Information about shader Location, bindings etc...
    static VkVertexInputAttributeDescription attributeDescription();
public slots:
    void frameStart();
    void mainPassRecordingStart();
protected:
    enum Stage {
        VertexStage,
        FragmentStage
    };
    void prepareShader(Stage stage, QString filename);
    VkShaderModule createShaderModule(QByteArray *code);
    //init info
    bool mInitialized = false;
    bool mBuffersLoaded = false;
    //Shader Code (SPIR-V)
    QByteArray mVert;
    QByteArray mFrag;
    //Vulkan Data
    QSize mViewportSize;
    SVulkanMeta *mMeta;
    VkBuffer mVertexBuffer = VK_NULL_HANDLE;
    VkDeviceMemory mVbufMem = VK_NULL_HANDLE;
    VkDeviceSize mAllocPerUbuf = 0;

    VkDescriptorSetLayout mResLayout = VK_NULL_HANDLE;
    VkPipelineLayout mPipelineLayout = VK_NULL_HANDLE;
    VkPipelineCache mPipelineCache = VK_NULL_HANDLE;
    VkPipeline mGraphicsPipeline = VK_NULL_HANDLE;

    VkDescriptorPool mDescriptorPool = VK_NULL_HANDLE;
    //VkDescriptorSet mUbufDescriptor = VK_NULL_HANDLE;
private:
    void init();
    void memoryInformation();
    void createGraphicsPipeline();
    //Helper Function
    uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);
};

#endif // MODEL_HPP

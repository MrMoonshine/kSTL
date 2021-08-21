#ifndef MODEL_HPP
#define MODEL_HPP

#include <QObject>
#include <QVector3D>
#include <QVector>
#include <QQuickWindow>

#include <QVulkanFunctions>
#include <QVulkanInstance>
#include <QObject>
#include <QtMath>
#include <QMatrix4x4>
#include <QVector3D>

#include <svulkanmeta.hpp>
#include <svkbuffer.hpp>
#include <errorhandler.hpp>

class Model : public QObject
{
    Q_OBJECT
public:
    Model(SVulkanMeta *meta);
    ~Model();
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

    //Vertex Buffer
    SVkBuffer *vertexbuffer = nullptr;
    //Index Buffers
    SVkBuffer *uniformbuffer = nullptr;
    //Descriptor set data
    VkDescriptorSetLayout mDescriptorSetLayout;
    VkDescriptorPool mDescriptorPool = VK_NULL_HANDLE;
    VkDescriptorSet mDescriptorSet = VK_NULL_HANDLE;
    VkPipelineLayout mDescriptorPipelineLayout;

    VkPipelineLayout mPipelineLayout = VK_NULL_HANDLE;
    VkPipelineCache mPipelineCache = VK_NULL_HANDLE;
    VkPipeline mGraphicsPipeline = VK_NULL_HANDLE;

    VkPipelineLayout mMeshPipelineLayout = VK_NULL_HANDLE;
private:
    void init();
    void memoryInformation();
    //Load all Vetex and Uniform Buffers
    VkResult loadBuffers();
    void createDescriptorSet();
    void createConstatntsPipeline();
    void createGraphicsPipeline();
    //Helper Function
    uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);

    SUniformBufferObject ubo;
    void updateUniformBuffer();
};

#endif // MODEL_HPP

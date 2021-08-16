#ifndef SQUIRCLERENDERER_HPP
#define SQUIRCLERENDERER_HPP

#include <QObject>
#include <QtCore/QRunnable>
#include <QtQuick/QQuickWindow>

#include <QVulkanInstance>
#include <QVulkanFunctions>
#include <QUrl>

class SquircleRenderer : public QObject
{
    Q_OBJECT
public:
    ~SquircleRenderer();

    void setT(qreal t) { m_t = t; }
    void setViewportSize(const QSize &size) { m_viewportSize = size; }
    void setWindow(QQuickWindow *window) { m_window = window; }

public slots:
    void frameStart();
    void mainPassRecordingStart();

private:
    enum Stage {
        VertexStage,
        FragmentStage
    };
    void prepareShader(Stage stage);
    void init(int framesInFlight);

    QSize m_viewportSize;
    qreal m_t = 0;
    QQuickWindow *m_window;

    QByteArray m_vert;
    QByteArray m_frag;

    bool m_initialized = false;
    VkPhysicalDevice m_physDev = VK_NULL_HANDLE;
    VkDevice m_dev = VK_NULL_HANDLE;
    QVulkanDeviceFunctions *m_devFuncs = nullptr;
    QVulkanFunctions *m_funcs = nullptr;

    VkBuffer m_vbuf = VK_NULL_HANDLE;
    VkDeviceMemory m_vbufMem = VK_NULL_HANDLE;
    VkBuffer m_ubuf = VK_NULL_HANDLE;
    VkDeviceMemory m_ubufMem = VK_NULL_HANDLE;
    VkDeviceSize m_allocPerUbuf = 0;

    VkPipelineCache m_pipelineCache = VK_NULL_HANDLE;

    VkPipelineLayout m_pipelineLayout = VK_NULL_HANDLE;
    VkDescriptorSetLayout m_resLayout = VK_NULL_HANDLE;
    VkPipeline m_pipeline = VK_NULL_HANDLE;

    VkDescriptorPool m_descriptorPool = VK_NULL_HANDLE;
    VkDescriptorSet m_ubufDescriptor = VK_NULL_HANDLE;
};

#endif // SQUIRCLERENDERER_HPP
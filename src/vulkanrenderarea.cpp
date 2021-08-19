#include <vulkanrenderarea.hpp>
#include <QtCore/QRunnable>
#include <QtQuick/QQuickWindow>

#include <QVulkanInstance>
#include <QVulkanFunctions>

VulkanRenderArea::VulkanRenderArea() :
    meta(window())
{
    //Change Render Area size on window resize
    connect(this, &QQuickItem::windowChanged, this, &VulkanRenderArea::handleWindowChanged);
}

VulkanRenderArea::~VulkanRenderArea(){
    delete mModelRenderer;
}

qreal VulkanRenderArea::t(){
    return mT;
}

void VulkanRenderArea::setT(qreal t)
{
    if (t == mT)
        return;
    mT = t;
    emit tChanged();
    if (window())
        window()->update();
}

void VulkanRenderArea::sync(){
    /*if(!mRenderer){
        mRenderer = new SquircleRenderer();
        // Initializing resources is done before starting to record the
        // renderpass, regardless of wanting an underlay or overlay.
        connect(window(), &QQuickWindow::beforeRendering, mRenderer, &SquircleRenderer::frameStart, Qt::DirectConnection);
        // Here we want an underlay and therefore connect to
        // beforeRenderPassRecording. Changing to afterRenderPassRecording
        // would render the squircle on top (overlay).
        connect(window(), &QQuickWindow::beforeRenderPassRecording, mRenderer, &SquircleRenderer::mainPassRecordingStart, Qt::DirectConnection);
    }
    if(!mModelRenderer){
        mModelRenderer = new Model(window());
    }
    mRenderer->setViewportSize(window()->size() * window()->devicePixelRatio());
    mRenderer->setT(mT);
    mRenderer->setWindow(window());*/
    if(!mModelRenderer){
        mModelRenderer = new Model(&meta);
        // Initializing resources is done before starting to record the
        // renderpass, regardless of wanting an underlay or overlay.
        connect(window(), &QQuickWindow::beforeRendering, mModelRenderer, &Model::frameStart, Qt::DirectConnection);
        // Here we want an underlay and therefore connect to
        // beforeRenderPassRecording. Changing to afterRenderPassRecording
        // would render the squircle on top (overlay).
        connect(window(), &QQuickWindow::beforeRenderPassRecording, mModelRenderer, &Model::mainPassRecordingStart, Qt::DirectConnection);
    }
    mModelRenderer->setViewportSize(window()->size() * window()->devicePixelRatio());
    meta.setWindow(window());
}

void VulkanRenderArea::cleanup(){
    /*delete mRenderer;
    mRenderer = nullptr;*/

    delete mModelRenderer;
    mModelRenderer = nullptr;
}

void VulkanRenderArea::handleWindowChanged(QQuickWindow *win){
    qDebug() << "Window Changed!";
    if (win) {
        connect(win, &QQuickWindow::beforeSynchronizing, this, &VulkanRenderArea::sync, Qt::DirectConnection);
        connect(win, &QQuickWindow::sceneGraphInvalidated, this, &VulkanRenderArea::cleanup, Qt::DirectConnection);

        // Ensure we start with cleared to black. The squircle's blend mode relies on this.
        win->setColor(Qt::black);
    }
}
//Helper CLass
class CleanupJob : public QRunnable
{
public:
    CleanupJob(SquircleRenderer *renderer) : m_renderer(renderer) { }
    void run() override { delete m_renderer; }
private:
    SquircleRenderer *m_renderer;
};

class CleanupModelJob : public QRunnable
{
public:
    CleanupModelJob(Model *renderer) : m_renderer(renderer) { }
    void run() override { delete m_renderer; }
private:
    Model *m_renderer;
};

void VulkanRenderArea::releaseResources(){
    /*window()->scheduleRenderJob(new CleanupJob(mRenderer), QQuickWindow::BeforeSynchronizingStage);
    mRenderer = nullptr;*/
    window()->scheduleRenderJob(new CleanupModelJob(mModelRenderer), QQuickWindow::BeforeSynchronizingStage);
    mModelRenderer = nullptr;
}

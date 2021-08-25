#include <sglarea.hpp>


//! [7]
SGlArea::SGlArea()
    : mT(0)
    , mRenderer(nullptr)
{
    connect(this, &QQuickItem::windowChanged, this, &SGlArea::handleWindowChanged);
}

SGlArea::~SGlArea(){

}
//! [7]
qreal SGlArea::t(){
    return mT;
}
//! [8]
void SGlArea::setT(qreal t)
{
    if (t == mT)
        return;
    mT = t;
    emit tChanged();
    if (window())
        window()->update();
}

void SGlArea::setFilament(QColor color){
    //qDebug() << "Color is now: " << color.name();
    if(mRenderer)
        mRenderer->setFilamentColor(color);
}
//! [8]

//! [1]
void SGlArea::handleWindowChanged(QQuickWindow *win)
{
    if (win) {
        connect(win, &QQuickWindow::beforeSynchronizing, this, &SGlArea::sync, Qt::DirectConnection);
        connect(win, &QQuickWindow::sceneGraphInvalidated, this, &SGlArea::cleanup, Qt::DirectConnection);
//! [1]
//! [3]
        // Ensure we start with cleared to black. The squircle's blend mode relies on this.
        win->setColor(Qt::black);
    }
}
//! [3]

//! [6]
void SGlArea::cleanup()
{
    delete mRenderer;
    mRenderer = nullptr;
}

class CleanupJob : public QRunnable
{
public:
    CleanupJob(SRenderToolbox *renderer) : mRenderer(renderer) { }
    void run() override { delete mRenderer; }
private:
    SRenderToolbox *mRenderer;
};

void SGlArea::releaseResources()
{
    window()->scheduleRenderJob(new CleanupJob(mRenderer), QQuickWindow::BeforeSynchronizingStage);
    mRenderer = nullptr;
}
//! [6]

//! [9]
void SGlArea::sync()
{
    if (!mRenderer) {
        mRenderer = new SRenderToolbox();
        connect(window(), &QQuickWindow::beforeRendering, mRenderer, &SRenderToolbox::init, Qt::DirectConnection);
        connect(window(), &QQuickWindow::beforeRenderPassRecording, mRenderer, &SRenderToolbox::paint, Qt::DirectConnection);
    }
    mRenderer->setViewportSize(window()->size() * window()->devicePixelRatio());
    mRenderer->setT(mT);
    mRenderer->setWindow(window());
}

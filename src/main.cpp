#include <QApplication>
#include <QQmlApplicationEngine>
#include <QtQml>
#include <QUrl>
#include <QIcon>
#include <QQuickView>
#include <QQuickItem>
#include <QtQuickWidgets/QQuickWidget>
#include <QVBoxLayout>
#include <QLabel>

#include <KLocalizedContext>
#include <KLocalizedString>
#include <KAboutData>

#include <QVulkanInstance>

#include <controller.hpp>
#include <backendtest.hpp>
#include <renderarea.hpp>
#include <vulkanwidget.hpp>
using namespace std;

Q_DECL_EXPORT int main(int argc, char *argv[])
{
    QGuiApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication app(argc, argv);
    app.setWindowIcon(QIcon::fromTheme("applications-engineering"));
    /*QCoreApplication::setOrganizationName("NONE");
    QCoreApplication::setOrganizationDomain("none");
    QCoreApplication::setApplicationName("kSTL");
    KLocalizedString::setApplicationDomain("kSTL");*/
    //Enable Vulkan
    //QQuickWindow::setSceneGraphBackend(QSGRendererInterface::VulkanRhi);
      /*---------------------------------*/
     /*          About Data             */
    /*---------------------------------*/
    /*KAboutData aboutData(
                //Internal program name
                QStringLiteral("kstl"),
                //displayable program name
                i18nc("@title", "kSTL"),
                //program version String
                QStringLiteral("1.0"),
                //Brief description
                i18n("Viewer for STL Meshes"),
                //License
                KAboutLicense::GPL_V3,
                // Copyright Statement.
                i18n("(c) 2021")
                );
    aboutData.addAuthor(
                i18nc("@info:credit", "MrMoonshine"),
                i18nc("@info:credit", "Author Role"),
                QStringLiteral("mrmoonshine.david@gmail.com"),
                QStringLiteral("https://github.com/MrMoonshine")
                );
    KAboutData::setApplicationData(aboutData);
    qmlRegisterSingletonInstance("kSTL", 1, 0, "Controller", &Controller::instance());

    QQmlApplicationEngine engine;
    engine.rootContext()->setContextObject(new KLocalizedContext(&engine));

    //RenderArea renderarea;
    //engine.addImageProvider("myprovider", &renderarea);

    engine.load(QUrl(QStringLiteral("qrc:///main.qml")));

    if (engine.rootObjects().isEmpty()) {
        return -1;
    }

    BackendTest backendtest;
    qmlRegisterSingletonInstance<BackendTest>("kSTL", 1, 0, "Backend", &backendtest);

    QObject* rootObject = engine.rootObjects().first();
    QQuickItem *item = rootObject->findChild< QQuickItem *>("vulkanRenderSpace");
    if (item){
        item->setProperty("height", 300);
        item->setRotation(30);
    }
    else{
        qWarning() << "Requested QML element not found";
    }*/

    QVulkanInstance inst;

    #ifndef Q_OS_ANDROID
        inst.setLayers(QByteArrayList() << "VK_LAYER_LUNARG_standard_validation");
    #else
        inst.setLayers(QByteArrayList()
                       << "VK_LAYER_GOOGLE_threading"
                       << "VK_LAYER_LUNARG_parameter_validation"
                       << "VK_LAYER_LUNARG_object_tracker"
                       << "VK_LAYER_LUNARG_core_validation"
                       << "VK_LAYER_LUNARG_image"
                       << "VK_LAYER_LUNARG_swapchain"
                       << "VK_LAYER_GOOGLE_unique_objects");
    #endif

    if (!inst.create())
    qFatal("Failed to create Vulkan instance: %d", inst.errorCode());

    VulkanWidget vw;
    vw.setVulkanInstance(&inst);

    vw.resize(DEFAULT_APP_WIDTH, DEFAULT_APP_HEIGHT);
    vw.show();

    return app.exec();
}

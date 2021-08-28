#include <QApplication>
#include <QQmlApplicationEngine>
#include <QGuiApplication>
#include <QtQml>
#include <QUrl>
#include <QIcon>
#include <QQuickView>

#include <KLocalizedContext>
#include <KLocalizedString>
#include <KAboutData>

#include <KirigamiPluginFactory>
#include <PlatformTheme>

#include <controller.hpp>
#include <sglarea.hpp>

using namespace std;

Q_DECL_EXPORT int main(int argc, char *argv[])
{
    QGuiApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication app(argc, argv);
    //app.setWindowIcon(QIcon::fromTheme("applications-engineering"));
    app.setWindowIcon(QIcon::fromTheme("image-x-3ds"));
    QCoreApplication::setOrganizationName("NONE");
    QCoreApplication::setOrganizationDomain("none");
    QCoreApplication::setApplicationName("kSTL");
    KLocalizedString::setApplicationDomain("kSTL");
    //Enable Vulkan
    //QQuickWindow::setSceneGraphBackend(QSGRendererInterface::VulkanRhi);
    //Register Render Space as QML type
    qmlRegisterType<SGlArea>("kSTL", 1, 0, "GlRenderArea");
      /*---------------------------------*/
     /*          About Data             */
    /*---------------------------------*/
    KAboutData aboutData(
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
    engine.load(QUrl(QStringLiteral("qrc:///main.qml")));

    if (engine.rootObjects().isEmpty()) {
        return -1;
    }
    return app.exec();
}

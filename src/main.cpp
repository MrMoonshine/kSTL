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

#include <controller.hpp>
#include <vulqanpix.hpp>

using namespace std;

Q_DECL_EXPORT int main(int argc, char *argv[])
{
    QGuiApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication app(argc, argv);
    app.setWindowIcon(QIcon::fromTheme("applications-engineering"));
    QCoreApplication::setOrganizationName("NONE");
    QCoreApplication::setOrganizationDomain("none");
    QCoreApplication::setApplicationName("kSTL");
    KLocalizedString::setApplicationDomain("kSTL");
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
    qmlRegisterSingletonInstance("kSTL.aboutData", 1, 0, "Controller", &Controller::instance());

    QQmlApplicationEngine engine;
    engine.rootContext()->setContextObject(new KLocalizedContext(&engine));
    engine.load(QUrl(QStringLiteral("qrc:///main.qml")));

    if (engine.rootObjects().isEmpty()) {
        return -1;
    }

    VulqanPix *vpro = new VulqanPix;
    engine.rootContext()->setContextProperty("frameProvider", vpro);
    engine.addImageProvider("colors",vpro);

    QObject* rootObject = engine.rootObjects().first();
    QQuickItem *item = rootObject->findChild< QQuickItem *>("vulkanRenderSpace");
    if (item){
        item->setProperty("height", 300);
        item->setRotation(30);
    }
    else{
        qWarning() << "Requested QML element not found";
    }
    return app.exec();
}

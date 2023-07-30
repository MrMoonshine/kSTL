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

#include <Kirigami/KirigamiPluginFactory>
#include <Kirigami/PlatformTheme>

#include <controller.hpp>
#include <sglarea.hpp>

using namespace std;

Q_DECL_EXPORT int main(int argc, char *argv[])
{
    QGuiApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication app(argc, argv);
    app.setWindowIcon(QIcon::fromTheme("image-x-3ds"));
    QCoreApplication::setOrganizationName("NONE");
    QCoreApplication::setOrganizationDomain("none");
    QCoreApplication::setApplicationName("kSTL");
    KLocalizedString::setApplicationDomain("kSTL");
    //Register Render Space as QML type
    qmlRegisterType<SGlArea>("kSTL", 1, 0, "GlRenderArea");
    //qmlRegisterType<GUIErrorHandle>("kSTL", 1, 0, "ErrorHandle");
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
    GUIErrorHandle err;
    qmlRegisterSingletonInstance("kSTLErrorMessage", 1,0, "ErrorHandle",  &err);

    QQmlApplicationEngine engine;
    engine.rootContext()->setContextObject(new KLocalizedContext(&engine));

    if(app.arguments().size() < 2){
        printf("No File Specified\n");
        engine.rootContext()->setContextProperty(QStringLiteral("initialFileName"), QString(""));
    }else{
        QString input = app.arguments().at(1);
        //If the user specifies a valid file, the filepath will be converted to a valid URL
        if(QFile(input).exists())
            input = "file://" + input;
        engine.rootContext()->setContextProperty(QStringLiteral("initialFileName"), input);
    }

    engine.load(QUrl(QStringLiteral("qrc:///main.qml")));

    Controller::instance().setMessage("A working inline message!");
    err.setMessage("Oida");

    if (engine.rootObjects().isEmpty()) {
        return -1;
    }
    return app.exec();
}

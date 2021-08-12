#include <QApplication>
#include <QQmlApplicationEngine>
#include <QtQml>
#include <QUrl>
#include <QIcon>

#include <KLocalizedContext>
#include <KLocalizedString>

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

    QQmlApplicationEngine engine;

    engine.rootContext()->setContextObject(new KLocalizedContext(&engine));
    engine.load(QUrl(QStringLiteral("qrc:///main.qml")));

    if (engine.rootObjects().isEmpty()) {
        return -1;
    }

    return app.exec();
}

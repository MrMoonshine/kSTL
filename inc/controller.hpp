#ifndef CONTROLLER_HPP
#define CONTROLLER_HPP

#include <QObject>
#include <KAboutData>

#define KIRIGAMI_QML_LOG_LEVEL_INFO 0
#define KIRIGAMI_QML_LOG_LEVEL_SUCCESS 1
#define KIRIGAMI_QML_LOG_LEVEL_WARNING 2
#define KIRIGAMI_QML_LOG_LEVEL_ERROR 3

class Controller : public QObject
{
    Q_OBJECT
    Q_PROPERTY(KAboutData aboutData READ aboutData CONSTANT)
    Q_PROPERTY(QString message READ message WRITE setMessage NOTIFY messageChanged)
public:
    static Controller &instance();
    [[nodiscard]] KAboutData aboutData() const;
    //Data for error Logging
    QString message(){return mMessage;}
    void setMessage(const QString &msg);
private:
    QString mMessage = "";
    int    mLogLevel = KIRIGAMI_QML_LOG_LEVEL_INFO;
signals:
    void messageChanged();
};

#endif // CONTROLLER_HPP

#ifndef CONTROLLER_HPP
#define CONTROLLER_HPP

#include <QObject>
#include <KAboutData>

class Controller : public QObject
{
    Q_OBJECT
    Q_PROPERTY(KAboutData aboutData READ aboutData CONSTANT)
public:
    static Controller &instance();

    [[nodiscard]] KAboutData aboutData() const;
    //explicit Controller(QObject *parent = nullptr);

signals:

};

#endif // CONTROLLER_HPP

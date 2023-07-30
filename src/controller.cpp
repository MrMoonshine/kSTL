#include <controller.hpp>
static Controller _instance;
Controller &Controller::instance()
{
    return _instance;
}

KAboutData Controller::aboutData() const
{
    return KAboutData::applicationData();
}

void Controller::setMessage(const QString &msg){
    mMessage = msg;
    emit messageChanged();
}

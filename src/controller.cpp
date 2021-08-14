#include <controller.hpp>

Controller &Controller::instance()
{
    static Controller _instance;
    return _instance;
}

KAboutData Controller::aboutData() const
{
    return KAboutData::applicationData();
}

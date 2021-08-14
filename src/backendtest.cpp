#include <backendtest.hpp>

BackendTest::BackendTest(QObject *parent) : QObject(parent){

}

QString BackendTest::introductionText() const
{
    return m_introductionText;
}

void BackendTest::setIntroductionText(const QString &introductionText)
{
    m_introductionText = introductionText;
    Q_EMIT introductionTextChanged();
}


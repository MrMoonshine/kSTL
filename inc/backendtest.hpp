#ifndef BACKENDTEST_HPP
#define BACKENDTEST_HPP

#include <QObject>

class BackendTest : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString introductionText READ introductionText WRITE setIntroductionText NOTIFY introductionTextChanged)
public:
    explicit BackendTest(QObject *parent = nullptr);
    QString introductionText() const;
    void setIntroductionText(const QString &introductionText);
    Q_SIGNAL void introductionTextChanged();
private:
    QString m_introductionText = "Hello Oida!";

signals:

};

#endif // BACKENDTEST_HPP

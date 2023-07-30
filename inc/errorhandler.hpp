#pragma once
#include <QString>
#include <QQmlContext>
#include <cstdio>
#include <controller.hpp>

#include <QQuickItem>

namespace ErrorHandle {
typedef enum {DEBUG, INFO, WARNING, ERROR} Severity;
    void logD(const char* TAG, const char* msg);
    void logI(const char* TAG, const char* msg);
    void logW(const char* TAG, const char* msg);
    void logE(const char* TAG, const char* msg);
    void logUniversal(const char* TAG, const char* msg,Severity severe = DEBUG);
    void setContext(QQmlContext *root);
}

class GUIErrorHandle : public QQuickItem{
    Q_OBJECT
    //Q_PROPERTY(QString message READ message WRITE setMessage NOTIFY messageChanged)
    Q_PROPERTY(QString message MEMBER mMessage WRITE setMessage NOTIFY messageChanged)
    QML_ELEMENT
public:
    GUIErrorHandle();
    ~GUIErrorHandle();
    void setMessage(QString msg);
private:
    QString mMessage;
    bool mVisible;
    ErrorHandle::Severity mSeverity;
signals:
    void messageChanged();
};


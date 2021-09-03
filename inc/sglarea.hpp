#ifndef SGLAREA_HPP
#define SGLAREA_HPP

#include <QQuickItem>
#include <QRunnable>
#include <QQuickWindow>
#include <QColor>
#include <QVector2D>
#include <srendertoolbox.hpp>

class SGlArea : public QQuickItem
{
    Q_OBJECT
    Q_PROPERTY(qreal t READ t WRITE setT NOTIFY tChanged)
    Q_PROPERTY(QColor filament WRITE setFilament)
    Q_PROPERTY(QVector2D deltaRotation WRITE setDeltaRotation)
    Q_PROPERTY(QVector2D deltaTransform WRITE setDeltaTransform)
    Q_PROPERTY(int deltaZoom WRITE setDeltaZoom)
    Q_PROPERTY(QVector3D view READ view WRITE setView)
    Q_PROPERTY(QUrl file READ file WRITE setFile)
    QML_ELEMENT
public:
    SGlArea();
    ~SGlArea();

    qreal t();
    QVector3D view(){ return mView; }
    void setT(qreal t);
    QUrl file(){return mFile;}

    void setFilament(QColor color);
    void setDeltaRotation(QVector2D deltaMouse);
    void setDeltaTransform(QVector2D deltaMouse);
    void setDeltaZoom(int deltaWheel);
    void setView(QVector3D view);
    void setFile(const QUrl &file);
signals:
    void tChanged();

public slots:
    void sync();
    void cleanup();

private slots:
    void handleWindowChanged(QQuickWindow *win);

private:
    void releaseResources() override;

    qreal mT = 0;
    QVector3D mView;
    QUrl mFile;
    SRenderToolbox *mRenderer = nullptr;
};

#endif // SGLAREA_HPP

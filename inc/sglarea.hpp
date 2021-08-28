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
    QML_ELEMENT
public:
    SGlArea();
    ~SGlArea();

    qreal t();
    void setT(qreal t);

    void setFilament(QColor color);
    void setDeltaRotation(QVector2D deltaMouse);
    void setDeltaTransform(QVector2D deltaMouse);
    void setDeltaZoom(int deltaWheel);
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
    SRenderToolbox *mRenderer = nullptr;
};

#endif // SGLAREA_HPP

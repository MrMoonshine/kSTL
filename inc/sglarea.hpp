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
    Q_PROPERTY(QVector2D deltaMouse WRITE setDeltaMouse)
    QML_ELEMENT
public:
    SGlArea();
    ~SGlArea();

    qreal t();
    void setT(qreal t);

    void setFilament(QColor color);
    void setDeltaMouse(QVector2D deltaMouse);
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

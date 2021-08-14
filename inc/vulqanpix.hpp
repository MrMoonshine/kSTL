#ifndef VULQANPIX_HPP
#define VULQANPIX_HPP

#include <QObject>
#include <QQuickItem>
#include <QQuickImageProvider>
#include <QPixmap>
#include <QPainter>

//VulqanPix is a wrapper class to use Vulkan with a QML Image
class VulqanPix : public QObject, public QQuickImageProvider
{
    Q_OBJECT
public:
    VulqanPix(QObject* parent = nullptr);
    ~VulqanPix();
    QPixmap requestPixmap(const QString &id, QSize *size, const QSize &requestedSize);
protected:
private:
};

#endif // VULQANPIX_HPP

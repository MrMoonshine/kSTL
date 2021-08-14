#include <renderarea.hpp>

RenderArea::RenderArea() : QQuickImageProvider(QQuickImageProvider::Image)
{

}

RenderArea::~RenderArea(){

}

QImage RenderArea::requestImage(const QString & id, QSize * size, const QSize & requestedSize){
    qDebug() << "id: " << id;
    qDebug() << "Size: " << size->width() + " " + size->height();
    qDebug() << "reequestedSize: " << requestedSize.width() + " " + requestedSize.height();
    QImage image(TEST_IMAGE);
    image.save("oida.png");
    return image.scaled(DEFAULT_RENDER_HEIGHT,DEFAULT_RENDER_WIDTH);
}

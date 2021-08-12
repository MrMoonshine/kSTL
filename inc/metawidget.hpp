#ifndef METAWIDGET_HPP
#define METAWIDGET_HPP

#include <QObject>
#include <QWidget>
#include <QFrame>
#include <QLabel>
#include <QVBoxLayout>

class MetaWidget : public QFrame{
    Q_OBJECT
public:
    explicit MetaWidget(QWidget *parent = nullptr);
    ~MetaWidget() override;
private:
    QLabel title;
    QVBoxLayout vlay;
signals:

};

#endif // METAWIDGET_HPP

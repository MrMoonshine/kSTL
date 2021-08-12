#include <metawidget.hpp>

MetaWidget::MetaWidget(QWidget *parent) : QFrame(parent){
    setFrameStyle(QFrame::Panel | QFrame::Raised);
    setLineWidth(2);
    setMaximumWidth(250);

    title.setText("Meta Informations");
    vlay.addWidget(&title);
    setLayout(&vlay);
}

MetaWidget::~MetaWidget(){

}

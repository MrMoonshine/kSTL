#include <svao.hpp>

static void dumpMat4(float *m4, const char* title){
    printf("----------------------------------------------\n");
    printf("\t%s\n",title);
    printf("----------------------------------------------\n");
    for(int a = 0; a < 16; a++){
        printf("%.2f\t", m4[a]);
        if(a % 4 == 3)
            printf("\n");
    }
    printf("----------------------------------------------\n");
}

SVao::SVao(QOpenGLShaderProgram *program, QObject *parent) : QObject(parent)
{
    mProgram = program;

    initializeOpenGLFunctions();
    mVao.create();
    mVao.bind();
}

SVao::~SVao(){
    mVao.destroy();
}

void SVao::setViewportSize(QSize *size){
    if(size){
        mViewportSize = size;
        //qDebug() << "Size is: " << mViewportSize->width() << "x" << mViewportSize->height();
    }
}

void SVao::draw(){

}

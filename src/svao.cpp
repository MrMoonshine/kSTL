#include <svao.hpp>

static const size_t rawVertexbufferSize = 12 * 3 * 3 * sizeof(float);
static const float rawVertexBuffer[] = {
    -1.0f,-1.0f,-1.0f,
    -1.0f,-1.0f, 1.0f,
    -1.0f, 1.0f, 1.0f,
     1.0f, 1.0f,-1.0f,
    -1.0f,-1.0f,-1.0f,
    -1.0f, 1.0f,-1.0f,
     1.0f,-1.0f, 1.0f,
    -1.0f,-1.0f,-1.0f,
     1.0f,-1.0f,-1.0f,
     1.0f, 1.0f,-1.0f,
     1.0f,-1.0f,-1.0f,
    -1.0f,-1.0f,-1.0f,
    -1.0f,-1.0f,-1.0f,
    -1.0f, 1.0f, 1.0f,
    -1.0f, 1.0f,-1.0f,
     1.0f,-1.0f, 1.0f,
    -1.0f,-1.0f, 1.0f,
    -1.0f,-1.0f,-1.0f,
    -1.0f, 1.0f, 1.0f,
    -1.0f,-1.0f, 1.0f,
     1.0f,-1.0f, 1.0f,
     1.0f, 1.0f, 1.0f,
     1.0f,-1.0f,-1.0f,
     1.0f, 1.0f,-1.0f,
     1.0f,-1.0f,-1.0f,
     1.0f, 1.0f, 1.0f,
     1.0f,-1.0f, 1.0f,
     1.0f, 1.0f, 1.0f,
     1.0f, 1.0f,-1.0f,
    -1.0f, 1.0f,-1.0f,
     1.0f, 1.0f, 1.0f,
    -1.0f, 1.0f,-1.0f,
    -1.0f, 1.0f, 1.0f,
     1.0f, 1.0f, 1.0f,
    -1.0f, 1.0f, 1.0f,
     1.0f,-1.0f, 1.0f
};

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

SVao::SVao(QOpenGLShaderProgram *program, QObject *parent) : QObject(parent), mVbo(QOpenGLBuffer::VertexBuffer)
{
    mProgram = program;

    initializeOpenGLFunctions();
    mVao.create();
    mVao.bind();
    mVbo.create();
    mVbo.setUsagePattern(QOpenGLBuffer::StaticDraw);
    mVbo.bind();
    mVbo.allocate(rawVertexBuffer, rawVertexbufferSize);
    mVbo.release();
}

SVao::~SVao(){
    mVbo.destroy();
    mVao.destroy();
}

void SVao::setColor(QColor *color){
    mFilamentColor = color;
}

void SVao::updateUniformBuffer(){
    QVector3D eye(40, 30, -30);
    QVector3D direction(0, 0, 0);
    QVector3D up(0, 1, 0);

    model.setToIdentity();
    view.setToIdentity();
    proj.setToIdentity();

    //float ratio = mViewportSize ? mViewportSize->width() / mViewportSize->height() : 4.0f/3.0f;
    float ratio = 4.0f/3.0f;
    model.scale(QVector3D(0.2,0.2,0.2));
    view.lookAt(eye, direction, up);
    proj.perspective(
                M_PI/4.0f,
                ratio,
                0.1f,
                100.0f
    );

    //view.setToIdentity();
    //proj.setToIdentity();

    //dumpMat4((proj * view * model).data() , "MVP");
}

void SVao::setViewportSize(QSize *size){
    if(size)
    mViewportSize = size;
}

void SVao::draw(){
    mVao.bind();
    mProgram->bind();

    glEnable(GL_DEPTH_TEST);
    glDisable(GL_BLEND);
    glDisable(GL_CULL_FACE);

    updateUniformBuffer();
    mProgram->setUniformValue("model", model);
    mProgram->setUniformValue("view", view);
    mProgram->setUniformValue("proj", proj);

    if(mFilamentColor)
        mProgram->setUniformValue("filament", QVector3D(mFilamentColor->redF(), mFilamentColor->greenF(), mFilamentColor->blueF()));
    else
        mProgram->setUniformValue("filament", QVector3D(0,0,0));

    mProgram->enableAttributeArray(0);
    mVbo.bind();
    mProgram->setAttributeBuffer(
                0,
                GL_FLOAT,
                0,
                3,
                0
    );
    //glBlendFunc(GL_SRC_ALPHA, GL_ONE);
    glDrawArrays(GL_TRIANGLES, 0, 12 * 3);

    mProgram->disableAttributeArray(0);
    mVbo.release();
    mProgram->release();
    mVao.release();
}

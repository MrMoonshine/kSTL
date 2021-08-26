#include <smeshstl.hpp>
static const char* TAG = "STL";
/*static const size_t rawVertexbufferSize = 12 * 3 * 3 * sizeof(float);
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
*/
SMeshSTL::SMeshSTL(QOpenGLShaderProgram *program, QObject *parent) :
    SVao(program, parent),
    mVertexBO(QOpenGLBuffer::VertexBuffer),
    mNormalsBO()
{
    mVertexBO.create();
    mNormalsBO.create();
    loadModel(QUrl("file:///home/david/3D-Druck/3DBenchy.stl"));
    //loadModel(QUrl("file:///home/david/3D-Druck/schwein.stl"));
    //loadModel(QUrl("file:///home/david/3D-Druck/Coding-Test/oida-Würfel.stl"));
    mModelUp = QVector3D(0, 1, 0);
}

SMeshSTL::~SMeshSTL(){
    mVertexBO.destroy();
    mNormalsBO.destroy();
}

void SMeshSTL::setColor(QColor *color){
    mFilamentColor = color;
}

void SMeshSTL::setDeltaMouse(QVector2D deltaMouse){
    //qDebug() << deltaMouse.x();
    mPhi += deltaMouse.x() / 4.0f;
    mTheta += deltaMouse.y() / 4.0f;
}

int SMeshSTL::loadModel(const QUrl &model){
    QByteArray data;
    if(model.isLocalFile()){
        qDebug() << model.fileName() << " is a local file: " << model.isLocalFile();
    }else{
        ErrorHandle::logW(TAG, "only local files are supported yet");
        return EXIT_FAILURE;
    }

    size_t verticesSize, normalsSize;
    float *vertices;
    float *normals;
    stl_model_init(model.toLocalFile().toStdString().c_str(), NULL, &verticesSize, NULL, &normalsSize);
    vertices = (float*)malloc(verticesSize);
    normals = (float*)malloc(normalsSize);
    stl_model_init(model.toLocalFile().toStdString().c_str(), vertices, &verticesSize, normals, &normalsSize);

    mVertexCount = verticesSize / (STL_VERTEX_FLOAT_COUNT * sizeof(float));

    mVertexBO.setUsagePattern(QOpenGLBuffer::DynamicDraw);
    mVertexBO.bind();
    mVertexBO.allocate(vertices, verticesSize);
    mVertexBO.release();

    mNormalsBO.setUsagePattern(QOpenGLBuffer::DynamicDraw);
    mNormalsBO.bind();
    mNormalsBO.allocate(normals, normalsSize);
    mNormalsBO.release();

    /*for(unsigned int a = 0; a < normalsSize/sizeof(float); a++){
        printf("%.2f\t", normals[a]);
        if(a % 3 == 2)
            printf("\n");
    }*/

    free(normals);
    free(vertices);
    return EXIT_SUCCESS;
}

void SMeshSTL::updateUniformBuffer(){
    QVector3D eye(120,80,80);
    QVector3D direction(0, 0, 0);
    QVector3D up(0, 1, 0);

    eye.setX(mRadius * sin(qDegreesToRadians(mPhi)) * cos(qDegreesToRadians(mTheta)));
    eye.setY(mRadius * sin(qDegreesToRadians(mPhi)) * sin(qDegreesToRadians(mTheta)));
    eye.setZ(mRadius * cos(qDegreesToRadians(mPhi)));

    model.setToIdentity();
    view.setToIdentity();
    proj.setToIdentity();
    //float ratio = mViewportSize ? mViewportSize->width() / mViewportSize->height() : 4.0f/3.0f;
    float ratio = 4.0f/3.0f;
    model.scale(QVector3D(0.02,0.02,0.02));

    //model.rotate(mPhi, QVector3D(0,1,0));
    //model.rotate(mTheta, QVector3D(1,0,0));
    model.rotate(-90, QVector3D(1,0,0));
    view.lookAt(eye, direction, up);
    proj.perspective(
                M_PI/4.0f,
                ratio,
                0.1f,
                300.0f
    );
    mProgram->setUniformValue("eye", eye);
}

void SMeshSTL::draw(){
    mVao.bind();
    mProgram->bind();

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
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
    mVertexBO.bind();
    mProgram->setAttributeBuffer(
                0,
                GL_FLOAT,
                0,
                3,
                0
    );
    mProgram->enableAttributeArray(1);
    mVertexBO.bind();
    mProgram->setAttributeBuffer(
                1,
                GL_FLOAT,
                0,
                3,
                0
    );
    //glBlendFunc(GL_SRC_ALPHA, GL_ONE);
    glDrawArrays(GL_TRIANGLES, 0, mVertexCount * 3);

    mProgram->disableAttributeArray(0);
    mProgram->disableAttributeArray(1);
    mVertexBO.release();
    mProgram->release();
    mVao.release();
}

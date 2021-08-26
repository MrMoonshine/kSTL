#include <smeshstl.hpp>
static const char* TAG = "STL";
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

SMeshSTL::SMeshSTL(QOpenGLShaderProgram *program, QObject *parent) :
    SVao(program, parent),
    mVertexBO(QOpenGLBuffer::VertexBuffer),
    mNormalsBO()
{
    mVertexBO.create();
    mVertexBO.setUsagePattern(QOpenGLBuffer::DynamicDraw);
    mVertexBO.bind();
    mVertexBO.allocate(rawVertexBuffer, rawVertexbufferSize);
    mVertexBO.release();
    loadModel(QUrl("file:///home/david/3D-Druck/3DBenchy.stl"));
    //loadModel(QUrl("file:///home/david/3D-Druck/Coding-Test/oida-Würfel.stl"));
    /*QVector3D eye;
    float theta = M_PI_4;
    float phi = M_PI_4;
    eye.setX(mRadius * sin(phi) * cos(theta));
    eye.setY(mRadius * sin(phi) * sin(theta));
    eye.setZ(mRadius * cos(phi));
    qDebug() << "My Eye Position is: (" << eye.x() << "|"<< eye.y() << "|"<< eye.z() << ")";*/
    mModelUp = QVector3D(0, 1, 0);
}

SMeshSTL::~SMeshSTL(){
    mVertexBO.destroy();
}

void SMeshSTL::setColor(QColor *color){
    mFilamentColor = color;
}

void SMeshSTL::setDeltaMouse(QVector2D deltaMouse){
    //qDebug() << deltaMouse.x();
    mPhi += deltaMouse.x() / 4.0f;
    mTheta += deltaMouse.y() / 4.0f;
}

static int32_t fptr_to_int_2_10_10_10_rev(float *vector){
    int32_t result = 0;
    result |= (int)(STL_GL_10_MAX * vector[2]) << 0;
    result |= (int)(STL_GL_10_MAX * vector[1]) << 10;
    result |= (int)(STL_GL_10_MAX * vector[0]) << 20;
    result |= (int)(0b11) << 30;
    return result;
}

int SMeshSTL::loadModel(const QUrl &model){
    QByteArray data;
    if(model.isLocalFile()){
        qDebug() << model.fileName() << " is a local file: " << model.isLocalFile();
        QFile file(model.toLocalFile());
        file.open(QIODevice::ReadOnly);
        data = file.readAll();
    }else{
        ErrorHandle::logW(TAG, "only local files are supported yet");
        return EXIT_FAILURE;
    }

    if(data.length() < 1){
        qWarning() << "Failed to read file";
        return EXIT_FAILURE;
    }
    //Read Header
    QString header(data.left(STL_HEADER_SIZE).data());
    data.remove(0,STL_HEADER_SIZE);
    //Read number of Vertices
    int32_t numberOfVertices;
    QByteArray numberOfVerticesBarr = data.left(sizeof(int32_t));
    memcpy(&numberOfVertices, numberOfVerticesBarr.data(), sizeof (int32_t));
    data.remove(0,sizeof(int32_t));
    //Prepare Buffers
    float *vertices;
    int32_t *normals;
    vertices = (float*)malloc(numberOfVertices * STL_VERTEX_FLOAT_COUNT * sizeof(float));
    //Each vertex has a normal vector. The open GL renderer requires 3 normals per vertex
    normals = (int32_t*)malloc(numberOfVertices * 3 * sizeof(int32_t));
    //Read Vertices
    for(int32_t a = 0; a < numberOfVertices; a++){
        qDebug() << "Vertex #" << a;
        //Normal Buffer
        float normalF[3];
        //12 bytes for the normal vector
        QByteArray normalArr = data.left(12);
        data.remove(0,12);
        memcpy(normalF, normalArr.data(), 3 * sizeof(float));
        int32_t normalU = fptr_to_int_2_10_10_10_rev(normalF);
        for(int b = 0; b < 3; b++)
        normals[a*3 + b] = normalU;

        //Vertex Buffer
        for(int b = 0; b < STL_VERTEX_FLOAT_COUNT; b++){
            uint32_t tentative_index = a * STL_VERTEX_FLOAT_COUNT + b;
            QByteArray vertexArr = data.left(sizeof(float));
            data.remove(0, sizeof(float));
            memcpy(vertices + tentative_index, vertexArr.data(), sizeof(float));
        }
        //Nobody cares about the attributes
        data.remove(0,2);
    }
    data.clear();
    free(vertices);
    free(normals);
    qDebug() << "Header is: " << header;
    qDebug() << "Vertices: " << numberOfVertices;
    return EXIT_SUCCESS;
}

void SMeshSTL::updateUniformBuffer(){
    QVector3D eye(60,40,40);
    QVector3D direction(0, 0, 0);
    QVector3D up(0, 1, 0);

    model.setToIdentity();
    view.setToIdentity();
    proj.setToIdentity();

    //float ratio = mViewportSize ? mViewportSize->width() / mViewportSize->height() : 4.0f/3.0f;
    float ratio = 4.0f/3.0f;
    model.scale(QVector3D(0.2,0.2,0.2));

    model.rotate(mPhi, QVector3D(0,1,0));
    model.rotate(mTheta, QVector3D(1,0,0));

    view.lookAt(eye, direction, up);
    proj.perspective(
                M_PI/4.0f,
                ratio,
                0.1f,
                100.0f
    );
}

void SMeshSTL::draw(){
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
    mVertexBO.bind();
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
    mVertexBO.release();
    mProgram->release();
    mVao.release();
}

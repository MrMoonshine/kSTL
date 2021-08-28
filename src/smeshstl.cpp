#include <smeshstl.hpp>
static const char* TAG = "STL";
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
    mDeltaMove = QVector2D(0,0);
    mTransform = QVector3D(0,0,0);

    //Initial 45°
    mPhi = 45;
    mTheta = 45;
}

SMeshSTL::~SMeshSTL(){
    mVertexBO.destroy();
    mNormalsBO.destroy();
}

void SMeshSTL::setColor(QColor *color){
    mFilamentColor = color;
}

void SMeshSTL::setDeltaRotation(QVector2D deltaMouse){
    //qDebug() << deltaMouse.x();
    if(mPhi + deltaMouse.y()/4.0f > 360){
        mPhi = mPhi + deltaMouse.y()/4.0f - 360;
        qDebug() << "Here 1";
    }else if(mPhi + deltaMouse.y()/4.0f < 0){
        mPhi = 360 + (mPhi + deltaMouse.y()/4.0f);
        qDebug() << "Here 2";
    }else{
        mPhi += deltaMouse.y() / 4.0f;
    }

    if(mTheta + deltaMouse.x()/4.0f > 360){
        mTheta = mTheta + deltaMouse.x()/4.0f - 360;
        qDebug() << "Here 3";
    }else if(mTheta + deltaMouse.x()/4.0f < 0){
        mTheta = 360 + (mTheta + deltaMouse.x()/4.0f);
        qDebug() << "Here 4";
    }else{
        mTheta += deltaMouse.x() / 4.0f;
    }
}

void SMeshSTL::setDeltaTransform(QVector2D deltaMouse){
    //qDebug() << "Delta Transform: (" << deltaMouse.x() << "|" << deltaMouse.y() << ")";
    mDeltaMove = deltaMouse;
}

void SMeshSTL::setDeltaZoom(int deltaWheel){
    //qDebug() << "Zoom delta: " << deltaWheel/MOUSE_WHEEL_ROTATION_STEP_SIZE;
    mZoom *= (25.0f - deltaWheel/MOUSE_WHEEL_ROTATION_STEP_SIZE)/25.0f;
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

void SMeshSTL::handleTransformation(const QVector3D &eye){
    if(mDeltaMove.x() == 0 && mDeltaMove.y() == 0)
        return;

    //qDebug() << "Moving..." << mDeltaMove.x();
    qDebug() << "Phi:\t" << mPhi << "\nTheta:\t" << mTheta;
    //qDebug() << "eye vector: (" << eye.x() << "|" << eye.y() << "|" << eye.z() << ")";
    QVector3D px(
                eye.z(),
                0,
                -eye.x()
    );
    px.normalize();

    QVector3D py = QVector3D::crossProduct(px, eye);
    py.normalize();

    //qDebug() << "X vector: (" << px.x() << "|" << px.y() << "|" << px.z() << ")";
    mTransform += 0.1 * (px * mDeltaMove.x() + py * mDeltaMove.y());

    mDeltaMove.setX(0);
    mDeltaMove.setY(0);
}

void SMeshSTL::updateUniformBuffer(){
    QVector3D eye(0,0,0);
    QVector3D direction(0, 0, 0);
    QVector3D up(0, 1, 0);

    /*if(mTheta == 360){
        mPhi++;
        mTheta = mPhi == 360 ? 0 : mTheta;
    }else if(mPhi == 360){
        mTheta++;
        mPhi = mTheta == 360 ? 0 : mPhi;
    }*/

    eye.setX(mRadius * sin(qDegreesToRadians(mPhi)) * cos(qDegreesToRadians(mTheta)));
    eye.setZ(mRadius * sin(qDegreesToRadians(mPhi)) * sin(qDegreesToRadians(mTheta)));
    eye.setY(mRadius * cos(qDegreesToRadians(mPhi)));

    model.setToIdentity();
    view.setToIdentity();
    proj.setToIdentity();
    //float ratio = mViewportSize ? mViewportSize->width() / mViewportSize->height() : 4.0f/3.0f;
    float ratio = 4.0f/3.0f;
    model.scale(QVector3D(0.02,0.02,0.02));
    //Handle the transformation from the mouse input
    handleTransformation(eye);
    model.translate(mTransform);
    //model.rotate(mPhi, QVector3D(0,1,0));
    //model.rotate(mTheta, QVector3D(1,0,0));
    model.rotate(-90, QVector3D(1,0,0));
    view.lookAt(eye, direction, up);

    float FOV = (M_PI/4.0f) * mZoom;
    proj.perspective(
                FOV,
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

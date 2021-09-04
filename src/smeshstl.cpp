#include <smeshstl.hpp>
static const char* TAG = "STL";
SMeshSTL::SMeshSTL(QOpenGLShaderProgram *program, QObject *parent) :
    SVao(program, parent)
{
    createBuffers();
    //loadModel(QUrl("file:///home/david/3D-Druck/3DBenchy.stl"));
    loadModel(QUrl("file:///home/david/3D-Druck/Coding-Test/hextest.stl"));
    mModelUp = QVector3D(0, 1, 0);
    mDeltaMove = QVector2D(0,0);
    mTransform = QVector3D(0,0,0);
    setView(QVector3D(1, 45, 45));
}

SMeshSTL::~SMeshSTL(){
    deleteBuffers();
}

void SMeshSTL::createBuffers(){
    mVao.bind();
    glGenBuffers(1, &mVertexBuff);
    glGenBuffers(1, &mNormalsBuff);
    mVao.release();
}

void SMeshSTL::deleteBuffers(){
    glDeleteBuffers(1, &mVertexBuff);
    glDeleteBuffers(1, &mNormalsBuff);
}

void SMeshSTL::setColor(QColor *color){
    mFilamentColor = color;
}

static float angleClamp(float angle, float deltaMouse){
    static const float mouseDivisor = 3.5f;
    float rangle = angle;
    if(angle + deltaMouse/mouseDivisor > 360){
        rangle = angle + deltaMouse/mouseDivisor - 360;
    }else if(angle + deltaMouse/mouseDivisor < 0){
        rangle = 360 + (angle + deltaMouse/mouseDivisor);
    }else{
        rangle += deltaMouse / mouseDivisor;
    }
    return rangle;
}

void SMeshSTL::setDeltaRotation(QVector2D deltaMouse){
    mPhi = angleClamp(mPhi, -deltaMouse.y());
    if(mPhi < 180){
        mHemisphere = UPPER;
        mTheta = angleClamp(mTheta, deltaMouse.x());
    }else{
        mHemisphere = LOWER;
        mTheta = angleClamp(mTheta, -deltaMouse.x());
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

void SMeshSTL::queueModelLoad(const QUrl &model){
    mURLNewFile = model;
    mLoadNewFile = true;
}

int SMeshSTL::loadModel(const QUrl &model){
    mLoadNewFile = false;
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

    stl_meta(&mMeta, vertices, verticesSize);

    mRadius = mMeta.xmax - mMeta.xmin;
    mRadius = mRadius < mMeta.ymax - mMeta.ymin ? mMeta.ymax - mMeta.ymin : mRadius;
    mRadius = mRadius < mMeta.zmax - mMeta.zmin ? mMeta.zmax - mMeta.zmin : mRadius;
    mRadius *= MODEL_RADIUS_INITIAL_FACTOR;

    mVertexCount = verticesSize / (STL_VERTEX_FLOAT_COUNT * sizeof(float));
    mVao.bind();

    glBindBuffer(GL_ARRAY_BUFFER, mVertexBuff);
    glBufferData(GL_ARRAY_BUFFER, verticesSize, vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, mNormalsBuff);
    glBufferData(GL_ARRAY_BUFFER, normalsSize, normals, GL_STATIC_DRAW);

       qDebug() << "Normals size is: " << normalsSize/(sizeof(float)*3);

    mVao.release();
    free(normals);
    free(vertices);
    return EXIT_SUCCESS;
}

void SMeshSTL::setView(const QVector3D &view){
    //Initial 45°
    mPhi = view.z();
    mTheta = view.y();
    mHemisphere = UPPER;

    mTransform.setX(0);
    mTransform.setY(0);
    mTransform.setZ(0);
}

void SMeshSTL::handleTransformation(const QVector3D &eye){
    if(mDeltaMove.x() == 0 && mDeltaMove.y() == 0)
        return;

    /*qDebug() << "Moving..." << mDeltaMove.x();
    qDebug() << "Phi:\t" << mPhi << "\nTheta:\t" << mTheta;
    qDebug() << "Sin Phi:\t" << sin(qDegreesToRadians(mPhi)) << "\nSin Theta:\t" << sin(qDegreesToRadians(mTheta));
    qDebug() << "eye vector: (" << eye.x() << "|" << eye.y() << "|" << eye.z() << ")";*/
    QVector3D px(
                eye.z(),
                0,
                -eye.x()
    );
    px.normalize();

    QVector3D py = QVector3D::crossProduct(px, eye);
    py.normalize();

    //qDebug() << "X vector: (" << px.x() << "|" << px.y() << "|" << px.z() << ")";
    mTransform += 0.25 * (px * mDeltaMove.x() + py * mDeltaMove.y());

    mDeltaMove.setX(0);
    mDeltaMove.setY(0);
}

void SMeshSTL::updateUniformBuffer(){
    QVector3D eye(0,0,0);
    QVector3D direction(0, 0, 0);
    QVector3D up(0, 1, 0);
    //Calculate the Spherical Coordinates
    eye.setX(mRadius * sin(qDegreesToRadians(mPhi)) * cos(qDegreesToRadians(mTheta)));
    eye.setY(mRadius * cos(qDegreesToRadians(mPhi)));
    eye.setZ(mRadius * sin(qDegreesToRadians(mPhi)) * sin(qDegreesToRadians(mTheta)));
    //Init all Matrices
    model.setToIdentity();
    view.setToIdentity();
    proj.setToIdentity();

    model.scale(QVector3D(0.02,0.02,0.02));
    //Handle the transformation from the mouse input
    handleTransformation(eye);
    model.translate(mTransform);
    //Rotate the model to "switch" the Z and Y axis
    model.rotate(-90, QVector3D(1,0,0));
    //Generate View Matrix
    view.lookAt(eye, direction, up);
    //Calculate Perspective Matrix
    float FOV = (M_PI/4.0f) * mZoom;
    float ratio = mViewportSize ? (float)mViewportSize->width() / (float)mViewportSize->height() : 4.0f/3.0f;
    proj.perspective(
                FOV,
                ratio,
                0.1f,
                300.0f
    );
    //A simple way to hide the flipping at the zenith
    if(mHemisphere == LOWER){
        proj.scale(-1,-1,1);
    }
    //Used to process the Fragment shader
    mProgram->setUniformValue("eye", eye);
}

void SMeshSTL::draw(){
    if(mLoadNewFile){
        loadModel(mURLNewFile);
    }
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
    glBindBuffer(GL_ARRAY_BUFFER, mVertexBuff);
    mProgram->setAttributeBuffer(
                0,
                GL_FLOAT,
                0,
                3,
                0
    );

    mProgram->enableAttributeArray(1);
    //glBindBuffer(GL_ARRAY_BUFFER, mNormalsBuff);
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

    mProgram->release();
    mVao.release();
}

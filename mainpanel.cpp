#include "mainpanel.h"

MainPanel::MainPanel(QWidget *parent) :
    QGLWidget(parent) {

    // shaders are disabled by default
    gbEnabled = false;
    bfEnabled = false;
    edEnabled = false;
}

void MainPanel::initializeGL() {
    qDebug() << "OpenGL version: " << (char*)glGetString(GL_VERSION);
    qDebug() << "initializing GL";
    initializeGLFunctions();

    // setting background color
    glClearColor( 0.0f, 0.0f, 0.0f, 1.0f );

    // specifying window coords
    glViewport(0, 0, width(), height());

    // creating 3D object to draw onto
    createQuad();

    // creating shaders
    createShaders();
}

void MainPanel::createShaders() {

    // creating the shader for the original image
    shaderProgram = new QGLShaderProgram;

    // the vertex shader
    vertexShader = new QGLShader(QGLShader::Vertex);
    vertexShader->compileSourceFile(":/shaders/vertex_shader.vsh");

    // the fragment shader
    fragmentShader = new QGLShader(QGLShader::Fragment);
    fragmentShader->compileSourceFile(":/shaders/original.fsh");

    // linking shaders in program
    shaderProgram->addShader(vertexShader);
    shaderProgram->addShader(fragmentShader);

    shaderProgram->bindAttributeLocation("in_Position", 0);
    shaderProgram->bindAttributeLocation("in_TexCoord", 1);
    shaderProgram->link();

    // creating the shader for gaussian blur algorithm
    gbShaderProgram = new QGLShaderProgram;

    // the vertex shader
    gbVertexShader = new QGLShader(QGLShader::Vertex);
    gbVertexShader->compileSourceFile(":/shaders/vertex_shader.vsh");

    // the fragment shader
    gbFragmentShader = new QGLShader(QGLShader::Fragment);
    gbFragmentShader->compileSourceFile(":/shaders/gaussian_blur.fsh");

    // linking shaders in program
    gbShaderProgram->addShader(gbVertexShader);
    gbShaderProgram->addShader(gbFragmentShader);
    gbShaderProgram->link();

    // creating the shader for bilateral filter algorithm
    bfShaderProgram = new QGLShaderProgram;

    // the vertex shader
    bfVertexShader = new QGLShader(QGLShader::Vertex);
    bfVertexShader->compileSourceFile(":/shaders/vertex_shader.vsh");

    // the fragment shader
    bfFragmentShader = new QGLShader(QGLShader::Fragment);
    bfFragmentShader->compileSourceFile(":/shaders/bilateral_filter.fsh");

    // linking shaders in program
    bfShaderProgram->addShader(bfVertexShader);
    bfShaderProgram->addShader(bfFragmentShader);
    bfShaderProgram->link();

    // creating the shader for edge detection algorithm
    edShaderProgram = new QGLShaderProgram;

    // the vertex shader
    edVertexShader = new QGLShader(QGLShader::Vertex);
    edVertexShader->compileSourceFile(":/shaders/vertex_shader.vsh");

    // the fragment shader
    edFragmentShader = new QGLShader(QGLShader::Fragment);
    edFragmentShader->compileSourceFile(":/shaders/edge_detection.fsh");

    // linking shaders in program
    edShaderProgram->addShader(edVertexShader);
    edShaderProgram->addShader(edFragmentShader);
    edShaderProgram->link();
}

void MainPanel::createQuad() {

    // vertices' position array of coords
    float position[] = {1.0f, -1.0f, 0.0f,
                        1.0f, 1.0f, 0.0f,
                        -1.0f, 1.0f, 0.0f,
                        -1.0f, 1.0f, 0.0f,
                        -1.0f, -1.0f, 0.0f,
                        1.0f, -1.0f, 0.0f
                       };

    // vertices' texture array of coords
    float texture[] = {1.0f, 0.0f,
                       1.0f, 1.0f,
                       0.0f, 1.0f,
                       0.0f, 1.0f,
                       0.0f, 0.0f,
                       1.0f, 0.0f
                      };

    // creating vertex array object
    vao = new QOpenGLVertexArrayObject(this);
    vao->create();
    vao->bind();

    // creating the position vertex buffer object
    vboPosition = new QOpenGLBuffer;
    vboPosition->setUsagePattern(QOpenGLBuffer::StaticDraw);
    vboPosition->create();
    vboPosition->bind();
    vboPosition->allocate(position, 6*3*sizeof(float));

    // giving the vao the 0 index
    glVertexAttribPointer(0, 3, GL_FLOAT, false, 0, 0);

    // creating the position vertex buffer object
    vboTexture = new QOpenGLBuffer;
    vboTexture->setUsagePattern(QOpenGLBuffer::StaticDraw);
    vboTexture->create();
    vboTexture->bind();
    vboTexture->allocate(texture, 6*2*sizeof(float));

    // giving the vao the 0 index
    glVertexAttribPointer(1, 2, GL_FLOAT, false, 0, 0);

    // releasing the objects
    vao->release();
    vboPosition->release();
    vboTexture->release();
}

void MainPanel::resizeGL(int w, int h) {
    qDebug() << "resizeGL";

    // specifying window coords
    glViewport(0, 0, w, h);
}

void MainPanel::paintGL() {
    qDebug() << "paintGL";

    // clearing the gl widget background
    glClear(GL_COLOR_BUFFER_BIT);

    if(gbEnabled) {
        gbShaderProgram->bind();
    } else if(bfEnabled) {
        bfShaderProgram->bind();
    } else if(edEnabled) {
        edShaderProgram->bind();
    } else {
        shaderProgram->bind();
    }
    glBindTexture(GL_TEXTURE_2D, textID[0]);

    // enabling the vao
    vao->bind();
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    // drawing the quad and disposing the vao
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);

    // releasing the vao
    vao->release();
}

void MainPanel::loadImage(QString fileName) {
    QImageReader reader(fileName);
    QImage image;
    reader.read(&image);
    image = convertToGLFormat(image);
    qDebug() << "image width: " << image.width() << ", image height: " << image.height();
    resize(image.width(), image.height());
    qDebug() << "widget width: " << width() << ", widget height: " << height();
    ((QMainWindow*)parent())->layout();

    makeCurrent();
    glGenTextures(1, &textID[0]);
    glBindTexture(GL_TEXTURE_2D, textID[0]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image.width(), image.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, image.bits());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

void MainPanel::saveImage(QString fileName) {
    QImage image = grabFrameBuffer(true);
    image.save(fileName, "png");
}

void MainPanel::updateGB(bool b) {
    gbEnabled = b;
    updateGL();
}

void MainPanel::updateBF(bool b) {
    bfEnabled = b;
    updateGL();
}

void MainPanel::updateED(bool b) {
    edEnabled = b;
    updateGL();
}

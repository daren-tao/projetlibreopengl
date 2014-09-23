#include "mainpanel.h"

/**
 * Main component of the application. Is the opengl container which will manage the opengl context.
 *
 * @brief MainPanel::MainPanel
 * @param parent
 */
MainPanel::MainPanel(QWidget *parent) :
    QGLWidget(parent) {

    // by default we only need one step
    onePass = true;

    // by default gaussian blur is disabled and the kernel size is 3
    gbEnabled = false;
    gbKernelSize = 3;
    gbDeviation = 0.5;

    // by default bilateral filter is disabled
    bfEnabled = false;
    bfKernelSize = 3;
    bfDeviation = 0.5;
    bfRange = 0.1;

    // by default sharpening is disabled and the scale factor is 0
    shEnabled = false;
    shScaleFactor = 0;

    // by default edge detection is disabled and the algorithm used is the 0th
    edEnabled = false;
    edAlgorithm = 0;
}

/**
 * Callback for opengl context initialization.
 * Initializes opengl functions such as glClearColor and glViewport.
 * Creates the 3D object quad to display a texture.
 * Creates and links all the shaders that will be used in the application.
 *
 * @brief MainPanel::initializeGL
 */
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

/**
 * Creates all the shaders for all the algorithms.
 * Compiles and links them.
 *
 * @brief MainPanel::createShaders
 */
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

    // creating the shader for bilateral filter algorithm
    shShaderProgram = new QGLShaderProgram;

    // the vertex shader
    shVertexShader = new QGLShader(QGLShader::Vertex);
    shVertexShader->compileSourceFile(":/shaders/vertex_shader.vsh");

    // the fragment shader
    shFragmentShader = new QGLShader(QGLShader::Fragment);
    shFragmentShader->compileSourceFile(":/shaders/sharpening.fsh");

    // linking shaders in program
    shShaderProgram->addShader(shVertexShader);
    shShaderProgram->addShader(shFragmentShader);
    shShaderProgram->link();

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

/**
 * Creates the 3D object that will host the texture.
 * Whether it is a plain image or a processed one.
 *
 * @brief MainPanel::createQuad
 */
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

    // giving the position vbo the index 0
    glVertexAttribPointer(0, 3, GL_FLOAT, false, 0, 0);

    // creating the position vertex buffer object
    vboTexture = new QOpenGLBuffer;
    vboTexture->setUsagePattern(QOpenGLBuffer::StaticDraw);
    vboTexture->create();
    vboTexture->bind();
    vboTexture->allocate(texture, 6*2*sizeof(float));

    // giving the texture vbo the index 1
    glVertexAttribPointer(1, 2, GL_FLOAT, false, 0, 0);

    // releasing the objects
    vao->release();
    vboPosition->release();
    vboTexture->release();
}

/**
 * Callback for the opengl context resizing.
 * Calls glViewport with the dimensions of the loaded image.
 *
 * @brief MainPanel::resizeGL
 * @param w
 * @param h
 */
void MainPanel::resizeGL(int w, int h) {
    // qDebug() << "resizeGL";

    // specifying window coords
    glViewport(0, 0, w, h);
}

/**
 * Loads the image with the specified path.
 * Binds it as a texture.
 *
 * @brief MainPanel::loadImage
 * @param fileName
 */
void MainPanel::loadImage(QString fileName) {

    // getting the image by its path
    QImageReader reader(fileName);
    QImage image;
    reader.read(&image);
    image = convertToGLFormat(image);
    resize(image.width(), image.height());
    xOffset = 1.0 / image.width();
    yOffset = 1.0 / image.height();

    // getting context focus
    makeCurrent();

    // creating the texture
    glGenTextures(1, &textureID[0]);

    // binding the texture
    glBindTexture(GL_TEXTURE_2D, textureID[0]);

    // loading the buffer into the gpu texture and parameterizing it
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image.width(), image.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, image.bits());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

    // unbinding texture
    glBindTexture(GL_TEXTURE_2D, 0);
}

/**
 * Callback for the opengl context loop cycle.
 * Clears the screen.
 * Uses the right shader program.
 * Draws the quad and binds the texture to it.
 *
 * @brief MainPanel::paintGL
 */
void MainPanel::paintGL() {
    // qDebug() << "paintGL";

    // clearing the gl widget background
    glClear(GL_COLOR_BUFFER_BIT);

    // if there is only one step, using directly the texture
    if(onePass) {
        onePassPaint();
    }

    // if there are 2 steps, using the fbo
    else {
        twoPassesPaint();
    }
}

/**
 * When there is only one pass necessary.
 * Will bind texture.
 * Will compute the right algorithm
 * Will enable the vao.
 * Will draw.
 * Will release everything that has been used.
 * @brief MainPanel::onePassPaint
 */
void MainPanel::onePassPaint() {

    // binding the texture
    glBindTexture(GL_TEXTURE_2D, textureID[0]);

    // choosing the right shader
    if(gbEnabled) { // gaussian blur
        computeGaussianBlur();
    } else if(bfEnabled) { // bilateral filter
        computeBilateralFilter();
    } else if(shEnabled) { // sharpening
        computeSharpening();
    } else if(edEnabled) { // edge detection
        computeEdgeDetection(true);
    } else { // original image
        shaderProgram->bind();
    }

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

    // unbinding the texture
    glBindTexture(GL_TEXTURE_2D, 0);
}

void MainPanel::twoPassesPaint() {

    // 1st pass
    // activing the texture and binding it
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureID[0]);

    // creating the frame buffer object
    glGenFramebuffers(1, &fboID);

    // creating the fbo's texture and activing it
    glGenTextures(1, &textureID[1]);
    glActiveTexture(GL_TEXTURE0 + 1);

    // binding the fbo
    glBindFramebuffer(GL_FRAMEBUFFER, fboID);

    // loading the texture into the fbo
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureID[1], 0);

    // choosing the right shader
    if(edEnabled) { // edge detection
        computeEdgeDetection(true);
    }

    // enabling the vao
    vao->bind();
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    // drawing the quad in the fbo and disposing the vbos
    glDrawArrays(GL_TRIANGLES, 0, 6);

    // unbinding the texture
    glBindTexture(GL_TEXTURE_2D, 0);

    // 2nd pass
    // binding the texture
    glBindTexture(GL_TEXTURE_2D, textureID[1]);

    // choosing the right shader
    if(edEnabled) { // edge detection
        computeEdgeDetection(false);
    }

    // drawing the quad and disposing the vao
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);

    // releasing the vao
    vao->release();

    // unbinding texture and fbo
    glBindTexture(GL_TEXTURE_2D, 0);
    glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
}

/**
 * Uses the shader for the gaussian blur algorithm.
 * Calculates the kernel.
 * Gets the kernel size uniform's location and sets the current value to it.
 *
 * @brief MainPanel::computeGaussianBlur
 */
void MainPanel::computeGaussianBlur() {

    // creating the kernel values array
    float kernel[gbKernelSize*gbKernelSize];

    // calculating it
    calculateKernel(kernel, gbKernelSize, gbDeviation);

    // using the gaussian blur shader program
    gbShaderProgram->bind();

    // getting all the uniforms' location
    int kernelSizeLocation = gbShaderProgram->uniformLocation("kernel_size");
    int xOffsetLocation = gbShaderProgram->uniformLocation("x_offset");
    int yOffsetLocation = gbShaderProgram->uniformLocation("y_offset");
    int kernelValueLocation = gbShaderProgram->uniformLocation("kernel_value");

    // setting all the uniforms' value
    gbShaderProgram->setUniformValue(kernelSizeLocation, gbKernelSize);
    gbShaderProgram->setUniformValue(xOffsetLocation, xOffset);
    gbShaderProgram->setUniformValue(yOffsetLocation, yOffset);
    gbShaderProgram->setUniformValueArray(kernelValueLocation, kernel, gbKernelSize*gbKernelSize, 1);
}

/**
 * Calculates the kernel.
 * @brief MainPanel::calculateKernel
 * @param kernel
 * @param kernelSize
 * @param deviation
 */
void MainPanel::calculateKernel(float kernel[], int kernelSize, float deviation) {

    // the sum of all the kernel values
    float sum = 0.0;

    // loops going from the upper left corner to the bottom right corner
    int index = 0;
    for(int y = kernelSize/2; y >= -kernelSize/2; y--) {
        for(int x = -kernelSize/2; x <= kernelSize/2; x++) {

            // calculating the value of the kernel in (x, y)
            kernel[index] = (1.0 / (2.0*M_PI*deviation*deviation)) * exp(- ((x*x) + (y*y)) / (2*deviation*deviation));

            // updating the sum
            sum += kernel[index];
            index++;
        }
    }

    // normalizing the values in the kernel
    for(int i = 0; i < kernelSize*kernelSize; i++) {
        kernel[i] /= sum;
    }

}

/**
 * Uses the shader for the bilateral filter algorithm.
 * Gets the kernel size and delta uniforms' location and sets the current values to them.
 *
 * @brief MainPanel::computeBilateralFilter
 */
void MainPanel::computeBilateralFilter() {

    // creating the kernel values array
    float kernel[bfKernelSize*bfKernelSize];

    // calculating it
    calculateKernel(kernel, bfKernelSize, bfDeviation);

    // using the gaussian blur shader program
    bfShaderProgram->bind();

    // getting all the uniforms' location
    int kernelSizeLocation = bfShaderProgram->uniformLocation("kernel_size");
    int xOffsetLocation = bfShaderProgram->uniformLocation("x_offset");
    int yOffsetLocation = bfShaderProgram->uniformLocation("y_offset");
    int rangeLocation = bfShaderProgram->uniformLocation("range");
    int kernelValueLocation = bfShaderProgram->uniformLocation("kernel_value");

    // setting all the uniforms' value
    bfShaderProgram->setUniformValue(kernelSizeLocation, bfKernelSize);
    bfShaderProgram->setUniformValue(xOffsetLocation, xOffset);
    bfShaderProgram->setUniformValue(yOffsetLocation, yOffset);
    bfShaderProgram->setUniformValue(rangeLocation, bfRange);
    bfShaderProgram->setUniformValueArray(kernelValueLocation, kernel, bfKernelSize*bfKernelSize, 1);
}

/**
 * Uses the shader for the sharpening algorithm.
 * Gets the scale factor uniform's location and sets the current value to it.
 *
 * @brief MainPanel::computeSharpening
 */
void MainPanel::computeSharpening() {

    // using the sharpening shader program
    shShaderProgram->bind();

    // getting all the uniforms' location
    int xOffsetLocation = shShaderProgram->uniformLocation("x_offset");
    int yOffsetLocation = shShaderProgram->uniformLocation("y_offset");
    int scaleFactorLocation = shShaderProgram->uniformLocation("scale_factor");
    int kernelValueLocation = shShaderProgram->uniformLocation("kernel_value");

    // setting all the uniforms' value
    shShaderProgram->setUniformValue(xOffsetLocation, xOffset);
    shShaderProgram->setUniformValue(yOffsetLocation, yOffset);
    shShaderProgram->setUniformValue(scaleFactorLocation, shScaleFactor);
    shShaderProgram->setUniformValueArray(kernelValueLocation, shKernel, 9, 1);

}

/**
 * Uses the shader for the edge detection algorithm.
 * Chooses between several algorithms.
 *
 * @brief MainPanel::computeEdgeDetection
 */
void MainPanel::computeEdgeDetection(bool firstPass) {

    // if this is for a one-pass algorithm
    if(onePass) {

        // laplacian of the gaussian kernel
        edKernel[0] = edKernel[2] = edKernel[6] = edKernel[8] = 0.0;
        edKernel[1] = edKernel[3] = edKernel[5] = edKernel[7] = 1.0;
        edKernel[4] = -4.0;

    }

    // if this is for a two-passes algorithm
    else {

        // if this is the first pass, going through x
        if(firstPass) {
            switch(edAlgorithm) {
            case 1: // sobel x kernel
                edKernel[0] = edKernel[6] = -1.0;
                edKernel[1] = edKernel[4] = edKernel[7] = 0.0;
                edKernel[2] = edKernel[8] = 1.0;
                edKernel[3] = -2.0;
                edKernel[5] = 2.0;
                break;
            case 2: // prewitt x kernel
                edKernel[0] = edKernel[3] = edKernel[6] = -1.0;
                edKernel[1] = edKernel[4] = edKernel[7] = 0.0;
                edKernel[2] = edKernel[5] = edKernel[8] = 1.0;
                break;
            }
        }

        // going through y
        else {
            switch(edAlgorithm) {
            case 1: // sobel y kernel
                edKernel[0] = edKernel[2] = 1.0;
                edKernel[3] = edKernel[4] = edKernel[5] = 0.0;
                edKernel[6] = edKernel[8] = -1.0;
                edKernel[7] = -2.0;
                edKernel[1] = 2.0;
                break;
            case 2: // prewitt y kernel
                edKernel[0] = edKernel[1] = edKernel[2] = 1.0;
                edKernel[3] = edKernel[4] = edKernel[5] = 0.0;
                edKernel[6] = edKernel[7] = edKernel[8] = -1.0;
                break;
            }
        }
    }

    // using the edge detection shader program
    edShaderProgram->bind();

    // getting all the uniforms' location
    int xOffsetLocation = edShaderProgram->uniformLocation("x_offset");
    int yOffsetLocation = edShaderProgram->uniformLocation("y_offset");
    int kernelValueLocation = edShaderProgram->uniformLocation("kernel_value");

    // setting all the uniforms' value
    edShaderProgram->setUniformValue(xOffsetLocation, xOffset);
    edShaderProgram->setUniformValue(yOffsetLocation, yOffset);
    edShaderProgram->setUniformValueArray(kernelValueLocation, edKernel, 9, 1);
}

/**
 * Gets the frame buffer.
 * Saves the current image to the specified path.
 *
 * @brief MainPanel::saveImage
 * @param fileName
 */
void MainPanel::saveImage(QString fileName) {
    QImage image = grabFrameBuffer(true);
    image.save(fileName);
}

/**
 * Updates the activation of the gaussian blur algorithm.
 *
 * @brief MainPanel::updateGB
 * @param enabled
 */
void MainPanel::updateGB(bool enabled) {
    gbEnabled = enabled;
    onePass = true;
    updateGL();
}

/**
 * Updates the kernel size of the gaussian blur algorithm.
 *
 * @brief MainPanel::updateGB
 * @param kernelSize
 */
void MainPanel::updateGB(int kernelSize) {
    gbKernelSize = kernelSize;
    updateGL();
}

/**
 * Updates the standard deviation of the gaussian blur algorithm.
 *
 * @brief MainPanel::updateGB
 * @param deviation
 */
void MainPanel::updateGB(float deviation) {
    gbDeviation = deviation;
    updateGL();
}

/**
 * Updates the activation of the bilateral filter algorithm.
 *
 * @brief MainPanel::updateBF
 * @param b
 */
void MainPanel::updateBF(bool enabled) {
    bfEnabled = enabled;
    updateGL();
}

/**
 * Updates the kernel size of the bilateral filter algorithm.
 *
 * @brief MainPanel::updateBF
 * @param kernelSize
 */
void MainPanel::updateBF(int kernelSize) {
    bfKernelSize = kernelSize;
    updateGL();
}

/**
 * Updates the standard deviation of the bilateral filter algorithm.
 *
 * @brief MainPanel::updateDeviationBF
 * @param deviation
 */
void MainPanel::updateDeviationBF(float deviation) {
    bfDeviation = deviation;
    updateGL();
}

/**
 * Updates the range of the bilateral filter algorithm.
 *
 * @brief MainPanel::updateRangeBF
 * @param deviation
 */
void MainPanel::updateRangeBF(float range) {
    bfRange = range;
    updateGL();
}

/**
 * Updates the activation of the sharpening algorithm.
 *
 * @brief MainPanel::updateSH
 * @param enabled
 */
void MainPanel::updateSH(bool enabled) {
    shEnabled = enabled;
    onePass = true;
    updateGL();
}

/**
 * Updates the scale factor of the sharpening algorithm.
 *
 * @brief MainPanel::updateSH
 * @param scaleFactor
 */
void MainPanel::updateSH(float scaleFactor) {
    shScaleFactor = scaleFactor;
    updateGL();
}

/**
 * Updates the activation of the edge detection algorithm.
 *
 * @brief MainPanel::updateED
 * @param enabled
 */
void MainPanel::updateED(bool enabled) {
    edEnabled = enabled;
    if(!enabled) {
        onePass = true;
    }
    updateGL();
}

/**
 * Updates the choice of the edge detection algorithm.
 *
 * @brief MainPanel::updateED
 * @param algorithm
 */
void MainPanel::updateED(int algorithm) {
    edAlgorithm = algorithm;
    if(edEnabled && algorithm > 0) {
        onePass = false;
    } else {
        onePass = true;
    }
    updateGL();
}

#ifndef MAINPANEL_H
#define MAINPANEL_H

#include <QtOpenGL>
#include <QOpenGLFunctions>
#include <QGLWidget>
#include <cmath>

class MainPanel : public QGLWidget, protected QGLFunctions
{
    Q_OBJECT

private:
    float xOffset;
    float yOffset;
    bool onePass;
    GLuint textureID[1];
    GLuint fboID;

    QGLShader* vertexShader;
    QGLShader* fragmentShader;
    QGLShaderProgram* shaderProgram;

    bool gbEnabled;
    int gbKernelSize;
    float gbDeviation;
    QGLShader* gbVertexShader;
    QGLShader* gbFragmentShader;
    QGLShaderProgram* gbShaderProgram;
    void computeGaussianBlur();

    bool bfEnabled;
    int bfKernelSize;
    float bfDeviation;
    float bfRange;
    QGLShader* bfVertexShader;
    QGLShader* bfFragmentShader;
    QGLShaderProgram* bfShaderProgram;
    void computeBilateralFilter();

    bool shEnabled;
    float shScaleFactor;
    float shKernel[9] = { 0.0f, -1.0f, 0.0f,
                          -1.0f, 4.0f, -1.0f,
                          0.0f, -1.0f, 0.0f
                        };
    QGLShader* shVertexShader;
    QGLShader* shFragmentShader;
    QGLShaderProgram* shShaderProgram;
    void computeSharpening();

    bool edEnabled;
    int edAlgorithm;
    float edKernel[9];
    QGLShader* edVertexShader;
    QGLShader* edFragmentShader;
    QGLShaderProgram* edShaderProgram;
    void computeEdgeDetection(bool);

    QOpenGLVertexArrayObject* vao;
    QOpenGLBuffer* vboPosition;
    QOpenGLBuffer* vboTexture;
    void createQuad();
    void createShaders();

    void onePassPaint();
    void twoPassesPaint();
    void calculateKernel(float kernel[], int kernelSize, float deviation);

public:
    explicit MainPanel(QWidget *parent = 0);
    void loadImage(QString fileName);
    void saveImage(QString fileName);

    void updateGB(bool);
    void updateGB(int);
    void updateGB(float);

    void updateBF(bool);
    void updateBF(int);
    void updateDeviationBF(float);
    void updateRangeBF(float);

    void updateSH(bool);
    void updateSH(float);

    void updateED(bool);
    void updateED(int);

protected:
    void initializeGL();
    void resizeGL(int w, int h);
    void paintGL();

signals:
public slots:
};

#endif // MAINPANEL_H

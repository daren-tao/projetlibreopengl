#ifndef MAINPANEL_H
#define MAINPANEL_H

#include <QtOpenGL>
#include <QOpenGLFunctions>
#include <QGLWidget>
#include "datamodel.h"

class MainPanel : public QGLWidget, protected QGLFunctions
{
    Q_OBJECT

private:
    GLuint textID[1];

    QGLShader* vertexShader;
    QGLShader* fragmentShader;
    QGLShaderProgram* shaderProgram;

    bool gbEnabled;
    QGLShader* gbVertexShader;
    QGLShader* gbFragmentShader;
    QGLShaderProgram* gbShaderProgram;

    bool bfEnabled;
    QGLShader* bfVertexShader;
    QGLShader* bfFragmentShader;
    QGLShaderProgram* bfShaderProgram;

    bool edEnabled;
    QGLShader* edVertexShader;
    QGLShader* edFragmentShader;
    QGLShaderProgram* edShaderProgram;

    QOpenGLVertexArrayObject* vao;
    QOpenGLBuffer* vboPosition;
    QOpenGLBuffer* vboTexture;
    void createQuad();
    void createShaders();

public:
    explicit MainPanel(QWidget *parent = 0);
    void loadImage(QString fileName);
    void saveImage(QString fileName);
    void updateGB(bool b);
    void updateBF(bool b);
    void updateED(bool b);

protected:
    void initializeGL();
    void resizeGL(int w, int h);
    void paintGL();

signals:
public slots:
};

#endif // MAINPANEL_H

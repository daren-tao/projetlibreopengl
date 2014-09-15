#ifndef IMAGEALGORITHM_H
#define IMAGEALGORITHM_H

#include <vector>
#include <QWidget>

using namespace std;

class ImageAlgorithm
{
private:
    string name;
    bool enabled;
    vector<QWidget*> controls;
    string shaderFileName;
public:
    ImageAlgorithm();
    void setName(string s);
    void setEnabled(bool b);
    void addControl(QWidget* w);
    void setShaderFileName(string s);
};

#endif // IMAGEALGORITHM_H

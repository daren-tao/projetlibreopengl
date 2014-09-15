#include "imagealgorithm.h"

ImageAlgorithm::ImageAlgorithm() {

}

void ImageAlgorithm::setName(string s) {
    name = s;
}

void ImageAlgorithm::setEnabled(bool b) {
    enabled = b;
}

void ImageAlgorithm::addControl(QWidget* w) {
    controls.push_back(w);
}

void ImageAlgorithm::setShaderFileName(string s) {
    shaderFileName = s;
}

#ifndef GLCOLORRGBA_H
#define GLCOLORRGBA_H

#include <QVector4D>
/**
 * @brief The GlColorRgba class is a convenience class to replace QVector4D for color variables.
 */
class GLColorRgba : public QVector4D {
public:
    GLColorRgba(float r = 0.0, float g = 0.0, float b = 0.0, float a = 1.0);
    GLColorRgba(const GLColorRgba& other);
    GLColorRgba(const QVector4D& other);
    /**
      * Returns the inverted color with a= 1.0
      */
    GLColorRgba inverted();

    GLColorRgba operator*(float factor);
    //convenience getters
    float red() const { return x(); }
    float green() const { return y(); }
    float blue() const { return z(); }
    float alpha() const { return w(); }

    //predefined colors
    static GLColorRgba clBlack;
    static GLColorRgba clRed;
    static GLColorRgba clYellow;
    static GLColorRgba clGreen;
    static GLColorRgba clCyan;
    static GLColorRgba clBlue;
    static GLColorRgba clLightBlue;
    static GLColorRgba clMagenta;
    static GLColorRgba clGray;
    static GLColorRgba clWhite;
};

#endif // GLCOLORRGBA_H

#ifndef GLCOURT_H
#define GLCOURT_H

#include <QVector>

#include "glbody.h"
#include "gltoken.h"

class GLCourt : public GLBody
{

public:

    static constexpr int ROWS = 6;
    static constexpr int COLUMNS = 7;
    static constexpr float WIDTH = 10.0f;
    static constexpr float HEIGHT = 8.5f;
    static constexpr float DEPTH = 0.65f;

    GLCourt(const QString & name, float radius = 1.0f, const QString textureFile = "", const GLColorRgba & color = GLColorRgba::clBlue);

    virtual void makeSurface(QVector<GLPoint> * pointContainer, QVector<IndexType> *indexContainer) Q_DECL_OVERRIDE;

    virtual void draw(GLESRenderer * renderer, bool useBuffers = false) Q_DECL_OVERRIDE;

    virtual void findMinMaxCoordinates() Q_DECL_OVERRIDE;

    bool isColliding(const GLToken *token) const;

    QVector3D fieldToPosition(const QPoint &field) const;

    int getColumnByPosition(const QVector3D &position) const;

    int getRows() const;

    int getColumns() const;

    float getWidth() const;

    float getHeight() const;

    float getDepth() const;

private:

    QVector<QVector<int>> m_court;

    int m_rows;

    int m_columns;

    float m_width;

    float m_height;

    float m_depth;

};

#endif // GLCOURT_H

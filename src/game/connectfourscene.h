#ifndef CONNECTFOURSCENE_H
#define CONNECTFOURSCENE_H

#include <QOpenGLBuffer>

#include "../gl/glcourt.h"
#include "../gl/glitem.h"
#include "../gl/glmouseray.h"
#include "../gl/gltableplate.h"
#include "../gl/gltokengreen.h"
#include "../gl/gltokenred.h"
#include "../sound/soundengine.h"
#include "connectfour.h"

class ConnectFourScene : public GLItem {
    Q_OBJECT

public:
    explicit ConnectFourScene();

protected:
    virtual void paintUnderQmlScene() Q_DECL_OVERRIDE;

    virtual void paintOnTopOfQmlScene() Q_DECL_OVERRIDE;

    virtual void setupGeometry() Q_DECL_OVERRIDE;

    virtual void setupBuffers();

signals:

    void gameOver(const QString& color);

public slots:

    virtual void doSynchronizeThreads() Q_DECL_OVERRIDE;

    void newGame();

    void startGame();

    void stopGame();

    void startRotation(float increment);

    void stopRotation();

    void mousePressed(int x, int y);

    void mousePositionChanged(int x, int y);

    void mouseReleased(int x, int y);

    void handleWheelEvent(int angleDelta);

    void toggleMouseRays()
    {
        m_drawMouseRays = !m_drawMouseRays;
    }

    void toggleAxes()
    {
        m_drawAxes = !m_drawAxes;
    }

private:
    ConnectFour* m_vierGewinnt;

    GLMouseRay* m_mouseRay;

    SoundEngine* m_sounds;

    QOpenGLBuffer* m_vertexBuffer;

    QOpenGLBuffer* m_indexBuffer;

    QPoint m_mousePressPosition;

    QPoint m_mousePositionChangedTo;

    QPoint m_mouseReleasePosition;

    QVector3D m_mouseNear;

    QVector3D m_mouseFar;

    QVector3D m_lastIntersection;

    QVector3D m_moveVector;

    bool m_mousePressReceived;

    bool m_mouseReleaseReceived;

    bool m_mousePositionChangedReceived;

    bool m_drawMouseRays;

    bool m_newGame;
};

#endif // CONNECTFOURSCENE_H

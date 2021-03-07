#ifndef VIERGEWINNTSCENE_H
#define VIERGEWINNTSCENE_H

#include <QKeyEvent>

#include "gl/glcourt.h"
#include "gl/glitem.h"
#include "gl/glmouseray.h"
#include "gl/gltableplate.h"
#include "gl/gltokengreen.h"
#include "gl/gltokenred.h"
#include "soundengine.h"
#include "viergewinnt.h"

class VierGewinntScene : public GLItem {
    Q_OBJECT

public:
    explicit VierGewinntScene();

protected:
    virtual void paintUnderQmlScene() Q_DECL_OVERRIDE;

    virtual void paintOnTopOfQmlScene() Q_DECL_OVERRIDE;

    virtual void setupGeometry() Q_DECL_OVERRIDE;

signals:

    void gameOver(QString message);

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

private:
    VierGewinnt* m_vierGewinnt;

    SoundEngine* m_sounds;

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
};

#endif // VIERGEWINNTSCENE_H

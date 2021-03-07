#ifndef VIERGEWINNT_H
#define VIERGEWINNT_H

#include <QObject>
#include <QVector>

#include "gl/glcourt.h"
#include "gl/glesrenderer.h"
#include "gl/glmouseray.h"
#include "gl/gltableplate.h"
#include "gl/gltokengreen.h"
#include "gl/gltokenred.h"
#include "player.h"

class VierGewinntScene;

class VierGewinnt : public QObject {

    Q_OBJECT

public:

    static constexpr float START_DISTANCE = 100000.0f;

    static constexpr int ANIMATION_STEPS = 35;

    VierGewinnt(VierGewinntScene* scene);

    ~VierGewinnt();

    virtual void draw(GLESRenderer* renderer);

    bool selectToken(const QVector3D& nearPoint, const QVector3D& farPoint, const QVector3D& camera);

    void checkForSelection(const QVector3D& nearPoint, const QVector3D& farPoint, const QVector3D& camera, GLToken* token);

    void deselectToken();

    void moveToken(const QVector3D& vMove);

    void insertToken(int column);

    void newToken();

    void changePlayer();

    void descentAnimation();

    void jumpUpAnimation();

    void jumpDownAnimation();

    void cameraRotationAnimation();

    bool animateJumpUp();

    bool animateJumpDown();

    bool animateDescent();

    QMatrix4x4 getMvMatrix();

signals:
    void playerChanged(Player player);

    void gameOver(QString message);

    void soundReqeuest(const QString& soundFileName);

private:
    VierGewinntScene* m_scene;

    GLTablePlate* m_tablePlate;

    GLCourt* m_court;

    QVector<GLTokenGreen*> m_greenTokens;

    QVector<GLTokenRed*> m_redTokens;

    Player m_player;

    GLToken* m_selectedToken;

    GLToken* m_descendingToken;

    GLfloat m_lastDistance;

    //Animation
    GLint m_animationStep;

    GLfloat m_rotationStep;

    GLfloat m_jumpMoveStep;

    GLfloat m_descentMoveStep;

    bool m_animateJumpUp;

    bool m_animateJumpDown;

    bool m_animateDescent;

    //Kamera
    QMatrix4x4 m_mvMatrix;

    GLfloat m_cameraRotationAngleStart;

    GLfloat m_cameraRotationAngle;

    GLfloat m_cameraRotationAngleTarget;

    GLint m_cameraRotationStep;
};

#endif // VIERGEWINNT_H

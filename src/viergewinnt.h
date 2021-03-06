#ifndef VIERGEWINNT_H
#define VIERGEWINNT_H

#include <QObject>

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

    void changePlayer();

    void descentAnimation();

    void jumpUpAnimation();

    void jumpDownAnimation();

    bool animateJumpUp();

    bool animateJumpDown();

    bool animateDescent();

signals:
    void playerChanged(Player player);

    void soundReqeuest(const QString& soundFileName);

private:
    VierGewinntScene* m_scene;

    GLTablePlate* m_tablePlate;

    GLCourt* m_court;

    QList<GLTokenGreen*> m_greenTokens;

    QList<GLTokenRed*> m_redTokens;

    Player m_player;

    GLToken* m_selectedToken;

    GLToken* m_descendingToken;

    float m_lastDistance;

    int m_animationStep;

    float m_rotationStep;

    float m_jumpMoveStep;

    float m_descentMoveStep;

    bool m_animateJumpUp;

    bool m_animateJumpDown;

    bool m_animateDescent;
};

#endif // VIERGEWINNT_H

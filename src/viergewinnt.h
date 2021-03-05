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

    static constexpr int ANIMATION_STEPS = 100;

public:
    explicit VierGewinnt(VierGewinntScene* scene);

    ~VierGewinnt();

    virtual void draw(GLESRenderer* renderer);

    bool selectToken(const QVector3D& nearPoint, const QVector3D& farPoint, const QVector3D& camera);

    void deselectToken();

    void checkForSelection(const QVector3D& nearPoint, const QVector3D& farPoint, const QVector3D& camera, GLToken* token);

    void moveToken(const QVector3D& vMove);

    void insertToken(int column);

private:
    VierGewinntScene* m_scene;

    GLTablePlate* m_tablePlate;

    GLCourt* m_court;

    QList<GLTokenGreen*> m_greenTokens;

    QList<GLTokenRed*> m_redTokens;

    Player m_player;

    GLToken* m_selectedToken;
};

#endif // VIERGEWINNT_H

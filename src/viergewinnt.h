#ifndef VIERGEWINNT_H
#define VIERGEWINNT_H

#include <QObject>

#include "gltableplate.h"
#include "glcourt.h"
#include "gltokengreen.h"
#include "gltokenred.h"
#include "glmouseray.h"
#include "glesrenderer.h"

class VierGewinntScene;


class VierGewinnt : public QObject
{
    Q_OBJECT

public:

    typedef enum {
        RedPlayer,
        GreenPlayer
    } Player;

    explicit VierGewinnt(VierGewinntScene *scene);

    ~VierGewinnt();

    virtual void draw(GLESRenderer *renderer);

    bool selectToken(const QVector3D &nearPoint, const QVector3D &farPoint, const QVector3D &camera);

    void deselectToken();

    void checkForSelection(const QVector3D &nearPoint, const QVector3D &farPoint, const QVector3D &camera, GLToken *token);

private:

    VierGewinntScene *m_scene;

    GLTablePlate *m_tablePlate;

    GLCourt *m_court;

    QList<GLTokenGreen *> m_greenTokens;

    QList<GLTokenRed *> m_redTokens;

    Player m_player;

    GLToken *m_selectedToken;
};

#endif // VIERGEWINNT_H

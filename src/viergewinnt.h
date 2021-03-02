#ifndef VIERGEWINNT_H
#define VIERGEWINNT_H

#include <QObject>

#include "gltableplate.h"
#include "glcourt.h"
#include "gltoken.h"
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

private:

    VierGewinntScene *m_scene;

    GLTablePlate *m_tablePlate;

    GLCourt *m_court;

    GLToken *m_token;
};

#endif // VIERGEWINNT_H

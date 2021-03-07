#ifndef GLCOURT_H
#define GLCOURT_H

#include <QVector>

#include "glbody.h"
#include "gltoken.h"
#include "../player.h"

class GLCourt : public GLBody {

public:

    static constexpr int ROWS = 6;
    static constexpr int COLUMNS = 7;
    static constexpr float WIDTH = 10.0f;
    static constexpr float HEIGHT = 8.5f;
    static constexpr float DEPTH = 0.65f;

    GLCourt(const QString& name, const GLColorRgba& color = GLColorRgba::clBlue);

    QVector3D fieldToPosition(const QPoint& field) const;

    QVector3D calulateInsertPosition(const GLToken* token);

    int getColumnByPosition(const QVector3D& position) const;

    QPoint getFreeField(int column);

    void setField(Player player, QPoint field);

    Player checkWin();

    void printCourt();

private:

    QVector<QVector<Player>> m_court;

    QVector3D m_vRow;

    QVector3D m_vColumn;

    QVector3D m_vCorner;

    QVector3D m_vOffset;
};

#endif // GLCOURT_H

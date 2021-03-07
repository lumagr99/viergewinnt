#include <iostream>
#include <math.h>

#include "glcourt.h"

GLCourt::GLCourt(const QString& name, const GLColorRgba& color)
    : GLBody(name, 1.0f, color)
{
    m_vRow = v_Y * (HEIGHT / ROWS);
    m_vColumn = v_X * (WIDTH / COLUMNS);
    m_vCorner = -v_X * (WIDTH / 2) + v_Y * HEIGHT;
    m_vOffset = 0.5f * (m_vColumn - m_vRow);
    m_center = v_Zero;

    GLBody::readBinaryModelFile(":/models/court.dat");

    for (int row = 0; row < ROWS; row++) {
        m_court.append(QVector<Player>(COLUMNS, Player::None));
    }
}

QVector3D GLCourt::fieldToPosition(const QPoint& field) const
{
    return m_vCorner + m_vOffset - m_vRow * field.x() + m_vColumn * field.y();
}

QVector3D GLCourt::calulateInsertPosition(const GLToken* token)
{
    int column = getColumnByPosition(token->getCenter());
    return m_vCorner + v_X * m_vOffset.x() - v_Y * m_vOffset.y() + m_vColumn * column;
}

int GLCourt::getColumnByPosition(const QVector3D& position) const
{
    return (position.x() + WIDTH / 2) / (WIDTH / COLUMNS);
}

QPoint GLCourt::getFreeField(int column)
{
    for (int row = ROWS - 1; row >= 0; row--) {
        if (m_court[row][column] == Player::None) {
            return QPoint(row, column);
        }
    }
    return QPoint(-1, -1);
}

void GLCourt::setField(Player player, QPoint field)
{
    int x = field.x();
    int y = field.y();

    if (x < 0 || x >= ROWS) {
        return;
    }

    if (y < 0 || y >= COLUMNS) {
        return;
    }

    if (m_court[x][y] == Player::None) {
        m_court[x][y] = player;
    }
}

Player GLCourt::checkWin()
{
    for(int row = 0; row < ROWS; row++) {
        for(int col = 0; col < COLUMNS; col++) {
            Player player = m_court[row][col];
            if(player == Player::None) {
                continue;
            }

            if (col + 3 < COLUMNS &&
                    player == m_court[row][col+1] && // look right
                    player == m_court[row][col+2] &&
                    player == m_court[row][col+3]) {

                return player;
            }
            if (row + 3 < ROWS) {
                if (player == m_court[row+1][col] &&
                        player == m_court[row+2][col] &&
                        player == m_court[row+3][col])
                    return player;
                if (col + 3 < COLUMNS &&
                        player == m_court[row+1][col+1] &&
                        player == m_court[row+2][col+2] &&
                        player == m_court[row+3][col+3])
                    return player;
                if (col - 3 >= 0 &&
                        player == m_court[row+1][col-1] &&
                        player == m_court[row+2][col-2] &&
                        player == m_court[row+3][col-3])
                    return player;
            }
        }
    }
    return Player::None;
}

void GLCourt::printCourt()
{
    for (int row = 0; row < ROWS; row++) {
        for (int col = 0; col < COLUMNS; col++) {
            if (m_court[row][col] == Player::None) {
                std::cout << "N";

            } else if (m_court[row][col] == Player::RedPlayer) {
                std::cout << "R";

            } else {
                std::cout << "G";
            }
            std::cout << " ";
        }
        std::cout << '\n';
    }
}

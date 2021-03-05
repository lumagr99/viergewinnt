#include <iostream>
#include <math.h>

#include "glcourt.h"

GLCourt::GLCourt(const QString& name, const GLColorRgba& color)
    : GLBody(name, 1.0f, color)
    , m_rows(ROWS)
    , m_columns(COLUMNS)
    , m_width(WIDTH)
    , m_height(HEIGHT)
    , m_depth(DEPTH)
{
    m_vRow = v_Y * (m_height / m_rows);
    m_vColumn = v_X * (m_width / m_columns);
    m_vCorner = -v_X * (m_width / 2) + v_Y * m_height;
    m_vOffset = 0.5f * (m_vColumn - m_vRow);
    m_center = v_Zero;

    GLBody::readBinaryModelFile(":/models/court.dat");

    for (int row = 0; row < m_rows; row++) {
        m_court.append(QVector<Status>(m_columns, Status::Free));
    }
}

QVector3D GLCourt::fieldToPosition(const QPoint& field) const
{
    return m_vCorner + m_vOffset - m_vRow * field.x() + m_vColumn * field.y();
}

QVector3D GLCourt::calulateInsertPosition(const GLToken* token)
{
    int column = getColumnByPosition(token->getCenter());
    return m_vCorner + 0.5f * (m_vColumn + m_vRow) + m_vColumn * column;
}

int GLCourt::getColumnByPosition(const QVector3D& position) const
{
    return (position.x() + m_width / 2) / (m_width / m_columns);
}

QPoint GLCourt::getFreeField(int column)
{
    for (int row = m_rows - 1; row >= 0; row--) {
        if (m_court[row][column] == Status::Free) {
            return QPoint(row, column);
        }
    }
    return QPoint(-1, -1);
}

void GLCourt::setField(Player player, QPoint field)
{
    int x = field.x();
    int y = field.y();
    if (x < 0 || x >= m_rows) {
        return;
    }

    if (y < 0 || y >= m_columns) {
        return;
    }

    if (m_court[x][y] == Status::Free) {
        if (player == Player::RedPlayer) {
            m_court[x][y] = Status::RedPlayer;
        } else {
            m_court[x][y] = Status::GreenPlayer;
        }
    }
}

void GLCourt::printCourt()
{
    for (int row = 0; row < m_rows; row++) {
        for (int col = 0; col < m_columns; col++) {
            if (m_court[row][col] == Status::Free) {
                std::cout << "F";

            } else if (m_court[row][col] == Status::RedPlayer) {
                std::cout << "R";

            } else {
                std::cout << "G";
            }
            std::cout << " ";
        }
        std::cout << '\n';
    }
}

int GLCourt::getRows() const
{
    return m_rows;
}

int GLCourt::getColumns() const
{
    return m_columns;
}

float GLCourt::getWidth() const
{
    return m_width;
}

float GLCourt::getHeight() const
{
    return m_height;
}

float GLCourt::getDepth() const
{
    return m_depth;
}

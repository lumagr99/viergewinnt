#include <iostream>
#include <math.h>

#include "glcourt.h"

GLCourt::GLCourt(const QString& name, float radius, const GLColorRgba& color, const QString textureFile)
    : GLBody(name, radius, color, textureFile)
{
    //Vektoren für die Positionsberechnung initialisieren
    m_vRow = v_Y * (HEIGHT / ROWS);
    m_vColumn = v_X * (WIDTH / COLUMNS);
    m_vCorner = -v_X * (WIDTH / 2) + v_Y * HEIGHT;
    m_vOffset = 0.5f * (m_vColumn - m_vRow);

    m_center = v_Zero;

    GLBody::readBinaryModelFile(":/models/Court.dat");

    //m_court initialisieren
    for (int row = 0; row < ROWS; row++) {
        m_court.append(QVector<Player>(COLUMNS, Player::None));
    }
}

GLCourt::~GLCourt()
{
    //Texturobjekt löschen
    GLBody::destroyTextureObjects();
}

QVector3D GLCourt::fieldToPosition(const QPoint& field) const
{
    // Position des Feldes (0,0) - Zeilenhöhe * Zeile des feldes + Spaltenbreite * Spalte des Feldes
    return m_vCorner + m_vOffset - m_vRow * field.x() + m_vColumn * field.y();
}

QVector3D GLCourt::calulateInsertPosition(const GLToken* token)
{
    //Spalte des Tokens bestimmen
    int column = getColumnByPosition(token->getCenter());

    // Position des Feldes (0,0) + Offset nach oben + Spaltenbreite * Spalte des Feldes
    return m_vCorner + v_X * m_vOffset.x() - v_Y * m_vOffset.y() + m_vColumn * column;
}

int GLCourt::getColumnByPosition(const QVector3D& position) const
{
    // Da m_center = 0 muss die halbe Breite des Spielbretts auf die x-Position addiert werden
    return (position.x() + WIDTH / 2) / (WIDTH / COLUMNS);
}

QPoint GLCourt::getFreeField(int column)
{
    //Unten beginnen
    for (int row = ROWS - 1; row >= 0; row--) {
        if (m_court[row][column] == Player::None) {
            return QPoint(row, column);
        }
    }
    return QPoint(-1, -1);
}

void GLCourt::setField(Player player, QPoint field)
{
    int row = field.x();
    int col = field.y();

    if (row < 0 || row >= ROWS) {
        return;
    }

    if (col < 0 || col >= COLUMNS) {
        return;
    }

    if (m_court[row][col] == Player::None) {
        m_court[row][col] = player;
    }
}

Player GLCourt::checkWin()
{
    for (int row = 0; row < ROWS; row++) {
        for (int col = 0; col < COLUMNS; col++) {
            Player player = m_court[row][col];

            //Freie Felder nicht überprüfen
            if (player == Player::None) {
                continue;
            }

            //Vertikal überprüfen
            if (col + 3 < COLUMNS
                && player == m_court[row][col + 1]
                && player == m_court[row][col + 2]
                && player == m_court[row][col + 3]) {
                return player;
            }
            if (row + 3 < ROWS) {
                //Horizontal überprüfen
                if (player == m_court[row + 1][col]
                    && player == m_court[row + 2][col]
                    && player == m_court[row + 3][col])
                    return player;
                //Diagonal überprüfen
                if (col + 3 < COLUMNS
                    && player == m_court[row + 1][col + 1]
                    && player == m_court[row + 2][col + 2]
                    && player == m_court[row + 3][col + 3])
                    return player;
                if (col - 3 >= 0
                    && player == m_court[row + 1][col - 1]
                    && player == m_court[row + 2][col - 2]
                    && player == m_court[row + 3][col - 3])
                    return player;
            }
        }
    }
    return Player::None;
}

bool GLCourt::isFull()
{
    //Nur oberste Zeile überprüfen
    int row = 0;
    for (int col = 0; col < COLUMNS; col++) {
        if (m_court[row][col] == Player::None) {
            return false;
        }
    }
    return true;
}

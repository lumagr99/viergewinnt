#ifndef GLCOURT_H
#define GLCOURT_H

#include <QVector>

#include "../game/player.h"
#include "glbody.h"
#include "gltoken.h"

/**
 * @brief Spielbrett des Vier Gewinnt Spiels.
 *
 * Das Spielfeld besteht aus sechs Zeilen und sieben Spalten und kann maximal 42 Token beinhalten.
 *
 * Die Attribute m_vRow, m_vColumn, m_vCorner und m_vOffset werden für die Berechnung der Tokenplatzierungen verwendet:
 * - m_vRow ist ein Vektor mit der Höhe einer Zeile.
 * - m_vColumn ist ein Vektor mit der Breite einer Spalte.
 * - m_vCorner beschreibt die Position der obereren linken Ecke.
 * - m_vOffset ist der Abstand der oberen linken Ecke zur ersten Feldposition (0,0).
 *
 * Die aktuell platzierten Tokens werden als Player-Objekte in m_court gespeichert. Player::None bezeichnet dabei eine
 * freie Position wärend Player::RedPlayer und Player::GreenPlayer ein vom entsprechenden Spieler platziertes Token
 * bezeichnen. Eine Position kann mit der Methode setField gesetzt werden.
 *
 * Die Feldpositionen des Spielfelds lauten wie folgt:
 *
 * [0,0] [0,1] [0,2] [0,3] [0,4] [0,5] [0,6]
 * [1,0] [1,1] [1,2] [1,3] [1,4] [1,5] [1,6]
 * [2,0] [2,1] [2,2] [2,3] [2,4] [2,5] [2,6]
 * [3,0] [3,1] [3,2] [3,3] [3,4] [3,5] [3,6]
 * [4,0] [4,1] [4,2] [4,3] [4,4] [4,5] [4,6]
 * [5,0] [5,1] [5,2] [5,3] [5,4] [5,5] [5,6]
 *
 * Mithilfe der Methode fieldToPosition lassen sich Feldposition auf Vektoren abbilden.
 */
class GLCourt : public GLBody {

public:
    /**
     * @brief Anzahl der Zeilen des Spielfelds.
     */
    static constexpr GLint ROWS = 6;

    /**
     * @brief Anzahl der Spalten des Spielfelds.
     */
    static constexpr GLint COLUMNS = 7;

    /**
     * @brief Breite des Spielfelds.
     */
    static constexpr GLfloat WIDTH = 10.0f;

    /**
     * @brief Höhe des Spielfelds.
     */
    static constexpr GLfloat HEIGHT = 8.5f;

    /**
     * @brief Tiefe des Spielfelds.
     */
    static constexpr GLfloat DEPTH = 0.65f;

    /**
     * @brief Konstruktor. Erstellt und Initialisiert ein neues GLCourt-Objekt.
     * @param name Name des Objekts.
     * @param radius Radius des Objekts.
     * @param color Farbe des Objekts.
     * @param textureFile Texturdatei des Objekts.
     */
    explicit GLCourt(const QString& name, float radius = 1.0f, const GLColorRgba& color = GLColorRgba::clBlue,
        const QString textureFile = ":/textures/Court.png");

    /**
     * @brief ~Destruktor. Löscht das Texturobjekt des Spielfelds.
     */
    virtual ~GLCourt() Q_DECL_OVERRIDE;

    /**
     * @brief Gibt die Position des übergebenen Feldes als QVector3D zurück.
     * @param field Feld dessen Position bestimmt werden soll.
     * @return Position des übergebenen Feldes.
     */
    QVector3D fieldToPosition(const QPoint& field) const;

    /**
     * @brief Berechnet die nächste Einfügeposition für das übergebene Token.
     *
     * Die nächste Einfügeposition ist die Position mit dem geringsten Abstand zum Token.
     *
     * @param token Token dessen Einfügeposition bestimmt werden soll.
     * @return Einfügeposition.
     */
    QVector3D calulateInsertPosition(const GLToken* token);

    /**
     * @brief Berechnet die der übergebenen Position nächsten Spalte.
     * @param position Position deren Spalte bestimmt werden soll.
     * @return Spalte.
     */
    int getColumnByPosition(const QVector3D& position) const;

    /**
     * @brief Bestimmt die erste freie Position in einer Spalte. Die erste freie Position wird dabei von unten bestimmt.
     * @param column Spalte deren erste freie Position bestimmt werden soll.
     * @return erste freie Position.
     */
    QPoint getFreeField(GLint column);

    /**
     * @brief Setzt das übergebene Feld für den übergebenen Spieler.
     * @param player Spieler dessen Token am übergebenen Feld ist.
     * @param field Feld das gesetzt werden soll.
     */
    void setField(Player player, const QPoint& field);

    /**
     * @brief Überprüft, ob einer der Spieler gewonnen hat.
     * @return Player::RedPlayer bzw. Player::GreenPlayer, wenn der entsprechende Spieler gewonnen hat
     * oder Player::None, wenn kein Gewinner feststeht.
     */
    Player checkWin();

    /**
     * @brief Überprüft, ob das Spielfeld voll ist.
     * @return true, falls ja, false sonst.
     */
    bool isFull();

private:
    /**
     * @brief Vektor mit der Höhe einer Zeile des Spielfelds.
     */
    QVector3D m_vRow;

    /**
     * @brief Vektor mit der Breite einer Spalte des Spielfelds.
     */
    QVector3D m_vColumn;

    /**
     * @brief Vektor der obereren linken Ecke.
     */
    QVector3D m_vCorner;

    /**
     * @brief Vektor mit dem Abstand der oberen linken Ecke zur ersten Feldposition (0,0).
     */
    QVector3D m_vOffset;

    /**
     * @brief Enthält die aktuell platzierten Tokens als Player-Objekte.
     *
     * Player::None bezeichnet eine freie Position
     *
     * Player::RedPlayer und Player::GreenPlayer bezeichnen ein vom entsprechenden Spieler platziertes Token
     */
    QVector<QVector<Player>> m_court;
};

#endif // GLCOURT_H

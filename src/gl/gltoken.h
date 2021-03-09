#ifndef GLTOKEN_H
#define GLTOKEN_H

#include "glbody.h"

/**
 * Vorwärtsdeklaration.
 */
class GLCourt;

/**
 * @brief The GLToken class
 *
 * Objekte der Klasse stellen ein Token des Vier Gewinnt Spiels dar.
 * Wird abgeleitetet durch die Klassen GLTokenRed und GLTokenGreen.
 *
 * Das Attribut m_rotated gibt an, ob das Token rotiert wurde.
 *
 * Über das Attribut m_movable kann gestuert werden, ob das Token bewegbar ist.
 *
 */
class GLToken : public GLBody {

public:
    /**
     * @brief HEIGHT
     *
     * Höhe eines Tokens.
     */
    static constexpr float HEIGHT = 0.325f;

    /**
     * @brief RADIUS
     *
     * Radius eines Tokens
     */
    static constexpr float RADIUS = 0.650f;

    /**
     * @brief GLToken
     *
     * Erstellt und Initialisiert ein neues GLToken-Objekt.
     *
     * @param name Name des Objekts.
     * @param textureFile Texturdatei des Objekts.
     */
    explicit GLToken(const QString& name, const QString& textureFile);

    /**
     * @brief ~GLToken
     *
     * Destruktor. Löscht das Texturobjekt des Tokens.
     */
    virtual ~GLToken() Q_DECL_OVERRIDE;

    /**
     * @brief isColliding
     *
     * Überprüft, ob das Token mit dem übergebenen Token kollidiert.
     *
     * @param token Token zu dem die Kollision überprüft werden soll.
     * @return true falls ja, false sonst.
     */
    bool isColliding(const GLToken* token) const;

    /**
     * @brief isColliding
     *
     * Überprüft, ob das Token mit dem übergebenen Spielbrett kollidiert.
     *
     * @param court Spielbrett zu dem die Kollision überprüft werden soll.
     * @return true falls ja, false sonst.
     */
    bool isColliding(const GLCourt* court) const;

    /**
     * @brief moveToPosition
     *
     * Bewegt das Token zur übergebenen Position.
     *
     * @param position Position zu dem das Token bewegt werden soll.
     */
    void moveToPosition(const QVector3D& position);

    /**
     * @brief rotate
     *
     * Rotiert das Token um den übergebenen Winkel um die X-Achse.
     *
     * @param angle Rotationswinkel.
     */
    void rotate(float angle);

    /**
     * @brief isRotated
     *
     * Gibt an, ob das Token rotiert wurde.
     *
     * @return true falls ja, false sonst.
     */
    bool isRotated() const { return m_rotated; }

    /**
     * @brief setRotated
     *
     * Setzt den Rotationsstatus auf den übergebenen wert.
     *
     * @param rotated Rotationsstatus.
     */
    void setRotated(bool rotated) { m_rotated = rotated; }

    /**
     * @brief isMovable
     *
     * Gibt an, ob das Token bewegbar ist.
     *
     * @return true falls ja, false sonst.
     */
    bool isMovable() const { return m_movable; }

    /**
     * @brief setMovable
     *
     * Setzt den Bewegbarkeitsstatus auf den übergebenen wert.
     *
     * @param movable Bewegbarkeitsstatus.
     */
    void setMovable(bool movable) { m_movable = movable; }

private:
    /**
     * @brief m_rotated
     *
     * Rotationsstatus.
     */
    bool m_rotated;

    /**
     * @brief m_movable
     *
     * Bewegbarkeitsstatus.
     */
    bool m_movable;
};

#endif // GLTOKEN_H

#ifndef GLTOKEN_H
#define GLTOKEN_H

#include "glbody.h"

/**
 * Vorwärtsdeklaration.
 */
class GLCourt;

/**
 * @brief Token des Vier Gewinnt Spiels.
 *
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
     * @brief Höhe eines Tokens.
     */
    static constexpr GLfloat HEIGHT = 0.325f;

    /**
     * @brief Radius eines Tokens.
     */
    static constexpr GLfloat RADIUS = 0.650f;

    /**
     * @brief Erstellt und Initialisiert ein neues GLToken-Objekt.
     * @param name Name des Objekts.
     * @param textureFile Texturdatei des Objekts.
     */
    explicit GLToken(const QString& name, const QString& textureFile);

    /**
     * @brief Destruktor. Löscht das Texturobjekt des Tokens.
     */
    virtual ~GLToken() Q_DECL_OVERRIDE;

    /**
     * @brief Überprüft, ob das Token mit dem übergebenen Token kollidiert.
     * @param token Token zu dem die Kollision überprüft werden soll.
     * @return true falls ja, false sonst.
     */
    bool isColliding(const GLToken* token) const;

    /**
     * @brief Überprüft, ob das Token mit dem übergebenen Spielbrett kollidiert.
     * @param court Spielbrett zu dem die Kollision überprüft werden soll.
     * @return true falls ja, false sonst.
     */
    bool isColliding(const GLCourt* court) const;

    /**
     * @brief Bewegt das Token zur übergebenen Position.
     * @param position Position zu dem das Token bewegt werden soll.
     */
    void moveToPosition(const QVector3D& position);

    /**
     * @brief Rotiert das Token um den übergebenen Winkel um die X-Achse.
     * @param angle Rotationswinkel.
     */
    void rotate(float angle);

    /**
     * @brief Gibt an, ob das Token rotiert wurde.
     * @return true falls ja, false sonst.
     */
    bool isRotated() const { return m_rotated; }

    /**
     * @brief Setzt den Rotationsstatus auf den übergebenen wert.
     * @param rotated Rotationsstatus.
     */
    void setRotated(bool rotated) { m_rotated = rotated; }

    /**
     * @brief Gibt an, ob das Token bewegbar ist.
     * @return true falls ja, false sonst.
     */
    bool isMovable() const { return m_movable; }

    /**
     * @brief Setzt den Bewegbarkeitsstatus auf den übergebenen wert.
     * @param movable Bewegbarkeitsstatus.
     */
    void setMovable(bool movable) { m_movable = movable; }

private:
    /**
     * @brief Rotationsstatus.
     */
    bool m_rotated;

    /**
     * @brief Bewegbarkeitsstatus.
     */
    bool m_movable;
};

#endif // GLTOKEN_H

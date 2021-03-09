#ifndef CONNECTFOURSCENE_H
#define CONNECTFOURSCENE_H

#include "../gl/glcourt.h"
#include "../gl/glitem.h"
#include "../gl/glmouseray.h"
#include "../gl/gltableplate.h"
#include "../gl/gltokengreen.h"
#include "../gl/gltokenred.h"
#include "../sound/soundengine.h"
#include "connectfour.h"

/**
 * @brief The ConnectFourScene class
 *
 * Die ConnectFourScene-Klasse ist abgeleitet von GLItem. Die Klasse verarbeitet Mauseingaben für
 * das Vier Gewinnt Spiel. Die Eingaben werden in m_mouseNear, m_mouseFar und m_lastIntersection gespeichert.
 * Mithilfe der Flags m_mousePressReceived, m_mousePositionChangedReceived und m_mouseReleaseReceived findet
 * anschließend eine Verarbeitung in der Methode doSynchronizeThreads statt.
 *
 * Töne werden mithilfe der Klasse SoundEngine abgespielt. Die Tondateien müssen im .wav-Format sein und
 * in den Ressourcendateien des Programms vorliegen.
 */
class ConnectFourScene : public GLItem {

    Q_OBJECT

public:
    /**
     * @brief ConnectFourScene
     *
     * Konstrutkor. Erstellt und Initialisiert ein neues ConnectFourScene-Objekt.
     */
    explicit ConnectFourScene();

protected:
    /**
     * @brief paintUnderQmlScene
     *
     * Zeichnet die Geometrien in m_points unter der QML-Szene.
     */
    virtual void paintUnderQmlScene() Q_DECL_OVERRIDE;

    /**
     * @brief paintOnTopOfQmlScene
     *
     * Zeichnet die Geometrien in m_points über der QML-Szene.
     */
    virtual void paintOnTopOfQmlScene() Q_DECL_OVERRIDE;

    /**
     * @brief setupGeometry
     *
     * Schreibt die Geometriedaten in m_points und setzt das Flag m_geometryIsValid auf true.
     */
    virtual void setupGeometry() Q_DECL_OVERRIDE;

signals:

    /**
     * @brief gameOver
     *
     * Wird bei Ende einer Spielrunde emittiert.
     *
     * @param color Farbe des Gewinners (Red, Green) oder Draw.
     */
    void gameOver(const QString& color);

public slots:

    /**
     * @brief doSynchronizeThreads
     *
     * Kopiert Daten vom GUI-Thread zum Render-Thread un vice versa.
     */
    virtual void doSynchronizeThreads() Q_DECL_OVERRIDE;

    /**
     * @brief newGame
     *
     * Erstellt ein neues Vier Gewinnt Spiel durch setzen des Flags m_newGame auf true.
     */
    void newGame();

    /**
     * @brief startGame
     *
     * Startet das Vier Gewinnt Spiel.
     */
    void startGame();

    /**
     * @brief stopGame
     *
     * Pausiert das Vier Gewinnt Spiel.
     */
    void stopGame();

    /**
     * @brief startRotation
     *
     * Startet die Kamerarotation mit der übergebenen Geschwindigkeit.
     *
     * @param increment Rotationsgeschwindigkeit.
     */
    void startRotation(float increment);

    /**
     * @brief stopRotation
     *
     * Stoppt die Kamerarotation.
     */
    void stopRotation();

    /**
     * @brief mousePressed
     *
     * Wird beim Klicken der Maus aufgerufen.
     * Speichert die Mausklickposition in m_mousePressPosition und setzt das Flag m_mousePressReceived.
     *
     * @param x X-Koordinate der Mausklick Position.
     * @param y Y-Koordinate der Mausklick Position.
     */
    void mousePressed(int x, int y);

    /**
     * @brief mousePositionChanged
     *
     * Wird beim Bewegen der Maus aufgerufen.
     * Speichert die Mausposition in m_m_mousePositionChangedTo und setzt das Flag m_mousePositionChangedReceived.
     *
     * @param x X-Koordinate der Maus.
     * @param y Y-Koordinate der Maus.
     */
    void mousePositionChanged(int x, int y);

    /**
     * @brief mousePressed
     *
     * Wird beim Loslassen des Mausklicks aufgerufen.
     * Speichert die Mausklick-Loslass Position in m_mouseReleasePosition und setzt das Flag m_mouseReleaseReceived.
     *
     * @param x X-Koordinate der Mausklick-Loslass Position.
     * @param y Y-Koordinate der Mausklick-Loslass Position.
     */
    void mouseReleased(int x, int y);

    /**
     * @brief handleWheelEvent
     *
     * Wird beim Bewegen des Mausrads aufgerufen. Ändert den Zoom entsprechend der Mausradbewegung.
     *
     * @param angleDelta Mausradrichtung.
     */
    void handleWheelEvent(int angleDelta);

    /**
     * @brief toggleMouseRays
     *
     * Schaltet das das Zeichnen der Maus Rays um.
     */
    void toggleMouseRays() { m_drawMouseRays = !m_drawMouseRays; }

    /**
     * @brief toggleAxes
     *
     * Schaltet das das Zeichnen der Koordinatenachsen um.
     */
    void toggleAxes() { m_drawAxes = !m_drawAxes; }

private:
    /**
     * @brief m_vierGewinnt
     *
     * Objekt mit der Spiellogik des Vier Gewinnt Spiels.
     */
    ConnectFour* m_vierGewinnt;

    /**
     * @brief m_mouseRay
     *
     * MouseRay
     */
    GLMouseRay* m_mouseRay;

    /**
     * @brief m_sounds
     *
     * SoundEngine
     */
    SoundEngine* m_sounds;

    /**
     * @brief m_mousePressPosition
     *
     * Mausklick Position.
     */
    QPoint m_mousePressPosition;

    /**
     * @brief m_mousePositionChangedTo
     *
     * Mausposition.
     */
    QPoint m_mousePositionChangedTo;

    /**
     * @brief m_mouseReleasePosition
     *
     * Mausklick-Loslass Position.
     */
    QPoint m_mouseReleasePosition;

    /**
     * @brief m_mouseNear
     *
     * Nahpunkt des Mausklicks.
     */
    QVector3D m_mouseNear;

    /**
     * @brief m_mouseFar
     *
     * Fernpunkt des Mausklicks.
     */
    QVector3D m_mouseFar;

    /**
     * @brief m_lastIntersection
     *
     * Letzer Schnittpunkt.
     */
    QVector3D m_lastIntersection;

    /**
     * @brief m_moveVector
     *
     * Bewegungsvektor.
     */
    QVector3D m_moveVector;

    /**
     * @brief m_mousePressReceived
     *
     * Flag für Mausklicks.
     */
    GLboolean m_mousePressReceived;

    /**
     * @brief m_mouseReleaseReceived
     *
     * Flag für das Loslassen von Mausklicks.
     */
    GLboolean m_mouseReleaseReceived;

    /**
     * @brief m_mousePositionChangedReceived
     *
     * Flag für die Mausbewegung.
     */
    GLboolean m_mousePositionChangedReceived;

    /**
     * @brief m_drawMouseRays
     *
     * Flag für das Zeichnen von Maus Rays.
     */
    GLboolean m_drawMouseRays;

    /**
     * @brief m_newGame
     *
     * Flag für das Starten eines neuen Spiels.
     */
    GLboolean m_newGame;

    /**
     * @brief m_player
     *
     * Aktuell aktiver Spieler.
     */
    Player m_player;
};

#endif // CONNECTFOURSCENE_H

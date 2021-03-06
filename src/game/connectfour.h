#ifndef CONNECTFOUR_H
#define CONNECTFOUR_H

#include <QMap>
#include <QObject>
#include <QVector>

#include "../gl/glcourt.h"
#include "../gl/glesrenderer.h"
#include "../gl/glmouseray.h"
#include "../gl/gltableplate.h"
#include "../gl/gltokengreen.h"
#include "../gl/gltokenred.h"
#include "player.h"

/**
 * Vorwärtsdeklaration.
 */
class ConnectFourScene;

/**
 * @brief Die ConnectFour-Klasse enthält die Spiellogik für ein Vier Gewinnt Spiel.
 *
 * Die Klasse enthält die Tischplatte, das Spielbrett und die 42 Spieltoken.
 *
 * Die draw-Methode zeichnet die enthaltenen Objekte und benötigt einen aktiven OpenGL-Kontext.
 * Über die Methode selectToken kann ein Token ausgewählt und anschließend mit der Methode moveToken bewegt werden.
 * Mithilfe der Methode deselectToken kann das ausgewählte Token deselektiert werden.
 *
 * Das Spiel startet mit dem im Konstruktor übergebenen Spieler. Bei Spielende wird ein gameOver Signal mit Gewinnerfarbe
 * emittiert.
 *
 * Bei folgenden Aktionen wird ein soundRequested Signal mit dem Dateinamen der abzuspielenden Datei emittiert:
 * - Bei Selektion eines Tokens
 * - Beim Einfügen eines Tokens in das Spielbrett
 * - Beim Spielerwechsel
 * - Beim Spielende
 */
class ConnectFour : public QObject {

    Q_OBJECT

public:
    /**
     * @brief Startdistanz, die bei der Selektionsberechnung verwendet wird.
     */
    static constexpr GLfloat START_DISTANCE = 100000.0f;

    /**
     * @brief Anzahl der Animationsschritte.
     * Eine höhere Anzahl von Animationsschritten führt zu flüssigeren Animationen, verlangsamt diese jedoch auch.
     */
    static constexpr GLint ANIMATION_STEPS = 35;

    /**
     * @brief Aufzählung für die Sound-Events.
     */
    typedef enum {
        TokenSelected,
        TokenInserted,
        PlayerChanged,
        GameOver
    } SoundEvent;

    /**
     * @brief Konnstruktor. Erstellt und Initialisiert ein neues ConnectFour-Objekt.
     * Der übergebene Spieler ist als erstes am Zug.
     *
     * @param scene Szene für die das Vier Gewinnt Spiel erstellt werden soll.
     * @param player Spieler der als erstes am Zug ist.
     */
    explicit ConnectFour(ConnectFourScene* scene, Player player);

    /**
     * @brief Destruktor. Gibt den Speicher der Tischplatte, des Spielbretts und der Token frei.
     */
    virtual ~ConnectFour();

    /**
     * @brief Zeichnet das Vier Gewinnt Spiel im aktuellen Kontext.
     *
     * Rotiert bei Aufruf die Kamera um m_cameraRotationAngle um die Y-Achse mithilfe der Modelview-Matrix m_mvMatrix.
     *
     * @param renderer Renderer mit dem das Spiel gezeichent werden soll.
     */
    virtual void draw(GLESRenderer* renderer);

    /**
     * @brief Selektiert das getroffene Token, dass am nächsten an der Kamera ist.
     *
     * @param nearPoint Nahpunkt des Mausklicks.
     * @param farPoint Fernpunkt des Mausklicks.
     * @param camera Kamera.
     * @return true, falls ein Token selektiert wurde, false sonst.
     */
    bool selectToken(const QVector3D& nearPoint, const QVector3D& farPoint, const QVector3D& camera);

    /**
     * @brief Deselektiert das aktuell ausgewählte Token.
     *
     * Befindet sich das Token in der nähe des Spielbretts wird die Methode insertToken aufgerufen,
     * um das Token in die nächste Spalte einzufügen.
     */
    void deselectToken();

    /**
     * @brief Bewegt das selektierte Token entsprechend der Vier Gewinnt Spielregeln.
     *
     * Bei Kollision mit anderen Spielsteinen wird die Bewegung verhindert.
     *
     * Bei Kollision mit dem Spielbrett wird das Flag m_animateJumpUp auf true gesetzt, was dazu führt dass
     * eine Animation zu einer möglichen Einfügeposition ausgeführt wird.
     *
     * Wird das Token von einer Einfügeposition wegbewegt, so wird das Flag m_animateJumpDown auf true gesetzt
     * und dadurch eine Animation zur Tischplatte ausgeführt.
     *
     * @param vMove Vektor in dessen Richtung das Token bewegt werden soll.
     */
    void moveToken(const QVector3D& vMove);

    /**
     * @brief Animiert den Abstieg eines eingefügten Tokens zur Zielposition.
     *
     * Die Methode wird alle 16 msec durch die Methode doSynchronizeThreads der Szene aufgerufen, falls das Flag
     * m_animateDescent true ist.
     *
     * Ist m_animationStep = ANIMATION_STEPS wird das Flag m_animateDescent auf false gesetzt.
     *
     * Die Anzahl der Animationsschritte entspricht ANIMATION_STEPS.
     */
    void descentAnimation();

    /**
     * @brief Animiert den Sprung des selektierten Tokens von der Tischplatte zu einer Einfügeposition.
     *
     * Das Token wird durch die Animation um -90° um die X-Achse gedreht und das Flag m_rotated auf true gesetzt.
     *
     * Die Methode wird alle 16 msec durch die Methode doSynchronizeThreads der Szene aufgerufen, falls das Flag
     * m_animateJumpUp true ist.
     *
     * Ist m_animationStep = ANIMATION_STEPS wird das Flag m_animateJumpUp auf false gesetzt.
     *
     * Die Anzahl der Animationsschritte entspricht ANIMATION_STEPS.
     */
    void jumpUpAnimation();

    /**
     * @brief Animiert den Sprung des selektierten Tokens von einer Einfügeposition zur Tischplatte.
     *
     * Das Token wird durch die Animation um 90° um die X-Achse gedreht und das Flag m_rotated auf false gesetzt.
     *
     * Die Methode wird alle 16 msec durch die Methode doSynchronizeThreads der Szene aufgerufen, falls das Flag
     * m_animateJumpDown true ist.
     *
     * Ist m_animationStep = ANIMATION_STEPS wird das Flag m_animateJumpDown auf false gesetzt.
     *
     * Die Anzahl der Animationsschritte entspricht ANIMATION_STEPS.
     */
    void jumpDownAnimation();

    /**
     * @brief Animiert die Kamerarotation.
     *
     * Wird alle 16 msec durch die Methode doSynchronizeThreads der Szene aufgerufen.
     */
    void cameraRotationAnimation();

    /**
     * @brief Gibt true zurück, wenn die jumpUpAnimation ausgeführt werden soll.
     * @return true, wenn die Animation ausgeführt werden soll, false sonst.
     */
    bool animateJumpUp() { return m_animateJumpUp; }

    /**
     * @brief Gibt true zurück, wenn die jumpDownAnimation ausgeführt werden soll.
     * @return true, wenn die Animation ausgeführt werden soll, false sonst.
     */
    bool animateJumpDown() { return m_animateJumpDown; }

    /**
     * @brief Gibt true zurück, wenn die descentAnimation ausgeführt werden soll.
     * @return true, wenn die Animation ausgeführt werden soll, false sonst.
     */
    bool animateDescent() { return m_animateDescent; }

    /**
     * @brief Gibt die Modelview-Matrix zurück. Die Modelview-Matrix wird für zusätzliche Transformationen verwendet.
     * @return Modelview-Matrix.
     */
    QMatrix4x4& getMvMatrix() { return m_mvMatrix; }

signals:

    /**
     * @brief Wird bei Ende einer Spielrunde emittiert.
     * @param color Farbe des Gewinners (Red, Green) oder Draw.
     */
    void gameOver(const QString& color);

    /**
     * @brief Wird emittiert, wenn ein Ton abgespielt werden soll.
     * @param soundFileName Dateiname des abzuspielenden Tons.
     */
    void soundReqeuest(const QString& soundFileName);

private:
    /**
     * @brief Wird von der Methode selectToken für jedes Token des Spiels aufgerufen.
     *
     * Überprüft, ob das Token durch die Linie durch die Punkte nearPoint und farPoint getroffen wurde.
     * Falls das getroffene Token am nächsten an der Kamera ist wird es selektiert.
     *
     * @param nearPoint Nahpunkt des Mausklicks.
     * @param farPoint Fernpunkt des Mausklicks.
     * @param camera Kamera.
     * @param token Token, dessen Selektion überprüft werden soll.
     */
    void checkForSelection(const QVector3D& nearPoint, const QVector3D& farPoint, const QVector3D& camera, GLToken* token);

    /**
     * @brief Versucht das selektierte Token in die erste freie Position der übergebenen Spalte einzufügen.
     *
     * Ist eine Position frei, so wird das Token in das Spielbrett eingefügt und unbewegbar gesetzt.
     * Anschließend wird ie Flag m_animateDescent auf true gesetzt und somit die Abstiegsanimation gestartet.
     * Zusätzlich wird überprüft, ob ein Gewinner vorliegt oder ob das Spiel unentschieden ist. Ist dies der Fall, so wird ein
     * gameOver Signal emittiert. Liegt kein Gewinner vor oder ist das Spiel nicht unentschieden findet ein Spielerwechsel statt.
     *
     * Ist keine Position frei, so geschieht nichts.
     *
     * @param column Spalte in die das Token eingefügt werden soll.
     */
    void insertToken(int column);

    /**
     * @brief Wechselt den aktuell aktiven Spieler.
     */
    void changePlayer();

    /**
     * @brief Szene in die das Vier Gewinnt Spiel gezeichnet wird.
     */
    ConnectFourScene* m_scene;

    /**
     * @brief Tischplatte des Spielts.
     */
    GLTablePlate* m_tablePlate;

    /**
     * @brief Spielbrett des Spiels.
     */
    GLCourt* m_court;

    /**
     * @brief ap mit den Dateinamen der Sound-Events.
     */
    QMap<SoundEvent, QString> soundFileNames;

    /**
     * @brief Grüne Tokens.
     */
    QVector<GLTokenGreen*> m_greenTokens;

    /**
     * @brief Rote Tokens.
     */
    QVector<GLTokenRed*> m_redTokens;

    /**
     * @brief Aktiver Spieler.
     */
    Player m_player;

    /**
     * @brief Selektiertes Token.
     */
    GLToken* m_selectedToken;

    /**
     * @brief Absteigendes Token.
     */
    GLToken* m_descendingToken;

    /**
     * @brief Distanz des letzten selektierten Tokens in checkForSelection.
     */
    GLfloat m_lastDistance;

    /**
     * @brief Aktueller Animationsschritt.
     */
    GLint m_animationStep;

    /**
     * @brief Rotation des Tokens pro Animationsschritt.
     */
    GLfloat m_rotationStep;

    /**
     * @brief Sprung des Tokens pro Animationsschritt.
     */
    GLfloat m_jumpMoveStep;

    /**
     * @brief Abstieg des Tokens pro Animationsschritt.
     */
    GLfloat m_descentMoveStep;

    /**
     * @brief Flag für die jumpUpAnimation.
     */
    GLboolean m_animateJumpUp;

    /**
     * @brief Flag für die jumpDownAnimation.
     */
    GLboolean m_animateJumpDown;

    /**
     * @brief Flag für die Abstiegsanimation.
     */
    GLboolean m_animateDescent;

    /**
     * @brief Modelview-Matrix.
     */
    QMatrix4x4 m_mvMatrix;

    /**
     * @brief Startwinkel der Kamerarotation.
     */
    GLfloat m_cameraRotationAngleStart;

    /**
     * @brief Kamerarotationswinkel.
     */
    GLfloat m_cameraRotationAngle;

    /**
     * @brief Zielwinkel der Kamerarotation.
     */
    GLfloat m_cameraRotationAngleTarget;

    /**
     * @brief Kamerarotationsschritt.
     */
    GLint m_cameraRotationStep;
};

#endif // CONNECTFOUR_H

#include "connectfour.h"
#include "connectfourscene.h"

ConnectFour::ConnectFour(ConnectFourScene* scene, Player player)
    : m_scene(scene)
    , m_player(player)
    , m_selectedToken(nullptr)
    , m_descendingToken(nullptr)
    , m_lastDistance(START_DISTANCE)
    , m_animationStep(0)
    , m_descentMoveStep(0)
    , m_animateJumpUp(false)
    , m_animateJumpDown(false)
    , m_animateDescent(false)

{
    //Tischplatte & Spielbrett erstellen
    m_tablePlate = new GLTablePlate("TablePlate");
    m_court = new GLCourt("Court");

    //Rotations & Sprungschritt berechnen
    m_rotationStep = 90.0f / ANIMATION_STEPS;
    m_jumpMoveStep = (GLCourt::HEIGHT + GLToken::RADIUS) / ANIMATION_STEPS;

    //Sounds initialisieren
    soundFileNames[TokenSelected] = ":/sounds/TokenSelected.wav";
    soundFileNames[TokenInserted] = ":/sounds/TokenInserted.wav";
    soundFileNames[PlayerChanged] = ":/sounds/PlayerChanged.wav";
    soundFileNames[GameOver] = ":/sounds/GameOver.wav";

    //Tokens platzieren
    for (GLint i = 0; i < GLCourt::COLUMNS; i++) {
        for (GLint j = 0; j < GLCourt::ROWS / 2; j++) {
            GLint n = (i + 1) * (j + 1);
            GLTokenRed* token = new GLTokenRed(QString("RedToken%1").arg(n));
            m_redTokens.append(token);
            token->move(QVector3D(-GLCourt::WIDTH / 2 + ((GLCourt::WIDTH / 6) * i), 0.0f, 3.0 + 2 * j));
        }
        for (GLint j = 0; j < GLCourt::ROWS / 2; j++) {
            GLint n = (i + 1) * (j + 1);
            GLTokenGreen* token = new GLTokenGreen(QString("GreenToken%1").arg(n));
            m_greenTokens.append(token);
            token->move(QVector3D(-GLCourt::WIDTH / 2 + ((GLCourt::WIDTH / 6) * i), 0.0f, -3.0 - 2 * j));
        }
    }

    //Kameraposition anpassen
    if (m_player == Player::RedPlayer) {
        m_cameraRotationAngleStart = 0.0f;
        m_cameraRotationAngle = 0.0f;
        m_cameraRotationAngleTarget = 0.0f;
    } else {
        m_cameraRotationAngleStart = 180.0f;
        m_cameraRotationAngle = 180.0f;
        m_cameraRotationAngleTarget = 180.0f;
    }
    m_cameraRotationStep = 100;
}

ConnectFour::~ConnectFour()
{
    for (auto& token : m_greenTokens) {
        delete token;
    }
    for (auto& token : m_redTokens) {
        delete token;
    }
    delete m_tablePlate;
    delete m_court;
}

void ConnectFour::draw(GLESRenderer* renderer)
{
    renderer->pushMvMatrix();

    //Modelview Matrix rotieren
    m_mvMatrix = renderer->getMvMatrix();
    m_mvMatrix.rotate(m_cameraRotationAngle, v_Y);
    renderer->setMvMatrix(m_mvMatrix);

    //Objekte speichern
    m_tablePlate->draw(renderer);
    m_court->draw(renderer);

    for (auto& token : m_greenTokens) {
        token->draw(renderer);
    }

    for (auto& token : m_redTokens) {
        token->draw(renderer);
    }
    renderer->popMvMatrix();
}

bool ConnectFour::selectToken(const QVector3D& nearPoint, const QVector3D& farPoint, const QVector3D& camera)
{
    if (m_player == Player::RedPlayer) {
        for (auto& token : m_redTokens) {
            checkForSelection(nearPoint, farPoint, camera, token);
        }
    } else {
        for (auto& token : m_greenTokens) {
            checkForSelection(nearPoint, farPoint, camera, token);
        }
    }
    if (m_selectedToken != nullptr) {
        m_lastDistance = START_DISTANCE;

        //Sound abspielen, wenn das Token bewegbar ist
        if (m_selectedToken->isMovable()) {
            emit soundReqeuest(soundFileNames[TokenSelected]);
        }
    }
    return m_selectedToken != nullptr;
}

void ConnectFour::checkForSelection(const QVector3D& nearPoint, const QVector3D& farPoint, const QVector3D& camera, GLToken* token)
{
    if (token->isHit(nearPoint, farPoint)) {
        if (m_selectedToken == nullptr) {
            token->setSelected(true);
            m_selectedToken = token;
        } else {
            GLfloat distance = (camera - token->getCenter()).length();
            //Token näher?
            if (distance < m_lastDistance) {
                m_lastDistance = distance;
                m_selectedToken->setSelected(false);
                token->setSelected(true);
                m_selectedToken = token;
            }
        }
    }
}

void ConnectFour::deselectToken()
{
    if (m_selectedToken == nullptr) {
        return;
    }

    if (!m_selectedToken->isMovable()) {
        return;
    }

    //Token kollidiert mit dem Spielbret -> einfügen
    if (m_selectedToken->isColliding(m_court)) {
        QVector3D center = m_selectedToken->getCenter();
        //Spalte bestimmen
        int column = m_court->getColumnByPosition(center);
        insertToken(column);
    }
    m_selectedToken->setSelected(false);
    m_selectedToken = nullptr;
}

void ConnectFour::moveToken(const QVector3D& vMove)
{
    if (m_selectedToken == nullptr) {
        return;
    }

    if (!m_selectedToken->isMovable()) {
        return;
    }

    //Neue Position berechnen und verhindern, dass der Spielstein auf die Gegnerseite bewegt wird
    QVector3D newPos = m_selectedToken->getCenter() + vMove;
    QVector3D vz = v_Zero;
    GLfloat z = newPos.z();
    if (m_player == Player::RedPlayer && newPos.z() < 0.0f) {
        vz = v_Z * z;
    }

    if (m_player == Player::GreenPlayer && newPos.z() > 0.0f) {
        vz = v_Z * z;
    }
    m_selectedToken->move(vMove - vz);

    //Kollisionserkennung Spielfeld
    if (m_selectedToken->isColliding(m_court)) {
        if (!m_selectedToken->isRotated()) {
            m_selectedToken->setMovable(false);
            m_animateJumpUp = true;
        }
    } else {
        if (m_selectedToken->isRotated()) {
            m_selectedToken->setMovable(false);
            m_animateJumpDown = true;
        }
    }

    //Kollisionserkennung Spieltokens
    for (auto& token : m_redTokens) {
        if (m_selectedToken->isColliding(token)) {
            m_selectedToken->move(-vMove);
        }
    }

    for (auto& token : m_greenTokens) {
        if (m_selectedToken->isColliding(token)) {
            m_selectedToken->move(-vMove);
        }
    }
}

void ConnectFour::insertToken(int column)
{
    QPoint field = m_court->getFreeField(column);
    if (field != QPoint(-1, -1)) {
        //Token zur endgültigen Einfügeposition bewegen
        QVector3D insertPosition = m_court->calulateInsertPosition(m_selectedToken);
        m_selectedToken->moveToPosition(insertPosition);
        m_selectedToken->setMovable(false);

        //Feldposition bestimmen
        QVector3D position = m_court->fieldToPosition(field);
        m_descentMoveStep = (m_selectedToken->getCenter().y() - position.y()) / ANIMATION_STEPS;
        m_descendingToken = m_selectedToken;

        //Feld setzen
        m_court->setField(m_player, field);

        //Abstiegsanimation starten
        m_animateDescent = true;

        //Spielstatus überprüfen
        Player player = m_court->checkWin();
        if (player == Player::RedPlayer) {
            emit gameOver("Red");
            emit soundReqeuest(soundFileNames[GameOver]);
        } else if (player == Player::GreenPlayer) {
            emit gameOver("Green");
            emit soundReqeuest(soundFileNames[GameOver]);
        } else if (m_court->isFull()) {
            emit gameOver("Draw");
            emit soundReqeuest(soundFileNames[GameOver]);
        } else {
            changePlayer();
        }
    }
}

void ConnectFour::changePlayer()
{
    if (m_player == Player::RedPlayer) {
        m_player = Player::GreenPlayer;
        m_cameraRotationAngleTarget = 180.0;
    } else {
        m_player = Player::RedPlayer;
        m_cameraRotationAngleTarget = 0.0;
    }
    emit soundReqeuest(soundFileNames[PlayerChanged]);
    m_cameraRotationAngleStart = m_cameraRotationAngle;
    m_cameraRotationStep = 0;
}

void ConnectFour::descentAnimation()
{
    if (m_descendingToken) {
        if (m_animationStep < ANIMATION_STEPS) {
            m_descendingToken->move(-v_Y * m_descentMoveStep);
            m_animationStep++;
        } else {
            m_descentMoveStep = 0.0f;
            m_animateDescent = false;
            m_descendingToken = nullptr;
            m_animationStep = 0;
        }
    }
}

void ConnectFour::jumpUpAnimation()
{
    if (m_selectedToken) {
        if (m_animationStep < ANIMATION_STEPS) {
            m_selectedToken->rotate(-m_rotationStep);
            m_selectedToken->move(v_Y * m_jumpMoveStep);
            m_animationStep++;
        } else {
            m_selectedToken->setMovable(true);
            m_selectedToken->setRotated(true);
            m_animateJumpUp = false;
            m_rotationStep = 90.0f / ANIMATION_STEPS;
            m_animationStep = 0.0f;
        }
    }
}

void ConnectFour::jumpDownAnimation()
{
    if (m_selectedToken) {
        if (m_animationStep < ANIMATION_STEPS) {
            m_selectedToken->rotate(m_rotationStep);
            m_selectedToken->move(-v_Y * m_jumpMoveStep);
            m_animationStep++;
        } else {
            m_selectedToken->setMovable(true);
            m_selectedToken->setRotated(false);
            m_animateJumpDown = false;
            m_rotationStep = 90.0f / ANIMATION_STEPS;
            m_animationStep = 0.0f;
        }
    }
}

void ConnectFour::cameraRotationAnimation()
{
    if (m_cameraRotationStep <= 100) {
        m_cameraRotationAngle = m_cameraRotationAngleStart + m_cameraRotationStep * (m_cameraRotationAngleTarget - m_cameraRotationAngleStart) / 100.0;
        m_cameraRotationStep++;
    }
}

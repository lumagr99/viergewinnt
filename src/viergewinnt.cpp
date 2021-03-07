#include "viergewinnt.h"
#include "viergewinntscene.h"

VierGewinnt::VierGewinnt(VierGewinntScene* scene)
    : m_scene(scene)
    , m_player(Player::RedPlayer)
    , m_selectedToken(nullptr)
    , m_descendingToken(nullptr)
    , m_lastDistance(START_DISTANCE)
    , m_animationStep(0)
    , m_descentMoveStep(0)
    , m_animateJumpUp(false)
    , m_animateJumpDown(false)
    , m_animateDescent(false)
    , m_cameraRotationAngleStart(0)
    , m_cameraRotationAngle(0)
    , m_cameraRotationAngleTarget(0)
    , m_cameraRotationStep(100)

{    
    m_tablePlate = new GLTablePlate("TablePlate");
    m_court = new GLCourt("Court");

    m_rotationStep = 90.0f / ANIMATION_STEPS;
    m_jumpMoveStep = (GLCourt::HEIGHT + GLToken::RADIUS) / ANIMATION_STEPS;

    newToken();
}

VierGewinnt::~VierGewinnt()
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

void VierGewinnt::draw(GLESRenderer* renderer)
{
    renderer->pushMvMatrix();
    m_mvMatrix = renderer->getMvMatrix();
    m_mvMatrix.rotate(m_cameraRotationAngle, v_Y);
    renderer->setMvMatrix(m_mvMatrix);

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

bool VierGewinnt::selectToken(const QVector3D& nearPoint, const QVector3D& farPoint, const QVector3D& camera)
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
        //emit Sound
    }
    return m_selectedToken != nullptr;
}

void VierGewinnt::checkForSelection(const QVector3D& nearPoint, const QVector3D& farPoint, const QVector3D& camera, GLToken* token)
{
    if (token->isHit(nearPoint, farPoint)) {
        if (m_selectedToken == nullptr) {
            token->setSelected(true);
            m_selectedToken = token;
        } else {
            GLfloat distance = (camera - token->getCenter()).length();
            if (distance < m_lastDistance) {
                m_lastDistance = distance;
                m_selectedToken->setSelected(false);
                token->setSelected(true);
                m_selectedToken = token;
            }
        }
    }
}

void VierGewinnt::deselectToken()
{
    if (m_selectedToken == nullptr) {
        return;
    }

    if(!m_selectedToken->isMovable()) {
        return;
    }

    if (m_selectedToken->isColliding(m_court)) {
        QVector3D center = m_selectedToken->getCenter();
        int column = m_court->getColumnByPosition(center);
        insertToken(column);
    }
    m_selectedToken->setSelected(false);
    m_selectedToken = nullptr;
}

void VierGewinnt::moveToken(const QVector3D& vMove)
{
    if (m_selectedToken == nullptr) {
        return;
    }

    if(!m_selectedToken->isMovable()) {
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

    //Kollisionserkennung Spielsteine
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

void VierGewinnt::insertToken(int column)
{
    QPoint field = m_court->getFreeField(column);
    if (field != QPoint(-1, -1)) {
        QVector3D insertPosition = m_court->calulateInsertPosition(m_selectedToken);
        m_selectedToken->moveToPosition(insertPosition);
        m_selectedToken->setMovable(false);

        QVector3D position = m_court->fieldToPosition(field);
        m_descentMoveStep = (m_selectedToken->getCenter().y() - position.y()) / ANIMATION_STEPS;
        m_descendingToken = m_selectedToken;

        m_court->setField(m_player, field);
        m_animateDescent = true;


        Player player = m_court->checkWin();
        if(player == Player::RedPlayer) {
            emit gameOver("Rot hat gewonnen!");
        } else if(player == Player::GreenPlayer) {
            emit gameOver("Grün hat gewonnen!");
        } else {
            changePlayer();
        }
    }
}

void VierGewinnt::newToken()
{
    if(m_player == Player::RedPlayer) {
        GLTokenRed* redToken = new GLTokenRed(QString("RedToken%1").arg(m_redTokens.size()));
        redToken->move(v_Z * 5.0f);
        m_redTokens.append(redToken);
    } else {
        GLTokenGreen* greenToken = new GLTokenGreen(QString("GreenToken%1").arg(m_redTokens.size()));
        greenToken->move(-v_Z * 5.0f);
        m_greenTokens.append(greenToken);
    }
}

void VierGewinnt::changePlayer()
{
    if (m_player == Player::RedPlayer) {
        m_player = Player::GreenPlayer;
        m_cameraRotationAngleTarget = 180.0;
    } else {
        m_player = Player::RedPlayer;
        m_cameraRotationAngleTarget = 0.0;
    }
    m_cameraRotationAngleStart = m_cameraRotationAngle;
    m_cameraRotationStep = 0;
    newToken();
}

void VierGewinnt::descentAnimation()
{
    if(m_descendingToken) {
        if(m_animationStep < ANIMATION_STEPS) {
            m_descendingToken->move(-v_Y * m_descentMoveStep);
            m_animationStep++;
        } else {
            m_descentMoveStep = 0.0f;
            m_animateDescent = false;
            m_descendingToken = nullptr;
            m_animationStep = 0.0f;
        }
    }
}

void VierGewinnt::jumpUpAnimation()
{
    if(m_selectedToken) {
        if(m_animationStep < ANIMATION_STEPS) {
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

void VierGewinnt::jumpDownAnimation()
{
    if(m_selectedToken) {
        if(m_animationStep < ANIMATION_STEPS) {
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

void VierGewinnt::cameraRotationAnimation()
{
    if(m_cameraRotationStep <= 100)
    {
        m_cameraRotationAngle = m_cameraRotationAngleStart + m_cameraRotationStep
                * (m_cameraRotationAngleTarget - m_cameraRotationAngleStart) / 100.0;
        m_cameraRotationStep++;
    }
}

bool VierGewinnt::animateJumpUp()
{
    return m_animateJumpUp;
}

bool VierGewinnt::animateJumpDown()
{
    return m_animateJumpDown;
}

bool VierGewinnt::animateDescent()
{
    return m_animateDescent;
}

QMatrix4x4 VierGewinnt::getMvMatrix()
{
    return m_mvMatrix;
}

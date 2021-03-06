#include "viergewinnt.h"
#include "viergewinntscene.h"

VierGewinnt::VierGewinnt(VierGewinntScene* scene)
    : m_scene(scene)
    , m_player(Player::RedPlayer)
    , m_selectedToken(nullptr)
    , m_lastDistance(START_DISTANCE)
    , m_animationStep(0)
    , m_descentMoveStep(0)
    , m_animateJumpUp(false)
    , m_animateJumpDown(false)
    , m_animateDescent(false)
{
    m_tablePlate = new GLTablePlate("TablePlate");
    m_court = new GLCourt("Court");
    m_rotationStep = 90.0f / ANIMATION_STEPS;
    m_jumpMoveStep = (GLCourt::HEIGHT + GLToken::RADIUS) / ANIMATION_STEPS;

    GLTokenRed* redToken1 = new GLTokenRed("RedToken1");
    redToken1->setShowFrame(false);
    redToken1->move(QVector3D(-2.0f, 0.0f, 5.0f));
    m_redTokens.append(redToken1);

    GLTokenRed* redToken2 = new GLTokenRed("RedToken2");
    redToken2->setShowFrame(false);
    redToken2->move(QVector3D(0.0f, 0.0f, 5.0f));
    m_redTokens.append(redToken2);

    GLTokenRed* redToken3 = new GLTokenRed("RedToken3");
    redToken3->setShowFrame(false);
    redToken3->move(QVector3D(2.0f, 0.0f, 5.0f));
    m_redTokens.append(redToken3);

    GLTokenGreen* greenToken = new GLTokenGreen("GreenToken-1");
    greenToken->move(QVector3D(0.0f, 0.0f, -5.0f));
    m_greenTokens.append(greenToken);

    /*
    for (int i = 0; i < GLCourt::ROWS; i++) {
        for (int j = 0; j < GLCourt::COLUMNS; j++) {
            GLTokenGreen* greenToken = new GLTokenGreen("GreenToken" + QString(i * j));
            greenToken->rotate(-90.0F);
            QVector3D pos = m_court->fieldToPosition(QPoint(i, j));
            greenToken->move(pos);
            m_greenTokens.append(greenToken);
        }
    }
    */
}

VierGewinnt::~VierGewinnt()
{
    qDebug() << "VierGewinnt::~VierGewinnt() called.";
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
    m_tablePlate->draw(renderer);
    m_court->draw(renderer);
    for (auto& token : m_greenTokens) {
        token->draw(renderer);
    }

    for (auto& token : m_redTokens) {
        token->draw(renderer);
    }
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
            token->setShowFrame(true);
            m_selectedToken = token;
        } else {
            float distance = (camera - token->getCenter()).length();
            if (distance < m_lastDistance) {
                m_lastDistance = distance;
                m_selectedToken->setSelected(false);
                m_selectedToken->setShowFrame(false);
                token->setSelected(true);
                token->setShowFrame(true);
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
    m_selectedToken->setShowFrame(false);
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
    float z = newPos.z();
    if (m_player == Player::RedPlayer && newPos.z() < 0) {
        vz = v_Z * z;
    }

    if (m_player == Player::GreenPlayer && newPos.z() > 0) {
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
    }
}

void VierGewinnt::changePlayer()
{
    if (m_player == Player::RedPlayer) {
        m_player = Player::GreenPlayer;
    } else {
        m_player = Player::RedPlayer;
    }
}

void VierGewinnt::descentAnimation()
{
    if(m_descendingToken) {
        if(m_animationStep < ANIMATION_STEPS) {
            m_descendingToken->move(-v_Y * m_descentMoveStep);
            m_animationStep++;
        } else {
            m_descentMoveStep = 0;
            m_animateDescent = false;
            m_descendingToken = nullptr;
            m_animationStep = 0;
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
            m_rotationStep = 90.f / ANIMATION_STEPS;
            m_animationStep = 0;
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
            m_rotationStep = 90.f / ANIMATION_STEPS;
            m_animationStep = 0;
        }
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

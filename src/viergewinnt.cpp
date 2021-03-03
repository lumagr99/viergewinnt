#include "viergewinnt.h"
#include "viergewinntscene.h"

VierGewinnt::VierGewinnt(VierGewinntScene *scene)
{
    qDebug() << "VierGewinnt::VierGewinnt() called.";
    m_scene = scene;
    m_tablePlate = new GLTablePlate("TablePlate");
    m_court = new GLCourt("Court");

    GLTokenRed *redToken = new GLTokenRed("RedToken1");
    redToken->move(QVector3D(0.0f, 0.0f, -5.0f));
    redToken->setShowFrame(false);
    m_redTokens.append(redToken);

    GLTokenGreen *greenToken = new GLTokenGreen("GreenToken1");
    greenToken->move(QVector3D(0.0f, 0.0f, 5.0f));
    greenToken->setShowFrame(false);
    m_greenTokens.append(greenToken);

}

VierGewinnt::~VierGewinnt()
{
    qDebug() << "VierGewinnt::~VierGewinnt() called.";
    for(auto &token : m_greenTokens) {
        delete token;
    }

    for(auto &token : m_redTokens) {
        delete token;
    }
}

void VierGewinnt::draw(GLESRenderer *renderer)
{
    renderer->pushMvMatrix();

    m_tablePlate->draw(renderer);
    m_court->draw(renderer);

    for(auto &token : m_greenTokens) {
        token->draw(renderer);
    }

    for(auto &token : m_redTokens) {
        token->draw(renderer);
    }
    renderer->popMvMatrix();
}

bool VierGewinnt::selectToken(const QVector3D &nearPoint, const QVector3D &farPoint, const QVector3D &camera)
{
    //qDebug() << "VierGewinnt::selectToken() called.";
    if (m_player == RedPlayer) {
        for(auto &token : m_redTokens) {
            checkForSelection(nearPoint, farPoint, camera, token);
        }
        m_player = GreenPlayer;
    } else {
        for(auto &token : m_greenTokens) {
            checkForSelection(nearPoint, farPoint, camera, token);
        }
        m_player = RedPlayer;
    }
    if(m_selectedToken != nullptr) {
        //emit Sound
    }
    return m_selectedToken != nullptr;
}

void VierGewinnt::deselectToken()
{
    //qDebug() << "VierGewinnt::deselectToken() called.";
    if(m_selectedToken == nullptr) {
        return;
    }
}

void VierGewinnt::checkForSelection(const QVector3D &nearPoint, const QVector3D &farPoint, const QVector3D &camera, GLToken *token)
{
    //qDebug() << "VierGewinnt::checkForSelection() called.";
    float lastDistance = 100000.0f;
    if(token->isHit(nearPoint, farPoint)) {
        if(m_selectedToken == nullptr) {
            token->setSelected(true);
            token->setShowFrame(true);
            m_selectedToken = token;
        } else {
            float distance = (camera - token->getCenter()).length();
            if(distance < lastDistance) {
                m_selectedToken->setSelected(false);
                m_selectedToken->setShowFrame(false);
                token->setSelected(true);
                token->setShowFrame(true);
                m_selectedToken = token;
            }
        }
    }
}

void VierGewinnt::moveToken(const QVector3D &vMove)
{
    if(m_selectedToken == nullptr)  {
        return;
    }

    m_selectedToken->move(vMove);

    if(m_court->isColliding(m_selectedToken)) {
        m_selectedToken->move(-vMove);
        if(!m_selectedToken->isRotated()) {
            m_selectedToken->rotate();
            m_selectedToken->move(v_Y * m_selectedToken->getRadius());
        }
    } else if(m_selectedToken->isRotated()){
        m_selectedToken->rotate();
        m_selectedToken->move(-v_Y * m_selectedToken->getRadius());
    }

    for(auto &token : m_redTokens) {
        if(m_selectedToken->isColliding(token)) {
            m_selectedToken->move(-vMove);
        }
    }

    for(auto &token : m_greenTokens) {
        if(m_selectedToken->isColliding(token)) {
            m_selectedToken->move(-vMove);
        }
    }
}

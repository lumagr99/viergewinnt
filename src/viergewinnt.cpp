#include "viergewinnt.h"
#include "viergewinntscene.h"

VierGewinnt::VierGewinnt(VierGewinntScene *scene)
{
    m_scene = scene;
    m_tablePlate = new GLTablePlate("TablePlate");
    m_court = new GLCourt("Court");

}

VierGewinnt::~VierGewinnt()
{

}

void VierGewinnt::draw(GLESRenderer *renderer)
{
    //qDebug() << "VierGewinnt::draw() called.";
    m_tablePlate->draw(renderer);
    m_court->draw(renderer);
}

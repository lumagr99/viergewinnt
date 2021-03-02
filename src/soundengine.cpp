#include "soundengine.h"
#include <QFile>
#include <QDebug>

#define DEBUG_SOUNDENGINE

SoundEngine::SoundEngine(QObject *parent) :
    QObject(parent)
{
    enabled = false;
}
/**
 * @brief SoundEngine::loadSound Preload a file, so that it can be played with low latency afterwrds.
 * @param fileName Must be a .wav file.
 * @return True on success
 */
bool SoundEngine::loadSound(const QString & fileName){
    if(!QFile::exists(fileName))
    {
        qDebug()<< "SoundEngine::playSound File not found: " << fileName;
        return false;
    }
#ifdef DEBUG_SOUNDENGINE
    qDebug() << "SoundEngine::playSound loading sound: " << fileName;
#endif
   QSoundEffect * sound = new QSoundEffect();
#ifdef Q_OS_ANDROID
   sound->setSource(QUrl(fileName));
#else
   sound->setSource(QUrl::fromLocalFile(fileName));
#endif
   sound->setLoopCount(1);
   sound->setVolume(1.0);
   m_sounds.insert( fileName, sound);
   return true;
}

/**
  * Load and play soundfile or resource.
  */
void SoundEngine::playSound(const QString & fileName)
{
    if(enabled)
    {
        QMap<QString, QSoundEffect * >::iterator it = m_sounds.find(fileName);
        if(it == m_sounds.end())//not yet loaded
        {
            if(loadSound(fileName))
               it = m_sounds.find(fileName);
        }
        if(it != m_sounds.end()) //we have got it
            it.value()->play();
    }
}


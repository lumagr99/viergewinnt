/****************************************************************************
 *                     Copyright by Walter Roth 2012                        *
 *                       Licence: GPL                                       *
 ****************************************************************************/
#ifndef SOUNDENGINE_H
#define SOUNDENGINE_H

#include <QObject>
#include <QString>
#include <QSoundEffect>
#include <QMap>

/**
  * Plays sound files when playSound is called. Needs the Phonon library to be installed.
  * Use setEnabled to switch sound playing off/on.
  */
class SoundEngine : public QObject
{
    Q_OBJECT
public:
    explicit SoundEngine(QObject *parent = 0);

    void setEnabled(bool newVal){enabled = newVal;}
    bool isEnabled()const{return enabled;}

signals:
    
public slots:
    /**
     * @brief SoundEngine::loadSound Preload a file, so that it can be played with low latency afterwrds.
     * @param fileName Must be a .wav file.
     * @return True on success
     */
    bool loadSound(const QString &fileName);
    /**
      * Load and play soundfile or resource.
      */
    void playSound(const QString & fileName);

private:
    QMap<QString, QSoundEffect *> m_sounds;
    bool enabled;
};

#endif // SOUNDENGINE_H

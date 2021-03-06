#ifndef QQTMPLAYER_H
#define QQTMPLAYER_H

#include <QProcess>

#include "qqtcore.h"
#include <qqt-local.h>

/**
 * @brief The QQtMPlayer class
 * 使用mplayer为后台播放音视频文件。
 */
class QQTSHARED_EXPORT QQtMPlayer : public QObject
{
    Q_OBJECT
public:
    explicit QQtMPlayer ( QObject* parent = 0 );

    void play ( QString filename, int wid = 0, int width = 20, int height = 20 );
    void pause();
    void stop();
    double timeLength();
    double timePos();
    int percent();
    void seekPos ( double second = 0 );
    void setVolume ( int v = 100 );
    void mute ( bool m = false );
    void setRect ( int x, int y, int width, int height );

signals:

public slots:

private:
    QProcess* app;
};

#endif // QQTMPLAYER_H

#ifndef CAPTURE_THREAD_H
#define CAPTURE_THREAD_H

#include <QString>
#include <QSettings>
#include <QDir>
#include <QThread>
#include <buffer.h>

class captureThread: public QThread
{
public:
    captureThread();
    void connectRTSP();
    void readConfig();
    void saveConfig();
    void stopRTSP();
    void setDistortion(int,int,int);
protected:
    void run();


private:
    QString ip, user, pwd;
    int ratio;
    qint16 nChannelID;
};
extern bool init;

#endif // CAPTURE_THREAD_H

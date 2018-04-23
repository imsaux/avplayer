#ifndef DISTORTION_PACK_H
#define DISTORTION_PACK_H

#include <QDir>
#include <inc/Distortion.h>
class distortion_pack
{
public:
    distortion_pack();
    Mapped* initD(int, int, int);
    void setD(Mapped*, int, int, int);
    void delD(Mapped*);
    void runD(Mapped*, char*, int, int, int);
};

#endif // DISTORTION_PACK_H

#ifndef GLOBAL_H
#define GLOBAL_H

#include "buffer.h"
#include <distortion_pack.h>

extern buffer *imgBuffer;
extern buffer *imgBuffer_distortion;
extern distortion_pack *dp;
extern int imgwidth;
extern int imgheight;
extern int imgratio;
extern QString rtspip, rtspuser, rtsppwd;
extern bool isOpenDistortion;
extern Mapped* map;

#endif // GLOBAL_H

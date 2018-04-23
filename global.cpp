#include "global.h"

buffer *imgBuffer = new buffer(100, true);
//buffer *imgBuffer_show = new buffer(100, true);
buffer *imgBuffer_distortion = new buffer(100, true);
int imgheight=2160;
int imgwidth=3840;
int imgratio=600;
bool isOpenDistortion = true;
distortion_pack *dp= new distortion_pack;
Mapped* map=NULL;
QString rtspip="";
QString rtsppwd="";
QString rtspuser="";

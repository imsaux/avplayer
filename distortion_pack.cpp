#include "distortion_pack.h"


distortion_pack::distortion_pack()
{
}

Mapped* distortion_pack::initD(int w, int h, int r)
{
    Mapped* m=DistortionCreat(w, h);
    DistortionSet(m, w, h, r);
    return m;
}

void distortion_pack::delD(Mapped* map)
{
    DistortionDelete(map);
}

void distortion_pack::setD(Mapped* map, int w, int h, int r)
{
    DistortionSet(map, w, h, r);
}

void distortion_pack::runD(Mapped* map, char* p, int w, int h, int chn)
{
    if(p!=NULL)
    {
        try
        {
            if (map!=NULL)
            {
                DistortionExport(map, (uchar*)p, w, h, chn);
            }
        }
        catch(...)
        {

        }
    }

}



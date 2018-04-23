//#include "stdafx.h"
#include <math.h>
#include <iostream>
#include <fstream>
#include <opencv2\opencv.hpp>

#define DISTORTION_API __declspec(dllexport)
typedef struct Mapped 
{
	int x;				
	int y;
	double w1;
	double w2;
	double w3;
	double w4;	
}Mapped;
#ifdef __cplusplus
extern "C" {
#endif

DISTORTION_API void DistortionExport(Mapped *maped, unsigned char* img, int imgwidth, int imgheight, int imgchannels);//����У��
DISTORTION_API void DistortionSet(Mapped *maped, int imgwidth, int imgheight, int k);//���û���ϵ��k
DISTORTION_API Mapped * DistortionCreat(int imgwidth, int imgheight);//��������ӳ���
DISTORTION_API void DistortionDelete(Mapped *maped);//���ٻ���ӳ���
int DistortionRound(double val);


#ifdef __cplusplus
}
#endif

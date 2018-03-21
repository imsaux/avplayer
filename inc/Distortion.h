//#include "stdafx.h"
#ifndef DISTORTION_API_H
#define DISTORTION_API_H
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

DISTORTION_API void DistortionExport(unsigned char* img, int imgwidth, int imgheight, int imgchannels);//����У��
DISTORTION_API void DistortionSet(int imgwidth, int imgheight, int k);//���û���ϵ��k
DISTORTION_API void DistortionCreat(int imgwidth, int imgheight);//��������ӳ���
DISTORTION_API void DistortionDelete(void);//���ٻ���ӳ���
int DistortionRound(double val);
Mapped* Map;

#ifdef __cplusplus
}
#endif
#endif

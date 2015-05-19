/*****************************************************************************************************
*
*	File Name : CarPlateLocate.h
*	Created : 2015/3/22
*	Author : 陈希明
*	Description:车牌定位类的头文件，包含了对车牌定位类成员变量和成员函数的声明
*
******************************************************************************************************/
#ifndef CARPLATELOCATE_H_
#define CARPLATELOCATE_H_

#include "highgui.h"
#include <cvaux.h>
#include <cv.h>
#include "MyType.h" //自定义数据类型头文件
#include <vector>
using namespace std;

class CarPlateLocate{
public:
	CarPlateLocate();
	explicit CarPlateLocate(char *pFilename);
	void ClearData();
	IplImage* ImgPreProcess(IplImage *pSrcImg);//对资源图像进行灰度化，二值化等图像预处理
	vector<CvRect> FindOutline(IplImage *pImg);//轮廓检测函数
	bool FindConnectDomain(vector<CvRect> *pSampleRects);//连通矩形检测函数
	CvRect SelectPlateRect(vector<CvRect> sampleRects);//筛选车牌矩形函数
	IplImage* LocateCarPlate(IplImage *pSrcImg);//车牌定位函数
	CvRect lastSelect(vector<CvRect> plateRects, IplImage *pSrcImg);//若筛选出符合车牌的矩形不唯一则进行此函数的操作
	double findBluePixArea(IplImage *pSrc);//找到蓝色像素区域，当符合车牌条件的矩形不唯一时，用来找到车牌
	void operator = (CarPlateLocate & carPlateLocate);//重载赋值运算符
	void SetSrcImg(IplImage *pSrcImg);
	IplImage* GetSrcImg();
	char* GetSrcImgName();
	IplImage* GetCarPlateImg();
	LocateErrorEnum GetErrorEnum();

private:
	char *m_srcImgName; //成员变量：包含车牌的资源图片名
	IplImage *m_srcImg; //成员变量：包含车牌的资源图像
	IplImage *m_carPlateImg; //成员变量：定位得到的车牌图像
	LocateErrorEnum m_locateError;//成员变量：错误记录枚举。Normal:正常,PositionException:定位异常,IOException:IO异常,FileTypeException:文件类型异常
};

#endif
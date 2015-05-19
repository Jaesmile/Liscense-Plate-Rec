/**************************************************************
***************************************
*
*	File Name : CarPlateRecog.h
*	Created : 2015/3/22
*	Author : 陈希明
*	Description:车牌识别类的头文件，包含了对车牌识别类成员变量和成员函数的声明
*
***************************************************************
***************************************/
#ifndef CARPLATERECOG_H_
#define CARPLATERECOG_H_

#include "highgui.h"
#include <cvaux.h>
#include <cv.h>
#include "MyType.h" //自定义数据类型头文件
#include <vector>
#include <string>
#include <string.h>
using namespace std;

class CarPlateRecog
{
public:
	CarPlateRecog();
	explicit CarPlateRecog(IplImage *pPlateImg);
	void ClearData();
	void DoGray(IplImage *pPlateImg);//灰度化处理
	void DoBinaryZ(IplImage *pGrayImg);//二值化处理
	void DoDenois(IplImage *pBinaryZImg);//去噪处理
	vector<CvRect> SortRectByXAsc(vector<CvRect> rawPlateCharRects);//按矩形X值由小到大排列矩形
	vector<CvRect> FindOutline(IplImage *pImg, bool fromBottom, bool fromTop);//初次车牌轮廓检测
	vector<CvRect> FindPlateCharRects(vector<CvRect> outlineRects);//车牌字符检测函数
	vector<CvRect> FindOutlineSecond(IplImage *pImg);//对截取后的车牌进行轮廓检测
	void SwitchDeal(vector<CvRect> plateCharRects);//分支处理函数
	void SegmPlateChar(IplImage *pPlateImg);//车牌字符分割函数
	void PlateCharRecog(vector<IplImage*> plateCharImgs);//车牌字符识别函数
	CString ProcPicDir(CString strPicDir, IplImage *pSrcImg);//模板匹配函数
	void operator = (CarPlateRecog & carPlateRecog);//重载的赋值运算符
	IplImage* GetCarPlateImg();
	void SetCarPlateImg(IplImage *pImg);
	vector<IplImage*> GetPlateCharImgs();
	void SetPlateCharImgs(vector<IplImage*> imgs);
	IplImage* GetGrayImg();
	IplImage* GetBinaryZImg();
	IplImage* GetDenoisImg();
	CString GetPlateChars();
	RecogErrorEnum GetErrorEnum();
	double GetAvgBaiFenBi();

	int fuck;

private:
	IplImage *m_carPlateImg;//成员变量,车牌资源图像
	IplImage *m_grayImg;//成员变量,灰度化处理后的车牌图像
	IplImage *m_binaryZImg;//成员变量,二值化处理后的车牌图像
	IplImage *m_denoisImg;//成员变量,去噪后的车牌图像
	vector<IplImage*> m_plateCharImgs;//成员变量,切割后的车牌字符图像
	CString m_plateCharInfo;//成员变量,识别后的车牌字符信息
	RecogErrorEnum m_recogErrorEnum;//成员变量,错误记录枚举。RecogNormal:正常,RecogIOException:IO异常,SegmException:切割异常,RecogException:识别异常
	double m_avgBaiFenBi;//成员变量,识别率
};

#endif
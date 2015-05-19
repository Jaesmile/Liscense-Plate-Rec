/*****************************************************************************************************
*
*	File Name : CarPlateLocate.h
*	Created : 2015/3/22
*	Author : ��ϣ��
*	Description:���ƶ�λ���ͷ�ļ��������˶Գ��ƶ�λ���Ա�����ͳ�Ա����������
*
******************************************************************************************************/
#ifndef CARPLATELOCATE_H_
#define CARPLATELOCATE_H_

#include "highgui.h"
#include <cvaux.h>
#include <cv.h>
#include "MyType.h" //�Զ�����������ͷ�ļ�
#include <vector>
using namespace std;

class CarPlateLocate{
public:
	CarPlateLocate();
	explicit CarPlateLocate(char *pFilename);
	void ClearData();
	IplImage* ImgPreProcess(IplImage *pSrcImg);//����Դͼ����лҶȻ�����ֵ����ͼ��Ԥ����
	vector<CvRect> FindOutline(IplImage *pImg);//������⺯��
	bool FindConnectDomain(vector<CvRect> *pSampleRects);//��ͨ���μ�⺯��
	CvRect SelectPlateRect(vector<CvRect> sampleRects);//ɸѡ���ƾ��κ���
	IplImage* LocateCarPlate(IplImage *pSrcImg);//���ƶ�λ����
	CvRect lastSelect(vector<CvRect> plateRects, IplImage *pSrcImg);//��ɸѡ�����ϳ��Ƶľ��β�Ψһ����д˺����Ĳ���
	double findBluePixArea(IplImage *pSrc);//�ҵ���ɫ�������򣬵����ϳ��������ľ��β�Ψһʱ�������ҵ�����
	void operator = (CarPlateLocate & carPlateLocate);//���ظ�ֵ�����
	void SetSrcImg(IplImage *pSrcImg);
	IplImage* GetSrcImg();
	char* GetSrcImgName();
	IplImage* GetCarPlateImg();
	LocateErrorEnum GetErrorEnum();

private:
	char *m_srcImgName; //��Ա�������������Ƶ���ԴͼƬ��
	IplImage *m_srcImg; //��Ա�������������Ƶ���Դͼ��
	IplImage *m_carPlateImg; //��Ա��������λ�õ��ĳ���ͼ��
	LocateErrorEnum m_locateError;//��Ա�����������¼ö�١�Normal:����,PositionException:��λ�쳣,IOException:IO�쳣,FileTypeException:�ļ������쳣
};

#endif
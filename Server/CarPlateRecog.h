/**************************************************************
***************************************
*
*	File Name : CarPlateRecog.h
*	Created : 2015/3/22
*	Author : ��ϣ��
*	Description:����ʶ�����ͷ�ļ��������˶Գ���ʶ�����Ա�����ͳ�Ա����������
*
***************************************************************
***************************************/
#ifndef CARPLATERECOG_H_
#define CARPLATERECOG_H_

#include "highgui.h"
#include <cvaux.h>
#include <cv.h>
#include "MyType.h" //�Զ�����������ͷ�ļ�
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
	void DoGray(IplImage *pPlateImg);//�ҶȻ�����
	void DoBinaryZ(IplImage *pGrayImg);//��ֵ������
	void DoDenois(IplImage *pBinaryZImg);//ȥ�봦��
	vector<CvRect> SortRectByXAsc(vector<CvRect> rawPlateCharRects);//������Xֵ��С�������о���
	vector<CvRect> FindOutline(IplImage *pImg, bool fromBottom, bool fromTop);//���γ����������
	vector<CvRect> FindPlateCharRects(vector<CvRect> outlineRects);//�����ַ���⺯��
	vector<CvRect> FindOutlineSecond(IplImage *pImg);//�Խ�ȡ��ĳ��ƽ����������
	void SwitchDeal(vector<CvRect> plateCharRects);//��֧������
	void SegmPlateChar(IplImage *pPlateImg);//�����ַ��ָ��
	void PlateCharRecog(vector<IplImage*> plateCharImgs);//�����ַ�ʶ����
	CString ProcPicDir(CString strPicDir, IplImage *pSrcImg);//ģ��ƥ�亯��
	void operator = (CarPlateRecog & carPlateRecog);//���صĸ�ֵ�����
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
	IplImage *m_carPlateImg;//��Ա����,������Դͼ��
	IplImage *m_grayImg;//��Ա����,�ҶȻ������ĳ���ͼ��
	IplImage *m_binaryZImg;//��Ա����,��ֵ�������ĳ���ͼ��
	IplImage *m_denoisImg;//��Ա����,ȥ���ĳ���ͼ��
	vector<IplImage*> m_plateCharImgs;//��Ա����,�и��ĳ����ַ�ͼ��
	CString m_plateCharInfo;//��Ա����,ʶ���ĳ����ַ���Ϣ
	RecogErrorEnum m_recogErrorEnum;//��Ա����,�����¼ö�١�RecogNormal:����,RecogIOException:IO�쳣,SegmException:�и��쳣,RecogException:ʶ���쳣
	double m_avgBaiFenBi;//��Ա����,ʶ����
};

#endif
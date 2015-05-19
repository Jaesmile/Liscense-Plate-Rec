#ifndef MYTYPE_H_
#define MYTYPE_H_

typedef struct tagChoiceDone{
	bool DoneLocate;
	bool DoneGray;//�Ƿ��ѽ��й��ҶȻ�����
	bool DoneBinaryZ;//�Ƿ��ѽ��й���ֵ������
	bool DoneDenois;//�Ƿ��ѽ���ȥ�봦��
	bool DoneSegm;//�Ƿ��ѽ����и��
}Choice_Done;

typedef enum tagLocateErrorEnum{
	Normal,//����
	IOException,//IO�쳣
	FileTypeException,//�ļ������쳣
	PositionException,//��λ�쳣
}LocateErrorEnum;

typedef enum tagRecogErrorEnum{
	RecogNormal,//����
	RecogIOException,//IO�쳣
	SegmException,//�и��쳣
	RecogException,//ʶ���쳣
}RecogErrorEnum;

#endif
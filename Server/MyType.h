#ifndef MYTYPE_H_
#define MYTYPE_H_

typedef struct tagChoiceDone{
	bool DoneLocate;
	bool DoneGray;//是否已进行过灰度化处理
	bool DoneBinaryZ;//是否已进行过二值化处理
	bool DoneDenois;//是否已进行去噪处理
	bool DoneSegm;//是否已进行切割处理
}Choice_Done;

typedef enum tagLocateErrorEnum{
	Normal,//正常
	IOException,//IO异常
	FileTypeException,//文件类型异常
	PositionException,//定位异常
}LocateErrorEnum;

typedef enum tagRecogErrorEnum{
	RecogNormal,//正常
	RecogIOException,//IO异常
	SegmException,//切割异常
	RecogException,//识别异常
}RecogErrorEnum;

#endif
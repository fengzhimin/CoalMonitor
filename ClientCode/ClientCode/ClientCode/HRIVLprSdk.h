#ifndef  _HRIVLPR_SDK_H
#define  _HRIVLPR_SDK_H

#ifdef HRIVLPR_API

#else
#define HRIVLPR_API extern "C" _declspec(dllexport)
#endif

#ifndef  NULL
#define  NULL   (0)
#endif

#define  MAX_LPR_NUM   4

typedef struct _HRIVLpr_Result
{
	int num;                                       //车牌数目
	int iLeftTopX[MAX_LPR_NUM];                  //车牌定位结果
	int iLeftTopY[MAX_LPR_NUM];                  //车牌定位结果
	int iRightBotX[MAX_LPR_NUM];                  //车牌定位结果
	int iRightBotY[MAX_LPR_NUM];                  //车牌定位结果
	char cLprCharacter[MAX_LPR_NUM][20];           //车牌字符
	char cLprColor[MAX_LPR_NUM][4];				   //车牌颜色
	int  iPlateReliability[MAX_LPR_NUM];           //车牌置信度
}HRIVLpr_Result, *lpHRIVLpr_Result;

/*感兴趣区域设置*/
typedef enum _ROI_MODE
{
	ROI_MOD_SET_IMAGE_ROI = 0, //设置识别区域
	ROI_MOD_SET_LPR_LOCATION_VALID_ROI = 1, //设置车牌定位有效区域
}ROI_MODE;

/*视频格式*/
typedef enum _VIDEOFORMAT
{
	YUV420P = 0, //yuv420 plane
	UYVY    = 1, //yuv422, uyvyuyvy
	YU12    = 2, //yuv422  yyyyyyuv
	YV12    = 3, //yyyyyyvu
	YVYU    = 4, //yuv422  yvyuyvyu
	RGB24   = 5, //RGB24，BGRBGR
	RGB32   = 6, //RGB32, RGBARGBA
	RGB555  = 7, //RGB16
	RGB565  = 8  //RGB16
}VIDEOFORMAT;

/*单帧图像信息*/
typedef  struct  _HRIVImageInfo
{
	int  format;				//0:YUV420P, 1:UYVY; 2:YU12, 3:YVYU, 4:RGB24, 5:RGB32, 6:RGB555, 7:RGB565
	int  width;					//video width
	int  height;				//video height	
	unsigned char *buffer[4];	//4 channels video buffer
	int  stride[4];				//4 channels video linesteps
	int  framerate;				//frame rates, fps
	unsigned int timestamp;		//time stamp
} HRIVImageInfo, *lpHRIVImageInfo;

/*视频流识别参数*/
typedef struct _HRIVVideoLprParam
{
	int frameinterval;        //视频识别间隔帧数,1-10
	unsigned int bSnap;       //是否抓图，1-抓图，0-不抓图
	unsigned int imageFormat; //图像输出格式，1-JPEG，2-BMP
	unsigned int lprframes;   //连续识别帧数
}HRIVVideoParam, *lpHRIVVideoLprParm;

#ifdef __cplusplus
extern "C"
{
#endif

	/*函 数 名：HRIVLprInit
	说    明：车牌识别初始化
	参数输入：cProvince，车牌识别优先省份
	参数输出：无
	返 回 值：  
	-1  加密初始化失败（初始化加密狗失败，程序过使用期限）
	1  初始化成功
	修改说明：
	*/
	HRIVLPR_API long __stdcall HRIVLprInit(void);

	/*函 数  HRIVLprSetRecogRoi
	说    明：设置感兴趣区域
	参数输入：lTopLeftX，感兴趣区域矩形左上角点x坐标
	lTopLeftY，感兴趣区域矩形左上角点y坐标
	lBotRightX，感兴趣区域矩形右下角点x坐标
	lBotRightY，感兴趣区域矩形右下角点y坐标
	mode，感兴趣区域设置模式
	参数输出：无
	返 回 值：设置成功：1，失败：-1
	修改说明：
	*/
	HRIVLPR_API long __stdcall HRIVLprSetRecogRoi(long lTopLeftX, long lTopLeftY, long lBotRightX, long lBotRightY, long mode);

	/*函 数 名：HRIVLprFree
	说    明：释放车牌识别
	参数输入：无
	参数输出：无
	返 回 值：
	修改说明：
	*/
	HRIVLPR_API void __stdcall HRIVLprFree(void);

	/*函 数 名：HRIVLprProc
	说    明：基于视频的车牌识别处理函数
	参数输入：
		pImgBuffer，  图像内存，排列形式BGRBGRBGR...
		m_lImgWidth， 图像宽度
		m_lImgHeight，图像高度
	参数输出：long
		pResult，     识别结果，参照 HRIVLpr_Result
	 返 回 值：成功：1，失败：-1
	修改说明：
	*/
	HRIVLPR_API long __stdcall HRIVLprProc(long pImgBuffer, long m_lImgWidth, long m_lImgHeight, long pResult);
	
	/*函 数 HRIVLprProcEx
	说    明：基于图片路径的车牌识别处理函数
	参数输入：
		filePath, 图片的绝对路径, 支持JPEG和BMP两种数据格式
	参数输出：
		pResult， 识别结果，参照 HRIVLpr_Result
	返 回 值：成功：1，失败：-1,  不支持的图像格式: 0
	修改说明：
	*/
	HRIVLPR_API long __stdcall HRIVLprProcEx(char *filePath, void* pResult);

	/*函 数 HRIVLprProcVideo
	说    明：基于视频流抓拍的车牌识别
	参数输入：
		pFrameBuffer， HRIVImageInfo 视频结构体
	参数输出：
		pResult， 识别结果，参照 HRIVLpr_Result
	返 回 值：抓拍输出：1，不抓拍：-1
	修改说明：
	*/
	HRIVLPR_API long __stdcall HRIVLprProcVideo(long pFrameBuffer, void *pResult);


	/*函 数 HRIVLprProcVideoEx
	说    明：基于视频流抓拍的车牌识别
	参数输入：
		pFrameBuffer， HRIVImageInfo 视频结构体
		pParam, 抓拍参数设置
	参数输出：
		pResult， 识别结果，参照 HRIVLpr_Result
	参数输入和输出:
		imagefilename, 抓拍图像路径全称，不抓图时，imagefilename设置为NULL
	返 回 值：识别输出：1，抓拍输出：2，不抓拍：-1
	修改说明：
	*/
	HRIVLPR_API long __stdcall HRIVLprProcVideoEx(long pFrameBuffer, long pParam, long pResult, char *imagefilename);

	/*函 数 名：HRIVLprSetCredit
	说    明：设置置信度
	参数输入：m_lCredit， 置信度0~1000
	参数输出：无
	返 回 值：成功：1，失败：-1
	修改说明：
	*/
	HRIVLPR_API long __stdcall HRIVLprSetCredit(long m_lCredit);

	/*函 数 名：HRIVLprSetProvince
	说    明：设置优先识别省份
	参数输入：cProvince， 识别优先省份内存地址，33个省市自治区直辖市，如char cProvine[2] = {"京"}传递参数为(long)cProvine
	参数输出：无
	返 回 值：成功：1，失败：-1
	修改说明：
	*/
	HRIVLPR_API long __stdcall HRIVLprSetProvince(char *cProvince);
	
	/*函 数 HRIVLprSetScale
	说    明：设置车牌识别图片比例因子，默认100，大小为1~200
	参数输入：
		m_lScale, 比例系数
	返 回 值：成功：1，失败：-1,  不支持的图像格式: 0
	修改说明：
	*/
	HRIVLPR_API long __stdcall HRIVLprSetScale(int m_lScale);

	/*函 数 HRIVLprGetScale
	说    明：获取车牌识别最佳比例因子
	参数输入：
		filePath, 图片的绝对路径, 支持JPEG和BMP两种数据格式
	返 回 值：获取车牌识别最佳比例因子
	修改说明：
	*/
	HRIVLPR_API long __stdcall HRIVLprGetScale(char *filePath);

	/*函 数 HRIVLprProcExNew
	说    明：基于图片路径的车牌识别处理函数
	参数输入：
		filePath, 图片的绝对路径, 支持JPEG和BMP两种数据格式
	参数输出：
		pResult， 识别结果，参照 HRIVLpr_Result
	返 回 值：成功：1，失败：-1,  不支持的图像格式: 0
	修改说明：
	*/
	HRIVLPR_API long __stdcall HRIVLprProcExNew(char *filePath, void* pResult);

#ifdef __cplusplus
};
#endif

#endif

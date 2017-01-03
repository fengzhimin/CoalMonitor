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
	int num;                                       //������Ŀ
	int iLeftTopX[MAX_LPR_NUM];                  //���ƶ�λ���
	int iLeftTopY[MAX_LPR_NUM];                  //���ƶ�λ���
	int iRightBotX[MAX_LPR_NUM];                  //���ƶ�λ���
	int iRightBotY[MAX_LPR_NUM];                  //���ƶ�λ���
	char cLprCharacter[MAX_LPR_NUM][20];           //�����ַ�
	char cLprColor[MAX_LPR_NUM][4];				   //������ɫ
	int  iPlateReliability[MAX_LPR_NUM];           //�������Ŷ�
}HRIVLpr_Result, *lpHRIVLpr_Result;

/*����Ȥ��������*/
typedef enum _ROI_MODE
{
	ROI_MOD_SET_IMAGE_ROI = 0, //����ʶ������
	ROI_MOD_SET_LPR_LOCATION_VALID_ROI = 1, //���ó��ƶ�λ��Ч����
}ROI_MODE;

/*��Ƶ��ʽ*/
typedef enum _VIDEOFORMAT
{
	YUV420P = 0, //yuv420 plane
	UYVY    = 1, //yuv422, uyvyuyvy
	YU12    = 2, //yuv422  yyyyyyuv
	YV12    = 3, //yyyyyyvu
	YVYU    = 4, //yuv422  yvyuyvyu
	RGB24   = 5, //RGB24��BGRBGR
	RGB32   = 6, //RGB32, RGBARGBA
	RGB555  = 7, //RGB16
	RGB565  = 8  //RGB16
}VIDEOFORMAT;

/*��֡ͼ����Ϣ*/
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

/*��Ƶ��ʶ�����*/
typedef struct _HRIVVideoLprParam
{
	int frameinterval;        //��Ƶʶ����֡��,1-10
	unsigned int bSnap;       //�Ƿ�ץͼ��1-ץͼ��0-��ץͼ
	unsigned int imageFormat; //ͼ�������ʽ��1-JPEG��2-BMP
	unsigned int lprframes;   //����ʶ��֡��
}HRIVVideoParam, *lpHRIVVideoLprParm;

#ifdef __cplusplus
extern "C"
{
#endif

	/*�� �� ����HRIVLprInit
	˵    ��������ʶ���ʼ��
	�������룺cProvince������ʶ������ʡ��
	�����������
	�� �� ֵ��  
	-1  ���ܳ�ʼ��ʧ�ܣ���ʼ�����ܹ�ʧ�ܣ������ʹ�����ޣ�
	1  ��ʼ���ɹ�
	�޸�˵����
	*/
	HRIVLPR_API long __stdcall HRIVLprInit(void);

	/*�� ��  HRIVLprSetRecogRoi
	˵    �������ø���Ȥ����
	�������룺lTopLeftX������Ȥ����������Ͻǵ�x����
	lTopLeftY������Ȥ����������Ͻǵ�y����
	lBotRightX������Ȥ����������½ǵ�x����
	lBotRightY������Ȥ����������½ǵ�y����
	mode������Ȥ��������ģʽ
	�����������
	�� �� ֵ�����óɹ���1��ʧ�ܣ�-1
	�޸�˵����
	*/
	HRIVLPR_API long __stdcall HRIVLprSetRecogRoi(long lTopLeftX, long lTopLeftY, long lBotRightX, long lBotRightY, long mode);

	/*�� �� ����HRIVLprFree
	˵    �����ͷų���ʶ��
	�������룺��
	�����������
	�� �� ֵ��
	�޸�˵����
	*/
	HRIVLPR_API void __stdcall HRIVLprFree(void);

	/*�� �� ����HRIVLprProc
	˵    ����������Ƶ�ĳ���ʶ������
	�������룺
		pImgBuffer��  ͼ���ڴ棬������ʽBGRBGRBGR...
		m_lImgWidth�� ͼ����
		m_lImgHeight��ͼ��߶�
	���������long
		pResult��     ʶ���������� HRIVLpr_Result
	 �� �� ֵ���ɹ���1��ʧ�ܣ�-1
	�޸�˵����
	*/
	HRIVLPR_API long __stdcall HRIVLprProc(long pImgBuffer, long m_lImgWidth, long m_lImgHeight, long pResult);
	
	/*�� �� HRIVLprProcEx
	˵    ��������ͼƬ·���ĳ���ʶ������
	�������룺
		filePath, ͼƬ�ľ���·��, ֧��JPEG��BMP�������ݸ�ʽ
	���������
		pResult�� ʶ���������� HRIVLpr_Result
	�� �� ֵ���ɹ���1��ʧ�ܣ�-1,  ��֧�ֵ�ͼ���ʽ: 0
	�޸�˵����
	*/
	HRIVLPR_API long __stdcall HRIVLprProcEx(char *filePath, void* pResult);

	/*�� �� HRIVLprProcVideo
	˵    ����������Ƶ��ץ�ĵĳ���ʶ��
	�������룺
		pFrameBuffer�� HRIVImageInfo ��Ƶ�ṹ��
	���������
		pResult�� ʶ���������� HRIVLpr_Result
	�� �� ֵ��ץ�������1����ץ�ģ�-1
	�޸�˵����
	*/
	HRIVLPR_API long __stdcall HRIVLprProcVideo(long pFrameBuffer, void *pResult);


	/*�� �� HRIVLprProcVideoEx
	˵    ����������Ƶ��ץ�ĵĳ���ʶ��
	�������룺
		pFrameBuffer�� HRIVImageInfo ��Ƶ�ṹ��
		pParam, ץ�Ĳ�������
	���������
		pResult�� ʶ���������� HRIVLpr_Result
	������������:
		imagefilename, ץ��ͼ��·��ȫ�ƣ���ץͼʱ��imagefilename����ΪNULL
	�� �� ֵ��ʶ�������1��ץ�������2����ץ�ģ�-1
	�޸�˵����
	*/
	HRIVLPR_API long __stdcall HRIVLprProcVideoEx(long pFrameBuffer, long pParam, long pResult, char *imagefilename);

	/*�� �� ����HRIVLprSetCredit
	˵    �����������Ŷ�
	�������룺m_lCredit�� ���Ŷ�0~1000
	�����������
	�� �� ֵ���ɹ���1��ʧ�ܣ�-1
	�޸�˵����
	*/
	HRIVLPR_API long __stdcall HRIVLprSetCredit(long m_lCredit);

	/*�� �� ����HRIVLprSetProvince
	˵    ������������ʶ��ʡ��
	�������룺cProvince�� ʶ������ʡ���ڴ��ַ��33��ʡ��������ֱϽ�У���char cProvine[2] = {"��"}���ݲ���Ϊ(long)cProvine
	�����������
	�� �� ֵ���ɹ���1��ʧ�ܣ�-1
	�޸�˵����
	*/
	HRIVLPR_API long __stdcall HRIVLprSetProvince(char *cProvince);
	
	/*�� �� HRIVLprSetScale
	˵    �������ó���ʶ��ͼƬ�������ӣ�Ĭ��100����СΪ1~200
	�������룺
		m_lScale, ����ϵ��
	�� �� ֵ���ɹ���1��ʧ�ܣ�-1,  ��֧�ֵ�ͼ���ʽ: 0
	�޸�˵����
	*/
	HRIVLPR_API long __stdcall HRIVLprSetScale(int m_lScale);

	/*�� �� HRIVLprGetScale
	˵    ������ȡ����ʶ����ѱ�������
	�������룺
		filePath, ͼƬ�ľ���·��, ֧��JPEG��BMP�������ݸ�ʽ
	�� �� ֵ����ȡ����ʶ����ѱ�������
	�޸�˵����
	*/
	HRIVLPR_API long __stdcall HRIVLprGetScale(char *filePath);

	/*�� �� HRIVLprProcExNew
	˵    ��������ͼƬ·���ĳ���ʶ������
	�������룺
		filePath, ͼƬ�ľ���·��, ֧��JPEG��BMP�������ݸ�ʽ
	���������
		pResult�� ʶ���������� HRIVLpr_Result
	�� �� ֵ���ɹ���1��ʧ�ܣ�-1,  ��֧�ֵ�ͼ���ʽ: 0
	�޸�˵����
	*/
	HRIVLPR_API long __stdcall HRIVLprProcExNew(char *filePath, void* pResult);

#ifdef __cplusplus
};
#endif

#endif

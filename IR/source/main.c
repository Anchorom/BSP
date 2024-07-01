#include "IR.h"
#include "STC15F2K60S2.H" //���롣
#include "displayer.h"
#include "key.h"
#include "sys.H" //���롣

code unsigned long SysClock = 11059200; // ���롣����ϵͳ����ʱ��Ƶ��(Hz)���û������޸ĳ���ʵ�ʹ���Ƶ�ʣ�����ʱѡ��ģ�һ��
#ifdef _displayer_H_					// ��ʾģ��ѡ��ʱ���롣���������ʾ������Ñ����޸ġ����ӵȣ�
code char decode_table[] = {0x3f, 0x06, 0x5b, 0x4f, 0x66, 0x6d, 0x7d, 0x07, 0x7f, 0x6f, 0x00, 0x08, 0x40, 0x01, 0x41, 0x48, 0x76, 0x38, 0x40, 0x00,
							/* ���:   0   	1    2	   3    4	    5    6	  7   8	   	9	 		10	11	 12   13   14   15    16   17   18   19	 */
							/* ��ʾ:   0   	1    2     3    4     5    6    7   8    9  (��)   ��-  ��-  ��-  ����-  ����-  H    L    - 	���ޣ�*/
							0x3f | 0x80, 0x06 | 0x80, 0x5b | 0x80, 0x4f | 0x80, 0x66 | 0x80, 0x6d | 0x80, 0x7d | 0x80, 0x07 | 0x80, 0x7f | 0x80, 0x6f | 0x80};
/* ��С����     20         21         22         23      24        25        26        27        28        29        */
#endif

unsigned char rxd[5];
unsigned char buffer[5] = {0x01, 0x01, 0x01, 0x01, 0x01};

void ir_callback();
void key_callback();

void main()
{
	// ��ʼ��
	KeyInit();
	IrInit(NEC_R05d);
	DisplayerInit();
	SetDisplayerArea(0, 7);
	Seg7Print(10, 10, 10, 10, 10, 10, 10, 10);
	SetIrRxd(rxd, 5);
	// ���ûص�
	SetEventCallBack(enumEventIrRxd, ir_callback);
	SetEventCallBack(enumEventKey, key_callback);
	MySTC_Init();
	while (1)
	{
		MySTC_OS();
	}
}

void ir_callback()
{
	unsigned char sum = 0x00;
	int i;
	for (i = 0; i < 5; i++)
	{
		sum += rxd[i];
	}
	LedPrint(sum);
}

void key_callback()
{
	if (GetKeyAct(enumKey1) == enumKeyPress)
		IrPrint(buffer, 5);
}

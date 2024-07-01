#include "M24C02.h"
#include "STC15F2K60S2.H" //���롣
#include "displayer.h"
#include "sys.H" //���롣

code unsigned long SysClock = 11059200; // ���롣����ϵͳ����ʱ��Ƶ��(Hz)���û������޸ĳ���ʵ�ʹ���Ƶ�ʣ�����ʱѡ��ģ�һ��
#ifdef _displayer_H_					// ��ʾģ��ѡ��ʱ���롣���������ʾ��������Ñ����޸ġ����ӵȣ�
code char decode_table[] = {0x3f, 0x06, 0x5b, 0x4f, 0x66, 0x6d, 0x7d, 0x07, 0x7f, 0x6f, 0x00, 0x08, 0x40, 0x01, 0x41, 0x48, 0x76, 0x38, 0x40, 0x00,
							/* ���:   0   	1    2	   3    4	    5    6	  7   8	   	9	 		10	11	 12   13   14   15    16   17   18   19	 */
							/* ��ʾ:   0   	1    2     3    4     5    6    7   8    9  (��)   ��-  ��-  ��-  ����-  ����-  H    L    - 	���ޣ�*/
							0x3f | 0x80, 0x06 | 0x80, 0x5b | 0x80, 0x4f | 0x80, 0x66 | 0x80, 0x6d | 0x80, 0x7d | 0x80, 0x07 | 0x80, 0x7f | 0x80, 0x6f | 0x80};
/* ��С����     20         21         22         23      24        25        26        27        28        29        */
#endif

void main()
{
	unsigned char x;
	DisplayerInit();
	x = M24C02_Read(0x01) + 1;
	M24C02_Write(0x01, x);
	SetDisplayerArea(0, 7);
	Seg7Print(10, 10, 10, 10, 10, 10, 10, 10);
	LedPrint(x);
	MySTC_Init();
	while (1)
	{
		MySTC_OS();
	}
}
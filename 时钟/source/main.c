//******* �û������1���û���������ļ� *************
#include "DS1302.h"
#include "STC15F2K60S2.H" //���롣
#include "displayer.H"
#include "sys.H"						//���롣
code unsigned long SysClock = 11059200; // ���롣����ϵͳ����ʱ��Ƶ��(Hz)���û������޸ĳ���ʵ�ʹ���Ƶ�ʣ�����ʱѡ��ģ�һ��
#ifdef _displayer_H_					// ��ʾģ��ѡ��ʱ���롣���������ʾ������Ñ����޸ġ����ӵȣ�
code char decode_table[] = {0x3f, 0x06, 0x5b, 0x4f, 0x66, 0x6d, 0x7d, 0x07, 0x7f, 0x6f, 0x00, 0x08, 0x40, 0x01, 0x41, 0x48,
							/* ���:   0   1    2	   3    4	    5    6	  7   8	   9	 10	   11		12   13    14     15     */
							/* ��ʾ:   0   1    2    3    4     5    6    7   8    9  (��)   ��-  ��-  ��-  ����-  ����-   */
							0x3f | 0x80, 0x06 | 0x80, 0x5b | 0x80, 0x4f | 0x80, 0x66 | 0x80, 0x6d | 0x80, 0x7d | 0x80, 0x07 | 0x80, 0x7f | 0x80, 0x6f | 0x80};
/* ��С����     0         1         2         3         4         5         6         7         8         9        */
#endif
unsigned char x[45];
float a, b, c, d;

struct_DS1302_RTC time = {0x30, 0, 3, 22, 8, 1, 0x22};
struct_DS1302_RTC tmp;
void my1mS_callback()
{
	tmp = RTC_Read();
	Seg7Print(tmp.hour / 16, tmp.hour % 16, 12, tmp.minute / 16, tmp.minute % 16, 12, tmp.second / 16, tmp.second % 16);
}

void main()
{
	DisplayerInit();
	SetDisplayerArea(0, 7);
	Seg7Print(10, 10, 10, 10, 10, 10, 10, 10);
	DS1302Init(time);
	RTC_Write(time);
	SetEventCallBack(enumEventSys1mS, my1mS_callback);
	MySTC_Init();
	while (1)
	{
		MySTC_OS();
	}
}

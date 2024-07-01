//******* 用户程序段1：用户程序包含文件 *************
#include "DS1302.h"
#include "STC15F2K60S2.H" //必须。
#include "displayer.H"
#include "sys.H"						//必须。
code unsigned long SysClock = 11059200; // 必须。定义系统工作时钟频率(Hz)，用户必须修改成与实际工作频率（下载时选择的）一致
#ifdef _displayer_H_					// 显示模块选用时必须。（数码管显示译码表，用戶可修改、增加等）
code char decode_table[] = {0x3f, 0x06, 0x5b, 0x4f, 0x66, 0x6d, 0x7d, 0x07, 0x7f, 0x6f, 0x00, 0x08, 0x40, 0x01, 0x41, 0x48,
							/* 序号:   0   1    2	   3    4	    5    6	  7   8	   9	 10	   11		12   13    14     15     */
							/* 显示:   0   1    2    3    4     5    6    7   8    9  (无)   下-  中-  上-  上中-  中下-   */
							0x3f | 0x80, 0x06 | 0x80, 0x5b | 0x80, 0x4f | 0x80, 0x66 | 0x80, 0x6d | 0x80, 0x7d | 0x80, 0x07 | 0x80, 0x7f | 0x80, 0x6f | 0x80};
/* 带小数点     0         1         2         3         4         5         6         7         8         9        */
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

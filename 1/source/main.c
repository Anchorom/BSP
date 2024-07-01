#include "STC15F2K60S2.H" //必须。
#include "sys.H"		  //必须。
#include "uart1.h"

code unsigned long SysClock = 11059200; // 必须。定义系统工作时钟频率(Hz)，用户必须修改成与实际工作频率（下载时选择的）一致
#ifdef _displayer_H_					// 显示模块选用时必须。（数码管显示译码表，用戶可修改、增加等）
code char decode_table[] = {0x3f, 0x06, 0x5b, 0x4f, 0x66, 0x6d, 0x7d, 0x07, 0x7f, 0x6f, 0x00, 0x08, 0x40, 0x01, 0x41, 0x48, 0x76, 0x38, 0x40, 0x00,
							/* 序号:   0   	1    2	   3    4	    5    6	  7   8	   	9	 		10	11	 12   13   14   15    16   17   18   19	 */
							/* 显示:   0   	1    2     3    4     5    6    7   8    9  (无)   下-  中-  上-  上中-  中下-  H    L    - 	（无）*/
							0x3f | 0x80, 0x06 | 0x80, 0x5b | 0x80, 0x4f | 0x80, 0x66 | 0x80, 0x6d | 0x80, 0x7d | 0x80, 0x07 | 0x80, 0x7f | 0x80, 0x6f | 0x80};
/* 带小数点     20         21         22         23      24        25        26        27        28        29        */
#endif

char rxd[10];
char buffer[10];
char matchhead[2] = {0xaa, 0x55};

void uart1rxd_callback();

void main()
{
	// 串口初始化
	Uart1Init(2400);
	SetUart1Rxd(rxd, 10, matchhead, 2);
	SetEventCallBack(enumEventUart1Rxd, uart1rxd_callback);
	MySTC_Init();
	while (1)
	{
		MySTC_OS();
	}
}

void uart1rxd_callback()
{
	int i;
	for (i = 9; i >= 0; i--)
	{
		buffer[i] = rxd[9 - i];
	}
	Uart1Print(buffer, 10);
	return;
}

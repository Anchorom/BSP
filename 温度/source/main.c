#include "STC15F2K60S2.H" //必须。
#include "adc.h"
#include "displayer.h"
#include "sys.H" //必须。

code unsigned long SysClock = 11059200; // 必须。定义系统工作时钟频率(Hz)，用户必须修改成与实际工作频率（下载时选择的）一致
#ifdef _displayer_H_					// 显示模块选用时必须。（数码管显示译码表，用戶可修改、增加等）
code char decode_table[] = {0x3f, 0x06, 0x5b, 0x4f, 0x66, 0x6d, 0x7d, 0x07, 0x7f, 0x6f, 0x00, 0x08, 0x40, 0x01, 0x41, 0x48, 0x76, 0x38, 0x40, 0x00,
							/* 序号:   0   	1    2	   3    4	    5    6	  7   8	   	9	 		10	11	 12   13   14   15    16   17   18   19	 */
							/* 显示:   0   	1    2     3    4     5    6    7   8    9  (无)   下-  中-  上-  上中-  中下-  H    L    - 	（无）*/
							0x3f | 0x80, 0x06 | 0x80, 0x5b | 0x80, 0x4f | 0x80, 0x66 | 0x80, 0x6d | 0x80, 0x7d | 0x80, 0x07 | 0x80, 0x7f | 0x80, 0x6f | 0x80};
/* 带小数点     20         21         22         23      24        25        26        27        28        29        */
#endif

unsigned int adc_10bit = 0; // 热敏电阻adc 10bit
unsigned long sumt = 0;		// 求和
unsigned int data_temp = 0; // 四舍五入的均值
unsigned int t = 0;			// 计数
unsigned int flag;			// 符号位
int temp = 0;
struct_ADC adc_data;
// 温度值对应表
int code tempdata[] = {239, 197, 175, 160, 150, 142, 135, 129, 124, 120, 116, 113, 109, 107, 104, 101,
					   99, 97, 95, 93, 91, 90, 88, 86, 85, 84, 82, 81, 80, 78, 77, 76,
					   75, 74, 73, 72, 71, 70, 69, 68, 67, 67, 66, 65, 64, 63, 63, 62,
					   61, 61, 60, 59, 58, 58, 57, 57, 56, 55, 55, 54, 54, 53, 52, 52,
					   51, 51, 50, 50, 49, 49, 48, 48, 47, 47, 46, 46, 45, 45, 44, 44,
					   43, 43, 42, 42, 41, 41, 41, 40, 40, 39, 39, 38, 38, 38, 37, 37,
					   36, 36, 36, 35, 35, 34, 34, 34, 33, 33, 32, 32, 32, 31, 31, 31,
					   30, 30, 29, 29, 29, 28, 28, 28, 27, 27, 27, 26, 26, 26, 25, 25,
					   24, 24, 24, 23, 23, 23, 22, 22, 22, 21, 21, 21, 20, 20, 20, 19,
					   19, 19, 18, 18, 18, 17, 17, 16, 16, 16, 15, 15, 15, 14, 14, 14,
					   13, 13, 13, 12, 12, 12, 11, 11, 11, 10, 10, 9, 9, 9, 8, 8, 8, 7,
					   7, 7, 6, 6, 5, 5, 54, 4, 3, 3, 3, 2, 2, 1, 1, 1, 0, 0, -1, -1, -1,
					   -2, -2, -3, -3, -4, -4, -5, -5, -6, -6, -7, -7, -8, -8, -9, -9,
					   -10, -10, -11, -11, -12, -13, -13, -14, -14, -15, -16, -16, -17,
					   -18, -19, -19, -20, -21, -22, -23, -24, -25, -26, -27, -28, -29,
					   -30, -32, -33, -35, -36, -38, -40, -43, -46, -50, -55, -63, 361};

// 1s更新一次温度值
void displayer1s_callback()
{
	Seg7Print(10, 10, 10, 10, 10, flag, temp / 10, temp % 10);
}
// 取多个值求均值
void adc1ms_callback()
{
	t++;
	if (t == 200)
	{
		data_temp = (sumt + t / 2) / t; // 四舍五入
		temp = tempdata[data_temp - 1]; // 查找对应的AD的温度值
		if (temp < 0)
		{
			flag = 12; // 若温度为负，显示负号
		}
		else
		{
			flag = 0;
		}
		sumt = 0;
		t = 0;
	}
	adc_data = GetADC();
	adc_10bit = adc_data.Rt;
	data_temp = adc_10bit >> 2; // 由10位AD值转换为8位AD值
	sumt += data_temp;			// 求t次AD值的和
}

void main()
{
	DisplayerInit();
	SetDisplayerArea(0, 7);
	AdcInit(ADCexpEXT);
	SetEventCallBack(enumEventSys1mS, adc1ms_callback);
	SetEventCallBack(enumEventSys1S, displayer1s_callback);
	MySTC_Init();
	while (1)
	{
		MySTC_OS();
	}
}

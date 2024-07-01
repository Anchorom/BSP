struct_FMRadio FM_NVMread(void) // 举例，取NVM中读出收音频率、音量
{
	struct_FMRadio a = {955, 5, 0xff, 0, 0xff};
	unsigned int tempuint;
	unsigned char tempuchar;
	tempuint = ((NVM_Read(0x01) << 8)) + NVM_Read(0x02);
	if ((tempuint >= 870) & (tempuint <= 1080))
		a.frequency = tempuint;
	tempuchar = NVM_Read(0x03);
	if (tempuchar < 15)
		a.volume = tempuchar;
	return (a);
}

void FM_NVMwrite(struct_FMRadio FM) // NVM中保存收音频率、音量
{
	NVM_Write(0x01, FM.frequency >> 8);
	NVM_Write(0x02, FM.frequency);
	NVM_Write(0x03, FM.volume);
}

void dealwithIrRxd()
{
	unsigned char a1, a2;
	if (GetIrRxNum() != 0) // 如果红外接口收到数据包
	{
		if ((*(rxd + 0) == 0xAA) & (*(rxd + 1) == 0x55)) // 如果包头 AA 55
		{
			switch (*(rxd + 2))
			{
			case 0xF1:
				a1 = *(rxd + 3);
				a2 = *(rxd + 4);
				FM.frequency = ((a1 / 16 * 10 + a1 % 16) * 100) + (a2 / 16 * 10 + a2 % 16); // 设置FM收音机,将第4、5字节调节FM收音频率,BCD码换成二进制
				FM.volume = *(rxd + 5);														// 将  第6字节 调节FM收音音量
				SetFMRadio(FM);																// 设置收音机新收音参数
				FM_NVMwrite(FM);															// 保存收音参数到NVM中
				break;
			case 0xF2:
				t.hour = (*(rxd + 3));	 // 设置RTC //将第4字节设成RTC时，BCD码
				t.minute = (*(rxd + 4)); // 将第5字节设成RTC分
				t.second = (*(rxd + 5)); // 将第6字节设成RTC秒
				RTC_Write(t);			 // 设置RTC
				break;
			case 0xF3:
				t.year = (*(rxd + 3));	// 设置RTC //将第4字节设成RTC年，BCD码
				t.month = (*(rxd + 4)); // 将第5字节设成RTC月
				t.day = (*(rxd + 5));	// 将第6字节设成RTC日
				RTC_Write(t);			// 设置RTC
				break;
			default:
				break;
			}
			(*(rxd + 6)) += 4; // 第7字节加4
			SetBeep(1000, 30);
		}
		else
			SetBeep(1000, 20);
		Uart1Print(&rxd, sizeof(rxd)); // 从串口1发送出去
	}
}

void dealwithDisp()
{
	unsigned char d0, d1, d2, d3, d4, d5, d6, d7;
	int a;
	static unsigned char ct100mS = 10;

	if (funcmode == Music)
		return; // 更新显示，（Music函数自带显示更新，不需更新）
	if (--ct100mS == 0)
		ct100mS = 10;
	switch (funcmode)
	{
	case RTC_YMD:
		if (tempadj == 0)
			t = RTC_Read(); // 非调时钟时， 读RTC
		d0 = t.year >> 4;
		d1 = t.year & 0x0f;
		d2 = 12;
		d3 = t.month >> 4;
		d4 = t.month & 0x0f;
		d5 = 12;
		d6 = t.day >> 4;
		d7 = t.day & 0x0f;
		break;
	case RTC_HMS:
		if (tempadj == 0)
			t = RTC_Read();
		d0 = t.hour >> 4;
		d1 = t.hour & 0x0f;
		d2 = 12;
		d3 = t.minute >> 4;
		d4 = t.minute & 0x0f;
		d5 = 12;
		d6 = t.second >> 4;
		d7 = t.second & 0x0f;
		break;
	case Rt_Rop:
		ADCresult = GetADC();
		d0 = ADCresult.Rt / 100;
		d1 = ADCresult.Rt / 10 % 10;
		d2 = ADCresult.Rt % 10;
		d3 = 10;
		d4 = 10;
		d5 = ADCresult.Rop / 100;
		d6 = ADCresult.Rop / 10 % 10;
		d7 = ADCresult.Rop % 10;
		break;
	case FMradio:
		d0 = FM.volume / 10;
		if (d0 == 0)
			d0 = 10;
		d1 = FM.volume % 10;
		d2 = 10;
		d3 = 10;
		d4 = FM.frequency / 1000;
		if (d4 == 0)
			d4 = 10;
		d5 = FM.frequency / 100 % 10;
		d6 = FM.frequency / 10 % 10 + 16; // 加小数点
		d7 = FM.frequency % 10;
		break;
	case UltroSonic:
		a = GetUltraSonic(); // a为测得距离，正数，单位cm
		d0 = 10;
		d1 = 10;
		d2 = 10;
		d3 = 10;
		d4 = a / 1000;
		if (d4 == 0)
			d4 = 10; // 不显示最高位0
		a = a % 1000;
		d5 = a / 100 + 16; // 显示小数点
		a = a % 100;
		d6 = a / 10;
		d7 = a % 10;
		break;
	case Weight:
		a = GetWeight(); // a为称重AD数据，带符号整数（16bit补码），未标定
		d0 = 10;
		d1 = 10;
		d2 = 10;
		if (a < 0)
		{
			a = -a;
			d2 = 12;
		} // a变正数，d2符号
		d3 = a / 10000;
		a = a % 10000;
		d4 = a / 1000;
		a = a % 1000;
		d5 = a / 100;
		a = a % 100;
		d6 = a / 10;
		d7 = a % 10;
		break;
	default:
		break;
	}

	if (ct100mS >= 8)
		switch (tempadj)
		{
		case 1:
			d0 = 10;
			d1 = 10;
			break;
		case 2:
			d3 = 10;
			d4 = 10;
			break;
		case 3:
			d6 = 10;
			d7 = 10;
			break;
		default:
			break;
		}
	Seg7Print(d0, d1, d2, d3, d4, d5, d6, d7);
}

void dealwithmyKN()
{
	unsigned char a;
	switch (funcmode)
	{
	case RTC_HMS:
		if (GetAdcNavAct(enumAdcNavKeyRight) == enumKeyPress)
		{
			if (tempadj)
				if (tempadj < 3)
				{
					tempadj++;
					SetBeep(1000, 20);
				}
		}
		if (GetAdcNavAct(enumAdcNavKeyLeft) == enumKeyPress)
		{
			if (tempadj)
				if (tempadj > 1)
				{
					tempadj--;
					;
					SetBeep(1000, 20);
				}
		}
		if (GetAdcNavAct(enumAdcNavKeyUp) == enumKeyPress)
		{
			if (tempadj == 1)
			{
				a = t.hour / 16 * 10 + t.hour % 16;
				if ((++a) >= 24)
					a = 0;
				t.hour = a / 10 * 16 + a % 10;
				SetBeep(1000, 20);
			}
			if (tempadj == 2)
			{
				a = t.minute / 16 * 10 + t.minute % 16;
				if ((++a) >= 60)
					a = 0;
				t.minute = a / 10 * 16 + a % 10;
				SetBeep(1000, 20);
			}
			if (tempadj == 3)
			{
				a = t.second / 16 * 10 + t.second % 16;
				if ((++a) >= 60)
					a = 0;
				t.second = a / 10 * 16 + a % 10;
				SetBeep(1000, 20);
			}
		}
		if (GetAdcNavAct(enumAdcNavKeyDown) == enumKeyPress)
		{
			if (tempadj == 1)
			{
				a = t.hour / 16 * 10 + t.hour % 16;
				if ((a--) == 0)
					a = 23;
				t.hour = a / 10 * 16 + a % 10;
				SetBeep(1000, 20);
			}
			if (tempadj == 2)
			{
				a = t.minute / 16 * 10 + t.minute % 16;
				if ((a--) == 0)
					a = 59;
				t.minute = a / 10 * 16 + a % 10;
				SetBeep(1000, 20);
			}
			if (tempadj == 3)
			{
				a = t.second / 16 * 10 + t.second % 16;
				if ((a--) == 0)
					a = 59;
				t.second = a / 10 * 16 + a % 10;
				SetBeep(1000, 20);
			}
		}
		if (GetAdcNavAct(enumAdcNavKeyCenter) == enumKeyPress)
		{
			SetBeep(1000, 20);
			if (tempadj == 0)
				tempadj = 3;
			else
			{
				tempadj = 0;
				RTC_Write(t);
			}
		}
		break;
	case RTC_YMD:
		if (GetAdcNavAct(enumAdcNavKeyRight) == enumKeyPress)
		{
			if (tempadj)
				if (tempadj < 3)
				{
					tempadj++;
					SetBeep(1000, 20);
				}
		}
		if (GetAdcNavAct(enumAdcNavKeyLeft) == enumKeyPress)
		{
			if (tempadj)
				if (tempadj > 1)
				{
					tempadj--;
					SetBeep(1000, 20);
				}
		}
		if (GetAdcNavAct(enumAdcNavKeyUp) == enumKeyPress)
		{
			if (tempadj == 1)
			{
				a = t.year / 16 * 10 + t.year % 16;
				if ((++a) >= 99)
					a = 0;
				t.year = a / 10 * 16 + a % 10;
				SetBeep(1000, 20);
			}
			if (tempadj == 2)
			{
				a = t.month / 16 * 10 + t.month % 16;
				if ((++a) >= 12)
					a = 1;
				t.month = a / 10 * 16 + a % 10;
				SetBeep(1000, 20);
			}
			if (tempadj == 3)
			{
				a = t.day / 16 * 10 + t.day % 16;
				if ((++a) >= 31)
					a = 1;
				t.day = a / 10 * 16 + a % 10;
				SetBeep(1000, 20);
			}
		}
		if (GetAdcNavAct(enumAdcNavKeyDown) == enumKeyPress)
		{
			if (tempadj == 1)
			{
				a = t.year / 16 * 10 + t.year % 16;
				if ((a--) == 0)
					a = 99;
				t.year = a / 10 * 16 + a % 10;
				SetBeep(1000, 20);
			}
			if (tempadj == 2)
			{
				a = t.month / 16 * 10 + t.month % 16;
				if ((a--) == 1)
					a = 12;
				t.month = a / 10 * 16 + a % 10;
				SetBeep(1000, 20);
			}
			if (tempadj == 3)
			{
				a = t.day / 16 * 10 + t.day % 16;
				if ((a--) == 1)
					a = 31;
				t.day = a / 10 * 16 + a % 10;
				SetBeep(1000, 20);
			}
		}
		if (GetAdcNavAct(enumAdcNavKeyCenter) == enumKeyPress)
		{
			SetBeep(1000, 20);
			if (tempadj == 0)
				tempadj = 3;
			else
			{
				tempadj = 0;
				RTC_Write(t);
			}
		}
		break;
	case Music:
		if (GetAdcNavAct(enumAdcNavKeyRight) == enumKeyPress)
		{
			if (Music_PM <= 210)
				Music_PM += 10;
		} // 节凑最高限每分钟220拍
		if (GetAdcNavAct(enumAdcNavKeyLeft) == enumKeyPress)
		{
			if (Music_PM >= 60)
				Music_PM -= 10;
		} //    最低限      50
		if (GetAdcNavAct(enumAdcNavKeyUp) == enumKeyPress)
		{
			if (Music_tone < 0xFF)
				Music_tone++;
		} // 音调最高 F调 （数码管显示：6）
		if (GetAdcNavAct(enumAdcNavKeyDown) == enumKeyPress)
		{
			if (Music_tone > 0xF9)
				Music_tone--;
		}													   //    最低 A           显示：0
		if (GetAdcNavAct(enumAdcNavKeyCenter) == enumKeyPress) // 控制音乐播放、暂停
			if (GetPlayerMode() == enumModePlay)
				SetPlayerMode(enumModePause);
			else
			{
				if (GetPlayerMode() == enumModeStop)
					SetMusic(Music_PM, Music_tone, &song, sizeof(song), enumMscDrvSeg7);
				SetPlayerMode(enumModePlay);
			}
		break;
	case FMradio:
		if (GetAdcNavAct(enumAdcNavKeyRight) == enumKeyPress)
		{
			if (FM.frequency < 1080)
				FM.frequency++;
		} // 频率+
		if (GetAdcNavAct(enumAdcNavKeyLeft) == enumKeyPress)
		{
			if (FM.frequency > 887)
				FM.frequency--;
		} // 频率1
		if (GetAdcNavAct(enumAdcNavKeyUp) == enumKeyPress)
		{
			if (FM.volume < 15)
				FM.volume++;
		} // 音量+
		if (GetAdcNavAct(enumAdcNavKeyDown) == enumKeyPress)
		{
			if (FM.volume > 0)
				FM.volume--;
		}				// 音量-
		SetFMRadio(FM); // 设置收音机新收音参数
		if (GetAdcNavAct(enumAdcNavKeyCenter) == enumKeyPress)
		{
			FM_NVMwrite(FM);
		} // 保存参数到NVM
		break;
	default:
		break;
	}

	if (GetAdcNavAct(enumAdcNavKey3) == enumKeyRelease) // 示例。按键3:(抬起时）向串口1发送二进制数据包：报告系统性能
	{
		SysPer = GetSysPerformance();
		if ((GetUart1TxStatus() == enumUart1TxFree))
			Uart1Print(&SysPer, sizeof(SysPer));
		SetBeep(1000, 20);
	}
}

void dealwithmykey()
{
	if (GetKeyAct(enumKey2) == enumKeyPress) // 示例。按键2:（按下时）切换显示和按键功能模式
	{
		SetBeep(1000, 20);
		if (++funcmode > Weight)
			funcmode = RTC_YMD;
		LedPrint(funcmode);
		M24C02_Write(0x00, funcmode);
		if (funcmode == Music)
			Seg7Print(10, 10, 12, 12, 12, 12, 10, 10);
		if (funcmode == Music + 1)
			SetMusic(enumModeInvalid, enumModeInvalid, enumModeInvalid, enumModeInvalid, enumMscNull); // 离开音乐播放模式，关音乐显示
		if (funcmode == Weight)
			EXTInit(enumEXTWeight);
		if (funcmode == UltroSonic)
			EXTInit(enumEXTUltraSonic);
	}

	if (GetKeyAct(enumKey1) == enumKeyPress) // 示例。按键1:（按下时）向红外发送文本
	{
		IrPrint("大家好\r\n", sizeof("大家好\r\n"));
		SetBeep(1000, 100);
	}
}

#include "STC15F2K60S2.H"
#include "beep.H"
#include "music.H"
#include "sys.H"

code unsigned long SysClock = 11059200;

code unsigned char song[] = {0x21, 0x10, 0x21, 0x10, 0x25, 0x10, 0x25, 0x10, 0x26, 0x10, 0x26, 0x10, 0x25, 0x10};
void main()
{
	BeepInit();
	MusicPlayerInit();
	SetMusic(90, 0xFA, &song, sizeof(song), enumMscNull);
	SetPlayerMode(enumModePlay);
	MySTC_Init();
	while (1)
	{
		MySTC_OS();
	}
}

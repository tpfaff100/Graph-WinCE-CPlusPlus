#include "stdafx.h"

MMRESULT wave_play(char *sd, int len, int volume, int repeatCount);
void wave_prepare(int len);
long wave_read(char* pData, long nLength);

WAVEFORMATEX* getwaveformat();
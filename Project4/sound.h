#pragma once
#include <stdio.h>
#include <windows.h>
#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")

void shootsound() {
	PlaySound(TEXT("6530.wav"), NULL, SND_FILENAME | SND_ASYNC);
}
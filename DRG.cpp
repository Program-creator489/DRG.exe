// Reccomended to compile with a x64 compiler so it doesn't get flagged as malware
// Credit's to Program-Creator-489 for making this GDI

#include <Windows.h>
#include <cmath>
#include <stdio.h>
#pragma comment(lib, "winmm.lib")
#define M_PI   3.14159265358979323846264338327950288

DWORD WINAPI sines(LPVOID lpParam) {
	HDC hdc = GetDC(0); HWND wnd = GetDesktopWindow();
	int sw = GetSystemMetrics(0), sh = GetSystemMetrics(1);
	double angle = 0;
	while (1) {
		hdc = GetDC(0);
		for (float i = 0; i < sw + sh; i += 0.99f) {	// This effects Creates Cool waves on your screen
			int a = sin(angle) * 40;
			BitBlt(hdc, 0, i, sw, 1, hdc, a, i, SRCCOPY);
			//BitBlt(hdc, i, 0, 1, sh, hdc, i, a, SRCCOPY);
			angle += M_PI / 20;
			DeleteObject(&i); DeleteObject(&a);
		}
		ReleaseDC(wnd, hdc);
		DeleteDC(hdc); DeleteObject(&sw); DeleteObject(&sh); DeleteObject(&angle);
	}
}

VOID WINAPI ci(int x, int y, int w, int h)
{
	HDC hdc = GetDC(0);
	HRGN hrgn = CreateEllipticRgn(x, y, w + x, h + y);
	SelectClipRgn(hdc, hrgn);								// This part is needed for the Target effect
	BitBlt(hdc, x, y, w, h, hdc, x, y, NOTSRCCOPY);
	DeleteObject(hrgn);
	ReleaseDC(NULL, hdc);
}

DWORD WINAPI wef(LPVOID lpParam)
{
	RECT rect;
	GetWindowRect(GetDesktopWindow(), &rect);
	int w = rect.right - rect.left - 500, h = rect.bottom - rect.top - 500;

	for (int t = 0;; t++)
	{
		const int size = 1000;
		int x = rand() % (w + size) - size / 2, y = rand() % (h + size) - size / 2;		// This effect creates a Target effect on your screen

		for (int i = 0; i < size; i += 100)
		{
			ci(x - i / 2, y - i / 2, i, i);
			Sleep(25);
		}
	}
}

DWORD WINAPI swirl(LPVOID lpParam) {
	HDC desk = GetDC(0);
	int sw = GetSystemMetrics(SM_CXSCREEN), sh = GetSystemMetrics(SM_CYSCREEN), xSize = sh / 10, ySize = 9;
	while (1) {
		HDC desk = GetDC(0);
		for (int i = 0; i < sh * 2; i++) {
			int wave = sin(i / ((float)xSize) * M_PI) * (ySize);
			BitBlt(desk, i, 0, 1, sh, desk, i, wave, SRCCOPY);
		}																// This creates a sinewave effect but on older mechines it will create a actual swirling effect
		for (int i = 0; i < sw * 2; i++) {
			int wave = sin(i / ((float)xSize) * M_PI) * (ySize);
			BitBlt(desk, 0, i, sw, 1, desk, wave, i, SRCCOPY);
		}
		ReleaseDC(0, desk);
	}
}

	// These are the programs sounds

VOID WINAPI sound1() {
	HWAVEOUT hWaveOut = 0;
	WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 22050, 22050, 1, 8, 0 };
	waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
	char buffer[22050 * 30] = {};
	for (DWORD t = 0; t < sizeof(buffer); ++t)
		buffer[t] = static_cast<char>(t * ((t >> 12 | t >> 8) & 63 & t >> 4));

	WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
	waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutClose(hWaveOut);
}

VOID WINAPI sound2() {
	HWAVEOUT hWaveOut = 0;
	WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 8000, 8000, 1, 8, 0 };
	waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
	char buffer[8000 * 30] = {};
	for (DWORD t = 0; t < sizeof(buffer); ++t)
		buffer[t] = static_cast<char>((t ^ t * (t >> (t >> 8 & 10) & 40) | t * 4));

	WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
	waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutClose(hWaveOut);
}

VOID WINAPI sound3() {
	HWAVEOUT hWaveOut = 0;
	WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 8000, 8000, 1, 8, 0 };
	waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
	char buffer[8000 * 30] = {};
	for (DWORD t = 0; t < sizeof(buffer); ++t)
		buffer[t] = static_cast<char>(t * (0xCA98 >> (t >> 9 & 14) & 15) | t >> 8);

	WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
	waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutClose(hWaveOut);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd) {

	if (MessageBoxW(NULL, L"This contains GDI and loud sounds. on older os like windows 7 and older these GDI effects work the Best", L"DRG.exe by Program-Creator-489", MB_YESNO | MB_ICONEXCLAMATION) == IDNO)
	{
		ExitProcess(0);
	}
	else
	{
		if (MessageBoxW(NULL, L"Are u sure you want to run it?", L"DRG.exe Final Warning", MB_YESNO | MB_ICONEXCLAMATION) == IDNO)
		{
			ExitProcess(0);
		}
		else

			// These are how the sounds and effects work at the same time
			// The Sleep funtion are for the amount of seconds the effects work before shifting 

		Sleep(1000);

		HANDLE thread0 = CreateThread(0, 0, sines, 0, 0, 0);

		sound1();
		Sleep(30000);
		TerminateThread(thread0, 0);
		CloseHandle(thread0);
		
		Sleep(500);

		
		HANDLE thread1 = CreateThread(0, 0, wef, 0, 0, 0);

		sound2();
		Sleep(30000);
		TerminateThread(thread1, 0);
		CloseHandle(thread1);

		Sleep(500);

		HANDLE thread2 = CreateThread(0, 0, swirl, 0, 0, 0);

		sound3();
		Sleep(30000);
		TerminateThread(thread2, 0);
		CloseHandle(thread2),
		
		Sleep(500);

		MessageBox(
			NULL,
			"GDI effects are done your screen should return to normal.",
			"DRG.exe",
			MB_OK | MB_ICONWARNING

		);
	}
}
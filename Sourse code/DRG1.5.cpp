// DRG1.5.exe - GDI + Bytebeat Demo
// Compiles with both g++ and cl.exe (x64 recommended)

#include <Windows.h>
#include <cmath>
#include <cstdlib>
#pragma comment(lib, "winmm.lib")
#pragma comment(linker, "/SUBSYSTEM:WINDOWS")

#ifndef M_PI
#define M_PI 3.14159265358979323846264338327950288
#endif

volatile bool g_running = true;

// ==================== EFFECTS ====================

DWORD WINAPI sines(LPVOID lpParam) {
    int sw = GetSystemMetrics(SM_CXSCREEN);
    int sh = GetSystemMetrics(SM_CYSCREEN);
    double angle = 0.0;

    while (g_running) {
        HDC hdc = GetDC(NULL);
        for (float i = 0; i < sw + sh; i += 0.99f) {
            int a = static_cast<int>(sin(angle) * 40);
            BitBlt(hdc, 0, static_cast<int>(i), sw, 1, hdc, a, static_cast<int>(i), SRCCOPY);
            angle += M_PI / 20;
        }
        ReleaseDC(NULL, hdc);
        Sleep(10);
    }
    return 0;
}

VOID WINAPI ci(int x, int y, int w, int h) {
    HDC hdc = GetDC(NULL);
    HRGN hrgn = CreateEllipticRgn(x, y, x + w, y + h);
    SelectClipRgn(hdc, hrgn);
    BitBlt(hdc, x, y, w, h, hdc, x, y, NOTSRCCOPY);
    DeleteObject(hrgn);
    ReleaseDC(NULL, hdc);
}

DWORD WINAPI wef(LPVOID lpParam) {
    RECT rect;
    GetWindowRect(GetDesktopWindow(), &rect);
    int w = rect.right - rect.left;
    int h = rect.bottom - rect.top;

    while (g_running) {
        const int size = 1000;
        int x = rand() % (w + size) - size / 2;
        int y = rand() % (h + size) - size / 2;

        for (int i = 0; i < size; i += 100) {
            if (!g_running) break;
            ci(x - i / 2, y - i / 2, i, i);
            Sleep(25);
        }
        Sleep(50);
    }
    return 0;
}

DWORD WINAPI swirl(LPVOID lpParam) {
    int sw = GetSystemMetrics(SM_CXSCREEN);
    int sh = GetSystemMetrics(SM_CYSCREEN);
    int xSize = sh / 10;
    int ySize = 9;

    while (g_running) {
        HDC desk = GetDC(NULL);

        for (int i = 0; i < sh * 2; i++) {
            if (!g_running) break;
            int wave = static_cast<int>(sin(i / static_cast<float>(xSize) * M_PI) * ySize);
            BitBlt(desk, i, 0, 1, sh, desk, i, wave, SRCCOPY);
        }

        for (int i = 0; i < sw * 2; i++) {
            if (!g_running) break;
            int wave = static_cast<int>(sin(i / static_cast<float>(xSize) * M_PI) * ySize);
            BitBlt(desk, 0, i, sw, 1, desk, wave, i, SRCCOPY);
        }

        ReleaseDC(NULL, desk);
        Sleep(10);
    }
    return 0;
}

// ==================== SOUNDS ====================

HWAVEOUT StartSound1() {
    HWAVEOUT hWaveOut = 0;
    WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 22050, 22050, 1, 8, 0 };
    waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);

    static char buffer[22050 * 30] = {};
    for (DWORD t = 0; t < sizeof(buffer); ++t)
        buffer[t] = static_cast<char>(t * ((t >> 12 | t >> 8) & 63 & t >> 4));

    static WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
    waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
    return hWaveOut;
}

HWAVEOUT StartSound2() {
    HWAVEOUT hWaveOut = 0;
    WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 8000, 8000, 1, 8, 0 };
    waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);

    static char buffer[8000 * 30] = {};
    for (DWORD t = 0; t < sizeof(buffer); ++t)
        buffer[t] = static_cast<char>((t ^ t * (t >> (t >> 8 & 10) & 40) | t * 4));

    static WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
    waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
    return hWaveOut;
}

HWAVEOUT StartSound3() {
    HWAVEOUT hWaveOut = 0;
    WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 8000, 8000, 1, 8, 0 };
    waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);

    static char buffer[8000 * 30] = {};
    for (DWORD t = 0; t < sizeof(buffer); ++t)
        buffer[t] = static_cast<char>(t * (0xCA98 >> (t >> 9 & 14) & 15) | t >> 8);

    static WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
    waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
    return hWaveOut;
}

void StopSound(HWAVEOUT hwo) {
    if (hwo) {
        waveOutReset(hwo);
        waveOutClose(hwo);
    }
}

// ==================== HELPER ====================

bool WaitThirtySecondsOrESC() {
    for (int sec = 0; sec < 30; sec++) {
        for (int ms = 0; ms < 10; ms++) {
            if (GetAsyncKeyState(VK_ESCAPE) & 0x8000) {
                g_running = false;
                return true;
            }
            Sleep(100);
        }
    }
    return false;
}

// ==================== MAIN ====================

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd) {
    // Declare all handles at the top so goto doesn't cross initialization
    HANDLE t1 = NULL, t2 = NULL, t3 = NULL;
    HWAVEOUT s1 = NULL, s2 = NULL, s3 = NULL;

    // === TWO IDENTICAL MESSAGE BOXES AT THE FRONT ===
    if (MessageBoxW(NULL, 
        L"This contains GDI effects and loud bytebeat sounds.\nWorks best on older Windows.\n\nPress ESC during a phase to stop immediately.", 
        L"DRG1.5.exe - Warning", MB_YESNO | MB_ICONEXCLAMATION) == IDNO) {
        ExitProcess(0);
    }

    if (MessageBoxW(NULL, 
        L"This contains GDI effects and loud bytebeat sounds.\nWorks best on older Windows.\n\nPress ESC during a phase to stop immediately.", 
        L"DRG1.5.exe - Warning", MB_YESNO | MB_ICONEXCLAMATION) == IDNO) {
        ExitProcess(0);
    }

    srand(static_cast<unsigned int>(GetTickCount()));

    // ========== PHASE 1: Sines + Sound1 ==========
    g_running = true;
    t1 = CreateThread(NULL, 0, sines, NULL, 0, NULL);
    s1 = StartSound1();

    if (WaitThirtySecondsOrESC()) {
        TerminateThread(t1, 0);
        CloseHandle(t1);
        StopSound(s1);
        goto done;
    }

    TerminateThread(t1, 0);
    CloseHandle(t1);
    StopSound(s1);
    Sleep(400);

    // ========== PHASE 2: Target (wef) + Sound2 ==========
    g_running = true;
    t2 = CreateThread(NULL, 0, wef, NULL, 0, NULL);
    s2 = StartSound2();

    if (WaitThirtySecondsOrESC()) {
        TerminateThread(t2, 0);
        CloseHandle(t2);
        StopSound(s2);
        goto done;
    }

    TerminateThread(t2, 0);
    CloseHandle(t2);
    StopSound(s2);
    Sleep(400);

    // ========== PHASE 3: Swirl + Sound3 ==========
    g_running = true;
    t3 = CreateThread(NULL, 0, swirl, NULL, 0, NULL);
    s3 = StartSound3();

    if (WaitThirtySecondsOrESC()) {
        TerminateThread(t3, 0);
        CloseHandle(t3);
        StopSound(s3);
        goto done;
    }

    TerminateThread(t3, 0);
    CloseHandle(t3);
    StopSound(s3);

done:
    // === FINAL MESSAGE BOX AFTER EFFECTS ===
    MessageBoxW(NULL, 
        L"GDI effects finished. Your screen should return to normal.\n\nThanks for testing DRG1.5.exe!", 
        L"DRG1.5.exe", MB_OK | MB_ICONINFORMATION);

    return 0;
}
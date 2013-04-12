#include <Windows.h>
#include <memory.h>
#include <tchar.h>
#include <stdio.h>
#include <assert.h>

#include "lcd2630.h"
#include "ArmletSimulator.h"

HINSTANCE hInst;
TCHAR*    szTitle       = "Title";
TCHAR*    szWindowClass = "WindowClass";
SIZE      ClientSize    = {160, 128};
HBITMAP   armlet_emu_hBmp;
HWND      hWnd;

BOOL              InitInstance(HINSTANCE, int);
LRESULT CALLBACK  WndProc(HWND, UINT, WPARAM, LPARAM);
void              putPixel(int x, int y, COLORREF c);
COLORREF          getPixel(int x, int y);


DWORD WINAPI runThread(
  _In_  LPVOID lpParameter
)
{
    Sleep(200);
    armlet_main();

    assert(false); // we should never return
    return 0;
}



int CALLBACK WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR    lpCmdLine,
                     int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    WNDCLASSEX wcex;

    ZeroMemory(&wcex, sizeof(WNDCLASSEX));
    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style         = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc   = WndProc;
    wcex.cbClsExtra    = 0;
    wcex.cbWndExtra    = 0;
    wcex.hInstance     = hInstance;
    wcex.hbrBackground = NULL;//(HBRUSH)(COLOR_WINDOW+1);
    wcex.hCursor       = LoadCursor(hInst, IDC_ARROW);
    wcex.lpszClassName = szWindowClass;

    RegisterClassEx(&wcex);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HDC hScreenDC = GetDC(HWND_DESKTOP);
    armlet_emu_hBmp = CreateCompatibleBitmap(hScreenDC, ClientSize.cx, ClientSize.cy);
    ReleaseDC(HWND_DESKTOP, hScreenDC);

    //for (int x = 30; x < 40; ++x)
    //    for (int y = 45; y < 55; ++y)
    //    {
    //        sim_putPixel(x, y, RGB(255,255,0));
    //    }


/*    sim_putPixel(10, 20, RGB(255,0,0));
    sim_putPixel(15, 25, RGB(0,255,0));
    sim_putPixel(20, 30, RGB(0,0,255));
    COLORREF c1 = getPixel(10, 20);
    COLORREF c2 = getPixel(15, 25);
    COLORREF c3 = getPixel(20, 30);
    */

    AllocConsole();
    freopen("CONOUT$", "wb", stdout);

    DWORD threadId = 0;
    HANDLE hndl = CreateThread(NULL, 0, runThread, NULL, 0, &threadId);


    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    fclose(stdout);
    FreeConsole();

    return (int) msg.wParam;
}

void sim_putPixel(int x, int y, COLORREF c)
{
    HDC hDC = GetDC(hWnd);
    HDC hMemDC = CreateCompatibleDC(hDC);
    HGDIOBJ oldBitmap = SelectObject(hMemDC, armlet_emu_hBmp);

    SetPixel(hMemDC, x, y, c);

    SelectObject(hMemDC, oldBitmap);
    DeleteDC(hMemDC);
    ReleaseDC(hWnd, hDC);

}

COLORREF sim_getPixel(int x, int y)
{
    HDC hDC = GetDC(hWnd);
    HDC hMemDC = CreateCompatibleDC(hDC);
    HGDIOBJ oldBitmap = SelectObject(hMemDC, armlet_emu_hBmp);

    COLORREF cref = GetPixel(hMemDC, x, y);

    SelectObject(hMemDC, oldBitmap);
    DeleteDC(hMemDC);
    ReleaseDC(hWnd, hDC);
    return cref;
}

void sim_repaint()
{
    InvalidateRect (hWnd, NULL, TRUE);
    UpdateWindow(hWnd);
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Store instance handle in our global variable

   int xk = GetSystemMetrics(SM_CXBORDER) * 2;
   int yk = GetSystemMetrics(SM_CYBORDER) * 2 + GetSystemMetrics(SM_CYCAPTION);

   hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      0, 0, ClientSize.cx+xk, ClientSize.cy+yk, NULL, NULL, hInstance, NULL);

   if (!hWnd)
   {
      return FALSE;
   }

//   ShowWindow(hWnd, nCmdShow);
//   UpdateWindow(hWnd);

   return TRUE;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    PAINTSTRUCT ps;
    HDC hdc;

    switch (message)
    {
    case WM_COMMAND:
        return DefWindowProc(hWnd, message, wParam, lParam);

    case WM_PAINT:
    {
        hdc = BeginPaint(hWnd, &ps);
        HDC hdcMem = CreateCompatibleDC(hdc);

        HGDIOBJ oldBitmap = SelectObject(hdcMem, armlet_emu_hBmp);
        BitBlt(hdc, 0, 0, ClientSize.cx, ClientSize.cy, hdcMem, 0, 0, SRCCOPY);
        SelectObject(hdcMem, oldBitmap);

        DeleteDC(hdcMem);
        EndPaint(hWnd, &ps);
    }
    break;

    case WM_DESTROY:
        PostQuitMessage(0);
        break;

    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

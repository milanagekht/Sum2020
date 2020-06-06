/* FILE NAME: T07GLOBE.C
 * PROGRAMMER: MG5
 * DATE: 04.06.2020 
 * PURPOSE: NONE
 */

#include <stdlib.h>
#include <windows.h>
#include <math.h>
#include <stdio.h>

#include "GLOBE.H"
#include "TIMER.H"
#include "MTH.H"

/* Window class name */
#define WND_CLASS_NAME "Globe"

/* Forward declaration */
LRESULT CALLBACK WinFunc( HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam );

/* The main program function */
int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, CHAR *CmdLine, INT CmdShow )
{
  HWND hWnd;
  MSG msg;
  WNDCLASS wc;

  /* Fill window class structure */
  wc.style = CS_HREDRAW | CS_VREDRAW;
  wc.cbClsExtra = 0;
  wc.cbWndExtra = 0;
  wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
  wc.hCursor = LoadCursor(NULL, IDC_HAND);
  wc.hIcon = LoadIcon(NULL, IDI_SHIELD);
  wc.hInstance = hInstance;
  wc.lpszClassName = WND_CLASS_NAME;
  wc.lpszMenuName = NULL;
  wc.lpfnWndProc = WinFunc;

  /* Register window class */
  if (!RegisterClass(&wc))
  {
    MessageBox(NULL, "Error register window class", "Error", MB_OK | MB_ICONERROR);
    return 0;
  }

  /* Window creation */
  hWnd = CreateWindow(WND_CLASS_NAME,
    WND_CLASS_NAME,
    WS_OVERLAPPEDWINDOW,
    CW_USEDEFAULT, CW_USEDEFAULT,
    CW_USEDEFAULT, CW_USEDEFAULT,
    /* 47 , 47, 47 * 20, 30 * 20,*/
    NULL, NULL, hInstance, NULL);

  /* Show and update window */
  ShowWindow(hWnd, CmdShow);

  /* Message loop */
  /*while (GetMessage(&msg, NULL, 0, 0))
    DispatchMessage(&msg); */
  while (TRUE)
    if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
    {
      if (msg.message == WM_QUIT)
        break;
      DispatchMessage(&msg);
    }
    else
      SendMessage(hWnd, WM_TIMER, 47, 0);

  return 0;
} /* End of 'WinMain' function */



/* Toggle window fullscreen mode function */
VOID FlipFullScreen( HWND hWnd )
{
  static BOOL IsFullScreen = FALSE;
  static RECT SaveRect;

  if (!IsFullScreen)
  {
    HMONITOR hmon;
    MONITORINFO moninfo;
    RECT rc;

    /* Go to fullscreen mode */
    GetWindowRect(hWnd, &SaveRect);

    /* Get nearest monitor */
    hmon = MonitorFromWindow(hWnd, MONITOR_DEFAULTTONEAREST);

    /* Get monitor information */
    moninfo.cbSize = sizeof(moninfo);
    GetMonitorInfo(hmon, &moninfo);

    rc = moninfo.rcMonitor;
    AdjustWindowRect(&rc, GetWindowLong(hWnd, GWL_STYLE), FALSE);
    SetWindowPos(hWnd, HWND_TOP,
      rc.left, rc.top,
      rc.right - rc.left, rc.bottom - rc.top,
      SWP_NOOWNERZORDER);
  }
  else
  {
    /* Restore window size and position */
    SetWindowPos(hWnd, HWND_TOP,
      SaveRect.left, SaveRect.top,
      SaveRect.right - SaveRect.left, SaveRect.bottom - SaveRect.top,
      SWP_NOOWNERZORDER);
  }
} /* End of 'FlipFullScreen' function */


/* Main window handle function */
LRESULT CALLBACK WinFunc( HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam )
{
  /*INT len;*/
  HDC hDC;
  RECT rc;
  PAINTSTRUCT ps;
  /*HFONT hFnt, hFntOld; */
  static HDC hMemDC;
  static HBITMAP hBm;
  static INT w, h;
  /*CHAR Buf[102];*/

  switch (Msg)
  {
  case WM_GETMINMAXINFO:
    ((MINMAXINFO *)lParam)->ptMaxTrackSize.y =
      GetSystemMetrics(SM_CYMAXTRACK) + GetSystemMetrics(SM_CYCAPTION) + 2 * GetSystemMetrics(SM_CYBORDER);
    return 0;
  case WM_CREATE:
    SetTimer(hWnd, 30, 2, NULL);
    hDC = GetDC(hWnd);
    hMemDC = CreateCompatibleDC(hDC);
    ReleaseDC(hWnd, hDC);
    hBm = NULL;
     
    GLB_TimerInit();
    return 0;
  case WM_SIZE:
    /* Obtain new window width and height */

    w = LOWORD(lParam);
    h = HIWORD(lParam);

    /* Recreate background frame image */
    if (hBm != NULL)
      DeleteObject(hBm);
    hDC = GetDC(hWnd);
    hBm = CreateCompatibleBitmap(hDC, w, h);
    ReleaseDC(hWnd, hDC);
    SelectObject(hMemDC, hBm);

    /* Redraw frame */
    GlobeSet(w / 2, h / 2, 2.0);
    SendMessage(hWnd, WM_TIMER, 0, 0);
    return 0;
  case WM_KEYDOWN:
    if (wParam == VK_ESCAPE)
      SendMessage(hWnd, WM_CLOSE, 0, 0);
    if (wParam == 'P')
      GLB_IsPause = !GLB_IsPause;
    if (wParam == 'F')
      FlipFullScreen(hWnd);
    return 0;
  case WM_TIMER: 
    /* Clear backgrond */
    SelectObject(hMemDC, GetStockObject(BLACK_BRUSH));
    SelectObject(hMemDC, GetStockObject(NULL_PEN));
    Rectangle(hMemDC, 0, 0, w + 1, h + 1);

    /* Draw Globe */
    DrawGlobe(hMemDC);

    /* Text output */
    /*hFnt = CreateFont(h / 32, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
             RUSSIAN_CHARSET,
             OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, ANTIALIASED_QUALITY,
             FF_ROMAN | VARIABLE_PITCH, "Consolas");
    hFntOld = SelectObject(hMemDC, hFnt);
    len = sprintf(Buf, "Frames Per Sec: %f", GLB_FPS);

    SetBkMode(hMemDC, OPAQUE);*/
    /*SetBkMode(hMemDC, TRANSPARENT);
    SetTextColor(hMemDC, RGB(255, 255, 255));
    SetTextColor(hMemDC, RGB(0, 0, 0));
    /*TextOut(hMemDC, w / 2, h / 2, Buf, len); */
    /*rc.left = 0;
    rc.right = w;
    rc.top = 2 * h / 3;
    rc.bottom = h;
    DrawText(hMemDC, Buf, len, &rc, DT_CENTER | DT_VCENTER);

    SelectObject(hMemDC, hFntOld);
    DeleteObject(hFnt);*/ 

    GLB_TimerResponse();

    /* Send repaint message */
    InvalidateRect(hWnd, NULL, FALSE);
    return 0;
  case WM_PAINT:
    hDC = BeginPaint(hWnd, &ps);
    /* Copy frame to window */
    BitBlt(hDC, 0, 0, w, h, hMemDC, 0, 0, SRCCOPY);
    EndPaint(hWnd, &ps);
    return 0;
  case WM_CLOSE:
    if (MessageBox(hWnd, "Are you sure you want to close the window?", "Close",
        MB_YESNO | MB_DEFBUTTON2 | MB_ICONQUESTION) == IDYES)
      DestroyWindow(hWnd);
    return 0;
  case WM_DESTROY:
    /* Delete resourses */
    if (hBm != NULL)
      DeleteObject(hBm);
    DeleteDC(hMemDC);

    PostMessage(hWnd, WM_QUIT, 0, 0);
    KillTimer(hWnd, 30);
    return 0;
  }

  return DefWindowProc(hWnd, Msg, wParam, lParam);

} /* End of 'WinFunc' function */

/* END OF 'T07GLOBE.C' FILE */
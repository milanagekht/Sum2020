/* Gekht Milana, 02.06.2020 */

#include <stdlib.h>
#include <windows.h>
#include <math.h>

/* Pi math constant */
#define PI 3.14159265358979323846

/* Window class name */
#define WND_CLASS_NAME "Clock"

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
  while (GetMessage(&msg, NULL, 0, 0))
    DispatchMessage(&msg); 

  return 0;
} /* End of 'WinMain' function */

/* Draw clock hand function */
VOID DrawHand( HDC hDC, INT X0, INT Y0, DOUBLE AngleInRadian, INT L, INT W, COLORREF Color )
{
  INT i;
  HPEN hPenOld;
  HBRUSH hBrOld;
  POINT pnts[] = 
  {
    {0, L}, {W, 0}, {0, -W}, {-W, 0}
  }, pnts1[sizeof(pnts) / sizeof(pnts[0])];

  /*Transform hand shape */
  for (i = 0; i < sizeof(pnts) / sizeof(pnts[0]); i++)
  {
    pnts1[i].x = X0 + pnts[i].x * cos(AngleInRadian) + pnts[i].y * sin(AngleInRadian);
    pnts1[i].y = Y0 - (pnts[i].y * cos(AngleInRadian) - pnts[i].x * sin(AngleInRadian));
  }

  hPenOld = SelectObject(hDC, GetStockObject(NULL_PEN));
  hBrOld = SelectObject(hDC, GetStockObject(DC_BRUSH));

  SetDCPenColor(hDC, RGB(255, 0, 0));
  SetDCBrushColor(hDC, Color);

  Polygon(hDC, pnts1, sizeof(pnts) / sizeof(pnts[0]));

  SelectObject(hDC, hPenOld);
  SelectObject(hDC, hBrOld);
}/* End of 'DrawHand' function */

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
  INT r, len;
  DOUBLE a;
  CHAR Buf[102];
  SYSTEMTIME st;
  HPEN hPen, hPenOld;
  HFONT hFnt, hFntOld;
  HDC hDC;
  RECT rc;
  PAINTSTRUCT ps;
  BITMAP bm;
  static HDC hMemDC, hMemDCImage;
  static HBITMAP hBm, hBmImage;
  static INT w, h;
  static CHAR *WD[] =
  {
    "бя", "ом", "бр", "яп", "вр", "ор", "яа"
  };

  switch (Msg)
  {
  case WM_GETMINMAXINFO:
    ((MINMAXINFO *)lParam)->ptMaxTrackSize.y =
      GetSystemMetrics(SM_CYMAXTRACK) + GetSystemMetrics(SM_CYCAPTION) + 2 * GetSystemMetrics(SM_CYBORDER);
    return 0;
  case WM_CREATE:
    hBmImage = LoadImage(NULL, "clockface.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

    SetTimer(hWnd, 30, 102, NULL);
    hDC = GetDC(hWnd);
    hMemDC = CreateCompatibleDC(hDC);
    hMemDCImage = CreateCompatibleDC(hDC);
    SelectObject(hMemDCImage, hBmImage);
    ReleaseDC(hWnd, hDC);
    hBm = NULL;
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
    SendMessage(hWnd, WM_TIMER, 0, 0);
    return 0;
  case WM_SYSKEYDOWN:
    if (wParam == VK_RETURN)
      FlipFullScreen(hWnd);
    return 0;
  case WM_KEYDOWN:
    if (wParam == VK_ESCAPE)
      SendMessage(hWnd, WM_CLOSE, 0, 0);
    else if (wParam == 'F')
      FlipFullScreen(hWnd);
    return 0;
  case WM_TIMER:
    srand(102); 
    /* Clear backgrond */
    SelectObject(hMemDC, GetStockObject(WHITE_BRUSH));
    SelectObject(hMemDC, GetStockObject(NULL_PEN));
    Rectangle(hMemDC, 0, 0, w + 1, h + 1);

    /* Draw clockface */
    r = (w < h ? w : h);
    GetObject(hBmImage, sizeof(bm), &bm);
    /* BitBlt(hMemDC, 0, 0, bm.bmWidth, bm.bmHeight, hMemDCImage, 0, 0, SRCCOPY); */
    SetStretchBltMode(hMemDC, COLORONCOLOR);
    StretchBlt(hMemDC, w / 2 - r / 2, h / 2 - r / 2, r, r, hMemDCImage, 0, 0, bm.bmWidth, bm.bmHeight, SRCCOPY);

    /* Draw hands */
    GetLocalTime(&st);

    hPen = CreatePen(PS_SOLID, 8, RGB(255, 0, 255));
    hPenOld = SelectObject(hMemDC, hPen);
    /*MoveToEx(hMemDC, w / 2, h/2, NULL);
    a = (st.wSecond + st.wMilliseconds / 1000.0) * 2 * PI / 60.0;
    LineTo(hMemDC, w / 2 + 0.32 * r * sin(a), h/2 - 0.32 * r * cos(a));*/

    DrawHand(hMemDC, w / 2, h/2, (st.wHour % 12 + st.wMinute / 60.0) * 2 * PI / 12.0, r *0.26, 24, RGB(0, 0, 0));
    DrawHand(hMemDC, w / 2, h/2, (st.wMinute + st.wSecond / 60.0) * 2 * PI / 60.0, r *0.30, 18, RGB(0, 0, 0));
    DrawHand(hMemDC, w / 2, h/2, (st.wSecond + st.wMilliseconds / 1000.0) * 2 * PI / 60.0, r *0.47, 13, RGB(0, 0, 0));
    
    SelectObject(hMemDC, hPenOld);
    DeleteObject(hPen);

    /* Output text */
    hFnt = CreateFont(r / 18, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
             RUSSIAN_CHARSET,
             OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, ANTIALIASED_QUALITY,
             FF_DECORATIVE | VARIABLE_PITCH, "Consolas");
    hFntOld = SelectObject(hMemDC, hFnt);
    len = wsprintf(Buf, "%02i.%02i.%i\n(%s)",
            st.wDay, st.wMonth, st.wYear, WD[st.wDayOfWeek]);

    SetBkMode(hMemDC, TRANSPARENT);
    SetTextColor(hMemDC, RGB(0, 0, 0));
    /* TextOut(hMemDCFrame, w / 2, h / 2, Buf, len); */
    rc.left = 0;
    rc.right = w;
    rc.top = 2* h / 3;
    rc.bottom = h;
    DrawText(hMemDC, Buf, len, &rc, DT_CENTER | DT_VCENTER);

    SelectObject(hMemDC, hFntOld);
    DeleteObject(hFnt);

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
    DeleteObject(hBmImage);
    DeleteDC(hMemDCImage);
    
    PostMessage(hWnd, WM_QUIT, 0, 0);
    KillTimer(hWnd, 30);
    return 0;
  }

  return DefWindowProc(hWnd, Msg, wParam, lParam);

} /* End of 'WinFunc' function */

/* END OF 'T03CLOCK.C' FILE */
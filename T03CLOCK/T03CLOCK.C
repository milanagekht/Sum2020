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

/* Main window handle function */
LRESULT CALLBACK WinFunc( HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam )
{
  INT r;
  DOUBLE a;
  SYSTEMTIME st;
  HPEN hPen, hPenOld;
  HDC hDC;
  PAINTSTRUCT ps;
  BITMAP bm;
  static HDC hMemDC, hMemDCImage;
  static HBITMAP hBm, hBmImage;
  static INT w, h;

  switch (Msg)
  {
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
    w = LOWORD(lParam);
    h = HIWORD(lParam);

    if (hBm != NULL)
      DeleteObject(hBm);
    hDC = GetDC(hWnd);
    hBm = CreateCompatibleBitmap(hDC, w, h);
    ReleaseDC(hWnd, hDC);
    SelectObject(hMemDC, hBm);
    SendMessage(hWnd, WM_TIMER, 0, 0);
    return 0;
  case WM_KEYDOWN:
    if (wParam == VK_ESCAPE)
      SendMessage(hWnd, WM_CLOSE, 0, 0);
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

    DrawHand(hMemDC, w / 2, h/2, (st.wHour % 12 + st.wMinute / 60.0) * 2 * PI / 12.0, r *0.18, 30, RGB(0, 0, 0));
    DrawHand(hMemDC, w / 2, h/2, (st.wMinute + st.wSecond / 60.0) * 2 * PI / 60.0, r *0.30, 18, RGB(0, 0, 0));
    DrawHand(hMemDC, w / 2, h/2, (st.wSecond + st.wMilliseconds / 1000.0) * 2 * PI / 60.0, r *0.47, 13, RGB(0, 0, 0));
    
    SelectObject(hMemDC, hPenOld);
    DeleteObject(hPen);

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
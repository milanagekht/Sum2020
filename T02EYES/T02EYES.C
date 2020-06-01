/* Gekht Milana, 01.06.2020 */

#include <stdlib.h>
#include <windows.h>
#include <math.h>

/* Window class name */
#define WND_CLASS_NAME "Big Brother Is Watching You"

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
    WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN,
    /* CW_USEDEFAULT, CW_USEDEFAULT,
    CW_USEDEFAULT, CW_USEDEFAULT, */
    47 , 47, 47 * 20, 30 * 20,
    NULL, NULL, hInstance, NULL);

  /* Show and update window */
  ShowWindow(hWnd, CmdShow);

  /* Message loop */
  while (GetMessage(&msg, NULL, 0, 0))
    DispatchMessage(&msg); 

  return 0;
} /* End of 'WinMain' function */

/* Draw eye function */
VOID DrawEye(HDC hDC, INT X, INT Y, INT R, INT R1, INT Mx, INT My )
{
  INT
    dx = Mx - X, dy = My - Y, delta = R - R1;
  DOUBLE
    len = sqrt(dx * dx + dy * dy), t;

  /* Big circle */
  SelectObject(hDC, GetStockObject(DC_BRUSH));
  SelectObject(hDC, GetStockObject(DC_PEN));
  SetDCPenColor(hDC, RGB(0, 0, 0));
  SetDCBrushColor(hDC, RGB(255, 255, 255));
  Ellipse(hDC, X - R, Y - R, X + R + 1, Y + R + 1);

  /* Small circle */
  SelectObject(hDC, GetStockObject(DC_BRUSH));
  SelectObject(hDC, GetStockObject(DC_PEN));
  SetDCPenColor(hDC, RGB(0, 0, 0));
  SetDCBrushColor(hDC, RGB(168, 0, 2));

  if (len == 0)
    t = 0;
  else if (delta > len)
    t = 0;
  else
    t = delta / len;
  dx *= t;
  dy *= t;
  Ellipse(hDC, X + dx - R1, Y + dy - R1, X + dx + R1 + 1, Y + dy + R1 + 1);
}/* End of 'DrawEye' function */

/* Main window handle function */
LRESULT CALLBACK WinFunc( HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam )
{
  INT i, j, r, r1;
  HDC hDC;
  PAINTSTRUCT ps;
  POINT pt;
  HPEN hPen, hOldPen;
  static HDC hMemDC;
  static HBITMAP hBm;
  static INT w, h;

  switch (Msg)
  {
  case WM_CREATE:
    SetTimer(hWnd, 30, 102, NULL);
    hDC = GetDC(hWnd);
    hMemDC = CreateCompatibleDC(hDC);
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

    /* Draw eye */
    GetCursorPos(&pt);
    ScreenToClient(hWnd, &pt);
    for (i = 0; i < 20; i++)
      DrawEye(hMemDC, rand() % w, rand() % h, 90, 47, pt.x, pt.y);
    InvalidateRect(hWnd, NULL, FALSE);
    return 0;
  case WM_PAINT:
    hDC = BeginPaint(hWnd, &ps);
    BitBlt(hDC, 0, 0, w, h, hMemDC, 0, 0, SRCCOPY);
    EndPaint(hWnd, &ps);
    return 0;
  case WM_CLOSE:
    if (MessageBox(hWnd, "Are you sure you want to close the window?", "Close",
        MB_YESNO | MB_DEFBUTTON2 | MB_ICONQUESTION) == IDYES)
      DestroyWindow(hWnd);
    return 0;
  case WM_DESTROY:
    PostMessage(hWnd, WM_QUIT, 0, 0);
    KillTimer(hWnd, 30);

    if (hBm != NULL)
      DeleteObject(hBm);
    DeleteDC(hMemDC);
    return 0;
  }

  return DefWindowProc(hWnd, Msg, wParam, lParam);

} /* End of 'WinFunc' function */

/* END OF 'T02EYES.C' FILE */
/* FILE NAME: MAIN.C
 * PROGRAMMER: MG5
 * DATE: 06.06.2020 
 * PURPOSE: 3D animation project.
            WinAPI Start up Mdule
 */
#include <time.h>

#include "../def.h"
#include "../anim/rnd/rnd.h"

#include <windows.h>


/* Window class name */
#define WND_CLASS_NAME "ANIM"

LRESULT CALLBACK WinFunc( HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam );

/* The main program function */
INT WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, CHAR *CmdLine, INT CmdShow )
{
  HWND hWnd;
  MSG msg;
  WNDCLASS wc;

  /* Fill window class structure */
  wc.style = 0;
  wc.cbClsExtra = 0;
  wc.cbWndExtra = 0;
  wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
  wc.hCursor = LoadCursor(NULL, IDC_HAND);
  wc.hIcon = LoadIcon(NULL, IDI_SHIELD);
  wc.hInstance = hInstance;
  wc.lpszClassName = WND_CLASS_NAME;
  wc.lpszMenuName = NULL;
  wc.lpfnWndProc = WinFunc;

  /* Class registration */
  if (!RegisterClass(&wc))
  {
    MessageBox(NULL, "Error register window class", "ERROR", MB_OK);
    return 0;
  }

  /*window creation */
  hWnd = CreateWindow(WND_CLASS_NAME, WND_CLASS_NAME, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
      NULL, NULL, hInstance, NULL);
  
  ShowWindow(hWnd, CmdShow);
  /* Message loop */
  while(GetMessage(&msg, NULL, 0, 0))
    DispatchMessage(&msg);
  return 0;
}/* End of 'WinMain' function */



/* Main window handle function */
LRESULT CALLBACK WinFunc( HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam )
{
  HDC hDC;
  PAINTSTRUCT ps;
  static mg5PRIM Pr;

  switch (Msg)
  {
  case WM_GETMINMAXINFO:
    ((MINMAXINFO *)lParam)->ptMaxTrackSize.y =
      GetSystemMetrics(SM_CYMAXTRACK) + GetSystemMetrics(SM_CYCAPTION) + 2 * GetSystemMetrics(SM_CYBORDER);
    return 0;
  case WM_CREATE:
    MG5_RndInit(hWnd);
    MG5_RndPrimCreateSphere(&Pr, VecSet(0, 0, 0), 8, 30, 47);
    SetTimer(hWnd, 30, 2, NULL);
    return 0;
  case WM_CLOSE:
    if (MessageBox(hWnd, "Are you sure you want to close the window?", "Close",
      MB_YESNO | MB_DEFBUTTON2 | MB_ICONQUESTION) == IDYES)
      DestroyWindow(hWnd);
  case WM_KEYDOWN:
    if (wParam == VK_ESCAPE)
      SendMessage(hWnd, WM_CLOSE, 0, 0);
    return 0;
  case WM_SIZE:
    MG5_RndResize(LOWORD(lParam), HIWORD(lParam));
    SendMessage(hWnd, WM_TIMER, 0, 0);
    return 0;
  case WM_TIMER:
    MG5_RndStart();
    MG5_RndPrimDraw(&Pr, MatrRotateX(30 * clock() / 2020.0));
    MG5_RndEnd();
    hDC = GetDC(hWnd);
    MG5_RndCopyFrame(hDC);

    /* Send repaint message */
    InvalidateRect(hWnd, NULL, FALSE);
    return 0;
  case WM_PAINT:
    hDC = BeginPaint(hWnd, &ps);
    MG5_RndCopyFrame(hDC);
    EndPaint(hWnd, &ps);
    return 0;
  case WM_DESTROY:
    MG5_RndPrimFree(&Pr);
    MG5_RndClose();
    KillTimer(hWnd, 30);
    PostQuitMessage(30);
    return 0;
  case WM_ERASEBKGND:
    return 1;
  }

return DefWindowProc(hWnd, Msg, wParam, lParam);

} /* End of 'WinFunc' function */

/* END OF 'MAIN.C' FILE */
 /* FILE NAME: ANIM.C
 * PROGRAMMER: MG5
 * DATE: 09.06.2020
 * PURPOSE: 3D animation project.
 */

#include "anim.h"

mg5ANIM MG5_Anim;

VOID MG5_AnimClose( VOID )
{
  INT i;

  for (i = 0; i < MG5_Anim.NumOfUnits; i++)
  {
    MG5_Anim.Units[i]->Close(MG5_Anim.Units[i], &MG5_Anim);
    free(MG5_Anim.Units[i]);
    /* MG5_Anim.Units[i] = NULL; */
  }
  MG5_Anim.NumOfUnits = 0;
  MG5_RndClose();
} /* End of 'MG5_AnimClose' function */


VOID MG5_AnimRender( VOID )
{
  INT i;

  MG5_TimerResponse();
  MG5_AnimKeyboardResponse();
  MG5_AnimMouseResponse();


  for (i = 0; i < MG5_Anim.NumOfUnits; i++)
    MG5_Anim.Units[i]->Response(MG5_Anim.Units[i], &MG5_Anim);
  MG5_RndStart();
  for (i = 0; i < MG5_Anim.NumOfUnits; i++)
    MG5_Anim.Units[i]->Render (MG5_Anim.Units[i], &MG5_Anim);
  MG5_RndEnd();

} /* End of 'MG5_AnimRender' finction */ 


VOID MG5_AnimUnitAdd(mg5UNIT *Uni)
{
  if (MG5_Anim.NumOfUnits < MG5_MAX_UNITS)
    MG5_Anim.Units[MG5_Anim.NumOfUnits++] = Uni, Uni->Init(Uni, &MG5_Anim); 
}/* End of 'MG5_AnimUnitAdd' function */


VOID MG5_AnimCopyFrame( VOID )
{
  MG5_RndCopyFrame();
  /*MG5_Anim.hDC = hDC;*/
}/* End of 'MG5_AnimCopyFrame' function */


VOID MG5_AnimEnd( VOID )
{
}/* End of 'MG5_AnimEnd' function */


VOID MG5_AnimResize(INT W, INT H)
{
  MG5_Anim.H = H;
  MG5_Anim.W = W;
  MG5_RndResize(W, H);
}/* End of 'MG5_AnimResize' function */



VOID MG5_AnimInit (HWND hWnd)
{
  MG5_RndInit(hWnd);
  MG5_TimerInit();

  MG5_Anim.hWnd = hWnd;
  MG5_Anim.hDC = MG5_hRndDC;
  MG5_Anim.H = MG5_RndFrameH;
  MG5_Anim.W = MG5_RndFrameW;
}/* End of 'MG5_AnimInit' function */

VOID MG5_AnimFlipFullScreen( VOID )
{
static BOOL IsFullScreen = FALSE;
static RECT SaveRect;


  if (!IsFullScreen)
  {
    HMONITOR hmon;
    MONITORINFO moninfo;
    RECT rc;

    /* Fullscreen mode */
    GetWindowRect(MG5_Anim.hWnd, &SaveRect);

    /* Get nearest monitor */
    hmon = MonitorFromWindow(MG5_Anim.hWnd, MONITOR_DEFAULTTONEAREST);

    /* Get monitor information */
    moninfo.cbSize = sizeof(moninfo);
    GetMonitorInfo(hmon, &moninfo);

    rc = moninfo.rcMonitor;
    AdjustWindowRect(&rc, GetWindowLong(MG5_Anim.hWnd, GWL_STYLE), FALSE);
    SetWindowPos(MG5_Anim.hWnd, HWND_TOP,
      rc.left, rc.top,
      rc.right - rc.left, rc.bottom - rc.top,
      SWP_NOOWNERZORDER);
  }
  else
  {
    /* Restore window size and position */
    SetWindowPos(MG5_Anim.hWnd, HWND_TOP,
      SaveRect.left, SaveRect.top,
      SaveRect.right - SaveRect.left, SaveRect.bottom - SaveRect.top,
      SWP_NOOWNERZORDER);
  }
   IsFullScreen = !IsFullScreen;
} /* End of 'FlipFullScreen' function */


VOID MG5_AnimDoExit ( VOID )
{
  SendMessage(MG5_Anim.hWnd, WM_CLOSE, 0, 0);
} /*End of 'AnimDoExit' function */

/* End of "ANIM.c" file */
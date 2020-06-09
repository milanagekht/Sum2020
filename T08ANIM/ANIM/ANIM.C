 /* FILE NAME: ANIM.C
 * PROGRAMMER: MG5
 * DATE: 09.06.2020
 * PURPOSE: 3D animation project.
 */

#include "..\DEF.h"

#include "anim.h"
#include <stdio.h>



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


VOID MG5_AnimCopyFrame(HDC hDC)
{
  MG5_RndCopyFrame(hDC);
}/* End of 'MG5_AnimCopyFrame' function */


VOID MG5_AnimEnd( VOID )
{
}/* End of 'MG5_AnimEnd' function */


VOID MG5_AnimResize(INT w, INT h)
{
  MG5_RndResize(w, h);
}/* End of 'MG5_AnimResize' function */



VOID MG5_AnimInit (HWND hWnd)
{
  MG5_RndInit(hWnd);
  MG5_TimerInit();
}/* End of 'MG5_AnimInit' function */

VOID MG5_AnimFlipFullScreen( VOID )
{
  static BOOL IsFullScreen = FALSE;
  static RECT SaveRC;

  if (IsFullScreen)
  {
    /* Restore window size and position */
    IsFullScreen = FALSE;
    SetWindowPos(MG5_Anim.hWnd, HWND_TOP, SaveRC.left, SaveRC.top,
      SaveRC.right - SaveRC.left, SaveRC.bottom - SaveRC.top, SWP_NOOWNERZORDER);
  }
  else
  {
    RECT rc;
    HMONITOR hMon;
    MONITORINFOEX moninfo;

    /* Go to full screen mode */
    IsFullScreen = TRUE;
    /* Save window position and size */
    GetWindowRect(MG5_Anim.hWnd, &SaveRC);
    /* Obtain closest monitor info */
    hMon = MonitorFromWindow(MG5_Anim.hWnd, MONITOR_DEFAULTTONEAREST);
    moninfo.cbSize = sizeof(MONITORINFOEX);
    GetMonitorInfo(hMon, (MONITORINFO *)&moninfo);

    rc = moninfo.rcMonitor;
    AdjustWindowRect(&rc, GetWindowLong(MG5_Anim.hWnd, GWL_STYLE), FALSE);
    SetWindowPos(MG5_Anim.hWnd, HWND_TOP, rc.left, rc.top,
      rc.right - rc.left, rc.bottom - rc.top, SWP_NOOWNERZORDER);
  }
} /* End of 'FlipFullScreen' function */


VOID MG5_AnimDoExit ( VOID )
{
  SendMessage(MG5_Anim.hWnd, WM_CLOSE, 0, 0);
}

/* End of "ANIM.c" file */
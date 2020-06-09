/* FILE NAME: INPUT.C
 * PROGRAMMER: MG5
 * DATE: 9.06.2018
 * PURPOSE: Functions to work with input devices.
 */
#include "anim.h"

/* Initilizate keyboard function */
static VOID MG5_AnimKeyboardInit( VOID )
{
} /* End of 'MG5_AnimKeyboardInit' function */


VOID MG5_AnimKeyboardResponse( VOID )
{
  INT i;

  GetKeyboardState(MG5_Anim.Keys);
  for (i = 0; i < 256; i++)
  {
    MG5_Anim.Keys[i] >>= 7;
    MG5_Anim.KeysClick[i] = MG5_Anim.Keys[i] && !MG5_Anim.KeysOld[i];
  }
  memcpy(MG5_Anim.KeysOld, MG5_Anim.Keys, 256);
}


/* Initilizate mouse function */
static VOID MG5_AnimMouseInit( VOID )
{
} /* End of 'MG5_AnimMouseInit' function */


VOID MG5_AnimMouseResponse( VOID )
{
  POINT pt;

  MG5_Anim.Mdz = MG5_MouseWheel;
  MG5_Anim.Mz += MG5_MouseWheel;
  MG5_MouseWheel = 0;

  GetCursorPos(&pt);
  ScreenToClient(MG5_Anim.hWnd, &pt);
  MG5_Anim.Mdx = pt.x - MG5_Anim.Mx;
  MG5_Anim.Mdy = pt.y - MG5_Anim.My;
  MG5_Anim.Mx = pt.x;
  MG5_Anim.My = pt.y;
}


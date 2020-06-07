/* FILE NAME: RNDBASE.C
 * PROGRAMMER: MG5
 * DATE: 06.06.2020
 * PURPOSE: 3D animation project.
 *          Render system render handle function
 */

#include <time.h>

#include "rnd.h"


/* Render system initialisation function.
 * ARGUMENTS:
 *   - HWND:
 *       HWND hWnd;
 * RETURNS: None.
 */
VOID MG5_RndInit( HWND hWnd )
{
  HDC hDC;

  MG5_hRndWnd = hWnd;

  /* Create frame */
  hDC = GetDC(hWnd);
  MG5_hRndDCFrame = CreateCompatibleDC(hDC);
  ReleaseDC(hWnd, hDC);
  MG5_hRndBmFrame = NULL;
  MG5_RndFrameH = 1000;
  MG5_RndFrameW = 1000;

  MG5_RndProjSize = MG5_RndProjDist = 1.0;
  MG5_RndProjFarClip = 30;

  MG5_RndProjSet();
  MG5_RndCamSet(VecSet(12, 8, 20)/*VecSet(sin(clock() * 2.26)* 2, sin(clock() * 2) * 7,5)*/, VecSet(0, 0, 0), VecSet(0, 1, 0));
} /* End of 'MG5_RndInit' function */


/* Render system initialisation function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID MG5_RndClose( VOID )
{
  if (MG5_hRndBmFrame != NULL)
    DeleteObject(MG5_hRndBmFrame);
  DeleteDC(MG5_hRndDCFrame);
} /* End of 'MG5_RndClose' function */


/* Change window size function.
 * ARGUMENTS:
 *   - window size:
 *       INT W, INT H
 * RETURNS: None.
 */
VOID MG5_RndResize( INT W, INT H )
{
  HDC hDC = GetDC(MG5_hRndWnd);

  if (MG5_hRndBmFrame)
    DeleteObject(MG5_hRndBmFrame);
  MG5_hRndBmFrame = CreateCompatibleBitmap(hDC, W, H);
  ReleaseDC(MG5_hRndWnd, hDC);
  SelectObject(MG5_hRndDCFrame, MG5_hRndBmFrame);

  MG5_RndFrameW = W;
  MG5_RndFrameH = H;
  MG5_RndProjSet();
} /* End of 'MG5_RndResize' function */


/* Change window size function.
 * ARGUMENTS:
 *   - window:
 *       HDC hDC.
 * RETURNS: None.
 */
VOID MG5_RndCopyFrame( HDC hDC )
{
  BitBlt(hDC, 0, 0, MG5_RndFrameW, MG5_RndFrameH, MG5_hRndDCFrame, 0, 0, SRCCOPY);
} /* End of 'MG5_RndCopyFrame' function */


/* Start drawing function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID MG5_RndStart( VOID )
{
  SelectObject(MG5_hRndDCFrame, GetStockObject(NULL_PEN));
  /*SelectObject(MG5_hRndDCFrame, GetStockObject(WHITE_BRUSH));*/
  SelectObject(MG5_hRndDCFrame, GetStockObject(DC_BRUSH));
  SetDCBrushColor(MG5_hRndDCFrame, RGB(0, 0, 0));
  Rectangle(MG5_hRndDCFrame, 0, 0, MG5_RndFrameW + 1, MG5_RndFrameH + 1);
} /* End of 'MG5_RndStart' function */


/* End draw function 
 * ARGUMENTS: None.
 *
 * RETURNS: None.
 */
VOID MG5_RndEnd( VOID )
{

} /* End of 'MG5_RndEnd' function */


/* Create camera function.
 * ARGUMENTS: None.
 *
 * RETURNS: None.
 */
VOID MG5_RndProjSet( VOID )
{
  DBL ratio_x, ratio_y;

  ratio_x = ratio_y = MG5_RndProjSize / 2;

  if (MG5_RndFrameW > MG5_RndFrameH)
    ratio_x *= (DBL)MG5_RndFrameW / MG5_RndFrameH;
  else
    ratio_y *= (DBL)MG5_RndFrameH / MG5_RndFrameW;

  MG5_RndMatrProj = MatrFrustum(-ratio_x, ratio_x, -ratio_y, ratio_y, 
                      MG5_RndProjDist, MG5_RndProjFarClip);
  MG5_RndMatrVP = MatrMulMatr(MG5_RndMatrView, MG5_RndMatrProj);
} /* End of 'MG5_RndProjSet' function */


/* Set camera function.
 * ARGUMENTS:
 *   - parametre of location:
 *       VEC Loc, VEC At, VEC Up.
 * RETURNS: None.
 */
VOID MG5_RndCamSet( VEC Loc, VEC At, VEC Up )
{
  MG5_RndMatrView = MatrView(Loc, At, Up);
  MG5_RndMatrVP = MatrMulMatr(MG5_RndMatrView, MG5_RndMatrProj);
} /* End of 'MG5_RndCamSet' function */


/* END OF 'RNDBASE.C' FILE */
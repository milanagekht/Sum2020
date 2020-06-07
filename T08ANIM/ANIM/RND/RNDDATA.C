 /* FILE NAME: RNDDATA.C
 * PROGRAMMER: MG5
 * DATE: 06.06.2020
 * PURPOSE: 3D animation project.
 *          Render system global data
 */

#include "rnd.h"


HWND MG5_hRndWnd;        /* Work window handle */
HDC MG5_hRndDCFrame;     /* Work window memory device context  */
HBITMAP MG5_hRndBmFrame; /* Work window background bitmap handle */
INT MG5_RndFrameH, MG5_RndFrameW; /* Work window size */


DBL
  MG5_RndProjSize,  /* Project plane fit square */
  MG5_RndProjDist,  /* Distance to project plane from viewer (near) */
  MG5_RndProjFarClip;  /* Distance to project far clip plane (far) */


MATR
  MG5_RndMatrView, /* View coordinate system matrix */
  MG5_RndMatrProj, /* Projection coordinate system matrix */
  MG5_RndMatrVP;   /* Stored (View * Proj) matrix */


/* END OF 'RNDDATA.C' FILE */
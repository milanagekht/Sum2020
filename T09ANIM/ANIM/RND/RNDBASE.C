/* FILE NAME: RNDBASE.C
 * PROGRAMMER: MG5
 * DATE: 06.06.2020
 * PURPOSE: 3D animation project.
 *          Handle function for render system
 */

#include "rnd.h"

/* Link libraryes */
#pragma comment(lib, "opengl32")


/* Render system initialisation function.
 * ARGUMENTS:
 *   - HWND:
 *       HWND hWnd;
 * RETURNS: None.
 */
VOID MG5_RndInit( HWND hWnd )
{
  INT i;
  /*HDC hDC;*/
  PIXELFORMATDESCRIPTOR pfd = {0};

  MG5_hRndWnd = hWnd;


  /* Create frame */
  MG5_hRndDC = GetDC(hWnd);
  /*  hDC = GetDC(hWnd);
  MG5_hRndDCFrame = CreateCompatibleDC(hDC);
  ReleaseDC(hWnd, hDC);
  MG5_hRndBmFrame = NULL;
  MG5_RndFrameH = 1000;
  MG5_RndFrameW = 1000;*/

  /* OpenGL initialisation */
  pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
  pfd.nVersion = 1;
  pfd.dwFlags = PFD_DOUBLEBUFFER | PFD_SUPPORT_OPENGL;
  pfd.cColorBits = 32;
  pfd.cDepthBits = 32;

  i = ChoosePixelFormat(MG5_hRndDC, &pfd);
  DescribePixelFormat(MG5_hRndDC, i, sizeof(pfd), &pfd);
  SetPixelFormat(MG5_hRndDC, i, &pfd);

  /* OpenGL init: setup rendering context */
  MG5_hRndGLRC = wglCreateContext(MG5_hRndDC);
  wglMakeCurrent(MG5_hRndDC, MG5_hRndGLRC);

  /* Initialize GLEW library */
  if (glewInit() != GLEW_OK ||
      !(GLEW_ARB_vertex_shader && GLEW_ARB_fragment_shader))
  {
    MessageBox(hWnd, "Error OpenGL initialization", "ERROR",
      MB_ICONERROR | MB_OK);
    exit(0);
  }

  /* Set default render parameters */
  glClearColor(0.30, 0.50, 0.8, 1);
  glEnable(GL_DEPTH_TEST);

  MG5_RndProjSize = MG5_RndProjDist = 1.0;
  MG5_RndProjFarClip = 300;

  MG5_RndProjSet();
  MG5_RndCamSet(VecSet(12, 8, 20)/*VecSet(sin(clock() * 2.26)* 2, sin(clock() * 2) * 7,5)*/, VecSet(0, 0, 0), VecSet(0, 1, 0));

} /* End of 'MG5_RndInit' function */


/* Render system initialisation function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID MG5_RndClose( VOID )
{
  wglMakeCurrent(NULL, NULL);
  wglDeleteContext(MG5_hRndGLRC);
  /*if (MG5_hRndBmFrame != NULL)
    DeleteObject(MG5_hRndBmFrame);*/
  ReleaseDC(MG5_hRndWnd,MG5_hRndDC);
} /* End of 'MG5_RndClose' function */


/* Change window size function.
 * ARGUMENTS:
 *   - window size:
 *       INT W, INT H
 * RETURNS: None.
 */
VOID MG5_RndResize( INT W, INT H )
{
 glViewport(0, 0, W, H);

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
VOID MG5_RndCopyFrame( VOID )
{
   wglSwapLayerBuffers(MG5_hRndDC, WGL_SWAP_MAIN_PLANE);
  /*BitBlt(hDC, 0, 0, MG5_RndFrameW, MG5_RndFrameH, MG5_hRndDCFrame, 0, 0, SRCCOPY);*/
} /* End of 'MG5_RndCopyFrame' function */


/* Start drawing function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID MG5_RndStart( VOID )
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  SelectObject(MG5_hRndDC, GetStockObject(NULL_PEN));
  /*SelectObject(MG5_hRndDCFrame, GetStockObject(WHITE_BRUSH));*/
  SelectObject(MG5_hRndDC, GetStockObject(DC_BRUSH));
  SetDCBrushColor(MG5_hRndDC, RGB(0, 0, 0));
  /*Rectangle(MG5_hRndDC, 0, 0, MG5_RndFrameW + 1, MG5_RndFrameH + 1);*/

} /* End of 'MG5_RndStart' function */


/* End draw function 
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID MG5_RndEnd( VOID )
{
  glFinish();
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

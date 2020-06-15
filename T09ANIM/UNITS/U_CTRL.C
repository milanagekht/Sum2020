/* FILE NAME  : U_CTRL.C
 * PROGRAMMER : MG5
 * LAST UPDATE: 09.06.2020
 * PURPOSE    : Simple WinAPI animation project.
 *              Control unit sample.
 */

#include <stdio.h>

#include "../units.h"

/* Animation unit representation type */
typedef struct tagmg5UNIT_CTRL
{
  MG5_UNIT_BASE_FIELDS;
  DBL
    Distance,      /* Camera offset */
    RotateAngle,   /* Camera rotate angle */
    ElevatorAngle; /* Camera elevator angle */
  mg5PRIM Axes;
} mg5UNIT_CTRL;


/* Unit initialization function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       mg5UNIT_CTRL *Uni;
 *   - animation context:
 *       mg5ANIM *Ani;
 * RETURNS: None.
 */
static VOID MG5_UnitInit( mg5UNIT_CTRL *Uni, mg5ANIM *Ani )
{
  HFONT hFnt, hFntOld;
  mg5VERTEX V[] = 
    {
      {{0, 0, 0}, {0, 0}, {0, 0, 0}, {1, 0, 0, 1}},
      {{3000, 0, 0}, {0, 0}, {0, 0, 0}, {1, 0, 0, 1}},
      {{0, 0, 0}, {0, 0}, {0, 0, 0}, {0, 1, 0, 1}},
      {{0, 3000, 0}, {0, 0}, {0, 0, 0}, {0, 1, 0, 1}},
      {{0, 0, 0}, {0, 0}, {0, 0, 0}, {0, 0, 1, 1}},
      {{0, 0, 3000}, {0, 0}, {0, 0, 0}, {0, 0, 1, 1}}
    };
  INT Ind[] = {0, 1, 2, 3, 4, 5};
 

  MG5_RndPrimCreate(&Uni->Axes, MG5_RND_PRIM_LINES, V, 6, Ind, 6);
  Uni->RotateAngle = 8;
  Uni->ElevatorAngle = 30;
  Uni->Distance = 47;

  /* Font creation for text drawing */
  hFnt = CreateFont(47, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
                    RUSSIAN_CHARSET, OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS,
                    ANTIALIASED_QUALITY, FF_SWISS | VARIABLE_PITCH, "Consolas");
  hFntOld = SelectObject(Ani->hDC, hFnt);
  wglUseFontBitmaps(Ani->hDC, 0, 30, 30);
  SelectObject(Ani->hDC, hFntOld);
  DeleteObject(hFnt);
} /* End of 'MG5_UnitInit' function */


/* Unit deinitialization function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       mg5UNIT_CTRL *Uni;
 *   - animation context:
 *       mg5ANIM *Ani;
 * RETURNS: None.
 */
static VOID MG5_UnitClose( mg5UNIT_CTRL *Uni, mg5ANIM *Ani )
{
} /* End of 'MG5_UnitClose' function */


/* Unit inter frame events handle function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       mg5UNIT_CTRL *Uni;
 *   - animation context:
 *       mg5ANIM *Ani;
 * RETURNS: None.
 */
static VOID MG5_UnitResponse( mg5UNIT_CTRL *Uni, mg5ANIM *Ani )
{
  VEC V = VecSet(0, 0, Uni->Distance);
 

  if (Ani->KeysClick['P'])
    Ani->IsPause = !Ani->IsPause;
  if (Ani->KeysClick['F'])
    MG5_AnimFlipFullScreen();
  if (Ani->KeysClick[VK_ESCAPE])
    ;//MG5_AnimDoExit();
  if (Ani->KeysClick['W'])
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  if (Ani->KeysClick['S'])
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

  /*
  Uni->Distance += Ani->GlobalDeltaTime * (2 * Ani->Mdz + 8 * (1 + Ani->Keys[VK_SHIFT] * 30) * (Ani->Keys[VK_NEXT] - Ani->Keys[VK_PRIOR]));
  Uni->RotateAngle += Ani->GlobalDeltaTime * (-300 * Ani->Keys[VK_LBUTTON] * Ani->Mdx + 150 * (Ani->Keys[VK_LEFT] - Ani->Keys[VK_RIGHT]));
  Uni->ElevatorAngle += Ani->GlobalDeltaTime * (-300 * Ani->Keys[VK_LBUTTON] * Ani->Mdy + 47 * (Ani->Keys[VK_UP] - Ani->Keys[VK_DOWN]));

   
  if (Uni->ElevatorAngle > 89.99)
    Uni->ElevatorAngle = 89.99;
  else if (Uni->ElevatorAngle < -89.99)
    Uni->ElevatorAngle = -89.99;

  if (Uni->Distance < 0.0002)
    Uni->Distance = 0.0002;
    */


  Uni->Distance += Ani->GlobalDeltaTime * Ani->Mdz * 15;
  if (Ani->Keys[VK_LBUTTON])
  {
    Uni->RotateAngle += Ani->GlobalDeltaTime * 15 * Ani->Mdx;
    Uni->ElevatorAngle += Ani->GlobalDeltaTime * 15 * Ani->Mdy;
  }
  V = PointTransform(V,
    MatrMulMatr3(MatrRotateZ(-Uni->Distance),
                 MatrRotateX(-Uni->ElevatorAngle),
                 MatrRotateY(-Uni->RotateAngle)));
  MG5_RndCamSet(V, VecSet(0, 1, 0), VecSet(0, 1, 0));

} /* End of 'MG5_UnitResponse' function */


/* U  nit render function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       mg5UNIT_CTRL *Uni;
 *   - animation context:
 *       mg5ANIM *Ani;
 * RETURNS: None.
 */
static VOID MG5_UnitRender( mg5UNIT_CTRL *Uni, mg5ANIM *Ani )
{  
  INT n[10], i, k = 3;
  MATR m = MatrOrtho(0, Ani->W - k, Ani->H - k, 0, -1, 1);
  static CHAR Buf[10][100];
 
  n[0] = sprintf(Buf[0], "FPS: %.3f", Ani->FPS);
  n[1] = sprintf(Buf[1], "Renderer: %s", glGetString(GL_RENDERER));
  n[2] = sprintf(Buf[2], "Vendor: %s", glGetString(GL_VENDOR));
  n[3] = sprintf(Buf[3], "Version: %s", glGetString(GL_VERSION));
  n[4] = sprintf(Buf[4], "GLSL ver: %s",
                 glGetString(GL_SHADING_LANGUAGE_VERSION));
  glPushAttrib(GL_ALL_ATTRIB_BITS);
  glUseProgram(0);
  glEnable(GL_DEPTH_TEST);
  glDisable(GL_CULL_FACE);
  glLoadMatrixf(m.A[0]);
  glListBase(102); /* 102 - base list index from UnitInit:
                    *       wglUseFontBitmaps(Ani->hDC, 0, 256, 102); */
  for (i = 0; i < 5; i++)
  {
    glRasterPos2d(10, 47 * (i + 1));
    glColor3d(0, 0, 0);
    glCallLists(n[i], GL_UNSIGNED_BYTE, Buf[i]);
 
    glRasterPos3d(10 + 3, 47 * (i + 1) + 3, -0.1);
    glColor3d(1, 1, 1);
    glCallLists(n[i], GL_UNSIGNED_BYTE, Buf[i]);
  }
  glPopAttrib();

  glLineWidth(18);
  MG5_RndPrimDraw(&Uni->Axes, MatrIdentity());
  glLineWidth(1);
} /* End of 'MG5_UnitRender' function */


/* Control unit creation function.
 * ARGUMENTS:
 *   - unit structure size in bytes:
 *       INT Size;
 * RETURNS:
 *   (mg5UNIT_CTRL *) pointer to created unit.
 */
mg5UNIT * MG5_UnitCreateControl( VOID )
{
  mg5UNIT_CTRL *Uni;

  /* Memory allocation */
  if ((Uni = (mg5UNIT_CTRL *)MG5_AnimUnitCreate(sizeof(mg5UNIT_CTRL))) == NULL)
    return NULL;
  /* Setup unit methods */
  Uni->Init = (VOID *)MG5_UnitInit;
  Uni->Close = (VOID *)MG5_UnitClose;
  Uni->Response = (VOID *)MG5_UnitResponse;
  Uni->Render = (VOID *)MG5_UnitRender;
  return (mg5UNIT *)Uni;
} /* End of 'MG5_UnitCreateControl' function */

/* END OF 'U_CTRL.C' FILE */

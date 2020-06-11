 /* FILE NAME  : U_CTRL.C
 * PROGRAMMER : MG5
 * LAST UPDATE: 09.06.2020
 * PURPOSE    : Simple 3D animation project.
 */


#include <stdio.h>

#include "../units.h"


typedef struct tagmg5UNIT_CTRL
{
  MG5_UNIT_BASE_FIELDS;
  /*HDC hDC;*/
  DBL
    Distance,      /* Camera offset */
    RotateAngle,   /* Camera rotate angle */
    ElevatorAngle; /* Camera elevator angle */
} mg5UNIT_CTRL;


/* Unit initialization function */
static VOID MG5_UnitInit( mg5UNIT_CTRL *Uni, mg5ANIM *Ani )
{
  /*HFONT hFnt, hFntOld;

  hFnt = CreateFont(102, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
             RUSSIAN_CHARSET,
             OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, ANTIALIASED_QUALITY,
             FF_DECORATIVE | VARIABLE_PITCH, "Consolas");
  Uni->CamLoc = VecSet(0, 8, 8); 

  hFntOld = SelectObject(Ani->hDC, hFnt);
  wglUseFontBitmaps(Ani->hDC, 0, 256, 102);
  SelectObject(Ani->hDC, hFntOld);
  DeleteObject(hFnt); */


  Uni->RotateAngle = 8;
  Uni->ElevatorAngle = 30;
  Uni->Distance = 47;
} /* End of 'MG5_UnitInit' function */


/* Unit deinitialization function */
static VOID MG5_UnitClose( mg5UNIT_CTRL *Uni, mg5ANIM *Ani )
{
} /* End of 'MG5_UnitClose' function */


/* Unit inter frame events handle function */
static VOID MG5_UnitResponse( mg5UNIT_CTRL *Uni, mg5ANIM *Ani )
{
  VEC N = VecSet(0, 0, Uni->Distance);

  /*if (Ani->KeysClick['P'])
    
  if (Ani->KeysClick['F'])
    MG5_AnimFlipFullScreen();*/
  if (Ani->KeysClick[VK_ESCAPE])
    MG5_AnimDoExit();

  if (Ani->Keys[VK_LBUTTON])
  {
    Uni->Distance += Ani->DeltaTime * Ani->Mdz * 0.03;
    Uni->RotateAngle += Ani->DeltaTime * 30 * Ani->Mdx;
    Uni->ElevatorAngle += Ani->DeltaTime * 30 * Ani->Mdy;
  }
  N = PointTransform(N,
    MatrMulMatr3(MatrRotateZ(-Uni->Distance),
                 MatrRotateX(-Uni->ElevatorAngle),
                 MatrRotateY(-Uni->RotateAngle))); 
  MG5_RndCamSet(N, VecSet(0, 1, 0), VecSet(0, 1, 0));

  //MG5_RndCamSet(VecSet(0, 0, 10), VecSet(0, 0, 0), VecSet(0, 1, 0));
} /* End of 'MG5_UnitResponse' function */


/* Unit render function */
static VOID MG5_UnitRender( mg5UNIT_CTRL *Uni, mg5ANIM *Ani )
{
 /* INT n;
  MATR m = MatrOrtho(0, Ani->W - 1, Ani->H - 1, 0, -1, 1);*/
  static CHAR Buf[100];

  /*n = sprintf(Buf, "FPS: %.3f", Ani->FPS);

  glDisable(GL_DEPTH_TEST);
  glLoadMatrixf(m.A[0]);
  glRasterPos2d(10, 102);

  glListBase(102);
  glCallLists(n, GL_UNSIGNED_BYTE, Buf);
  glEnable(GL_DEPTH_TEST);*/
  if (Ani->KeysClick['P'])
    Ani->IsPause = !Ani->IsPause;
  if (Ani->KeysClick['F'])
    MG5_AnimFlipFullScreen();

sprintf(Buf, "My Animation. Frames Per Second: %.2f", Ani->FPS);
SetWindowText(Ani->hWnd, Buf); 
} /* End of 'MG5_UnitRender' function */


/* Control unit creation function */
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
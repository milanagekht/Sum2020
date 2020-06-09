 /* FILE NAME  : U_CTRL.C
 * PROGRAMMER : MG5
 * LAST UPDATE: 09.06.2020
 * PURPOSE    : Simple 3D animation project.
 */


#include <stdio.h>

#include "unit.h"


typedef struct tagmg5UNIT_CTRL
{
  MG5_UNIT_BASE_FIELDS;
  DBL
    Distance,      /* Camera offset */
    RotateAngle,   /* Camera rotate angle */
    ElevatorAngle; /* Camera elevator angle */
} mg5UNIT_CTRL;


/* Unit initialization function */
static VOID MG5_UnitInit( mg5UNIT_CTRL *Uni, mg5ANIM *Ani )
{
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
  VEC L = VecSet(0, 0, Uni->Distance);
  static CHAR Buf[102];

  if (Ani->KeysClick['P'])
    Ani->IsPause = !Ani->IsPause;
  if (Ani->KeysClick[VK_RETURN] && Ani->Keys[VK_MENU])
    MG5_AnimFlipFullScreen();
  if (Ani->KeysClick[VK_ESCAPE])
    MG5_AnimDoExit();

  if (Ani->Keys[VK_LBUTTON])
  {
    Uni->Distance += Ani->DeltaTime * Ani->Mdz * 0.03;
    Uni->RotateAngle += Ani->DeltaTime * 30 * Ani->Mdx;
    Uni->ElevatorAngle += Ani->DeltaTime * 30 * Ani->Mdy;
  }
  L = PointTransform(L,
    MatrMulMatr3(MatrRotateZ(-Uni->Distance),
                 MatrRotateX(-Uni->ElevatorAngle),
                 MatrRotateY(-Uni->RotateAngle)));
  MG5_RndCamSet(L, VecSet(0, 1, 0), VecSet(0, 1, 0));

  SetTextColor(Ani->hDC, RGB(0, 255, 255));
  SetBkMode(Ani->hDC, OPAQUE);
  TextOut(Ani->hDC, 0, 0, Buf, sprintf(Buf, "Frames per second: %.2f", Ani->FPS));
} /* End of 'MG5_UnitResponse' function */


/* Unit render function */
static VOID MG5_UnitRender( mg5UNIT_CTRL *Uni, mg5ANIM *Ani )
{
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
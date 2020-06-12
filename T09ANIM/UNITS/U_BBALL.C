#include "..\units.h"

typedef struct
{
  MG5_UNIT_BASE_FIELDS;
  VEC Pos;
  mg5PRIM Ball;
} mg5UNIT_BALL;

/* Bounce ball unit initialization function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       mg5UNIT_BALL *Uni;
 *   - animation context:
 *       mg5ANIM *Ani;
 * RETURNS: None.
 */
static VOID MG5_UnitInit( mg5UNIT_BALL *Uni, mg5ANIM *Ani )
{
  //mg5VERTEX V[] = {{{0, 0, 0}}, {{1, 0, 0}}, {{0, 1, 0}}};

  //MG5_RndPrimCreate(&Uni->Ball, V, 3, NULL, 0);
  MG5_RndPrimCreateSphere(&Uni->Ball, VecSet(0, 0, 0), 5, 23, 18);
  Uni->Pos = VecMulNum(VecSet(Rand1(), Rand1(), Rand1()), 100);
} /* End of 'MG5_UnitInit' function */

/* Bounce ball unit inter frame events handle function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       mg5UNIT_BALL *Uni;
 *   - animation context:
 *       mg5ANIM *Ani;
 * RETURNS: None.
 */
static VOID MG5_UnitResponse( mg5UNIT_BALL *Uni, mg5ANIM *Ani )
{
  /*Uni->Pos.X += Ani->DeltaTime * 2.5;*/
} /* End of 'MG5_UnitResponse' function */

/* Bounce ball unit render function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       mg5UNIT_BALL *Uni;
 *   - animation context:
 *       mg5ANIM *Ani;
 * RETURNS: None.
 */
static VOID MG5_UnitRender( mg5UNIT_BALL *Uni, mg5ANIM *Ani )
{
  MG5_RndPrimDraw(&Uni->Ball, 
                  MatrMulMatr(MatrScale(VecSet(1, 1, 1)),
                  MatrTranslate(Uni->Pos)));
  /*MG5_RndPrimDraw(&Uni->Ball, MatrIdentity());*/
} /* End of 'MG5_UnitRender' function */
 
static VOID MG5_UnitClose( mg5UNIT_BALL *Uni, mg5ANIM *Ani )
{
  MG5_RndPrimFree(&Uni->Ball);
}

/* Unit ball creation function.
 * ARGUMENTS: None.
 * RETURNS:
 *   (mg5UNIT *) pointer to created unit.
 */
mg5UNIT * MG5_UnitCreateBall( VOID )
{
  mg5UNIT_BALL *Uni;

  if ((Uni = (mg5UNIT_BALL *)MG5_AnimUnitCreate(sizeof(mg5UNIT_BALL))) == NULL)
    return NULL;

  /* Setup unit methods */
  Uni->Init = (VOID *)MG5_UnitInit;
  Uni->Response = (VOID *)MG5_UnitResponse;
  Uni->Render = (VOID *)MG5_UnitRender;
  Uni->Close = (VOID *)MG5_UnitClose;

  return (mg5UNIT *)Uni;
} /* End of 'MG5_UnitCreateBall' function */

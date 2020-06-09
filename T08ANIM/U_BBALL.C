#include "..\anim\anim.h"
#include <time.h>

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
  MG5_RndPrimCreateTorus(&Uni->Ball, VecSet(0, 0, 0), 6, 20, 30);
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
  Uni->Pos.X += Ani->DeltaTime * 2.5;
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
  MG5_RndPrimDraw(&Uni->Ball, MatrRotateZ( clock() /20.0));
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

  return (mg5UNIT *)Uni;
} /* End of 'MG5_UnitCreateBall' function */

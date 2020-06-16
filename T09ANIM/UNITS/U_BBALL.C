#include "..\units.h"

typedef struct
{
  MG5_UNIT_BASE_FIELDS;
  mg5PRIM Ball;
  mg5PRIMS Model;
} mg5UNIT_BALL;



/* Bounce ball unit deinitialization function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       mg5UNIT_BALL *Uni;
 *   - animation context:
 *       mg5ANIM *Ani;
 * RETURNS: None.
 */
static VOID MG5_UnitClose( mg5UNIT_BALL *Uni, mg5ANIM *Ani )
{
  MG5_RndPrimFree(&Uni->Ball);
  MG5_RndPrimsFree(&Uni->Model);
} /* End of 'MG5_UnitClose' function */

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
  
  mg5MATERIAL mtl = MG5_RndMaterials[0];

  MG5_RndPrimCreateSphere(&Uni->Ball, VecSet1(0), 1000, 1000, 1000, 8 * 18, 8* 18);
  mtl.Tex[0] = MG5_RndTexAdd("sky1.bmp");
  mtl.Ka = VecSet1(0);
  mtl.Kd = VecSet1(0.8);
  mtl.Ks = VecSet1(0.8);
  mtl.Ph = 90;
  mtl.ShdNo = MG5_RndShdAdd("SKY");
  Uni->Ball.MtlNo = MG5_RndMtlAdd(&mtl);

  MG5_RndPrimsLoad(&Uni->Model,"domik.g3dm");
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
  MG5_RndPrimsDraw(&Uni->Model, MatrScale(VecSet(0.1, 0.1, 0.1)));
  MG5_RndPrimDraw(&Uni->Ball, MatrIdentity());
} /* End of 'MG5_UnitRender' function */

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

/* END OF 'U_BALLS.C' FILE */
/* FILE NAME: UNIT.C
 * PROGRAMMER: MG5
 * DATE: 09.06.2020
 * PURPOSE: 3D animation project.
 */

#include  "anim.h"


/* Unit initialization function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       mg5UNIT *Uni;
 *   - animation context:
 *       mg5ANIM *Ani;
 * RETURNS: None.
 */
static VOID MG5_UnitInit( mg5UNIT *Uni, mg5ANIM *Ani )
{
} /* End of 'MG5_UnitInit' function */
/* Unit deinitialization function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       mg5UNIT *Uni;
 *   - animation context:
 *       mg5ANIM *Ani;
 * RETURNS: None.
 */
static VOID MG5_UnitClose( mg5UNIT *Uni, mg5ANIM *Ani )
{
} /* End of 'MG5_UnitClose' function */

/* Unit inter frame events handle function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       mg5UNIT *Uni;
 *   - animation context:
 *       mg5ANIM *Ani;
 * RETURNS: None.
 */
static VOID MG5_UnitResponse( mg5UNIT *Uni, mg5ANIM *Ani )
{
} /* End of 'MG5_UnitResponse' function */

/* Unit render function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       mg5UNIT *Uni;
 *   - animation context:
 *       mg5ANIM *Ani;
 * RETURNS: None.
 */
static VOID MG5_UnitRender( mg5UNIT *Uni, mg5ANIM *Ani )
{
} /* End of 'MG5_UnitRender' function */


/* Unit creation function.
 * ARGUMENTS:
 *   - unit structure size in bytes:
 *       INT Size;
 * RETURNS:
 *   (mg5UNIT *) pointer to created unit.
 */
mg5UNIT * MG5_AnimUnitCreate( INT Size )
{
  mg5UNIT *Uni;

  /* Memory allocation */
  if (Size < sizeof(mg5UNIT) || (Uni = malloc(Size)) == NULL)
    return NULL;
  memset(Uni, 0, Size);

  /* Setup unit methods */
  Uni->Init = MG5_UnitInit;
  Uni->Close = MG5_UnitClose;
  Uni->Response = MG5_UnitResponse;
  Uni->Render = MG5_UnitRender;

  return Uni;
} /* End of 'MG5_AnimUnitCreate' function */

/* End of "UNIT.C" file */
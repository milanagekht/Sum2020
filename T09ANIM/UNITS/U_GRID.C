/* FILE NAME: U_GRID.C
 * PROGRAMMER: MG5
 * DATE: 12.06.2020
 * PURPOSE: WinAPI windowed application sample.
 */

#include "../units.h"
#include "../anim/rnd/res/image.h"


typedef struct tagmg5UNIT_GRID
{
  MG5_UNIT_BASE_FIELDS;
  mg5PRIM Grid;
} mg5UNIT_GRID;

/* Unit deinitialization function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       mg5UNIT_GRID *Uni;
 *   - animation context:
 *       mg5ANIM *Ani;
 * RETURNS: None.
 */
static VOID MG5_UnitClose( mg5UNIT_GRID *Uni, mg5ANIM *Ani )
{
} /* End of 'MG5_UnitClose' function */

/* Bounce ball unit initialization function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       mg5UNIT_GRID *Uni;
 *   - animation context:
 *       mg5ANIM *Ani;
 * RETURNS: None.
 */
static VOID MG5_UnitInit( mg5UNIT_GRID *Uni, mg5ANIM *Ani )
{
  mg5IMAGE img1, img2;
  mg5VERTEX *V;
  INT x, y, b, g, r, a, d;

  if (MG5_ImgLoad(&img1, "hf.bmp") && MG5_ImgLoad(&img2, "hftex.bmp") && (V = malloc(sizeof(mg5VERTEX) * img1.H * img1.W)) != NULL)
  {
    for (y = 0; y < img1.H; y++)
      for (x = 0; x < img1.W; x++)
      {
        b = img2.Pixels[(img2.W * y + x) * 4 + 0];
        g = img2.Pixels[(img2.W * y + x) * 4 + 1];
        r = img2.Pixels[(img2.W * y + x) * 4 + 2];
        a = img2.Pixels[(img2.W * y + x) * 4 + 3];
        
        d = img1.Pixels[(img1.W * y + x) * 4 + 1];

        V[img1.W * y + x].P = VecSet(x - img1.W / 2, d, img1.H / 2 - y);
        V[img1.W * y + x].C = Vec4Set(r / 255.0, g / 255.0, b / 255.0, 1);
      }
    MG5_RndPrimCreateFromGrid(&Uni->Grid, V, img1.W, img1.H, TRUE);
    Uni->Grid.MtlNo = 0;
  }

  MG5_ImgFree(&img1);
  MG5_ImgFree(&img2);
  free(V);
} /* End of 'MG5_UnitInit' function */

/* Bounce ball unit inter frame events handle function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       mg5UNIT_GRID *Uni;
 *   - animation context:
 *       mg5ANIM *Ani;
 * RETURNS: None.
 */
static VOID MG5_UnitResponse( mg5UNIT_GRID *Uni, mg5ANIM *Ani )
{
} /* End of 'MG5_UnitResponse' function */

/* Bounce ball unit render function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       mg5UNIT_GRID *Uni;
 *   - animation context:
 *       mg5ANIM *Ani;
 * RETURNS: None.
 */
static VOID MG5_UnitRender( mg5UNIT_GRID *Uni, mg5ANIM *Ani )
{
  MG5_RndPrimDraw(&Uni->Grid, MatrIdentity());
} /* End of 'MG5_UnitRender' function */

/* Unit Grid creation function.
 * ARGUMENTS: None.
 * RETURNS:
 *   (mg5UNIT *) pointer to created unit.
 */
mg5UNIT * MG5_UnitCreateGrid( VOID )
{
  mg5UNIT_GRID *Uni;

  if ((Uni = (mg5UNIT_GRID *)MG5_AnimUnitCreate(sizeof(mg5UNIT_GRID))) == NULL)
    return NULL;

  /* Setup unit methods */
  Uni->Init = (VOID *)MG5_UnitInit;
  Uni->Response = (VOID *)MG5_UnitResponse;
  Uni->Render = (VOID *)MG5_UnitRender;
  Uni->Close = (VOID *)MG5_UnitClose;
  return (mg5UNIT *)Uni;
} /* End of 'MG5_UnitCreateGrid' function */

/* END OF 'U_GRID.C' FILE */

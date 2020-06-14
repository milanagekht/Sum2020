/* FILE NAME: U_GRID.C
 * PROGRAMMER: MG5
 * DATE: 12.06.2020
 * PURPOSE: WinAPI windowed application sample.
 */

#include "../units.h"

#define GRID_H 102
#define GRID_W 102

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
  static HBITMAP hBm, hBm1;
  static BYTE *Pixels, *Pixels1;
  static BITMAP bm, bm1;
  static mg5VERTEX V[GRID_H][GRID_W];
  INT b, g, r, x, y, b1, g1, r1;

  hBm = LoadImage(NULL, "hftex.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
  hBm1 = LoadImage(NULL, "hf.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
  GetObject(hBm, sizeof(bm), &bm);
  GetObject(hBm1, sizeof(bm1), &bm1);

  Pixels = bm.bmBits;
  Pixels1 = bm1.bmBits;
  for (y = 0; y < GRID_H; y++)
    for (x = 0; x < GRID_W - 2; x++)
    {
      b = Pixels[bm.bmWidthBytes * y + x * (bm.bmBitsPixel / 8) + 0];
      g = Pixels[bm.bmWidthBytes * y + x * (bm.bmBitsPixel / 8) + 1];
      r = Pixels[bm.bmWidthBytes * y + x * (bm.bmBitsPixel / 8) + 2];
      
      b1 = Pixels1[bm1.bmWidthBytes * y + x * (bm1.bmBitsPixel / 8) + 0];
      g1 = Pixels1[bm1.bmWidthBytes * y + x * (bm1.bmBitsPixel / 8) + 1];
      r1 = Pixels1[bm1.bmWidthBytes * y + x * (bm1.bmBitsPixel / 8) + 2];

      V[y][x].P = VecSet(x - GRID_W / 2, (b1 + g1 + r1), GRID_H / 2 - y);
      V[y][x].C = Vec4Set(r / 255.0, g / 255.0, b / 255.0, 1);
    }
  MG5_RndPrimCreateFromGrid(&Uni->Grid, V[0], GRID_W, GRID_H, TRUE);

   /*HBITMAP hBm = LoadImage(NULL, "hftex.bmp", IMAGE_BITMAP, 0, 0,
                          LR_LOADFROMFILE | LR_CREATEDIBSECTION);

  if (hBm != NULL)
  {
    BITMAP bm;
    mg5VERTEX *V;
    INT W, H, BPL;*/ /* width, hight and bytes per line for image */
    /*BYTE *ptr;

    GetObject(hBm, sizeof(bm), &bm);
    W = bm.bmWidth;
    H = bm.bmHeight;
    BPL = bm.bmWidthBytes;
    ptr = bm.bmBits;

    if ((V = malloc(sizeof(mg5VERTEX) * W * H)) != NULL)
    {
      INT x, y;

      memset(V, 0, sizeof(mg5VERTEX) * W * H);
      for (y = 0; y < H; y++)
        for (x = 0; x < W; x++)
        {
          BYTE
            b = ptr[y * BPL + x * 3 + 0], *//* для 8 bit - bm.BitsPixel: [y * BPL + x] для rgb */
            /*g = ptr[y * BPL + x * 3 + 1],
            r = ptr[y * BPL + x * 3 + 2];

          V[y * W + x].P = VecSet(x - W / 2,
                                  47 * g / 255.0,
                                  H / 2 - y);
          V[y * W + x].C = Vec4Set(r / 255.0, g / 255.0, b / 255.0, 1);*/
          /* V[y * W + x].C = Vec4Set(0, 1, 0, 1); */
       /* }
        MG5_RndPrimCreateFromGrid(&Uni->Grid, V, W, H, TRUE);
      free(V);
    }
    DeleteObject(hBm);
  }*/

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

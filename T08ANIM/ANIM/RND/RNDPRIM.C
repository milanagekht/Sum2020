/* FILE NAME: RNDPRIM.C
 * PROGRAMMER: MG5
 * DATE: 06.06.2020
 * PURPOSE: 3D animation project.
 *          Render system primitive handle function
 */

#include <string.h>

#include "rnd.h"

/* Create primitive function.
 * ARGUMENTS:
 *   - primitive:
 *       mg5PRIM *Pr.
 *   - number of vertices:
 *       INT NoofV.
 *   - number of indexes:
 *       INT NoofI.
 * RETURNS:
 *   (BOOL) result.
 */
BOOL MG5_RndPrimCreate( mg5PRIM *Pr, INT NoofV, INT NoofI )
{
  INT size;

  memset(Pr, 0, sizeof(mg5PRIM));
  size = sizeof(mg5VERTEX) * NoofV + sizeof(INT) * NoofI;

  if ((Pr->V = malloc(size)) == NULL)
    return FALSE;
  Pr->I = (INT *)(Pr->V + NoofV);
  Pr->NumOfV = NoofV;
  Pr->NumOfI = NoofI;
  Pr->Trans = MatrIdentity();
  memset(Pr->V, 0, size);

  return TRUE;
} /* End of 'MG5_RndPrimCreate' function */


/* Delete primitive function.
 * ARGUMENTS:
 *   - primitive:
 *       mg5PRIM *Pr.
 * RETURNS: None.
 */
VOID MG5_RndPrimFree( mg5PRIM *Pr )
{
  if (Pr->V != NULL)
    free(Pr->V);
  memset(Pr, 0, sizeof(mg5PRIM));
} /* End of 'MG5_RndPrimFree' function */


/* Draw primitive function.
 * ARGUMENTS:
 *   - primitive:
 *       mg5PRIM *Pr.
 *   - wotld matrix:
 *       MATR Wolrd.
 * RETURNS:
 *   (BOOL) result.
 */
VOID MG5_RndPrimDraw( mg5PRIM *Pr, MATR World )
{
  MATR wvp = MatrMulMatr3(Pr->Trans, World, MG5_RndMatrVP);
  POINT *pnts;
  INT i;

  if ((pnts = malloc(sizeof(POINT) * Pr->NumOfV)) == NULL)
    return;

  /* Projection building*/
  for (i = 0; i < Pr->NumOfV; i++)
  {
    VEC p = VecMulMatr(Pr->V[i].P, wvp);

    pnts[i].x = (INT)((p.X + 1) * MG5_RndFrameW / 2);
    pnts[i].y = (INT)((-p.Y + 1) * MG5_RndFrameH / 2);
  }
  SelectObject(MG5_hRndDCFrame, GetStockObject(DC_PEN));
  SetDCPenColor(MG5_hRndDCFrame, RGB(47, 168, 188));

  /* Triangles */
  for (i = 0; i < Pr->NumOfI; i += 3)
  {
    MoveToEx(MG5_hRndDCFrame, pnts[Pr->I[i]].x, pnts[Pr->I[i]].y, NULL);
    LineTo(MG5_hRndDCFrame, pnts[Pr->I[i + 1]].x, pnts[Pr->I[i + 1]].y);
    LineTo(MG5_hRndDCFrame, pnts[Pr->I[i + 2]].x, pnts[Pr->I[i + 2]].y);
    LineTo(MG5_hRndDCFrame, pnts[Pr->I[i]].x, pnts[Pr->I[i]].y);
  }
  free(pnts);
} /* End of 'MG5_RndPrimDraw' function */


/* Create primitive of sphere function 
 * ARGUMENTS:
 *   - prim struct:
 *       mg5PRIM *Pr.
 *   - centre coordinate:
 *       VEC C.
 *   - radius
 *       DBL R.
 *   - number of segments.
 *       INT SplitW, INT SplitH
 * RETURNS:
 *   (BOOL) result.
 */
BOOL MG5_RndPrimCreateSphere( mg5PRIM *Pr, VEC C, DBL R, INT SplitW, INT SplitH )
{
  DBL theta, phi;
  INT i, j, m, n;

  /* Create sphere premitive */
  if (!MG5_RndPrimCreate(Pr, SplitW * SplitH, (SplitW - 1) * (SplitH - 1) * 4 * 6))
    return FALSE;

  /* Build vertex array */
  for (theta = 0, i = 0, m = 0; i < SplitH; i++, theta += PI / (SplitH - 1))
    for (phi = 0, j = 0; j < SplitW; j++, phi += 2 * PI / (SplitW - 1))
      Pr->V[m++].P = VecSet(C.X + R * sin(phi) * sin(theta),
                            C.Y + R * cos(theta),
                            C.Z + R * cos(phi) * sin(theta));

  /* Build index array */
  for (i = 0, m = 0, n = 0; i < SplitH - 1; i++, m++)
    for (j = 0; j < SplitW - 1; j++, m++)
    {
      /* first triangle */
      Pr->I[n++] = m;
      Pr->I[n++] = m + 1;
      Pr->I[n++] = m + SplitW;

      /* second triangle */
      Pr->I[n++] = m + SplitW;
      Pr->I[n++] = m + 1;
      Pr->I[n++] = m + SplitW + 1;
    }

  return TRUE;
} /* End of 'MG5_RndPrimCreateSphere' function */



/* END OF 'RNDPRIM.C' FILE */
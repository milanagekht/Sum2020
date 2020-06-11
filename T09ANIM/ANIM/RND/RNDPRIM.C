/* FILE NAME: RNDPRIM.C
 * PROGRAMMER: MG5
 * DATE: 06.06.2020
 * PURPOSE: 3D animation project.
 *          Render system primitive handle function
 */

#include <string.h>
#include <stdio.h>

#include "rnd.h"
#include "../anim/anim.h"

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
VOID MG5_RndPrimCreate( mg5PRIM *Pr, mg5VERTEX *V, INT NumOfV, INT *I, INT NumOfI )
{
  memset(Pr, 0, sizeof(mg5PRIM));

  if (V != NULL)
  {
    glGenBuffers(1, &Pr->VBuf);
    glGenVertexArrays(1, &Pr->VA);

    /* Fill vertex array and buffer */
    glBindVertexArray(Pr->VA);
    glBindBuffer(GL_ARRAY_BUFFER, Pr->VBuf);
    glBufferData(GL_ARRAY_BUFFER, sizeof(mg5VERTEX) * NumOfV, V, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, FALSE, sizeof(mg5VERTEX),
                          (VOID *)0); /* position */
    glVertexAttribPointer(1, 2, GL_FLOAT, FALSE, sizeof(mg5VERTEX),
                          (VOID *)sizeof(VEC)); /* texture coordinates */
    glVertexAttribPointer(2, 3, GL_FLOAT, FALSE, sizeof(mg5VERTEX),
                          (VOID *)(sizeof(VEC) + sizeof(VEC2))); /* normale */
    glVertexAttribPointer(3, 4, GL_FLOAT, FALSE, sizeof(mg5VERTEX),
                          (VOID *)(sizeof(VEC) * 2 + sizeof(VEC2))); /* color */

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    glEnableVertexAttribArray(3);
    

    glBindVertexArray(0);
    Pr->NumOfElements = NumOfV;
  }
  if (I != NULL)
  {
    glGenBuffers(1, &Pr->IBuf);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Pr->IBuf);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(INT) * NumOfI, I, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    Pr->NumOfElements = NumOfI;
  }
  Pr->Trans = MatrIdentity();
  
} /* End of 'MG5_RndPrimCreate' function */


/* Delete primitive function.
 * ARGUMENTS:
 *   - primitive:
 *       mg5PRIM *Pr.
 * RETURNS: None.
 */
VOID MG5_RndPrimFree( mg5PRIM *Pr )
{
  if (Pr->VA != 0)
  {
    glBindVertexArray(Pr->VA);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glDeleteBuffers(1, &Pr->VBuf);
    glBindVertexArray(0);
    glDeleteVertexArrays(1, &Pr->VA);
  }
  if (Pr->IBuf != 0)
    glDeleteBuffers(1, &Pr->IBuf);
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
  INT loc;



  /* Send matrix to OpenGL /v.1.0 */
  glLoadMatrixf(wvp.A[0]);

    /*glBegin(GL_LINES);
    glColor3d(1, 0, 0);
    glVertex3d(0, 0, 0);
    glVertex4d(1, 0, 0, 0);
    glColor3d(0, 1, 0);
    glVertex3d(0, 0, 0);
    glVertex4d(0, 1, 0, 0);
    glColor3d(0, 0, 1);
    glVertex3d(0, 0, 0);
    glVertex4d(0, 0, 1, 0);
  glEnd();*/

  glUseProgram(MG5_RndProgId);

  /* Pass render uniforms */
  if ((loc = glGetUniformLocation(MG5_RndProgId, "MatrWVP")) != -1)
    glUniformMatrix4fv(loc, 1, FALSE, wvp.A[0]);
  if ((loc = glGetUniformLocation(MG5_RndProgId, "Time")) != -1)
    glUniform1f(loc, MG5_Anim.Time);
  if ((loc = glGetUniformLocation(MG5_RndProgId, "GlobalTime")) != -1)
    glUniform1f(loc, MG5_Anim.GlobalTime);

  if (Pr->IBuf != 0)
  {
    glBindVertexArray(Pr->VA);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Pr->IBuf);
    glDrawElements(GL_TRIANGLES, Pr->NumOfElements, GL_UNSIGNED_INT, NULL);
    glBindVertexArray(0);
  }
  else
  {
    glBindVertexArray(Pr->VA);
    glDrawArrays(GL_TRIANGLES, 0, Pr->NumOfElements);
    glBindVertexArray(0);
  }
  glUseProgram(0);
} /* End of 'MG5_RndPrimDraw' function */

/* Load primitive from .OBJ file function.
 * ARGUMENTS:
 *   - pointer to primitive to be build:
 *       mg5PRIM *Pr;
 *   - .OBJ file name:
 *       CHAR *FileName;
 * RETURNS:
 *   (BOOL) TRUE is successful, FALSE otherwise.
 */
BOOL MG5_RndPrimLoad( mg5PRIM *Pr, CHAR *FileName )
{
  FILE *F;
  INT nv = 0, nf = 0, size;
  mg5VERTEX *V;
  INT *Ind;
  static CHAR Buf[1000];

  memset(Pr, 0, sizeof(mg5PRIM));

  if ((F = fopen(FileName, "r")) == NULL)
    return FALSE;

  /* Vertex and factes count */
  while (fgets(Buf, sizeof(Buf) - 1, F) != NULL)
    if (Buf[0] == 'v' && Buf[1] == ' ')
      nv++;
    else if (Buf[0] == 'f' && Buf[1] == ' ')
      nf++;

  size = sizeof(mg5VERTEX) * nv + sizeof(INT) * (nf * 3);

  if ((V = malloc(size)) == NULL)
    return FALSE;
  Ind = (INT *)(V + nv);

  /* Load geometry data */
  rewind(F);
  nv = 0;
  nf = 0;
  while (fgets(Buf, sizeof(Buf) - 1, F) != NULL)
    if (Buf[0] == 'v' && Buf[1] == ' ')
    {
      DBL x, y, z;

      sscanf(Buf + 2, "%lf %lf %lf", &x, &y, &z);
      V[nv++].P = VecSet(x, y, z);
    }
    else if (Buf[0] == 'f' && Buf[1] == ' ')
    {
      INT n1, n2, n3;

      sscanf(Buf + 2, "%d/%*d/%*d %d/%*d/%*d %d/%*d/%*d", &n1, &n2, &n3) == 3 ||
        sscanf(Buf + 2, "%d//%*d %d//%*d %d//%*d", &n1, &n2, &n3) == 3 ||
        sscanf(Buf + 2, "%d/%*d %d/%*d %d/%*d", &n1, &n2, &n3) == 3 ||
        sscanf(Buf + 2, "%d %d %d", &n1, &n2, &n3) == 3;
      Ind[nf++] = n1 - 1;
      Ind[nf++] = n2 - 1;
      Ind[nf++] = n3 - 1;
    }
  MG5_RndPrimCreate(Pr, V, nv, Ind, nf);
  fclose(F);
  free(V);
  return TRUE;
} /* End of 'MG5_RndPrimCreateCone' function */


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
  INT i, j, m, n, noofv, noofi, size;
  mg5VERTEX *V;
  INT *Ind;

  memset(Pr, 0, sizeof(mg5PRIM));

  noofv = SplitW * SplitH;
  noofi = (SplitW - 1) * (SplitH - 1) * 2 * 3;
  size = sizeof(mg5VERTEX) * noofv + sizeof(INT) * noofi;

  if ((V = malloc(size)) == NULL)
    return FALSE;
  Ind = (INT *)(V + noofv);

  /* Build vertex array */
  for (theta = 0, i = 0, m = 0; i < SplitH; i++, theta += PI / (SplitH - 1))
    for (phi = 0, j = 0; j < SplitW; j++, phi += 2 * PI / (SplitW - 1))
      V[m++].P = VecSet(C.X + R * sin(phi) * sin(theta),
                        C.Y + R * cos(theta),
                        C.Z + R * cos(phi) * sin(theta));

  /* Build index array */
  for (i = 0, m = 0, n = 0; i < SplitH - 1; i++, m++)
    for (j = 0; j < SplitW - 1; j++, m++)
    {
      /* first triangle */
      Ind[n++] = m;
      Ind[n++] = m + 1;
      Ind[n++] = m + SplitW;

      /* second triangle */
      Ind[n++] = m + SplitW;
      Ind[n++] = m + 1;
      Ind[n++] = m + SplitW + 1;
    }
  MG5_RndPrimCreate(Pr, V, noofv, Ind, noofi);
  free(V);
  return TRUE;
} /* End of 'MG5_RndPrimCreateSphere' function */

/* Create primitive of torus function 
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
BOOL MG5_RndPrimCreateTorus( mg5PRIM *Pr, VEC C, DBL R, INT SplitW, INT SplitH )
{
  DBL theta, phi;
  INT i, j, m, n, noofv, noofi, size, r = 3;
  mg5VERTEX *V;
  INT *Ind;

  memset(Pr, 0, sizeof(mg5PRIM));

  noofv = SplitW * SplitH;
  noofi = (SplitW - 1) * (SplitH - 1) * 2 * 3;
  size = sizeof(mg5VERTEX) * noofv + sizeof(INT) * noofi;

  if ((V = malloc(size)) == NULL)
    return FALSE;
  Ind = (INT *)(V + noofv);

  /* Build vertex array */
  for (theta = 0, i = 0, m = 0; i < SplitH; i++, theta +=  2 * PI / (SplitH - 1))
    for (phi = 0, j = 0; j < SplitW; j++, phi += 2 * PI / (SplitW - 1))
      V[m++].P = VecSet( C.X + (R + r * cos(phi)) * cos(theta),
                             C.Y + (R + r * cos(phi)) * sin (theta),
                             C.Z +r * sin(phi));

  /* Build index array */
  for (i = 0, m = 0, n = 0; i < SplitH - 1; i++, m++)
    for (j = 0; j < SplitW - 1; j++, m++)
    {
      /* first triangle */
      Ind[n++] = m;
      Ind[n++] = m + 1;
      Ind[n++] = m + SplitW;

      /* second triangle */
      Ind[n++] = m + SplitW;
      Ind[n++] = m + 1;
      Ind[n++] = m + SplitW + 1;
    }
  MG5_RndPrimCreate(Pr, V, noofv, Ind, noofi);
  free(V);
  return TRUE;
} /* End of 'MG5_RndPrimCreateTorus' function */



/* END OF 'RNDPRIM.C' FILE */

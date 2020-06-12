/* FILE NAME: RNDPRIM.C
 * PROGRAMMER: MG5
 * DATE: 06.06.2020
 * PURPOSE: 3D animation project.
 *          Render system primitive handle function
 */

#include <stdio.h>
#include <string.h>


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
VOID MG5_RndPrimCreate( mg5PRIM *Pr, mg5VERTEX *V, INT NoofV, INT *I, INT NoofI, mg5PRIM_TYPE Type )
{
  memset(Pr, 0, sizeof(mg5PRIM));
  Pr->Type = Type;

  if (V != NULL)
  {
    glGenBuffers(1, &Pr->VBuf);
    glGenVertexArrays(1, &Pr->VA);

    /* Activate vertex array */
    glBindVertexArray(Pr->VA);

    /* Activate vertex buffer */
    glBindBuffer(GL_ARRAY_BUFFER, Pr->VBuf);

    /* Fill data */
    glBufferData(GL_ARRAY_BUFFER, sizeof(mg5VERTEX) * NoofV, V, GL_STATIC_DRAW);
    
    /* set data order */
    glVertexAttribPointer(0, 3, GL_FLOAT, FALSE, sizeof(mg5VERTEX),
                        (VOID *)0);
    glVertexAttribPointer(1, 2, GL_FLOAT, FALSE, sizeof(mg5VERTEX),
                        (VOID *)(sizeof(VEC)));
    glVertexAttribPointer(2, 3, GL_FLOAT, FALSE, sizeof(mg5VERTEX),
                        (VOID *)(sizeof(VEC) + sizeof(VEC2)));
    glVertexAttribPointer(3, 4, GL_FLOAT, FALSE, sizeof(mg5VERTEX),
                        (VOID *)(sizeof(VEC) * 2 + sizeof(VEC2)));
  
    /* Enable layouts */
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    glEnableVertexAttribArray(3);

    /* Disable vertex array */
    glBindVertexArray(0);
  }

  if (I != 0)
  {
    glGenBuffers(1, &Pr->IBuf);
    /* Activate index buffer */
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Pr->IBuf);

    /* Fill data */
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(INT) * NoofI, I, GL_STATIC_DRAW);
    
    Pr->NumOfElements = NoofI;
  }
  else
    Pr->NumOfElements = NoofV;
  Pr->Trans = MatrIdentity();
} /* End of MG5_RndPrimCreate func */

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
} /* End of MG5_RndPrimFree func */

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
  INT loc, gl_prim_type;

  gl_prim_type = Pr->Type == MG5_RND_PRIM_LINES ? GL_LINES :
             Pr->Type == MG5_RND_PRIM_TRIMESH ? GL_TRIANGLES :
             Pr->Type == MG5_RND_PRIM_TRISTRIP ? GL_TRIANGLE_STRIP :
             GL_POINTS;
  glLoadMatrixf(wvp.A[0]);

  glUseProgram(MG5_RndProgId);

  /* Pass render uniform */
  if ((loc = glGetUniformLocation(MG5_RndProgId, "MatrWVP")) != -1)
    glUniformMatrix4fv(loc, 1, FALSE, wvp.A[0]);
  if ((loc = glGetUniformLocation(MG5_RndProgId, "Time")) != -1)
    glUniform1f(loc, MG5_Anim.Time);
  if ((loc = glGetUniformLocation(MG5_RndProgId, "GlobalTime")) != -1)
    glUniform1f(loc, MG5_Anim.GlobalTime);

  /* Draw all triangles */
  if (Pr->IBuf != 0)
  {
    glBindVertexArray(Pr->VA);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Pr->IBuf);
    glDrawElements(gl_prim_type, Pr->NumOfElements, GL_UNSIGNED_INT, NULL);
    glBindVertexArray(0);
  }
  else
  {
    glBindVertexArray(Pr->VA);
    glDrawArrays(gl_prim_type, 0, Pr->NumOfElements);
    glBindVertexArray(0);
  }
  glUseProgram(0);
} /* End of 'MG5_RndPrimDraw' function */


/*  Create primitive of sphere function.
 * ARGUMENTS:
 *   -Prim structure:
 *       mg5PRIM *Pr;
 *   -Vector (position):
 *       VEC C;
 *   - Radius:
 *       DBL R;
 *   - WIdth and height:
 *       INT SplitW, SplitH;
 * RETURNS:
 *   (BOOL) result.
 */
BOOL MG5_RndPrimCreateSphere( mg5PRIM *Pr, VEC C, DBL R, INT SplitW, INT SplitH )
{
  INT i, j, m, n, noofv, noofi, size;
  DOUBLE theta, phi;
  mg5VERTEX *V;
  INT *Ind;

  memset(Pr, 0, sizeof(mg5PRIM));

  noofv = SplitW * SplitH;
  noofi = (SplitW - 1) * 2 * (SplitH - 1) * 3; 
  size = sizeof(mg5VERTEX) * noofv + sizeof(INT) * noofi;

  if ((V = malloc(size)) == NULL)
    return FALSE;
  Ind = (INT *)(V + noofv);

  for (i = 0, theta = 0, m = 0; i < SplitH; i++, theta += PI / (SplitH - 1))
    for (j = 0, phi = 0; j < SplitW; j++, phi += 2 * PI / (SplitW - 1))
    {

       FLT
        x = cos(phi) * sin(theta),
        y = cos(theta),
        z = sin(phi) * sin(theta);

       V[m].P = (VecSet(C.X + R * x,
                        C.Y + R * y,
                        C.Z + R * z));
       V[m].C = Vec4Set(0.9, 0.13, 0.47, 1);
       V[m].N = VecSet(x, y, z);
       m++;
    }

  for (i = 0, m = 0, n = 0; i < SplitH - 1; i++, m++)
    for (j = 0; j < SplitW - 1; j++, m++)
    {
      Ind[n++] = m;
      Ind[n++] = m + SplitW;
      Ind[n++] = m + 1;

      Ind[n++] = m + SplitW;
      Ind[n++] = m + SplitW + 1;
      Ind[n++] = m + 1;
    }
  MG5_RndPrimCreate(Pr, V, noofv, Ind, noofi, MG5_RND_PRIM_TRIMESH);
  free(V);
  return TRUE;
} /* End of 'MG5_RndPrimCreateSphere' func */


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
  INT i, j, m, n, noofv, noofi, size, r = 3;
  DOUBLE theta, phi;
  mg5VERTEX *V;
  INT *Ind;

  memset(Pr, 0, sizeof(mg5PRIM));

  noofv = SplitW * SplitH;
  noofi = (SplitW - 1) * 2 * (SplitH - 1) * 3; 
  size = sizeof(mg5VERTEX) * noofv + sizeof(INT) * noofi;

  if ((V = malloc(size)) == NULL)
    return FALSE;
  Ind = (INT *)(V + noofv);

  for (i = 0, theta = 0, m = 0; i < SplitH; i++, theta += PI / (SplitH - 1))
    for (j = 0, phi = 0; j < SplitW; j++, phi += 2 * PI / (SplitW - 1))
    {

       FLT
        x = (R + r * cos(phi)) * cos(theta),
        y = (R + r * cos(phi)) * sin (theta),
        z = r * sin(phi);

       V[m].P = (VecSet(C.X + x,
                        C.Y + y,
                        C.Z + z));
       V[m].C = Vec4Set(0.8, 0.3, 0.47, 1);
       V[m].N = VecSet(x, y, z);
       m++;
    }

  for (i = 0, m = 0, n = 0; i < SplitH - 1; i++, m++)
    for (j = 0; j < SplitW - 1; j++, m++)
    {
      Ind[n++] = m;
      Ind[n++] = m + SplitW;
      Ind[n++] = m + 1;

      Ind[n++] = m + SplitW;
      Ind[n++] = m + SplitW + 1;
      Ind[n++] = m + 1;
    }
  MG5_RndPrimCreate(Pr, V, noofv, Ind, noofi, MG5_RND_PRIM_TRIMESH);
  free(V);
  return TRUE;
} /* End of 'MG5_RndPrimCreateTorus' function */


/* Load primitive from .OBJ file function.
 * ARGUMENTS:
 *   -Prim struct:
 *       mg5PRIM *Pr;
 *   -File name:
 *       CHAR *FileName;
 * RETURNS:
 *   (BOOL) result.
 */
BOOL MG5_RndPrimLoad( mg5PRIM *Pr, CHAR *FileName )
{
  FILE *F;
  static CHAR Buf[1000];
  mg5VERTEX *V;
  INT *Ind;
  INT nv = 0, nf = 0, size, i;

  memset(Pr, 0, sizeof(mg5PRIM));

  if ((F = fopen(FileName, "r")) == NULL)
    return FALSE;
  
  /* Vertex and facet count */
  while (fgets(Buf, sizeof(Buf), F) != NULL)
    if (Buf[0] == 'v' && Buf[1] == ' ')
      nv++;
    else if (Buf[0] == 'f' && Buf[1] == ' ')
      nf++;
  
  size = sizeof(mg5VERTEX) * nv + sizeof(INT) * (nf * 3);

  if ((V = malloc(size)) == NULL)
  {
    fclose(F);
    return FALSE;
  }

  memset(V, 0, size);
  Ind = (INT *)(V + nv);

  /* Load geometry data */
  rewind(F);
  nv = nf = 0;
  while (fgets(Buf, sizeof(Buf), F) != NULL)
    if (Buf[0] == 'v' && Buf[1] == ' ')
    {
      DBL x, y, z;
      sscanf(Buf + 2, "%lf %lf %lf", &x, &y, &z);
      V[nv++].P = VecSet(x, y, z);
    }
    else if (Buf[0] == 'f' && Buf[1] == ' ')
    {
      INT n1, n2, n3;

      sscanf(Buf + 2, "%d %d %d", &n1, &n2, &n3) == 3 ||
      sscanf(Buf + 2, "%d/%*d %d/%*d %d/%*d", &n1, &n2, &n3) == 3 ||
      sscanf(Buf + 2, "%d//%*d %d//%*d %d//%*d", &n1, &n2, &n3) == 3 ||
      sscanf(Buf + 2, "%d/%*d/%*d %d/%*d/%*d %d/%*d/%*d", &n1, &n2, &n3) == 3;

      Ind[nf++] = n1 - 1;
      Ind[nf++] = n2 - 1;
      Ind[nf++] = n3 - 1;
    }

  for (i = 0; i < nv; i++)
    V[i].N = VecSet(0, 0, 0);

  for (i = 0; i < nf; i += 3)
  {
    VEC
      p0 = V[Ind[i]].P,
      p1 = V[Ind[i + 1]].P,
      p2 = V[Ind[i + 2]].P,
      N = VecNormalize(VecCrossVec(VecSubVec(p1, p2), VecSubVec(p2, p0)));

      V[Ind[i]].N = VecAddVec(V[Ind[i]].N, N);
      V[Ind[i + 1]].N = VecAddVec(V[Ind[i + 1]].N, N);
      V[Ind[i + 2]].N = VecAddVec(V[Ind[i + 2]].N, N);
  }
  for (i = 0; i < nv; i++)
  { 
    V[i].N = VecNormalize(V[i].N);
    V[i].C = Vec4Set(0.8, 0.47, 0.28, 1);
  }

  fclose(F);
  MG5_RndPrimCreate(Pr, V, nv, Ind, nf, MG5_RND_PRIM_TRIMESH);
  free(V);
  return TRUE;
} /* End of 'MG5_RndPrimLoad' func */

/* Prim creation from vertex grid function 
 * ARGUMENTS:
 *    -pointer to prim:
 *       mg5PRIM *Pr;
 *    -pointer to vertex array:
 *       mg5VERTEX *V;
 *    -weight and heidht:
 *       INT W, H;
 *    - normal evalation flag:
 *       BOOL IsNormalsNeed;
 * RETURNS: BOOL;
 */
BOOL MG5_RndPrimCreateFromGrid( mg5PRIM *Pr, mg5VERTEX *V, INT W, INT H, BOOL IsNormalsNeed )
{
  INT *Ind;
  INT i, j, k;

  memset(Pr, 0, sizeof(mg5PRIM));
  if ((Ind = malloc(((2 * W + 1) * (H - 1) - 1) * sizeof(INT))) == NULL)
    return FALSE;

  for (i = 0, k = 0; i < H; i++)
  {
    for (j = 0; j < W; j++)
    {
      Ind[k++] = (i + 1) * W + j;
      Ind[k++] = i * W + j;
    }
    if (i != H - 2)
      Ind[k++] = -1;
  }

  if (IsNormalsNeed)
  {
    for (i = 0; i < W * H; i++)
      V[i].N = VecSet(0, 0, 0);

    for (i = 0; i < H - 1; i++)
      for (j = 0; j < W - 1; j++)
      {
        mg5VERTEX *P00 = V + i * W + j,
          *P01 = V + i * W + j + 1,
          *P10 = V + (i + 1) * W + j,
          *P11 = V + (i + 1) * W + j + 1;
        VEC N;

        N = VecNormalize(VecCrossVec(VecSubVec(P00->P, P10->P),
                                     VecSubVec(P11->P, P10->P)));
        P00->N = VecAddVec(P00->N, N);
        P10->N = VecAddVec(P10->N, N);
        P11->N = VecAddVec(P11->N, N);

        N = VecNormalize(VecCrossVec(VecSubVec(P11->P, P01->P),
                                     VecSubVec(P00->P, P01->P)));
        P00->N = VecAddVec(P00->N, N);
        P01->N = VecAddVec(P01->N, N);
        P11->N = VecAddVec(P11->N, N);
       }
    for (i = 0; i < W * H; i++)
       V[i].N = VecNormalize(V[i].N);
  }
  MG5_RndPrimCreate(Pr, V, W * H, Ind, (2 * W + 1) * (H - 1) - 1,MG5_RND_PRIM_TRISTRIP);
  free(Ind);
  return TRUE;
} /* End of 'MG5_RndPrimCreateFromGrid' */

/* END OF 'RNDPRIM.C' FILE */
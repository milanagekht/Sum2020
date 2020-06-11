 /* FILE NAME: MTH.H
 * PROGRAMMER: MG5
 * DATE: 05.06.2020
 * PURPOSE: 3D math implementation module.
 */

#ifndef __MTH_H_
#define __MTH_H_

#pragma warning(disable: 4244 4305)

#include <math.h>
#include <windows.h>

/* Pi math constant */
#define PI 3.14159265358979323846

/* Degrees to radians conversion */
#define D2R(A) ((A) * (PI / 180.0))
#define Degree2Radian(a) D2R(a)

/* Helpful macros */
#define MatrMulMatr2(A, B) MatrMulMatr(A, B)
#define MatrMulMatr3(A, B, C) MatrMulMatr(MatrMulMatr(A, B), C)
#define MatrMulMatr4(A, B, C, D) MatrMulMatr(MatrMulMatr(A, B), MatrMulMatr(C, D))

/* Base float point types */
typedef double DBL;
typedef float FLT;

/* Space vector/point representation type */
typedef struct tagVEC
{
  FLT X, Y, Z; /* Vector coordinates */
} VEC;

typedef struct tagVEC2
{
  FLT X, Y; /* Vector coordinates */
} VEC2;

typedef struct tagVEC4
{
  FLT X, Y, Z, W; /* Vector coordinates */
} VEC4;
/* Matrix representation type */
 typedef struct tagMATR
{
  FLT A[4][4];
} MATR;

__inline DBL Rand0( VOID )
{ 
  return rand() / (DBL)RAND_MAX;
}

__inline DBL Rand1( VOID )
{ 
  return 2.0 * rand() / (DBL)RAND_MAX - 1;
}


__inline VEC VecSet(FLT X, FLT Y, FLT Z)
{
  VEC v = { X, Y, Z };

  return v;
}

__inline VEC4 Vec4Set(FLT X, FLT Y, FLT Z, FLT W)
{
  VEC4 v = { X, Y, Z, W };

  return v;
}


__inline VEC VecAddVec(VEC V1, VEC V2)
{
  return VecSet(V1.X + V2.X, V1.Y + V2.Y, V1.Z + V2.Z);
}


__inline VEC VecSubVec(VEC V1, VEC V2)
{
  return VecSet(V1.X - V2.X, V1.Y - V2.Y, V1.Z - V2.Z);
}


__inline VEC VecMulNum(VEC V1, FLT N)
{
  return VecSet(N * V1.X, N * V1.Y, N * V1.Z);
}

__inline VEC VecDivNum(VEC V1, FLT N)
{
  return VecSet(V1.X / N, V1.Y / N, V1.Z / N);
}

__inline VEC VecNeg(VEC V)
{
  return VecSet(-V.X, -V.Y, -V.Z);
}

__inline FLT VecDotVec(VEC V1, VEC V2)
{
  return V1.X * V2.X + V1.Y * V2.Y + V1.Z * V2.Z;
}

__inline VEC VecCrossVec(VEC V1, VEC V2)
{
  return VecSet(V1.Y * V2.Z - V1.Z * V2.Y, V1.Z * V2.X - V1.X * V2.Z, V1.X * V2.Y - V1.Y * V2.X);
}


__inline FLT VecLen2(VEC V)
{
  return (V.X * V.X + V.Y * V.Y + V.Z * V.Z);
}

__inline FLT VecLen(VEC V)
{
 return sqrt(V.X * V.X + V.Y * V.Y + V.Z * V.Z);
}

__inline VEC VecNormalize(VEC V)
{
  FLT len = VecDotVec(V, V);

  if (len != 1 && len != 0)
      len = sqrt(len), V.X /= len, V.Y /= len, V.Z /= len;
  return V;
} 


__inline VEC VecMulMatr(VEC V, MATR M)
{
  FLT w = V.X * M.A[0][3] + V.Y * M.A[1][3] + V.Z * M.A[2][3] + M.A[3][3];

  return VecSet((V.X * M.A[0][0] + V.Y * M.A[1][0] + V.Z * M.A[2][0] + M.A[3][0]) / w,
               (V.X * M.A[0][1] + V.Y * M.A[1][1] + V.Z * M.A[2][1] + M.A[3][1]) / w,
               (V.X * M.A[0][2] + V.Y * M.A[1][2] + V.Z * M.A[2][2] + +M.A[3][2]) / w);
}

__inline VEC PointTransform(VEC V, MATR M)
{
  return VecSet(V.X * M.A[0][0] + V.Y * M.A[1][0] + V.Z * M.A[2][0] + M.A[3][0],
                V.X * M.A[0][1] + V.Y * M.A[1][1] + V.Z * M.A[2][1] + M.A[3][1], 
                V.X * M.A[0][2] + V.Y * M.A[1][2] + V.Z * M.A[2][2] + +M.A[3][2]);
}

__inline VEC VectorTransform(VEC V, MATR M)
{
  return VecSet(V.X * M.A[0][0] + V.Y * M.A[1][0] + V.Z * M.A[2][0],
         V.X * M.A[0][1] + V.Y * M.A[1][1] + V.Z * M.A[2][1],
         V.X * M.A[0][2] + V.Y * M.A[1][2] + V.Z * M.A[2][2]);
}



static MATR UnitMatrix =
{
  {
    {1, 0, 0, 0},
    {0, 1, 0, 0},
    {0, 0, 1, 0},
    {0, 0, 0, 1}
  }
};

__inline MATR SetMatr(FLT A00, FLT A01, FLT A02, FLT A03,
                      FLT A10, FLT A11, FLT A12, FLT A13,
                      FLT A20, FLT A21, FLT A22, FLT A23,
                      FLT A30, FLT A31, FLT A32, FLT A33)
{
  MATR m;

  m.A[0][0] = A00;
  m.A[0][1] = A01;
  m.A[0][2] = A02;
  m.A[0][3] = A03;
  m.A[1][0] = A10;
  m.A[1][1] = A11;
  m.A[1][2] = A12;
  m.A[1][3] = A13;
  m.A[2][0] = A20;
  m.A[2][1] = A21;
  m.A[2][2] = A22;
  m.A[2][3] = A23;
  m.A[3][0] = A30;
  m.A[3][1] = A31;
  m.A[3][2] = A32;
  m.A[3][3] = A33;

  return m;
}


__inline MATR MatrIdentity(VOID)
{
  return UnitMatrix;
}

__inline MATR MatrTranslate(VEC T)
{
  MATR m = UnitMatrix;

  m.A[3][0] = T.X;
  m.A[3][1] = T.Y;
  m.A[3][2] = T.Z;

  return m;
}


__inline MATR MatrRotate( FLT AngleInDegree, VEC V )
{
  FLT
    a = D2R(AngleInDegree),
    c = cos(a),
    s = sin(a);
  VEC
    A = VecNormalize(V);
  MATR m =
  {
    {
      {c + A.X * A.X * (1 - c),
        A.X * A.Y * (1 - c) + A.Z * s,
        A.X * A.Z * (1 - c) - A.Y * s, 0},
      {A.Y * A.X * (1 - c) - A.Z *s,
         c + A.Y * A.Y * (1 - c),
        A.Y * A.Z * (1 - c) + A.X * s, 0},
      { A.Z * A.X * (1 - c) + A.Y *s,
          A.Z * A.Y * (1 - c) - V.X * s,
         c + A.Z * A.Z * (1 - c), 0},
      {0, 0, 0, 1}
    }
  };

  return m;
}



__inline MATR MatrScale(VEC S)
{
  return SetMatr(S.X, 0, 0, 0, 0, S.Y, 0, 0, 0, 0, S.Z, 0, 0, 0, 0, 1);
}

__inline MATR MatrRotateX(FLT AngleInDegree)
{
  return SetMatr(1, 0, 0, 0,
                 0, cos(D2R(AngleInDegree)), sin(D2R(AngleInDegree)), 0,
                 0, -sin(D2R(AngleInDegree)), cos(D2R(AngleInDegree)), 0,
                 0, 0, 0, 1);
}

__inline MATR MatrRotateY(FLT AngleInDegree)
{
  return SetMatr(cos(D2R(AngleInDegree)), 0, -sin(D2R(AngleInDegree)), 0,
                 0, 1, 0, 0,
                 sin(D2R(AngleInDegree)), 0, cos(D2R(AngleInDegree)), 0,
                 0, 0, 0, 1);
}
__inline MATR MatrRotateZ(FLT AngleInDegree)
{
  return SetMatr(cos(D2R(AngleInDegree)), sin(D2R(AngleInDegree)), 0, 0,
                -sin(D2R(AngleInDegree)), cos(D2R(AngleInDegree)), 0, 0,
                 0, 0, 1, 0,
                 0, 0, 0, 1);
}

__inline MATR MatrMulMatr(MATR M1, MATR M2)
{
  INT i, j, k;
  MATR r;

  for (i = 0; i < 4; i++)
    for (j = 0; j < 4; j++)
          for (r.A[i][j] = 0, k = 0; k < 4; k++)
              r.A[i][j] += M1.A[i][k] * M2.A[k][j];

  return r;
}

__inline MATR MatrTranspose(MATR M)
{
  MATR M1;

  M1.A[0][0] = M.A[0][0];
  M1.A[0][1] = M.A[1][0];
  M1.A[0][2] = M.A[2][0];
  M1.A[0][3] = M.A[3][0];
  M1.A[1][0] = M.A[0][1];
  M1.A[1][1] = M.A[1][1];
  M1.A[1][2] = M.A[2][1];
  M1.A[1][3] = M.A[3][1];
  M1.A[2][0] = M.A[0][2];
  M1.A[2][1] = M.A[1][2];
  M1.A[2][2] = M.A[2][2];
  M1.A[2][3] = M.A[3][2];
  M1.A[3][0] = M.A[0][3];
  M1.A[3][1] = M.A[1][3];
  M1.A[3][2] = M.A[2][3];
  M1.A[3][3] = M.A[3][3];

  return M1;
}

__inline FLT MatrDeterm3x3(FLT A11, FLT A12, FLT A13,
                           FLT A21, FLT A22, FLT A23,
                           FLT A31, FLT A32, FLT A33)
{
  return A11 * A22 * A33 - A11 * A23 * A32 - A12 * A21 * A33 +
         A12 * A23 * A31 + A13 * A21 * A32 - A13 * A22 * A31;
}

__inline FLT MatrDeterm(MATR M)
{
  return
      M.A[0][0] * MatrDeterm3x3(M.A[1][1], M.A[1][2], M.A[1][3],
                                M.A[2][1], M.A[2][2], M.A[2][3],
                                M.A[3][1], M.A[3][2], M.A[3][3]) +
     -M.A[0][1] * MatrDeterm3x3(M.A[1][0], M.A[1][2], M.A[1][3],
                                M.A[2][0], M.A[2][2], M.A[2][3],
                                M.A[3][0], M.A[3][2], M.A[3][3]) +
      M.A[0][2] * MatrDeterm3x3(M.A[1][0], M.A[1][1], M.A[1][3],
                                M.A[2][0], M.A[2][1], M.A[2][3],
                                M.A[3][0], M.A[3][1], M.A[3][3]) +
     -M.A[0][3] * MatrDeterm3x3(M.A[1][0], M.A[1][1], M.A[1][2],
                                M.A[2][0], M.A[2][1], M.A[2][2],
                                M.A[3][0], M.A[3][1], M.A[3][2]);
}


__inline MATR MatrInverse( MATR M )
{
  MATR r;
  FLT det = MatrDeterm(M);

  if (det == 0)
    return UnitMatrix;

  r.A[0][0] =
    MatrDeterm3x3(M.A[1][1], M.A[1][2], M.A[1][3],
                  M.A[2][1], M.A[2][2], M.A[2][3],
                  M.A[3][1], M.A[3][2], M.A[3][3]);
  r.A[1][0] =
    -MatrDeterm3x3(M.A[1][0], M.A[1][2], M.A[1][3],
                   M.A[2][0], M.A[2][2], M.A[2][3],
                   M.A[3][0], M.A[3][2], M.A[3][3]);
  r.A[2][0] =
    MatrDeterm3x3(M.A[1][0], M.A[1][1], M.A[1][3],
                  M.A[2][0], M.A[2][1], M.A[2][3],
                  M.A[3][0], M.A[3][1], M.A[3][3]);
  r.A[3][0] =
    -MatrDeterm3x3(M.A[1][0], M.A[1][1], M.A[1][2],
                   M.A[2][0], M.A[2][1], M.A[2][2],
                   M.A[3][0], M.A[3][1], M.A[3][2]);

  r.A[0][1] =
    MatrDeterm3x3(M.A[0][1], M.A[0][2], M.A[0][3],
                  M.A[2][1], M.A[2][2], M.A[2][3],
                  M.A[3][1], M.A[3][2], M.A[3][3]);
  r.A[1][1] =
    -MatrDeterm3x3(M.A[0][0], M.A[0][2], M.A[0][3],
                   M.A[2][0], M.A[2][2], M.A[2][3],
                   M.A[3][0], M.A[3][2], M.A[3][3]);
  r.A[2][1] =
    MatrDeterm3x3(M.A[0][0], M.A[0][1], M.A[0][3],
                  M.A[2][0], M.A[2][1], M.A[2][3],
                  M.A[3][0], M.A[3][1], M.A[3][3]);
  r.A[3][1] =
    -MatrDeterm3x3(M.A[0][0], M.A[0][1], M.A[0][2],
                   M.A[2][0], M.A[2][1], M.A[2][2],
                   M.A[3][0], M.A[3][1], M.A[3][2]);


  r.A[0][2] =
    MatrDeterm3x3(M.A[0][1], M.A[0][2], M.A[0][3],
                  M.A[1][1], M.A[1][2], M.A[1][3],
                  M.A[3][1], M.A[3][2], M.A[3][3]);
  r.A[1][2] =
    -MatrDeterm3x3(M.A[0][0], M.A[0][2], M.A[0][3],
                   M.A[1][0], M.A[1][2], M.A[1][3],
                   M.A[3][0], M.A[3][2], M.A[3][3]);
  r.A[2][2] =
    MatrDeterm3x3(M.A[0][0], M.A[0][1], M.A[0][3],
                  M.A[1][0], M.A[1][1], M.A[1][3],
                  M.A[3][0], M.A[3][1], M.A[3][3]);
  r.A[3][2] =
    -MatrDeterm3x3(M.A[0][0], M.A[0][1], M.A[0][2],
                   M.A[1][0], M.A[1][1], M.A[1][2],
                   M.A[3][0], M.A[3][1], M.A[3][2]);

  r.A[0][3] =
    MatrDeterm3x3(M.A[0][1], M.A[0][2], M.A[0][3],
                  M.A[1][1], M.A[1][2], M.A[1][3],
                  M.A[2][1], M.A[2][2], M.A[2][3]);
  r.A[1][3] =
    -MatrDeterm3x3(M.A[0][0], M.A[0][2], M.A[0][3],
                   M.A[1][0], M.A[1][2], M.A[1][3],
                   M.A[2][0], M.A[2][2], M.A[2][3]);
  r.A[2][3] =
    MatrDeterm3x3(M.A[0][0], M.A[0][1], M.A[0][3],
                  M.A[1][0], M.A[1][1], M.A[1][3],
                  M.A[2][0], M.A[2][1], M.A[2][3]);
  r.A[3][3] =
    -MatrDeterm3x3(M.A[0][0], M.A[0][1], M.A[0][2],
                   M.A[1][0], M.A[1][1], M.A[1][2],
                   M.A[2][0], M.A[2][1], M.A[2][2]);

  r.A[1][0] /= det;
  r.A[2][0] /= det;
  r.A[3][0] /= det;
  r.A[1][1] /= det;
  r.A[2][1] /= det;
  r.A[3][1] /= det;
  r.A[1][2] /= det;
  r.A[2][2] /= det;
  r.A[3][2] /= det;
  r.A[1][3] /= det;
  r.A[2][3] /= det;
  r.A[3][3] /= det;
  r.A[0][0] /= det;
  r.A[0][1] /= det;
  r.A[0][2] /= det;
  r.A[0][3] /= det;

  return r;
}

__inline MATR MatrView(VEC Loc, VEC At, VEC Up1)
{
  VEC
      Dir = VecNormalize(VecSubVec(At, Loc)),
      Right = VecNormalize(VecCrossVec(Dir, Up1)),
      Up = VecNormalize(VecCrossVec(Right, Dir));
  MATR m =
  {
    {
      {Right.X, Up.X, -Dir.X, 0},
      {Right.Y, Up.Y, -Dir.Y, 0},
      {Right.Z, Up.Z, -Dir.Z, 0},
      {-VecDotVec(Loc, Right), -VecDotVec(Loc, Up), VecDotVec(Loc, Dir), 1}
    }
  };
  return m;
}

__inline MATR MatrOrtho(FLT l, FLT r, FLT b, FLT t, FLT n, FLT f)
{
  MATR m =
  {
    {
      {2 / (r - l), 0, 0, 0},
      {0, 2 / (t - b), 0, 0},
      {0, 0, ( - 2 ) / (f - n), 0},
      {( - (r + l) )/ (r - l),( - (t + b) )/ (t - b), ( - (f + n) )/ (f - n),  1}
     }
  };

  return m;
}

__inline MATR MatrFrustum(FLT l, FLT r, FLT b, FLT t, FLT n, FLT f)
{
  MATR m =
  {
    {
      {2 * n / (r - l), 0, 0, 0},
      {0, 2 * n / (t - b), 0, 0},
      {(r + l) / (r - l), (t + b) / (t - b), -(f + n) / (f - n), -1},
      {0, 0, -2 * n * f / (f - n),  0}
     }
  };

  return m;
}






#endif /*__MTH_H_ */

/* END OF 'MATH.H' FILE */

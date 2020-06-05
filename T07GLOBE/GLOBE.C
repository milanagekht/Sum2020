/* FILE NAME: GLOBE.C
 * PROGRAMMER: MG5
 * DATE: 04.06.2020 
 * PURPOSE: NONE
 */

#include <math.h>
#include <time.h>
#include <stdio.h>

#include <windows.h>

#include "GLOBE.H"
#include "TIMER.H"


/* Geometry data arfray */
static VEC Geom[N][M];

/* Store coordinate system center */
static INT CenterX, CenterY;

VEC VecRotateZ( VEC V, DBL Angle )
{
  VEC r;

  Angle *= PI / 180;
  r.X = V.X * cos(Angle) - V.Y * sin(Angle);
  r.Y = V.X * sin(Angle) + V.Y * cos(Angle);
  r.Z = V.Z;
  return r;
}

VEC VecRotateY( VEC V, DBL Angle )
{
  VEC r;

  Angle *= PI / 180;
  r.Z = V.Z * cos(Angle) - V.X * sin(Angle);
  r.X = V.Z * sin(Angle) + V.X * cos(Angle);
  r.Y = V.Y;
  return r;
}

VEC VecRotateX( VEC V, DBL Angle )
{
  VEC r;

  Angle *= PI / 180;
  r.Y = V.Y * cos(Angle) - V.Z * sin(Angle);
  r.Z = V.Y * sin(Angle) - V.Z * cos(Angle);
  r.X = V.X;
  return r;
}



VOID GlobeSet( DBL Xc, DBL Yc, DBL R )
{
  INT i, j;
  DBL theta;
  DBL phi;
   
  CenterX = (INT)Xc;
  CenterY = (INT)Yc;
  for (i = 0, theta = 0; i < N; i++, theta += PI / (N - 1))
    for (j = 0, phi = 0; j < M; j++, phi += 2 * PI / (M - 1))
    {
      Geom[i][j].X = R * sin(theta) * sin(phi);
      Geom[i][j].Y = R * cos(theta);
      Geom[i][j].Z = R * sin(theta) * cos(phi);
    }
} 



VOID DrawGlobe( HDC hDC )
{
  int i, j, k, s = 3;
  DBL t = clock() / CLOCKS_PER_SEC;
  static POINT pnts[N][M];
  static DBL z[N][M];
  CHAR Buf[102];
  MATR m;

  SelectObject(hDC, GetStockObject(NULL_PEN));
  SelectObject(hDC, GetStockObject(DC_BRUSH));
  SetDCBrushColor(hDC, RGB(55, 55, 55));

  m = MatrMulMatr3(MatrRotateY(GLB_Time * 8), 
                   MatrRotateX(GLB_Time * 8), 
                   MatrRotate(sin(GLB_Time) * 30, VecSet(1, 1, 1)));
  /*m = MatrView(VecSet(GLB_Time * 0.26, GLB_Time * 2,5), 
               VecSet(0, 2, 0), 
               VecSet(0, 1, 0));*/
  for(i = 0; i < N; i++)
    for (j = 0; j < M; j++)
    {
      VEC 
        /*v1 = VecRotateZ(Geom[i][j], 30 * t),*/
        v = PointTransform(Geom[i][j], m) ;

      z[i][j] = v.Z;
      pnts[i][j].x = CenterX + (INT)v.X,
      pnts[i][j].y = CenterY - (INT)v.Y;
    }
    /* Drawing all points */
  /*SelectObject(hDC, GetStockObject(NULL_PEN));
  SelectObject(hDC, GetStockObject(DC_BRUSH));
  SetDCBrushColor(hDC, RGB(55, 55, 55));
      for(i = 0; i < N; i++)
        for (j = 0; j < M; j++)
          if (z[i][j] > 0)
            Ellipse(hDC, pnts[i][j].x - s, pnts[i][j].y - s, pnts[i][j].x + s, pnts[i][j].y + s);
      */
    /* Drawing horisontal lines */
  /*SelectObject(hDC, GetStockObject(BLACK_PEN));
  SelectObject(hDC, GetStockObject(DC_BRUSH));
  SetDCBrushColor(hDC, RGB(55, 55, 55));
  for (i = 0; i < N; i++)
    {
      MoveToEx(hDC, pnts[i][0].x, pnts[i][0].y, NULL);
      for (j = 1; j < M; j++) 
        LineTo(hDC, pnts[i][j].x, pnts[i][j].y);
    } */
      /* Drawing vertical lines */
    /*for (j = 0; j < M; j++) 
    {
      MoveToEx(hDC, pnts[0][j].x, pnts[0][j].y, NULL);
      for (i = 0; i < N; i++)
        LineTo(hDC, pnts[i][j].x, pnts[i][j].y);
    }*/
    SelectObject(hDC, GetStockObject(DC_PEN));
    SelectObject(hDC, GetStockObject(DC_BRUSH));
    /*SetDCPenColor(hDC, RGB(46, 147, 152));*/
    SetDCPenColor(hDC, RGB(0, 0, 0));
    /*srand(102);*/
    for (k = 0; k < 2; k++)
      for(i = 0; i < N - 1; i++)
        for (j = 0; j < M - 1; j++)
        { 
          POINT p[4];
          INT sign;

          p[0] = pnts[i][j];
          p[1] = pnts[i][j + 1];
          p[2] = pnts[i + 1][j + 1];
          p[3] = pnts[i + 1][j];

          /*SetDCBrushColor(hDC, RGB(0, 0, 0));*/ 
          /*SetDCBrushColor(hDC, RGB(rand() % 256, rand() % 256, rand() % 256)); */
          SetDCBrushColor(hDC, RGB( (i * 46 / N), (i * 147 / N), (i * 152 / N)));
          sign =
            (p[0].x - p[1].x) * (p[0].y + p[1].y) +
            (p[1].x - p[2].x) * (p[1].y + p[2].y) +
            (p[2].x - p[3].x) * (p[2].y + p[3].y) +
            (p[3].x - p[0].x) * (p[3].y + p[0].y);
          if (k == 0 && sign <= 0 || k == 1 && sign > 0)
          Polygon(hDC, p, 4);
         }

    SetTextColor(hDC, RGB(2, 5, 55));
    TextOut(hDC, 8, 8, Buf, sprintf(Buf, "Frames Per Sec: %.2f", GLB_FPS));


} /* End of Draw function */ 
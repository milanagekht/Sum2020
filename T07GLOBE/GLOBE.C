/* FILE NAME: GLOBE.C
 * PROGRAMMER: MG5
 * DATE: 04.06.2020 
 * PURPOSE: NONE
 */

#include <math.h>
#include <windows.h>
#include <math.h>

#include "GLOBE.H"



VEC Gem[N][M];

VOID GlobeSet( DBL Xc, DBL Yc, DBL R )
{
  INT i, j;
  DBL theta;
  DBL phi;
   
  for (i = 0, theta = 0; i < N; i++, theta += PI / (N - 1))
    for (j = 0, phi = 0; j < M; j++, phi += 2 * PI / (M - 1))
    {
      Gem[i][j].X = R * sin(theta) * sin(phi);
      Gem[i][j].Y = R * cos(theta);
      Gem[i][j].Z = R * sin(theta) * cos(phi);
    }
} 



VOID DrawGlobe( HDC hDC )
{
  POINT Geom[N][M];
  int i, j, s = 3;

     for(i = 0; i < N; i++)
       for (j = 0; j < M; j++)
       {
         Ellipse(hDC, Geom[i][j].x - s, Geom[i][j].y - s, Geom[i][j].x + s, Geom[i][j].y + s);
       }
     /* Drawing horisontal lines */
      for (i = 0; i < N; i++)
      {
        MoveToEx(hDC, Geom[i][0].x, Geom[i][0].y, 0);
        for (j = 1; j < M; j++) 
          LineTo(hDC, Geom[i][j].x, Geom[i][j].y);
      }
         /* Drawing vertical lines */
      for (j = 0; j < M; j++) 
      {
        MoveToEx(hDC, Geom[0][j].x, Geom[0][j].y, 0);
        for (i = 1; i < N; i++)
          LineTo(hDC, Geom[i][j].x, Geom[i][j].y);
       }
} /* End of Draw function */ 
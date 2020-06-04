/* FILE NAME: GLOBE.H
 * PROGRAMMER: MG5
 * DATE: 04.06.2020 
 * PURPOSE: NONE
 */

#include <windows.h>

#define M 50
#define N 25

/* PI math constant */
#define PI 3.14159265358979323846

/* Set new type */ 
typedef DOUBLE DBL;

/* VEC type */
typedef struct
{
  DBL X, Y, Z;
} VEC;

VOID GlobeSet( DBL Xc, DBL Yc, DBL R );
VOID DrawGlobe( HDC hDC );
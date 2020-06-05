/* FILE NAME: GLOBE.H
 * PROGRAMMER: MG5
 * DATE: 04.06.2020 
 * PURPOSE: NONE
 */

#include <windows.h>

#include "mth.h"

#define M (30 * 2)
#define N (20 * 2)

/* PI math constant */
#define PI 3.14159265358979323846

VOID GlobeSet( DBL Xc, DBL Yc, DBL R );
VOID DrawGlobe( HDC hDC );
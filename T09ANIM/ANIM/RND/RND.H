/* FILE NAME: RND.H
 * PROGRAMMER: MG5
 * DATE: 06.06.2020
 * PURPOSE: 3D animation project.
 *          Common declaration module.
 */

#ifndef __RND_H_
#define __RND_H_

#define GLEW_STATIC
#include "glew.h"
#include <gl/gl.h>


#include "../../def.h"
#include "res/rndres.h"

/* Data */

extern HWND MG5_hRndWnd;                 /* Work window handle */
/*extern HDC MG5_hRndDCFrame;*/          /* Work window memory device context  */ 
extern HDC MG5_hRndDC;                   /* Work window memory device context  */
/*extern HBITMAP MG5_hRndBmFrame;*/      /* Work window background bitmap handle */
extern HGLRC MG5_hRndGLRC;
extern INT MG5_RndFrameH, MG5_RndFrameW; /* Work window size */
         


extern DBL
  MG5_RndProjSize,  /* Project plane fit square */
  MG5_RndProjDist,  /* Distance to project plane from viewer (near) */
  MG5_RndProjFarClip;  /* Distance to project far clip plane (far) */


extern MATR
  MG5_RndMatrView, /* View coordinate system matrix */
  MG5_RndMatrProj, /* Projection coordinate system matrix */
  MG5_RndMatrVP;   /* Stored (View * Proj) matrix */

extern VEC 
  MG5_RndCamLoc,
  MG5_RndCamRight,
  MG5_RndCamUp,
  MG5_RndCamDir;

/* Vertex type */
typedef struct tagmg5VERTEX
{
  VEC P;
  VEC2 T; 
  VEC N;
  VEC4 C;
} mg5VERTEX;

typedef enum tagmg5PRIM_TYPE
{
  MG5_RND_PRIM_TRIMESH, /* Array of triangles */
  MG5_RND_PRIM_TRISTRIP, /* Array of stripped triangles */
  MG5_RND_PRIM_LINES,
  MG5_RND_PRIM_POINTS, /* Array of points */
} mg5PRIM_TYPE;


/* Primitive type */
typedef struct tagmg5PRIM
{
  mg5PRIM_TYPE Type;
  INT
    VA,
    VBuf, 
    IBuf;
  INT NumOfElements;   /* Number of vertices */
  INT MtlNo; /* Material number in material array */
  MATR Trans;   /* Transformation matrix */
} mg5PRIM;


typedef struct tagmg5PRIMS
{
  INT NumOfPrims; /* Number of primitives in array */  
  mg5PRIM *Prims; /* Array of primitives */
  MATR Trans;     /* Common transformation matrix */
} mg5PRIMS;



/* Render system initialisation function.
 * ARGUMENTS:
 *   - HWND:
 *       HWND hWnd;
 * RETURNS: None.
 */
VOID MG5_RndInit( HWND hWnd );


/* Render system initialisation function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID MG5_RndClose( VOID );


/* Change window size function.
 * ARGUMENTS:
 *   - window size:
 *       INT W, INT H
 * RETURNS: None.
 */
VOID MG5_RndResize( INT W, INT H );


/* Change window size function.
 * ARGUMENTS:
 *   - window:
 *       HDC hDC.
 * RETURNS: None.
 */
VOID MG5_RndCopyFrame( VOID );


/* Start draw function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID MG5_RndStart( VOID );


/* End draw function 
 * ARGUMENTS: None.
 *
 * RETURNS: None.
 */
VOID MG5_RndEnd( VOID );


/* Create camera function.
 * ARGUMENTS: None.
 *
 * RETURNS: None.
 */
VOID MG5_RndProjSet( VOID );


/* Set camera function.
 * ARGUMENTS:
 *   - parametre of location:
 *       VEC Loc, VEC At, VEC Up.
 * RETURNS: None.
 */
VOID MG5_RndCamSet( VEC Loc, VEC At, VEC Up );


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
VOID MG5_RndPrimCreate( mg5PRIM *Pr, mg5PRIM_TYPE Type, mg5VERTEX *V, INT NoofV, INT *I, INT NoofI );


/* Delete primitive function.
 * ARGUMENTS:
 *   - primitive:
 *       mg5PRIM *Pr.
 * RETURNS: None.
 */
VOID MG5_RndPrimFree( mg5PRIM *Pr );


/* Draw primitive function.
 * ARGUMENTS:
 *   - primitive:
 *       mg5PRIM *Pr.
 *   - wotld matrix:
 *       MATR Wolrd.
 * RETURNS:
 *   (BOOL) result.
 */
VOID MG5_RndPrimDraw( mg5PRIM *Pr, MATR World );


/* Load primitive function.
 * ARGUMENTS:
 *   - primitive:
 *       mg5PRIM *Pr.
 *   - file:
 *       CHAR *FileName.
 * RETURNS:
 *   (BOOL) result.
 */
/*VOID MG5_RndPrimLoad( mg5PRIM *Pr, CHAR *FileName );*/


/* Load primitive from .OBJ file function.
 * ARGUMENTS:
 *   - pointer to primitive to be build:
 *       mg5PRIM *Pr;
 *   - .OBJ file name:
 *       CHAR *FileName;
 * RETURNS:
 *   (BOOL) TRUE is successful, FALSE otherwise.
 */
BOOL MG5_RndPrimLoad( mg5PRIM *Pr, CHAR *FileName );


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

BOOL MG5_RndPrimCreateSphere( mg5PRIM *Pr, VEC C, DBL RX, DBL RY, DBL RZ, INT SplitW, INT SplitH );

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
BOOL MG5_RndPrimCreateTorus( mg5PRIM *Pr, VEC C, DBL RX, DBL r, INT SplitW, INT SplitH );



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
BOOL MG5_RndPrimCreateFromGrid( mg5PRIM *Pr, mg5VERTEX *V, INT W, INT H, BOOL IsNormalsNeed );



/* Create array of primitives function.
 * ARGUMENTS:
 *   - pointer to primitives structure:
 *       mg5PRIMS *Prs;
 *   - number of primitives to be add:
 *       INT NumOfPrims;
 * RETURNS:
 *   (BOOL) TRUE if successful, FALSE otherwise.
 */
BOOL MG5_RndPrimsCreate( mg5PRIMS *Prs, INT NumOfPrims );

/* Delete array of primitives function.
 * ARGUMENTS:
 *   - pointer to primitives structure:
 *       mg5PRIMS *Prs;
 * RETURNS: None.
 */
VOID MG5_RndPrimsFree( mg5PRIMS *Prs );

/* Draw array of primitives function.
 * ARGUMENTS:
 *   - pointer to primitives structure:
 *       mg5PRIMS *Prs;
 *   - global transformation matrix:
 *       MATR World;
 * RETURNS: None.
 */
VOID MG5_RndPrimsDraw( mg5PRIMS *Prs, MATR World );

/* Load array of primitives from .G3DM file function.
 * ARGUMENTS:
 *   - pointer to primitives structure:
 *       mg5PRIMS *Prs;
 *   - file name:
 *       CHAR *FileName;
 * RETURNS:
 *   (BOOL) TRUE if successful, FALSE otherwise.
 */
BOOL MG5_RndPrimsLoad( mg5PRIMS *Prs, CHAR *FileName );


#endif /* __DEF_H_ */

/* END OF 'RND.H' FILE */
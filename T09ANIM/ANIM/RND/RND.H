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

/* Data */

extern HWND MG5_hRndWnd;                 /* Work window handle */
/*extern HDC MG5_hRndDCFrame;*/          /* Work window memory device context  */ 
extern HDC MG5_hRndDC;                   /* Work window memory device context  */
/*extern HBITMAP MG5_hRndBmFrame;*/      /* Work window background bitmap handle */
extern HGLRC MG5_hRndGLRC;
extern INT MG5_RndFrameH, MG5_RndFrameW; /* Work window size */
extern INT MG5_RndProgId;                /* Global shader for checking */ 


extern DBL
  MG5_RndProjSize,  /* Project plane fit square */
  MG5_RndProjDist,  /* Distance to project plane from viewer (near) */
  MG5_RndProjFarClip;  /* Distance to project far clip plane (far) */


extern MATR
  MG5_RndMatrView, /* View coordinate system matrix */
  MG5_RndMatrProj, /* Projection coordinate system matrix */
  MG5_RndMatrVP;   /* Stored (View * Proj) matrix */


/* Vertex type */
typedef struct tagmg5VERTEX
{
  VEC P;
  VEC2 T; 
  VEC N;
  VEC4 C;
} mg5VERTEX;


/* Primitive type */
typedef struct tagmg5PRIM
{
  INT
    VA,
    VBuf, 
    IBuf;
  INT NumOfElements;   /* Number of vertices */
  MATR Trans;   /* Transformation matrix */
} mg5PRIM;


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
VOID MG5_RndPrimCreate( mg5PRIM *Pr, mg5VERTEX *V, INT NumOfV, INT *I, INT NumOfI );


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
BOOL MG5_RndPrimCreateSphere( mg5PRIM *Pr, VEC C, DBL R, INT SplitW, INT SplitH );

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
BOOL MG5_RndPrimCreateTorus( mg5PRIM *Pr, VEC C, DBL R, INT SplitW, INT SplitH );

/* Load shader program from .GLSL files function.
 * ARGUMENTS:
 *   - shader file path to load (path\***.glsl):
 *       CHAR *ShaderFileNamePrefix;
 * RETURNS:
 *   (INT) load shader program Id.
 */
INT MG5_RndShdLoad( CHAR *ShaderFileNamePrefix );

/* Unload shader program from memory files function.
 * ARGUMENTS:
 *   - shader program Id:
 *       INT ProgId;
 * RETURNS: None.
 */
VOID MG5_RndShdDelete( INT ProgId );

#endif /* __DEF_H_ */

/* END OF 'RND.H' FILE */
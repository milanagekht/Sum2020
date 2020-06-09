 /* FILE NAME: ANIM.C
 * PROGRAMMER: MG5
 * DATE: 09.06.2020
 * PURPOSE: 3D animation project.
 */

#ifndef __ANIM_H_
#define __ANIM_H_

#include "RND/RND.h"

#define MG5_MAX_UNITS 3000

extern INT MG5_MouseWheel;

#define MG5_UNIT_BASE_FIELDS \
  VOID (*Init)( mg5UNIT *Uni, mg5ANIM *Ani );      \
  VOID (*Close)( mg5UNIT *Uni, mg5ANIM *Ani );     \
  VOID (*Response)( mg5UNIT *Uni, mg5ANIM *Ani );  \
  VOID (*Render)( mg5UNIT *Uni, mg5ANIM *Ani )


typedef struct tagmg5UNIT mg5UNIT;
typedef struct tagmg5ANIM
{ 
  HWND hWnd;
  HDC hDC;
  INT W, H;
  mg5UNIT *Units[MG5_MAX_UNITS];
  INT NumOfUnits;
  DBL
    GlobalTime, GlobalDeltaTime, /* Global time and interframe interval */
    Time, DeltaTime,             /* Time with pause and interframe interval */
    FPS;                         /* Frames per second value */
  BOOL 
    IsPause;                     /* Pause flag */
  /* Keyboard data */
  BYTE
    Keys[256],                   /* Keyboard state (true if key pressed) */
    KeysOld[256],                /* Previous frame keyboard state */
    KeysClick[256];              /* Key 'one-click' info */
  /* Mouse handle data */
  INT
    Mx, My, Mz,                  /* Absolute mouse coordinates */
    Mdx, Mdy, Mdz;               /* Relative mouse coordinates */
} mg5ANIM;



typedef struct tagmg5UNIT
{
  VOID (*Init)( mg5UNIT *Uni, mg5ANIM *Ani );     
  VOID (*Close)( mg5UNIT *Uni, mg5ANIM *Ani );     
  VOID (*Response)( mg5UNIT *Uni, mg5ANIM *Ani );  
  VOID (*Render)( mg5UNIT *Uni, mg5ANIM *Ani );
};


extern mg5ANIM MG5_Anim;



VOID MG5_AnimInit( HWND hWnd );
VOID MG5_AnimClose( VOID );
VOID MG5_AnimResize(INT w, INT h);
VOID MG5_AnimRender( VOID );
VOID MG5_AnimCopyFrame(HDC hDC);
VOID MG5_AnimUnitAdd(mg5UNIT *Uni);
mg5UNIT * MG5_AnimUnitCreate( INT size);
VOID MG5_TimerResponse( VOID );
VOID MG5_TimerInit( VOID );
VOID MG5_AnimDoExit ( VOID );
VOID MG5_AnimFlipFullScreen( VOID );
VOID MG5_AnimKeyboardResponse( VOID );
VOID MG5_AnimMouseResponse( VOID );



#endif /* __ANIM_H_ */

/* End of "ANIM.H" file */
 /* FILE NAME: MG.H
 * PROGRAMMER: MG5
 * DATE: 04.06.2020 
 * PURPOSE: NONE
 */

#include <windows.h>

/* Set new type */ 
typedef DOUBLE DBL;

DBL GLB_Time;
DBL GLB_DeltaTime;
DBL GLB_FPS;
BOOL GLB_IsPause;
LONG StartTime, PauseTime;
LONG OldTime;
LONG OldFPSTime;
LONG FrameCount;

VOID GLB_TimerInit( VOID );
VOID Timer( VOID );
VOID GLB_TimerResponse( VOID );
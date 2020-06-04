/* FILE NAME: TIMER.C
 * PROGRAMMER: MG5
 * DATE: 04.06.2020 
 * PURPOSE: NONE
 */

#include <time.h>

#include "TIMER.H"


DOUBLE SyncTime;

VOID GLB_TimerInit( VOID )
{
  StartTime = OldTime = OldFPSTime = clock();
  FrameCount = 0;
  PauseTime = 0;
  GLB_IsPause = FALSE;
}


VOID GLB_TimerResponse( VOID ) 
{
  LONG t = clock();

     if (!GLB_IsPause)
  {
    GLB_Time = (DBL)(t - PauseTime - StartTime) / CLOCKS_PER_SEC;
    GLB_DeltaTime = (DBL)(t - OldTime) / CLOCKS_PER_SEC;
  }
  else
  {
    PauseTime += t - OldTime;
    GLB_DeltaTime = 0;
  }

  FrameCount++;
  if (t - OldFPSTime > CLOCKS_PER_SEC)
  {
    GLB_FPS = FrameCount / ((DBL)(t - OldFPSTime) / CLOCKS_PER_SEC);
    OldFPSTime = t;
    FrameCount = 0;
  }
  OldTime = t;
}/* End of 'TimeResponse' function */

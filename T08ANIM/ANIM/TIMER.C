/* FILE NAME: TIMER.C
 * PROGRAMMER: MG5
 * DATE: 09.06.2020
 * PURPOSE: 3D animation project.
 */

#include "..\def.h"

#include "anim.h"

static UINT64
  StartTime,    /* Start program time */
  OldTime,      /* Previous frame time */
  OldTimeFPS,   /* Old time FPS measurement */
  PauseTime,    /* Time during pause period */
  TimePerSec,   /* Timer resolution */
  FrameCounter; /* Frames counter */

VOID MG5_TimerInit( VOID )
{
  LARGE_INTEGER t;

  QueryPerformanceFrequency(&t);
  TimePerSec = t.QuadPart;
  QueryPerformanceCounter(&t);
  StartTime = OldTime = OldTimeFPS = t.QuadPart;
  FrameCounter = 0;
  MG5_Anim.IsPause = FALSE;
  MG5_Anim.FPS = 30.0;
  PauseTime = 0;
}/* end of 'MG5_TimerInit' function*/


VOID MG5_TimerResponse( VOID )
{
  LARGE_INTEGER t;

  QueryPerformanceCounter(&t);
  /* Global time */
  MG5_Anim.GlobalTime = (DBL)(t.QuadPart - StartTime) / TimePerSec;
  MG5_Anim.GlobalDeltaTime = (DBL)(t.QuadPart - OldTime) / TimePerSec;

  /* Time with pause */
  if (MG5_Anim.IsPause)
  {
    MG5_Anim.DeltaTime = 0;
    PauseTime += t.QuadPart - OldTime;
  }
  else
  {
    MG5_Anim.DeltaTime = MG5_Anim.GlobalDeltaTime;
    MG5_Anim.Time = (DBL)(t.QuadPart - PauseTime - StartTime) / TimePerSec;
  }
  /* FPS */
  FrameCounter++;
  if (t.QuadPart - OldTimeFPS > TimePerSec)
  {
    MG5_Anim.FPS = FrameCounter * TimePerSec / (DBL)(t.QuadPart - OldTimeFPS);
    OldTimeFPS = t.QuadPart;
    FrameCounter = 0;
  }
  OldTime = t.QuadPart;
} /* End of 'MG5_TimerResponse' function*/

/* End of 'TIMER.C' file */
 /* FILE NAME: RNDRES.C
 * PROGRAMMER: MG5
 * DATE: 13.06.2020
 * PURPOSE: 3D animation project.
 *          Resurses module.
 */
#include "rndres.h"

VOID MG5_RndResInit( VOID )
{
  MG5_RndShdInit();
  MG5_RndMtlInit();
  MG5_RndTexInit();
}

/* Deinitialisation resourse struct function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID MG5_RndResClose( VOID )
{
  MG5_RndMtlClose();
  MG5_RndTexClose();
  MG5_RndShdClose();
}

/* END OF 'RNDRES.C' FILE */
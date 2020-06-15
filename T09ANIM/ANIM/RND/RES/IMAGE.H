/* FILE NAME: IMAGE.H
 * PROGRAMMER: MG5
 * DATE: 13.06.2020
 * PURPOSE: 3D animation project.
 *          Image constract module.
 */

#ifndef __IMAGE_H_
#define __IMAGE_H_

#include "rndres.h"

typedef struct tagmg5IMAGE
{
  INT W, H;           /*Image size */
  BYTE *Pixels;       /* Pixel array */
  DWORD *PixelsDW;    /* Pixel array */
  BYTE (*PixelsC)[4]; /* Pixel array */
} mg5IMAGE;

/* Create image function.
 * ARGUMENTS:
 *   - pointer to image to create:
 *       mg5IMAGE *Im;
 *   - size of image:
 *       INT W, H;
 * RETURNS:
 *  (BOOL) TRUE if success, FALSE otherwise.
 */
BOOL MG5_ImgCreate( mg5IMAGE *Im, INT W, INT H );

/* Load image function.
 * ARGUMENTS:
 *   - pointer to image to create:
 *       mg5IMAGE *Im;
 *   - name of image:
 *       CHAR *FileName;
 * RETURNS:
 *  (BOOL) TRUE if success, FALSE otherwise.
 */
BOOL MG5_ImgLoad( mg5IMAGE *Im, CHAR *FileName );

/* Create image function.
 * ARGUMENTS:
 *   - pointer to image to create:
 *       mg5IMAGE *Im;
 * RETURNS: None.
 */
VOID MG5_ImgFree( mg5IMAGE *Im );

#endif /* __IMAGE_H_ */

/* END OF 'IMAGE.H' FILE */
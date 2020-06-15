/* FILE NAME: RNDTXT.C
 * PROGRAMMER: MG5
 * DATE: 13.06.2020
 * PURPOSE: 3D animation project.
 *          Textures module.
 */
#include <stdio.h>

#include "rndres.h"

mg5TEXTURE MG5_RndTextures[MG5_MAX_TEXTURES]; /* Array of textures */
INT MG5_RndTexturesSize = 0;                  /* Textures array store size */

/* Initialization texture struct function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID MG5_RndTexInit( VOID )
{
  MG5_RndTexturesSize = 0;
} /* End of 'MG5_RndTexInit' function */

/* Deinitialization texture struct function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID MG5_RndTexClose( VOID )
{
   INT i;

   for (i = 0; i < MG5_RndTexturesSize; i++)
     glDeleteTextures(1, &MG5_RndTextures[i].TexId);
   MG5_RndTexturesSize = 0;
} /* End of 'MG5_RndTexClose' function */


/* Add texture from  image file function.
 * ARGUMENTS:
 
 * RETURNS:
 *   (INT) added texture table number.
 */
INT MG5_RndTexAddImg( CHAR *Name, INT W, INT H, INT C, VOID *Bits )
{
  if (MG5_RndTexturesSize >= MG5_MAX_TEXTURES)
    return -1;

  strncpy(MG5_RndTextures[MG5_RndTexturesSize].Name, Name, MG5_STR_MAX - 1);
  MG5_RndTextures[MG5_RndTexturesSize].W = W;
  MG5_RndTextures[MG5_RndTexturesSize].H = H;
  glGenTextures(1, &MG5_RndTextures[MG5_RndTexturesSize].TexId);

  glBindTexture(GL_TEXTURE_2D, MG5_RndTextures[MG5_RndTexturesSize].TexId);
  glTexImage2D(GL_TEXTURE_2D, 0, C, W, H, 0,
    C == 3 ? GL_BGR : C  == 4 ? GL_BGRA : GL_LUMINANCE,
    GL_UNSIGNED_BYTE, Bits);
    glGenerateMipmap(GL_TEXTURE_2D);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

  return MG5_RndTexturesSize++;
} /* End of 'MG5_RndTexAddImg' function */

/* Add texture to stock from file function.
 * ARGUMENTS:
 *   - texture file name:
 *       CHAR *FileName;
 * RETURNS:
 *   (INT) new texture stock number (-1 if error occured).
 */
INT MG5_RndTexAdd( CHAR *FileName )
{
  INT n;
  mg5IMAGE Img;

  if (!MG5_ImgLoad(&Img, FileName))
    return -1;
  n = MG5_RndTexAddImg(FileName, Img.W, Img.H, 4, Img.Pixels);
  MG5_ImgFree(&Img);
  return n;
} /* End of 'MG5_RndTexAdd' function */



/* END OF 'RNDTXT.C' FILE */
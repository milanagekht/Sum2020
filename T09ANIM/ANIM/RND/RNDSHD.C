/* FILE NAME: RNDSHD.C
 * PROGRAMMER: MG5
 * DATE: 11.06.2020
 * PURPOSE: 3D animation project.
 *          Render system shader handle module.
 */

#include <stdio.h>
#include <string.h>

#include "rnd.h"

/* Load text from file function.
 * ARGUMENTS:
 *   - file name to be load from:
 *       CHAR *FileName;
 * RETURNS:
 *   (CHAR *) load text (allocated through malloc).
 */
static CHAR * MG5_RndShdLoadTextFromFile( CHAR *FileName )
{
  FILE *F;
  CHAR *txt;
  INT flen;

  /* Open file */
  if ((F = fopen(FileName, "rb")) == NULL)
    return NULL;

  /* Obtain file length */
  fseek(F, 0, SEEK_END);
  flen = ftell(F);
  rewind(F); /* <=> fseek(F, 0, SEEK_SET); */

  /* Allocate memory for file text */
  if ((txt = malloc(flen + 1)) == NULL)
  {
    fclose(F);
    return NULL;
  }

  /* Load text from file */
  memset(txt, 0, flen + 1);
  fread(txt, 1, flen, F);

  fclose(F);
  return txt;
} /* End of 'MG5_RndShdLoadTextFromFile' function */


/* Store log to file function.
 * ARGUMENTS:
 *   - message file prefix, shader name and text:
 *       CHAR *Prefix, *PartName, *Text;
 * RETURNS: None.
 */
static VOID MG5_RndShdLog( CHAR *Prefix, CHAR *PartName, CHAR *Text )
{
  FILE *F;

  if ((F = fopen("BIN/SHADERS/MG5{30}SHD.LOG", "a")) == NULL)
    return;
  fprintf(F, "%s/%s.GLSL\n%s\n", Prefix, PartName, Text);
  fclose(F);
} /* End of 'MG5_RndShdLog' function */

/* Load shader program from .GLSL files function.
 * ARGUMENTS:
 *   - shader file path to load (path\***.glsl):
 *       CHAR *ShaderFileNamePrefix;
 * RETURNS:
 *   (INT) load shader program Id.
 */
INT MG5_RndShdLoad( CHAR *ShaderFileNamePrefix )
{
  struct
  {
    CHAR *Name; /* Shader file prefix name (e.g. "VERT") */
    INT Type;   /* Shader OpenFL type (e.g. GL_VERTEX_SHADER) */
    INT Id;     /* Obtained shader Id from OpenGL */
  } shd[] =
  {
    {"VERT", GL_VERTEX_SHADER},
    {"FRAG", GL_FRAGMENT_SHADER},
  };
  INT NoofS = sizeof(shd) / sizeof(shd[0]), i, res, prg;
  CHAR *txt;
  BOOL is_ok = TRUE;
  static CHAR Buf[1000];

  for (i = 0; i < NoofS; i++)
  {
    /* Build file name */
    sprintf(Buf, "BIN/SHADERS/%s/%s.GLSL", ShaderFileNamePrefix, shd[i].Name);
    /* Load shader text from file */
    txt = MG5_RndShdLoadTextFromFile(Buf);
    if (txt == NULL)
    {
      MG5_RndShdLog(ShaderFileNamePrefix, shd[i].Name, "Error load file");
      is_ok = FALSE;
      break;
    }
    /* Create shader */
    if ((shd[i].Id = glCreateShader(shd[i].Type)) == 0)
    {
      MG5_RndShdLog(ShaderFileNamePrefix, shd[i].Name, "Error create shader");
      free(txt);
      is_ok = FALSE;
      break;
    }
    /* Attach shader text to shader */
    glShaderSource(shd[i].Id, 1, &txt, NULL);
    free(txt);

    /* Compile shader */
    glCompileShader(shd[i].Id);
    glGetShaderiv(shd[i].Id, GL_COMPILE_STATUS, &res);
    if (res != 1)
    {
      glGetShaderInfoLog(shd[i].Id, sizeof(Buf), &res, Buf);
      MG5_RndShdLog(ShaderFileNamePrefix, shd[i].Name, Buf);
      is_ok = FALSE;
      break;
    }
  }

  /* Create shader program */
  if (is_ok)
  {
    if ((prg = glCreateProgram()) == 0)
    {
      MG5_RndShdLog(ShaderFileNamePrefix, "PROG", "Error create program");
      is_ok = FALSE;
    }
    else
    {
      /* Attach shaders to program */
      for (i = 0; i < NoofS; i++)
        if (shd[i].Id != 0)
          glAttachShader(prg, shd[i].Id);
      /* Link shader program */
      glLinkProgram(prg);
      glGetProgramiv(prg, GL_LINK_STATUS, &res);
      if (res != 1)
      {
        glGetProgramInfoLog(prg, sizeof(Buf), &res, Buf);
        MG5_RndShdLog(ShaderFileNamePrefix, "PROG", Buf);
        is_ok = FALSE;
      }
    }
  }

  if (!is_ok)
  {
    /* Delete all created shaders */
    for (i = 0; i < NoofS; i++)
      if (shd[i].Id != 0)
      {
        if (prg)
          glDetachShader(prg, shd[i].Id);
        glDeleteShader(shd[i].Id);
      }
    /* Delete program */
    if (prg)
      glDeleteProgram(prg);
    prg = 0;
  }
  return prg;
} /* End of 'MG5_RndShdLoad' function */

/* Unload shader program from memory files function.
 * ARGUMENTS:
 *   - shader program Id:
 *       INT ProgId;
 * RETURNS: None.
 */
VOID MG5_RndShdDelete( INT ProgId )
{
  INT shdrs[5], n, i;

  if (ProgId == 0)
    return;

  glGetAttachedShaders(ProgId, 5, &n, shdrs);
  for (i = 0; i < n; i++)
  {
    glDetachShader(ProgId, shdrs[i]);
    glDeleteShader(shdrs[i]);
  }
  glDeleteProgram(ProgId);
} /* End of 'MG5_RndShdDelete' function */

/* END OF 'RNDSHD.C' FILE */
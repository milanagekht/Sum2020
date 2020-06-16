 /* FILE NAME: RNDMTL.C
 * PROGRAMMER: MG5
 * DATE: 13.06.2020
 * PURPOSE: 3D animation project.
 *          Material module.
 */
#include "rndres.h"

mg5MATERIAL MG5_RndMaterials[MG5_MAX_MATERIALS]; /* Array of materials */
INT MG5_RndMaterialsSize = 0;                    /* Materials array store size */

/* Initialisation default matereal struct function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID MG5_RndMtlInit( VOID )
{
  INT i;
  mg5MATERIAL mtl;

  strncpy(mtl.Name, "DEFAULT", MG5_STR_MAX - 1);

  memset(&mtl, 0, sizeof(mtl));
  mtl.Ka = VecSet1(0.1);
  mtl.Kd = VecSet1(0.9);
  mtl.Ks = VecSet1(0.0);
  mtl.Ph = 30;
  mtl.ShdNo = 0;
  mtl.Trans = 1;
  for (i = 1; i < 8; i++)
    MG5_RndMaterials[0].Tex[i] = -1;
  MG5_RndMtlAdd(&mtl);
} /* End of 'MG5_RndMtlInit' function */

/* Deinitialisation matereal struct function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID MG5_RndMtlClose( VOID )
{
  INT i;

  for (i = 0; i < MG5_RndMaterialsSize; i++)
    memset(&MG5_RndMaterials[i], 0, sizeof(mg5MATERIAL));
} /* End of 'MG5_RndMtlClose' function */


/* Add material function.
 * ARGUMENTS:
 *        mtl.
 * RETURNS:
 *   (INT) added texture table number.
 */
INT MG5_RndMtlAdd( mg5MATERIAL *Mtl )
{
/* Checking free space */
  if (MG5_RndMaterialsSize >= MG5_MAX_MATERIALS)
    return -1;

  MG5_RndMaterials[MG5_RndMaterialsSize] = *Mtl;
  return MG5_RndMaterialsSize++;
}

/* Load shader program from .GLSL files function.
 * ARGUMENTS:
 *   - number of material:
 *       INT MtlNo;
 * RETURNS:
 *   (INT) shader program Id.
 */
INT MG5_RndMtlApply( INT MtlNo )
{
  INT prg, loc, i;
  mg5MATERIAL *mtl;

  /* Set material pointer */
  if (MtlNo < 0 || MtlNo >= MG5_RndMaterialsSize)
    MtlNo = 0;
  mtl = &MG5_RndMaterials[MtlNo];

  /* Set shader program Id */
  prg = mtl->ShdNo;
  if (prg < 0 || prg >= MG5_RndShadersSize)
    prg = 0;
    prg = MG5_RndShaders[prg].ProgId;

  glUseProgram(prg);
  /* Set shading parameters */
  if ((loc = glGetUniformLocation(prg, "Time")) != -1)
    glUniform1f(loc, MG5_Anim.Time);
  if ((loc = glGetUniformLocation(prg, "GlobalTime")) != -1)
    glUniform1f(loc, MG5_Anim.GlobalTime);
  if ((loc = glGetUniformLocation(prg, "Ka")) != -1)
    glUniform3fv(loc, 1, &mtl->Ka.X);
  if ((loc = glGetUniformLocation(prg, "Kd")) != -1)
    glUniform3fv(loc, 1, &mtl->Kd.X);
  if ((loc = glGetUniformLocation(prg, "Ks")) != -1)
    glUniform3fv(loc, 1, &mtl->Ks.X);
  if ((loc = glGetUniformLocation(prg, "Ph")) != -1)
    glUniform1f(loc, mtl->Ph);
  if ((loc = glGetUniformLocation(prg, "Trans")) != -1)
    glUniform1f(loc, mtl->Trans);

  /* Set textures */
  for (i = 0; i < 8; i++)
  {
    CHAR tname[] = "IsTexture0";

    tname[9] = '0' + i;
    if (mtl->Tex[i] != -1)
    {
      glActiveTexture(GL_TEXTURE0 + i);
      glBindTexture(GL_TEXTURE_2D, MG5_RndTextures[mtl->Tex[i]].TexId);
    }
    if ((loc = glGetUniformLocation(prg, tname)) != -1)
      glUniform1i(loc, mtl->Tex[i] != -1);
  }
  return prg;
} /* End of 'MG5_RndMtlApply' function */

/* Get default material  function.
 * ARGUMENTS:  
 *            None.
 */
mg5MATERIAL MG5_RndMtlGetDef( VOID )
{
  return MG5_RndMaterials[0];
}

/* Get material pointer by material table number function.
 * ARGUMENTS:  
 *            number of material at material table.
 */
mg5MATERIAL * MG5_RndMtlGet( INT MtlNo )
{
  /* Get material pointer */
  if (MtlNo < 0 || MtlNo >= MG5_RndMaterialsSize)
    MtlNo = 0;
  return &MG5_RndMaterials[MtlNo];
}



/* END OF 'RNDMTL.C' FILE */
/* FILE NAME: RNDRES.H
 * PROGRAMMER: MG5
 * DATE: 13.06.2020
 * PURPOSE: 3D animation project.
 *          Resurses module.
*/
#ifndef __RNDRES_H_
#define __RNDRES_H_

#include "../../anim.h"
#include "image.h"

#define MG5_STR_MAX  300
#define MG5_MAX_TEXTURES 30000
#define MG5_MAX_SHADERS  47
#define MG5_MAX_MATERIALS 500

/* Shader program store type */
typedef struct tagmg5SHADER
{
  CHAR Name[MG5_STR_MAX]; /* Shader filename prefix */
  INT ProgId;             /* Shader program Id */
} mg5SHADER;

extern mg5SHADER MG5_RndShaders[MG5_MAX_SHADERS];
extern INT MG5_RndShadersSize;

/* Texture representation */
typedef struct tagmg5TEXTURE
{
  CHAR Name[MG5_STR_MAX]; /* Texture name */
  INT W, H;               /* Texture size in pixels */
  UINT TexId;             /* OpenGL texture Id */ 
} mg5TEXTURE;

extern mg5TEXTURE MG5_RndTextures[MG5_MAX_TEXTURES]; /* Array of textures */
extern INT MG5_RndTexturesSize;                      /* Textures array store size */

/* Material store type */
typedef struct tagmg5MATERIAL
{
  CHAR Name[MG5_STR_MAX]; /* Material name */

  /* Illumination coefficients */    
  VEC Ka, Kd, Ks;           /* Ambient, diffuse, specular coefficients */
  FLT Ph;                   /* Phong power coefficient */
  FLT Trans;                /* Transparency factor */
  INT Tex[8];               /* Texture references from texture table (or -1) */

  INT ShdNo;                /* Shader number in shader array */
} mg5MATERIAL;

extern mg5MATERIAL MG5_RndMaterials[MG5_MAX_MATERIALS]; /* Array of materials */
extern INT MG5_RndMaterialsSize;                        /* Materials array store size */

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
 *   - shader number:
 *       INT ProgId;
 * RETURNS: None.
 */
VOID MG5_RndShdDelete( INT ShdNo );

/* Update all shaders function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */

 /* Shader store system deinitialization function
 * ARGUMENTS: None.
 * RETURNS: None;
 */
 VOID MG5_RndShdClose( VOID );

 /* Shader store system initialization function
 * ARGUMENTS: None.
 * RETURNS: None;
 */
 VOID MG5_RndShdInit( VOID );

VOID MG5_RndShdUpdate( VOID );

/* Add shader program function.
 * ARGUMENTS:
 *   - shader file path to load (path\***.glsl):
 *       CHAR *ShaderFileNamePrefix;
 * RETURNS:
 *   (INT) added shader table number.
 */
INT MG5_RndShdAdd( CHAR *ShederFileNamePrefix );

/* Initialisation texture struct function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID MG5_RndTexInit( VOID );

/* Deinitialisation texture struct function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID MG5_RndTexClose( VOID );

/* Add texture from  imagr file function.
 * ARGUMENTS:
 *   - Name of file:
 *       CHAR *FileName;
 * RETURNS:
 *   (INT) added texture table number.
 */
INT MG5_RndTexAddImg( CHAR *Name, INT W, INT H, INT C, VOID *Bits );

/* Add texture from file function.
 * ARGUMENTS:
 *   - Name of file:
 *       CHAR *FileName;
 * RETURNS:
 *   (INT) added texture table number.
 */
INT MG5_RndTexAdd( CHAR *FileName );

/* Load shader program from .GLSL files function.
 * ARGUMENTS:
 *   - number of material:
 *       INT MtlNo;
 * RETURNS:
 *   (INT) shader program Id.
 */
INT MG5_RndMtlApply( INT MtlNo );

/* Initialisation default matirial struct function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID MG5_RndMtlInit( VOID );

/* Deinitialisation matirial struct function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID MG5_RndMtlClose( VOID );

/* Deinitialisation resourse struct function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID MG5_RndResClose( VOID );

/* Add material function.
 * ARGUMENTS:
 *        mtl.
 * RETURNS:
 *   (INT) added texture table number.
 */
INT MG5_RndMtlAdd( mg5MATERIAL *Mtl );

/* Get material pointer by material table number function.
 * ARGUMENTS:  
 *            number of material at material table.
 */
mg5MATERIAL * MG5_RndMtlGet( INT MtlNo );

/* Get default material  function.
 * ARGUMENTS:  
 *            None.
 */
mg5MATERIAL MG5_RndMtlGetDef( VOID );

VOID MG5_RndResInit( VOID );
#endif /* __RNDRES_H_ */

/* END OF 'RNDRES.H' FILE */
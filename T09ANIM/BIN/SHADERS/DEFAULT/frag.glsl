/* FILE NAME: frag.glsl
 * PROGRAMMER: MG5
 * DATE: 06.06.2020
 * PURPOSE: 3D animation project.
 *          Fragment shader 
 */                            
#version 420

layout(location = 0) out vec4 OutColor;

in vec4 DrawColor;
in vec2 DrawTexCoord;
in vec3 DrawNormal;
in vec3 DrawPos;

uniform vec3 Ka, Kd, Ks;
uniform float Ph;
uniform vec3 CamLoc;
uniform float Time;

layout(binding = 0) uniform sampler2D Texture0;
uniform bool IsTexture0;

vec3 Shade( vec3 P, vec3 N, vec3 L )
{
  vec3 V = -normalize(P);
  vec3 color = DrawColor.rgb;
  // Ambient
  color += Ka;

  // Duffuse
  float nl = max(0.1, dot(N, L));

  vec4 tc = texture(Texture0, DrawTexCoord);
  
  if (IsTexture0)
    color += Kd * tc.rgb * nl;
  else
    color += Kd * nl;
    
  // Specular
  vec3 R = reflect(V, N);
  float rl = dot(R, L);
  if (rl > 0)
    color += Ks * pow(rl, Ph);

  return color;
}

void main( void )
{
  vec3 N = normalize(DrawNormal);
  vec3 L = normalize(vec3(sin(Time * 2), sin(Time) + 4, cos(Time * 2)));  // light source

  OutColor = vec4(Shade(DrawPos, N, L), 1);
 // OutColor = vec4(N, 1);
}


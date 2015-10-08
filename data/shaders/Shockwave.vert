#version 110

attribute vec4 in_Color;

varying vec4 var_Color;
varying vec2 var_TexCoord;
varying vec4 verpos;

void main()
{
  gl_Position = ftransform();
  verpos = gl_ModelViewMatrix*gl_Vertex;
  var_Color = in_Color;
  vec4 vertex = gl_ModelViewMatrix * gl_Vertex;
  var_TexCoord = gl_TextureMatrix[0] * gl_MultiTexCoord0;
  //var_TexCoord = vertex.xy;
}
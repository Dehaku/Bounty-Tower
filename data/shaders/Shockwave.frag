#version 110

uniform sampler2D in_Texture; // 0
uniform vec2 in_Center; // Mouse position
uniform float in_Time; // effect elapsed time. Multiply this to affect speed.

// Amplitude?, Refraction?, Width? e.g. 10.0, 0.8, 0.1
uniform vec3 in_ShockParams;

uniform float in_WindowWidth;
uniform float in_WindowHeight;
uniform vec2 position;

varying vec2 var_TexCoord;
varying vec4 verpos;

void main()
{
  vec2 uv = var_TexCoord;
  float verX = verpos.x / in_WindowWidth;
  float verY = verpos.y / in_WindowHeight;
  vec2 offset = uv;
  offset.x = offset.x;
  offset.y = offset.y;
  vec2 texCoord = offset;
  float x = in_Center.x / float(in_WindowWidth);
  float y = in_Center.y / float(in_WindowHeight);
  x -= verX;
  y -= verY;
  //float y = (float(in_WindowHeight) - in_Center.y) / float(in_WindowHeight);
  float distance = distance(vec2(verX,verY), vec2(x, y));

  if ( (distance <= (in_Time + in_ShockParams.z)) &&
       (distance >= (in_Time - in_ShockParams.z)) )
  {
    float diff = (distance - in_Time);
    float powDiff = 1.0 - pow(abs(diff * in_ShockParams.x),
                                in_ShockParams.y);
    float diffTime = diff * powDiff;
	vec2 exper = in_Center - verpos;
	
    vec2 diffUV = normalize(offset - exper); // in_Center);
    texCoord = offset + (diffUV * diffTime);
  }

  gl_FragColor = texture2D(in_Texture, texCoord);
}
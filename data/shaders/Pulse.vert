#ifdef GL_ES
precision mediump float;
#endif

uniform float time;
uniform vec2 mouse;
uniform vec2 resolution;
varying vec2 surfacePosition;

void main( void ) {

	vec2 p = 2.0 * (( gl_FragCoord.xy / resolution.xy ) - 0.5);
	vec2 mouseNorm=2.0*(mouse - vec2(0.5));
	p.x *= resolution.x/resolution.y;
	mouseNorm.x *= resolution.x/resolution.y;
	
	
	float r = length(p);
	
	float c = .01/abs(r-length(mouseNorm));
	
	float d = 1.0/(length(surfacePosition*100.0));
	
	vec3 col = vec3(c, c+d, d);
	
	gl_FragColor = vec4(col, 1.0);

}
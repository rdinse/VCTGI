#version 330

in vec2 passUVCoord;

uniform sampler2D positionMap;
uniform sampler2D normalMap;
uniform mat4 sceneProjectionMatrix;
uniform float radius;
uniform float quality;

out vec4 fragmentColor;

float rand(vec2 co){
    return fract(sin(dot(co.xy ,vec2(12.9898,78.233))) * 43758.5453);
}
vec3 rand3(int i, vec3 v, vec2 v2){
	float x = rand(vec2(v.x + i*3+0, v.x + ((i*3+0)*1531)%113) + v2*19789)*2 - 1.0;
	float y = rand(vec2(v.y + i*3+1, v.y + ((i*3+1)*1531)%113) + v2*19789)*2 - 1.0;
	float z = rand(vec2(v.z + i*3+2, v.z + ((i*3+2)*1531)%113) + v2*19789)*2 - 1.0;
	return vec3(x,y,z);
}

void main() {
	vec3 pSphere[16] = vec3[](
			vec3(0.53812504, 0.18565957, -0.43192),
			vec3(0.13790712, 0.24864247, 0.44301823),
			vec3(0.33715037, 0.56794053, -0.005789503),
			vec3(-0.6999805, -0.04511441, -0.0019965635),
			vec3(0.06896307, -0.15983082, -0.85477847),
			vec3(0.056099437, 0.006954967, -0.1843352),
			vec3(-0.014653638, 0.14027752, 0.0762037),
			vec3(0.010019933, -0.1924225, -0.034443386),
			vec3(-0.35775623, -0.5301969, -0.43581226),
			vec3(-0.3169221, 0.106360726, 0.015860917),
			vec3(0.010350345, -0.58698344, 0.0046293875),
			vec3(-0.08972908, -0.49408212, 0.3287904),
			vec3(0.7119986, -0.0154690035, -0.09183723),
			vec3(-0.053382345, 0.059675813, -0.5411899),
			vec3(0.035267662, -0.063188605, 0.54602677),
			vec3(-0.47761092, 0.2847911, -0.0271716));
	vec3 pos = texture( positionMap, passUVCoord).xyz;
	vec3 normal = normalize( texture( normalMap, passUVCoord).rgb);// + vec3(0,0,0.00001));
	float d = texture( positionMap, passUVCoord).z + 0.001;
	
	if ( texture( positionMap, passUVCoord).w < 0.5) return;
	
	float occ = 1.0;
	int j=0;
	for(int i=0; i<quality; i++) {
		vec3 v = rand3( i+j, pSphere[i%16], passUVCoord);
		v = sign( dot( v, normal)) * v;
		v = pos + v * radius;
		
		vec4 diff = sceneProjectionMatrix * vec4( v, 1);
		diff = (diff / diff.w)*0.5 + 0.5;
		
		vec4 td = texture( positionMap, diff.xy);
		if (td.w < 0.5) continue;
		
		if (td.z > v.z) occ -= 1.0/quality;
	}
	fragmentColor = vec4( vec3( occ), 1.0);
}


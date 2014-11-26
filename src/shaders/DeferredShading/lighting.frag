#version 330

in vec4 passPosition;
in vec4 midPosition;

in vec4 color;

uniform sampler2D positionMap;
uniform sampler2D normalMap;

layout(location = 0) out vec4 lightingOutput;

void main() {
	vec2 uv = gl_FragCoord.xy/800.0;
    vec4 pos = texture(positionMap, uv);
	vec3 nor = texture(normalMap, uv).xyz;
	
	if(pos.w < 0.5) discard;
	
	float diffuseStrength = 2 - length(pos - midPosition);
	float specularStrength = clamp(diffuseStrength * diffuseStrength, 0, 1);
	if (diffuseStrength < 0) discard;
	
    vec3 nPosToLight = normalize(vec3(midPosition.xyz - pos.xyz));

    vec3  reflection = normalize(reflect(-nPosToLight,nor.xyz));
    float diffuse = max(dot(nor.xyz, nPosToLight), 0);
    float specular = pow(max(dot(reflection, -normalize(pos.xyz)),0),50);
	
	lightingOutput = vec4(passPosition.xyz - midPosition.xyz, 1.0);
	lightingOutput = vec4(nor.xyz, 1.0);
	
    lightingOutput = color * (diffuseStrength * vec4(vec3(diffuse),1) * 0.7 + specularStrength * vec4(1,1,1,1) * specular);
}


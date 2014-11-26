#version 430

layout (points) in;
layout (triangle_strip, max_vertices = 24) out;

in vec3 texCoords[];
out vec4 outColor;

layout(binding = 0) uniform sampler3D voxelMaps[6];

uniform int   uniformMipLevel;
uniform float uniformHalfVoxelSize;
uniform mat4  uniformViewProjectionMatrix;

const vec4 cubeVertices[8] = vec4[8] (
	vec4( 1.0,  1.0,  1.0, 1.0),  // 0
	vec4( 1.0,  1.0, -1.0, 1.0),  // 1
	vec4( 1.0, -1.0,  1.0, 1.0),  // 2
	vec4( 1.0, -1.0, -1.0, 1.0),  // 3
	vec4(-1.0,  1.0,  1.0, 1.0),  // 4
	vec4(-1.0,  1.0, -1.0, 1.0),  // 5
	vec4(-1.0, -1.0,  1.0, 1.0),  // 6
	vec4(-1.0, -1.0, -1.0, 1.0)); // 7

/*	 5         1
				
				
4    7    0    3
				
				
6         2   */

const int cubeIndices[24]  = int[24] (
	0, 2, 1, 3,  // X+
	6, 4, 7, 5,  // X-
	5, 4, 1, 0,  // Y+
	6, 7, 2, 3,  // Y-
	4, 6, 0, 2,  // Z+
	1, 3, 5, 7); // Z-

void main() {
  vec4 projectedCubeVertices[8];
  for (int i = 0; i < 8; i++) {
    projectedCubeVertices[i] = uniformViewProjectionMatrix
      * (gl_in[0].gl_Position + (uniformHalfVoxelSize * cubeVertices[i]));
  }
    
  for (int dir = 0; dir < 6; dir++) {
    vec4 voxelValue = texelFetch(voxelMaps[dir],
                                 ivec3(texCoords[0].xyz * (int((1/uniformHalfVoxelSize)) >> uniformMipLevel)),
                                 uniformMipLevel); // textureLod

    if (voxelValue.a != 0) {
      voxelValue.rgb /= voxelValue.a;
    } else {
      continue;
    }
		
    for (int vert = 0; vert < 4; vert++) {
      gl_Position = projectedCubeVertices[cubeIndices[dir*4 + vert]];
      outColor = voxelValue;
      EmitVertex();
    }
    EndPrimitive();
  }
}

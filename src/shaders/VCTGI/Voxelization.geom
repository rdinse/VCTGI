#version 430

layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

in vertPassS {
  vec3 normal;
  vec2 uv;
} vertPass[];

out geomPassS {
	vec3 worldPos;
    vec3 voxelPos;
    vec3 normal;
    vec2 uv;
} geomPass;

uniform mat4 uniformWorldToVoxelsMatrix;
uniform mat4 uniformViewMatrixX;
uniform mat4 uniformViewMatrixY;
uniform mat4 uniformViewMatrixZ;
uniform mat4 uniformProjectionMatrix;

uniform int uniformVoxelRes;

flat out vec4 triangleBoundingBox;

void main() {
  const bool conservatively = false;

  mat4 viewMatrix;
  vec3 absNormal = abs(vertPass[0].normal);
  if (absNormal.x >= absNormal.y && absNormal.x >= absNormal.z) {
    viewMatrix = uniformViewMatrixX;
  } else if (absNormal.y >= absNormal.x && absNormal.y >= absNormal.z) {
    viewMatrix = uniformViewMatrixY;
  } else {
    viewMatrix = uniformViewMatrixZ;
  }

  mat4 viewProjectionMatrix = uniformProjectionMatrix * viewMatrix;
  mat4 viewProjectionInverseMatrix = inverse(viewProjectionMatrix);

  // Conservative rasterization
  vec4 vsProjs[3] = vec4[3] (viewProjectionMatrix * gl_in[0].gl_Position,
                             viewProjectionMatrix * gl_in[1].gl_Position,
                             viewProjectionMatrix * gl_in[2].gl_Position);

  vec4 projNormal = transpose(viewProjectionInverseMatrix) * vec4(vertPass[0].normal, 0);

  // Pass axis-aligned, extended bounding box in NDCs.
  triangleBoundingBox = vec4(min(vsProjs[0].xy, min(vsProjs[1].xy, vsProjs[2].xy)),
                             max(vsProjs[0].xy, max(vsProjs[1].xy, vsProjs[2].xy)));
  triangleBoundingBox = (triangleBoundingBox * vec4(0.5) + vec4(0.5)) * uniformVoxelRes;
  triangleBoundingBox.xy -= vec2(1.0);
  triangleBoundingBox.zw += vec2(1.0);

  // Calculate normal equation of triangle plane.
  vec3 normal = cross(vsProjs[1].xyz - vsProjs[0].xyz, vsProjs[0].xyz - vsProjs[2].xyz);
  normal = projNormal.xyz;
  float d = dot(vsProjs[0].xyz, normal);
  float normalSign = (projNormal.z > 0) ? 1.0 : -1.0;

  // Convert edges to homogeneous line equations and dilate triangle.
  // See:  http://http.developer.nvidia.com/GPUGems2/gpugems2_chapter42.html
  vec3 planes[3]; vec3 intersection[3]; float z[3];
  vec2 halfPixelSize = vec2(1.0 / uniformVoxelRes);
  planes[0] = cross(vsProjs[0].xyw - vsProjs[2].xyw, vsProjs[2].xyw);
  planes[1] = cross(vsProjs[1].xyw - vsProjs[0].xyw, vsProjs[0].xyw);
  planes[2] = cross(vsProjs[2].xyw - vsProjs[1].xyw, vsProjs[1].xyw);
  planes[0].z += normalSign * dot(halfPixelSize, abs(planes[0].xy));
  planes[1].z += normalSign * dot(halfPixelSize, abs(planes[1].xy));
  planes[2].z += normalSign * dot(halfPixelSize, abs(planes[2].xy));
  intersection[0] = cross(planes[0], planes[1]);
  intersection[1] = cross(planes[1], planes[2]);
  intersection[2] = cross(planes[2], planes[0]);
  intersection[0] /= intersection[0].z;
  intersection[1] /= intersection[1].z;
  intersection[2] /= intersection[2].z;
  z[0] = (-intersection[0].x * normal.x - intersection[0].y * normal.y + d) / normal.z;
  z[1] = (-intersection[1].x * normal.x - intersection[1].y * normal.y + d) / normal.z;
  z[2] = (-intersection[2].x * normal.x - intersection[2].y * normal.y + d) / normal.z;
  vsProjs[0].xyz = vec3(intersection[0].xy, z[0]);
  vsProjs[1].xyz = vec3(intersection[1].xy, z[1]);
  vsProjs[2].xyz = vec3(intersection[2].xy, z[2]);

  // Pass vertex data.
  for (int i = 0; i < gl_in.length(); i++) {
    if(conservatively) {
      gl_Position = vsProjs[i];
      geomPass.voxelPos = (uniformWorldToVoxelsMatrix * viewProjectionInverseMatrix * vsProjs[i]).xyz;
    } else {
      gl_Position = viewProjectionMatrix * gl_in[i].gl_Position;
      geomPass.voxelPos = (uniformWorldToVoxelsMatrix * gl_in[i].gl_Position).xyz;
    }

    geomPass.worldPos = gl_in[i].gl_Position.xyz / gl_in[i].gl_Position.w;
    geomPass.normal = vertPass[i].normal;
    geomPass.uv = vertPass[i].uv;
    EmitVertex();
  }
  EndPrimitive();
}

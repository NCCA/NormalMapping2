#version 330 core
// base on http://apoorvaj.io/exploring-bump-mapping-with-webgl.html
layout(location=0)in vec3 inPos;
layout(location=1)in vec3 inTangent;
layout(location=2)in vec3 inBiTangent;
layout(location=3)in vec2 inUV;
layout( std140) uniform TransformUBO
{
  mat4 MVP;
  mat4 normalMatrix;
  mat4 M;
}transforms;

out vec2 frag_uv;
out vec3 ts_light_pos; // Tangent space values
out vec3 ts_view_pos;  //
out vec3 ts_frag_pos;  //

mat3 transpose(in mat3 inMatrix)
{
    vec3 i0 = inMatrix[0];
    vec3 i1 = inMatrix[1];
    vec3 i2 = inMatrix[2];

    mat3 outMatrix = mat3(
        vec3(i0.x, i1.x, i2.x),
        vec3(i0.y, i1.y, i2.y),
        vec3(i0.z, i1.z, i2.z)
    );

    return outMatrix;
}

void main(void)
{
    gl_Position = transforms.MVP * vec4(inPos, 1.0);
    ts_frag_pos = vec3(transforms.M * vec4(inPos, 1.0));
    vec3 vert_norm = cross(inBiTangent, inTangent);

    vec3 t = normalize(mat3(transforms.normalMatrix) * inTangent);
    vec3 b = normalize(mat3(transforms.normalMatrix) * inBiTangent);
    vec3 n = normalize(mat3(transforms.normalMatrix) * vert_norm);
    mat3 tbn = transpose(mat3(t, b, n));

    vec3 light_pos = vec3(1, 2, 0);
    ts_light_pos = tbn * light_pos;
    // Our camera is always at the origin
    ts_view_pos = tbn * vec3(0, 0, 0);
    ts_frag_pos = tbn * ts_frag_pos;

    frag_uv = inUV;
}

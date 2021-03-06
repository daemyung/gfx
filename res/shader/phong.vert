//----------------------------------------------------------------------------------------------------------------------

layout(location = 0) in vec3 vertex_position;
layout(location = 1) in vec3 vertex_normal;
layout(location = 2) in vec2 vertex_tex_coord;

//----------------------------------------------------------------------------------------------------------------------

layout(location = 0) out vec3 position;
layout(location = 1) out vec3 normal;

//----------------------------------------------------------------------------------------------------------------------

layout(binding = 0) uniform Matrix_info {
    mat4 model;
    mat4 view;
    mat4 projection;
    mat4 mv;
    mat4 mvp;
    mat4 normal;
} matrix_info;

//----------------------------------------------------------------------------------------------------------------------

void main()
{
    gl_Position = matrix_info.mvp * vec4(vertex_position, 1.0);
    position = (matrix_info.mv * vec4(vertex_position, 1.0)).xyz;
    normal = mat3(matrix_info.normal) * vertex_normal;
}

//----------------------------------------------------------------------------------------------------------------------
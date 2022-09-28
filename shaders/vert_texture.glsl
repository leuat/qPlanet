#version 330
#ifdef GL_ES
// Set default precision to medium
precision mediump int;
precision mediump float;
#endif

uniform mat4 mvp_matrix;
uniform mat3 rot_matrix;

in vec4 a_position;
in vec2 a_texcoord;
in vec3 a_normal;


out vec2 v_texcoord;
out vec3 v_normal;
out vec3 v_pos;




//! [0]
void main()
{
    // Calculate vertex position in screen space
    gl_Position = mvp_matrix * a_position;

    // Pass texture coordinate to fragment shader
    // Value will be automatically interpolated to fragments inside polygon faces
    v_texcoord = a_texcoord;
    v_normal = rot_matrix*a_normal;
    v_pos = a_position.xyz;//-vec3(0.0,0.0,-5);





}
//! [0]

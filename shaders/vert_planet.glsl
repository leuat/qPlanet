#version 330

#ifdef GL_ES
// Set default precision to medium
precision mediump int;
precision mediump float;
#endif

uniform mat4 mvp_matrix;



in vec4 a_position;
in vec2 a_texcoord;
in vec3 a_normal;

in vec3 a_light1;

out vec2 v_texcoord;
out vec3 v_normal;
out vec3 v_pos;
out vec3 v_light1;
out float v_height;

//! [0]
void main()
{
    // Calculate vertex position in screen space
    gl_Position = mvp_matrix * a_position;

    // Pass texture coordinate to fragment shader
    // Value will be automatically interpolated to fragments inside polygon faces
    v_texcoord = a_texcoord;
    v_normal = a_normal;
    v_light1 = a_light1;
    v_height = length(a_position);
    v_pos = a_position.xyz-vec3(0.0,0.0,-5);
}
//! [0]

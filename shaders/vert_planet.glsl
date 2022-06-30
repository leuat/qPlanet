#ifdef GL_ES
// Set default precision to medium
precision mediump int;
precision mediump float;
#endif

uniform mat4 mvp_matrix;

attribute vec4 a_position;
attribute vec2 a_texcoord;
attribute vec3 a_normal;

attribute vec3 a_light1;

varying vec2 v_texcoord;
varying vec3 v_normal;
varying vec3 v_pos;
varying vec3 v_light1;
varying float v_height;

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
    v_pos = a_position-vec3(0,0,-5);
}
//! [0]

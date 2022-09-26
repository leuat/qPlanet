#ifdef GL_ES
// Set default precision to medium
precision mediump int;
precision mediump float;
#endif

uniform mat4 mvp_matrix;
uniform mat3 rot_matrix;

attribute vec4 a_position;
attribute vec2 a_texcoord;
attribute vec3 a_normal;


varying vec2 v_texcoord;
varying vec3 v_normal;
varying vec3 v_pos;




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

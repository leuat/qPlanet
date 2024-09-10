#version 330

#ifdef GL_ES
// Set default precision to medium
precision mediump int;
precision mediump float;
#endif

uniform mat4 mvp_matrix;
uniform mat3 rot_matrix;

uniform float u_time;

layout(location = 0) in vec3 a_position;
layout(location = 1) in vec2 a_texcoord;
layout(location = 2) in vec3 a_normal;
layout(location = 3) in vec3 a_light;


out vec2 v_texcoord;
out vec3 v_normal;
out vec3 v_pos;
out vec3 v_light;
out vec4 s_pos;
out vec4 wind;



void main()
{
    // Calculate vertex position in screen space
    gl_Position = mvp_matrix * vec4(a_position.xyz,1.0);

    // Pass texture coordinate to fragment shader
    // Value will be automatically interpolated to fragments inside polygon faces
    v_texcoord = a_texcoord;
    v_normal = rot_matrix*a_normal;
    v_pos = a_position;//-vec3(0.0,0.0,-5);
    v_light = a_light;
    s_pos =  gl_Position;

    vec2 uv = a_position.xz;

    vec2 windDir = normalize(vec2(1,1));

    wind.xy = windDir;
    wind.zw = windDir * u_time/10.0;

    v_texcoord*=100.0;


}

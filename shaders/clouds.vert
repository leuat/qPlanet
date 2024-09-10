#version 330

#ifdef GL_ES
// Set default precision to medium
precision mediump int;
precision mediump float;
#endif

uniform mat4 mvp_matrix;
uniform mat3 rot_matrix;

uniform float u_time;
uniform vec3 camPos;

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



float hash(vec3 p)  // replace this by something better
{
    p  = fract( p*0.3183099+.1 );
        p *= 17.0;
    return fract( p.x*p.y*p.z*(p.x+p.y+p.z) );
}

float noise( in vec3 x )
{
    vec3 i = floor(x);
    vec3 f = fract(x);
    f = f*f*(3.0-2.0*f);

    return mix(mix(mix( hash(i+vec3(0,0,0)),
                        hash(i+vec3(1,0,0)),f.x),
                   mix( hash(i+vec3(0,1,0)),
                        hash(i+vec3(1,1,0)),f.x),f.y),
               mix(mix( hash(i+vec3(0,0,1)),
                        hash(i+vec3(1,0,1)),f.x),
                   mix( hash(i+vec3(0,1,1)),
                        hash(i+vec3(1,1,1)),f.x),f.y),f.z);
}

void main()
{
    // Calculate vertex position in screen space
    vec3 pos = a_position.xyz;
    float t = u_time/10.0;
    float s = 0.1;
    pos.y +=pow(noise((pos*10.2+vec3(t,0,0))*s),0.5);
    pos.y -=pow(noise((pos*25.8+vec3(0,t,t))*s*0.5),0.5);
//    pos.x +=camPos.x;
//    pos.z +=camPos.z;

  //  pos.x +=noise(pos*0.12+vec3(0,t,0));
  //  pos.z +=noise(pos*0.11+vec3(0,0,t));

    gl_Position = mvp_matrix * vec4(pos,1.0);

    // Pass texture coordinate to fragment shader
    // Value will be automatically interpolated to fragments inside polygon faces
    v_texcoord = a_texcoord;
    v_normal = rot_matrix*a_normal;
    v_pos = pos;//-vec3(0.0,0.0,-5);
    v_light = a_light;
    s_pos =  gl_Position;

    vec2 uv = a_position.xz;




}

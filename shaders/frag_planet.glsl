#version 330
#ifdef GL_ES

// Set default precision to medium
precision mediump int;
precision mediump float;
#endif

uniform sampler2D texture;

out vec2 v_texcoord;
out vec3 v_normal;
out vec3 v_pos;
out vec3 v_light1;
out float v_height;
out vec4 fragColor;
//! [0]


vec3 bottom = vec3(0.3,0.8,0.1);
vec3 top = vec3(1.0,1.0,1.0);
vec3 medium = vec3(0.6,0.4,0.2);

vec3 sides = vec3(0.4,0.4,0.4);

void main()
{
    // Set fragment color from texture

    vec3 color = vec3(0.9,0.4,0.2);
    float str = clamp(dot(normalize(v_normal),normalize(v_light1)),0.0,1.0);
    float t_bottom = 1.78;
    float t_medium = 1.82;
    float t_top = 1.87;
    float w = 30.0;
    float n = clamp((dot(v_normal, normalize(v_pos))),0.0,1.0);
    color = bottom*exp(-(pow((v_height-t_bottom)*w,2.0)));
    color += medium*exp(-(pow((v_height-t_medium)*w,2.0)));
    color += top*exp(-(pow((v_height-t_top)*w,2.0)));

    color = color*n + (1.0-n)*sides;

    fragColor = vec4(color*str,1);//texture(texture, v_texcoord);
 //   fragColor = vec4(1,1,1,1);//texture(texture, v_texcoord);
}
//! [0]


#ifdef GL_ES
// Set default precision to medium
precision mediump int;
precision mediump float;
#endif

uniform sampler2D texture;

varying vec2 v_texcoord;
varying vec3 v_normal;
varying vec3 v_pos;
varying vec3 v_light1;
varying float v_height;

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
    float w = 30;
    float n = clamp((dot(v_normal, normalize(v_pos))),0.0,1.0);
    color = bottom*exp(-(pow((v_height-t_bottom)*w,2.0)));
    color += medium*exp(-(pow((v_height-t_medium)*w,2.0)));
    color += top*exp(-(pow((v_height-t_top)*w,2.0)));

    color = color*n + (1.0-n)*sides;

    gl_FragColor = vec4(color*str,1);//texture2D(texture, v_texcoord);
 //   gl_FragColor = vec4(1,1,1,1);//texture2D(texture, v_texcoord);
}
//! [0]


#version 110
#ifdef GL_ES
// Set default precision to medium
precision mediump int;
precision mediump float;
#endif


//uniform sampler2D texture;

varying vec2 v_texcoord;
varying vec3 v_normal;
varying vec3 v_pos;

uniform vec3 u_color;
uniform float u_specular;
uniform float u_shininess;
uniform vec3 u_light_dir;
uniform vec3 u_light_col;

//! [0]



void main()
{
    // Set fragment color from texture

    vec3 color = u_color*clamp(dot(u_light_dir,v_normal),0.1,1.0);
//    vec3 color = v_normal*0.5 + vec3(0.5,0.5,0.5);//v_color*
//    color = color*clamp(dot(vec3(0,0,1),v_normal),0.1,1.0);
//    gl_FragColor = vec4(1.0,1.0,0.0,1.0);
    gl_FragColor = vec4(color,1);//texture(texture, v_texcoord);
//    gl_FragColor = vec4(1.0,1.0,1.0,1.0);
 //   fragColor = vec4(1,1,1,1);//texture(texture, v_texcoord);
}
//! [0]


#ifdef GL_ES
// Set default precision to medium
precision mediump int;
precision mediump float;
#endif

//uniform sampler2D texture;
varying vec3 v_color;
varying float v_specular;
varying float v_shininess;
varying vec3 v_light_dir;
varying vec3 v_light_col;

varying vec2 v_texcoord;
varying vec3 v_normal;
varying vec3 v_pos;

//! [0]



void main()
{
    // Set fragment color from texture

    vec3 color = v_color*clamp(dot(v_light_dir,v_normal),0.1,1.0);
//    vec3 color = v_normal*0.5 + vec3(0.5,0.5,0.5);//v_color*
//    color = color*clamp(dot(vec3(0,0,1),v_normal),0.1,1.0);

    gl_FragColor = vec4(color,1);//texture2D(texture, v_texcoord);
 //   gl_FragColor = vec4(1,1,1,1);//texture2D(texture, v_texcoord);
}
//! [0]


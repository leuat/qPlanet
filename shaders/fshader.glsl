#ifdef GL_ES
// Set default precision to medium
precision mediump int;
precision mediump float;
#endif

uniform sampler2D texture;

varying vec2 v_texcoord;
varying vec3 v_normal;

//! [0]
void main()
{
    // Set fragment color from texture
    gl_FragColor = vec4(v_normal,1);//texture2D(texture, v_texcoord);
 //   gl_FragColor = vec4(1,1,1,1);//texture2D(texture, v_texcoord);
}
//! [0]


#ifdef GL_ES
// Set default precision to medium
precision mediump int;
precision mediump float;
#endif

uniform sampler2D texture;

out vec2 v_texcoord;
out vec3 v_normal;

//! [0]
void main()
{
    // Set fragment color from texture
    fragColor = vec4(v_normal,1);//texture(texture, v_texcoord);
 //   fragColor = vec4(1,1,1,1);//texture(texture, v_texcoord);
}
//! [0]


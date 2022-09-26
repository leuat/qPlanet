#ifdef GL_ES
// Set default precision to medium
precision mediump int;
precision mediump float;
#endif

uniform sampler2D tex1;
uniform vec2 tex_scale;
uniform vec3 eye;
uniform vec3 eye_dir;

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
uniform vec3 u_color;
uniform float u_specular;
uniform float u_shininess;
uniform vec3 u_light_dir;
uniform vec3 u_light_col;



void main()
{
    vec4 spec = vec4(0.0);
    // Set fragment color from texture
    vec3 h = normalize(u_light_dir + normalize(eye));
           // compute the specular term into spec
    float intSpec = max(dot(h,v_normal), 0.0);
    spec = vec4(1.0,1.0,1.0,1.0)*u_specular * pow(intSpec,u_shininess);

    float intensity = max(dot(v_normal,u_light_dir), 0.0);

    vec4 texColor = texture2D(tex1, v_texcoord*tex_scale);
    vec4 diffColor = intensity *  vec4(u_light_col,1.0) * texColor;
    vec4 ambColor = vec4(u_color*0.0,1.0) * texColor;

     gl_FragColor = max(diffColor + spec, ambColor);
    //vec3 color = v_color*clamp(dot(v_light_dir,v_normal),0.1,1.0);
    //color = color*texture2D(tex1,v_texcoord*tex_scale).xyz;
//    vec3 color = v_normal*0.5 + vec3(0.5,0.5,0.5);//v_color*
//    color = color*clamp(dot(vec3(0,0,1),v_normal),0.1,1.0);

    //gl_FragColor = vec4(color,1);//texture2D(texture, v_texcoord);
 //   gl_FragColor = vec4(1,1,1,1);//texture2D(texture, v_texcoord);
}
//! [0]


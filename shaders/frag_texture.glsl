#version 330
#ifdef GL_ES

// Set default precision to medium
precision mediump int;
precision mediump float;
#endif

uniform sampler2D tex1;
uniform vec2 tex_scale;
uniform vec3 eye;
uniform vec3 eye_dir;
out vec4 fragColor;
//uniform sampler2D texture;
out vec3 v_color;
out float v_specular;
out float v_shininess;
out vec3 v_light_dir;
out vec3 v_light_col;



out vec2 v_texcoord;
out vec3 v_normal;
out vec3 v_pos;

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

    vec4 texColor = texture(tex1, v_texcoord*tex_scale);
    vec4 diffColor = intensity *  vec4(u_light_col,1.0) * texColor;
    vec4 ambColor = vec4(u_color*0.0,1.0) * texColor;

     fragColor = max(diffColor + spec, ambColor);
    //vec3 color = v_color*clamp(dot(v_light_dir,v_normal),0.1,1.0);
    //color = color*texture(tex1,v_texcoord*tex_scale).xyz;
//    vec3 color = v_normal*0.5 + vec3(0.5,0.5,0.5);//v_color*
//    color = color*clamp(dot(vec3(0,0,1),v_normal),0.1,1.0);

    //fragColor = vec4(color,1);//texture(texture, v_texcoord);
 //   fragColor = vec4(1,1,1,1);//texture(texture, v_texcoord);
}
//! [0]


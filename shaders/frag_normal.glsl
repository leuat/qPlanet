#version 330
#ifdef GL_ES

// Set default precision to medium
precision mediump int;
precision mediump float;
#endif

uniform sampler2D tex1;
uniform sampler2D tex2;
uniform vec2 tex_scale;
uniform vec2 normal_scale;
uniform vec3 eye;
uniform vec3 eye_dir;

uniform float u_normalIntensity;
out vec4 fragColor;
//uniform sampler2D texture;


in vec2 v_texcoord;
in vec3 v_normal;
in vec3 v_pos;

//! [0]
uniform vec3 u_color;
uniform float u_specular;
uniform float u_shininess;
uniform vec3 u_light_dir;
uniform vec3 u_light_col;
in vec3 T;
in vec3 B;



void main()
{
    mat3 TBN = mat3 (normalize(T), normalize(B), normalize(v_normal));
    vec3 normal = normalize (texture(tex2,v_texcoord*tex_scale).xyz*2.0 - 1.0);
//    normal = T;

    normal = normalize(TBN * normal);
    float t = u_normalIntensity;
    normal = v_normal*t +(1.0-t)*normal;
    vec4 spec = vec4(0.0);
    // Set fragment color from texture
    vec3 h = normalize(u_light_dir + normalize(eye));
           // compute the specular term into spec
    float intSpec = max(dot(h,normal), 0.0);
    spec = vec4(1.0,1.0,1.0,1.0)*u_specular * pow(intSpec,u_shininess);

    float intensity = max(dot(normal,u_light_dir), 0.0);

    vec4 texColor = texture(tex1, v_texcoord*tex_scale)*vec4(u_color,1.0);
    vec4 diffColor = intensity *  vec4(u_light_col,1.0) * texColor + vec4(0.1,0.05,0.1,1.0);;
    vec4 ambColor = vec4(u_color*0.0,1.0) * texColor;

     fragColor = max(diffColor + spec, ambColor);
}


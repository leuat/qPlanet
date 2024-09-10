
layout(location = 0, index = 0) out vec4 fragColor;

//uniform sampler2D texture;

in vec2 v_texcoord;
in vec3 v_normal;
in vec3 v_light;
in vec3 v_pos;
in vec3 s_pos;

uniform vec3 u_color;
uniform float u_specular;
uniform float u_shininess;
uniform vec3 u_light_dir;
uniform vec3 u_light_col;

//! [0]



void main()
{
    // Set fragment color from texture

    vec3 col = getAtmosphere(sun, s_pos, v_pos);


    vec3 color = u_color*clamp(dot(normalize(u_light_dir),v_normal),0.1,1.0);
    color = col * 0.5 + color*0.5;
    fragColor = vec4(color,1);
}
//! [0]


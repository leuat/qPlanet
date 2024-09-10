

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


out vec4 fragColor;





void main()
{
//    if (length(col)<0.4) col = vec3(0.1,0.08,0.09)*2.0;

    vec3 col = getAtmosphere(sun, s_pos, v_pos);

    float p = noise(v_pos*0.11);
    float p2 = int((noise(v_pos*40.1)*16))/16.0;

    vec3 color = saturate(u_color,p*0.8+0.2);


    if (abs(dot(v_normal,vec3(0,0,1)))>0.9 || abs(dot(v_normal,vec3(1,0,0)))>0.9) {
        color = saturate(color,0.7)*0.75;
    }
    else
        color = color*(0.75+p2*0.25);

    color = color*clamp(dot(normalize(u_light_dir),v_normal),0.15,1.0) *v_light;

    color = color*clamp(sqrt(sun.y)+0.15,0,1);

    fogColor = fogColor*clamp(sun.y,0,1);
    float dist = clamp(1-pow(length(v_pos-camPos),1.5)*0.0010,0.1,1.0);
//    dist = 0.;

    color = mix(color, fogColor, clamp(-(8+v_pos.y)*0.03,0.0,1.0));

//    col = mix(col, color*(col+vec3(1,1,1)), dist);
    col = mix(col, color, dist);
    fragColor = vec4(col,1.0);
}


//! [0]


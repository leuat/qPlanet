
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
uniform sampler2D tex1;
uniform sampler2D tex2;

//! [0]


out vec4 fragColor;


void main()
{

    vec3 col = getAtmosphere(sun, s_pos, v_pos);


    vec3 T = vec3(0,1,0);
    vec3 B = vec3(0,0,1);
    if (v_normal.y>0.9)
        T = vec3(1,0,0);
    if (v_normal.z>0.9)
        B = vec3(1,0,0);

    float tex_scale = 1.0;


    mat3 TBN = mat3 (normalize(T), normalize(B), normalize(v_normal));
    vec3 normal = normalize (texture(tex2,v_texcoord*tex_scale).xyz*2.0 - 1.0);
//    normal = T;

    normal = normalize(TBN * normal);
    float t = 0;
    normal = v_normal*t +(1.0-t)*normal;



    // Fix aspect
//    if (length(col)<0.4) col = vec3(0.1,0.08,0.09)*2.0;

//    float p = noise(v_pos*0.11);
  //  float p2 = int((noise(v_pos*40.1)*16))/16.0;

    vec3 color = texture(tex1, v_texcoord*tex_scale).rgb;
//    color +=texture(tex1, v_texcoord*tex_scale/4.89).rgb;
    color +=texture(tex1, v_texcoord*noise(v_pos*2.53)).rgb;
    color = color/2.0;


    if (abs(dot(normal,vec3(0,0,1)))>0.9 || abs(dot(normal,vec3(1,0,0)))>0.9) {
        color = saturate(color,0.7)*0.75;
    }
//    else
  //      color = color*(0.75+p2*0.25);

    color = color*clamp(dot(normalize(u_light_dir),normal),0.15,1.0) *v_light;

    color = color*clamp(sqrt(sun.y)+0.1,0,1);

    fogColor = fogColor*clamp(sun.y,0,1);
    float dist = clamp(1-pow(length(v_pos-camPos),1.5)*0.0010,0.1,1.0);
//    dist = 0.;

    color = mix(color, fogColor, clamp(-(8+v_pos.y)*0.03,0.0,1.0));

//    col = mix(col, color*(col+vec3(1,1,1)), dist);
    col = mix(col, color, dist);
    fragColor = vec4(col,1.0);
}


//! [0]


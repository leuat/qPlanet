
layout(location = 0, index = 0) out vec4 fragColor;

//uniform sampler2D texture;

in vec2 v_texcoord;
in vec3 v_normal;
in vec3 v_light;
in vec3 v_pos;
in vec4 s_pos;
in vec3 wind;

uniform vec3 u_color;
uniform float u_specular;
uniform float u_shininess;
uniform vec3 u_light_dir;
uniform vec3 u_light_col;
uniform float u_time;
uniform sampler2D tex1;


float iTime = u_time/15.0;


uniform float ls_time;
uniform float ls_cloudscale;
uniform float ls_cloudscattering;
uniform float ls_cloudintensity;
uniform float ls_cloudsharpness;
uniform float ls_shadowscale;
uniform float ls_distScale;
uniform float ls_cloudthickness;
uniform vec3 ls_cloudcolor;


float getPerlinVal(vec3 p) {

    float v = 0;
    float scale = 0.005;
    vec3 shift = vec3(10.2,0.11, 30.543);
    p+=shift*100009.11;
    for (int i=1;i<10;i++) {
        v+=noise((p*i*2 + shift*i*1.11)*scale + vec3(ls_time*0.5,0,0))/(2*i);
    }
    return pow(v,0.5);


}



float getCloud(float scale, float disp, out float light) {

    vec3 raySunDir = normalize(v_pos - sun);
    vec3 p = v_pos - raySunDir*10;
    float I = 0.0;
    light = 0.5;
    for (int i=0;i<10;i++) {
        float v = getPerlinVal(p);
        p-=raySunDir;
        if (v>0.0) {
            I = v;
            vec3 homeDir = normalize(p - camPos);
            for (int j=0;j<5;j++) {
                light += getPerlinVal(p)*0.05;
                p-=homeDir*1.0;
            }
            i = 10;
        }
        //if (getPerlinVal(p)>0.2)
        //    I*=0.9;
    }
    //light = clamp(light,0,1);
//    return clamp( pow(ls_cloudscattering/I, ls_cloudsharpness),0,1.0);
    return I;
}

float getCloudOrg(vec2 uv, float scale, float disp) {
    float y = 0.0f;
    // Perlin octaves
    const int NN = 5;
    for(int i=0;i < NN; i++) {
            float k = scale*i  + 0.11934;
            y+= texture( tex1, k*uv + vec2(0.1234*i*ls_time*0.015 - 0.04234*i*i*ls_time*0.015 + 0.9123559 + 0.23411*i , 0.31342  + 0.5923*i + disp) ).x;
    }
    // Normalize
    y /= 0.5f*NN;
    return clamp( pow(ls_cloudscattering/y, ls_cloudsharpness),0,1.0);
}

// returns cloud value, outputs normal to N.
/*
float getNormal(vec2 uv, float scale, float dst, out vec3 n, float nscale, float disp) {
    float height = getCloud(uv, scale, disp);
    const int N =5;
    for (int i=0;i<N;i++) {

            vec2 du1 = vec2(dst*cos((i)*2*3.14159 / (N)), dst*sin(i*2*3.14159/(N)));
            vec2 du2 = vec2(dst*cos((i+1)*2*3.14159 / (N)), dst*sin((i+1)*2*3.14159/(N)));

            float hx = getCloud(uv + du1, scale, disp);
            float hy = getCloud(uv + du2, scale, disp);

            vec3 d2 = vec3(0,height*nscale,0) - vec3(du1.x,hx*nscale,du1.y);
            vec3 d1 = vec3(0,height*nscale,0) - vec3(du2.x,hy*nscale,du2.y);

            n = n + normalize(cross(d1,d2));
    }
    n = normalize(n);
    return height;

}
*/
// main
// sky
vec3 getSkyColor(vec3 e) {
    e.y = (max(e.y,0.0)*0.8+0.2)*0.8;
    return vec3(pow(1.0-e.y,2.0), 1.0-e.y, 0.6+(1.0-e.y)*0.4) * 1.1;
}


void main() {
    float time = iTime * 5.0;

    vec3 skyColOrg = getAtmosphere(sun, s_pos.xyz, v_pos);
    vec3 skyCol = (getSkyColor(normalize(v_pos-camPos)) + skyColOrg)*0.5;
    vec3 worldSpacePosition = v_pos;
    vec3 N;
    vec3 lightDir = sun;

    vec3 viewDirection = normalize(camPos - worldSpacePosition);
    float dist = clamp(1.0/pow(length(0.5 + 0.0001*ls_distScale*(camPos - worldSpacePosition)),1.0),0,1);
    vec2 newPos = worldSpacePosition.xz*0.0005;
    float x = 1.;
//    float x = getNormal(newPos, 1.73252*ls_cloudscale*0.1, 0.005*ls_shadowscale, N, 0.05*ls_shadowscale, worldSpacePosition.y/1381.1234f + ls_time*0.0002);//getCloud(IN.uv, 1.729134);
  //  vec3 albedoColor = x*ls_cloudcolor;
    float light;
    float I = getCloud(0,0, light);
    vec3 albedoColor = ls_cloudcolor*light;
    float globalLight = clamp(dot(v_normal, lightDir),0.,1.);

    float spec = pow(max(0.0, dot(
        reflect(-lightDir, N),
        viewDirection)), 2);

    float NL = 0.3*ls_cloudintensity*(1 + spec + clamp((pow((dot(-N, lightDir)),1)),0.,1.));
    albedoColor*=NL*globalLight;
    albedoColor = mix(albedoColor, skyCol, I);

    float dist2 = length(camPos - v_pos.xyz);
 //   float dist2 = clamp(dist*0.005,0.3,1.);
    dist2 = clamp(dist2*0.0005,0.,1.);
    albedoColor = albedoColor*(1-dist2) + vec3(0.40,0.45,0.5)*(dist2);


    fragColor = vec4(albedoColor,1.0);

}

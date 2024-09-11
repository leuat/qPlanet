
in vec2 v_pos;

out vec4 fragColor;

uniform float ls_time;

float getPerlinVal(vec3 p) {

    float v = 0;
    float scale = 0.001;
    float sz = 0.8;
    vec3 shift = vec3(10.2,0.11, 30.543);
//    p+=shift*100009.11;
    for (int i=1;i<6;i++) {
        v+=noise((p*i*2)*vec3(scale,scale,scale*sz) + vec3(i*ls_time*0.1,i*ls_time*0.0001,0)+shift*i*1.11)/(2*i);
    }
//    return pow(v,1.2);
    return (0.4/(v+0.2))*(abs(noise(p*0.0002 + vec3(0,0,ls_time*0.02)))*4);
//    return pow(v,0.8);


}



float getCloud(vec3 v_pos,  float dist, out float light) {

    vec3 raySunDir = normalize(v_pos - sun);
    vec3 p = v_pos;// - raySunDir*10;
    float I = 0.0;
    light = 0.20;
    const float density = 0;

//    for (int i=0;i<10;i++) {
//        I = clamp(getPerlinVal(p)+density,0,1);

//        p-=raySunDir*10.0;
  //      if (v>0.0) {
            vec3 homeDir = normalize(p - camPos);
            float v = getPerlinVal(p) + density;
            I = v;
            for (int j=0;j<4;j++) {
                light += pow(v,2)*(0.25);
//                I+=v*0.15;
                p+=homeDir*100*(1.0-dist)*1.5;
                v = getPerlinVal(p);

            }

      //      i = 10;
    //        break;
    //    }
        //if (getPerlinVal(p)>0.2)
        //    I*=0.9;
  //  }
    //light = clamp(light,0,1);
//    return clamp( pow(ls_cloudscattering/I, ls_cloudsharpness),0,1.0);
    return clamp(I,0,1);
}


void main( )
{
    sundir = normalize(sun);
    vec2 fragCoord = vec2(-v_pos.x, v_pos.y)*0.8;
    // Normalized pixel coordinates (from -1 to 1)
    vec2 uv = fragCoord;///iResolution.xy * 2. - 1.;

    // Fix aspect
    uv.x *= 1600.0/1024.0;

    float ls_distScale = 10.0;


    vec3 O = vec3(0,0.0,0);
    vec3 U = normalize(cross(camForward, camRight));
    vec3 D = normalize(camForward*-1 + uv.y*normalize(camRight) + uv.x*U);

    float light = 1.0;
//    vec3 plane = vec3(0,sin(ls_time*0.1)*600+,0);
    const vec3 plane = vec3(0,1000,0);
    vec3 n = vec3(0,1,0);
    float t = dot((O - plane),n)/(dot(n,D));

    float I = 1.;
    if (D.y>-0.0) {
        // Distance damper
        float dist2 = clamp(abs(t)/(plane.y*100.), 0.,1.);
        I = clamp(getCloud(D*t, dist2,light)+dist2,0,1);
    }
    vec3 worldSpacePosition = D*t;

//    O = camPos;
    vec3 lightDir = sun;
    vec3 viewDirection = D;
    float dist = clamp(1.0/pow(length(0.5 + 0.0001*ls_distScale*(camPos - worldSpacePosition)),1.0),0,1);
    vec2 newPos = worldSpacePosition.xz*0.0005;
    float x = 1.;
//    float x = getNormal(newPos, 1.73252*ls_cloudscale*0.1, 0.005*ls_shadowscale, N, 0.05*ls_shadowscale, worldSpacePosition.y/1381.1234f + ls_time*0.0002);//getCloud(IN.uv, 1.729134);
  //  vec3 albedoColor = x*ls_cloudcolor;
    vec3 albedoColor = vec3(1,0.95,0.98)*light;
    float globalLight = clamp(dot(n, lightDir)+0.2,0.2,1.);

    float spec = pow(max(0.0, dot(
        reflect(-lightDir, n),
        viewDirection)), 2);
    float ls_intensity=4.0;
    float NL = 0.3*ls_intensity*(1 + spec + clamp((pow((dot(-n, lightDir)),1)),0.,1.));
    albedoColor*=NL*globalLight;








    vec3 col = vec3(0.);
    float L = escape(O, D, Ra);
    col = scatter(O, D, L, col);
    fragColor = vec4(mix(sqrt(col),albedoColor,1-I), 1.);
}

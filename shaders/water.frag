
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

float iTime = u_time/15.0;

vec2 iResolution = vec2(100000.,100000.);

//! [0]



/*
 * "Seascape" by Alexander Alekseev aka TDM - 2014
 * License Creative Commons Attribution-NonCommercial-ShareAlike 3.0 Unported License.
 * Contact: tdmaav@gmail.com
 */

const int NUM_STEPS = 2;
const float PI	 	= 3.141592;
const float EPSILON	= 1e-3;
#define EPSILON_NRM (0.00002)
//#define AA

// sea
const int ITER_GEOMETRY = 2;
const int ITER_FRAGMENT = 4;
const float SEA_HEIGHT = 0.25;
const float SEA_CHOPPY = 2.2;
const float SEA_SPEED = 0.05;
const float SEA_FREQ = 1.8;
vec3 SEA_BASE = vec3(0.0,0.11,0.18);
vec3 SEA_WATER_COLOR = vec3(0.8,0.9,0.6)*0.9;
#define SEA_TIME (1.0 + iTime * SEA_SPEED)
const mat2 octave_m = mat2(1.6,1.2,-1.2,1.6);

// math
mat3 fromEuler(vec3 ang) {
        vec2 a1 = vec2(sin(ang.x),cos(ang.x));
    vec2 a2 = vec2(sin(ang.y),cos(ang.y));
    vec2 a3 = vec2(sin(ang.z),cos(ang.z));
    mat3 m;
    m[0] = vec3(a1.y*a3.y+a1.x*a2.x*a3.x,a1.y*a2.x*a3.x+a3.y*a1.x,-a2.y*a3.x);
        m[1] = vec3(-a2.y*a1.x,a1.y*a2.y,a2.x);
        m[2] = vec3(a3.y*a1.x*a2.x+a1.y*a3.x,a1.x*a3.x-a1.y*a3.y*a2.x,a2.y*a3.y);
        return m;
}
float hash( vec2 p ) {
        float h = dot(p,vec2(127.1,311.7));
    return fract(sin(h)*43758.5453123);
}
float noise( in vec2 p ) {
    vec2 i = floor( p );
    vec2 f = fract( p );
        vec2 u = f*f*(3.0-2.0*f);
    return -1.0+2.0*mix( mix( hash( i + vec2(0.0,0.0) ),
                     hash( i + vec2(1.0,0.0) ), u.x),
                mix( hash( i + vec2(0.0,1.0) ),
                     hash( i + vec2(1.0,1.0) ), u.x), u.y);
}

// lighting
float diffuse(vec3 n,vec3 l,float p) {
    return pow(dot(n,l) * 0.4 + 0.6,p);
}
float specular(vec3 n,vec3 l,vec3 e,float s) {
    float nrm = (s + 8.0) / (PI * 8.0);
    return pow(max(dot(reflect(e,n),l),0.0),s) * nrm;
}

// sky
vec3 getSkyColor(vec3 e) {
    e.y = (max(e.y,0.0)*0.8+0.2)*0.8;
    return vec3(pow(1.0-e.y,2.0), 1.0-e.y, 0.6+(1.0-e.y)*0.4) * 1.1;
}

// sea
float sea_octave(vec2 uv, float choppy) {
    uv += noise(uv);
    vec2 wv = 1.0-abs(sin(uv));
    vec2 swv = abs(cos(uv));
    wv = mix(wv,swv,wv);
    return pow(1.0-pow(wv.x * wv.y,0.65),choppy);
}

float map(vec3 p) {
    float freq = SEA_FREQ;
    float amp = SEA_HEIGHT;
    float choppy = SEA_CHOPPY;
    vec2 uv = p.xz; uv.x *= 0.75;

    float d, h = 0.0;
    for(int i = 0; i < ITER_GEOMETRY; i++) {
        d = sea_octave((uv+SEA_TIME)*freq,choppy);
        d += sea_octave((uv-SEA_TIME)*freq,choppy);
        h += d * amp;
        uv *= octave_m; freq *= 1.9; amp *= 0.22;
        choppy = mix(choppy,1.0,0.2);
    }
    return p.y - h;
}

float map_detailed(vec3 p) {
    float freq = SEA_FREQ;
    float amp = SEA_HEIGHT;
    float choppy = SEA_CHOPPY;
    vec2 uv = p.xz; uv.x *= 0.75;

    float d, h = 0.0;
    for(int i = 0; i < ITER_FRAGMENT; i++) {
        d = sea_octave((uv+SEA_TIME)*freq,choppy);
        d += sea_octave((uv-SEA_TIME)*freq,choppy);
        h += d * amp;
        uv *= octave_m; freq *= 1.9; amp *= 0.22;
        choppy = mix(choppy,1.0,0.2);
    }
    return p.y - h;
}

vec3 getSeaColor(vec3 p, vec3 n, vec3 l, vec3 eye, vec3 dist, vec3 skyColor) {
    float fresnel = clamp(1.0 - dot(n,-eye), 0.0, 1.0);
    fresnel = min(pow(fresnel,3.0), 0.5);

    vec3 reflected = skyColor*0.5;//getSkyColor(reflect(eye,n));
    vec3 refracted = SEA_BASE + diffuse(n,l,80.0) * SEA_WATER_COLOR * 0.12;

    vec3 color = mix(refracted,reflected,fresnel);

    float atten = max(1.0 - dot(dist,dist) * 0.001, 0.0);
    color += SEA_WATER_COLOR * (p.y - SEA_HEIGHT) * 0.18 * atten;

    color += vec3(specular(n,l,eye,60.0))*atten;

    return color;
}

// tracing
vec3 getNormal(vec3 p, float eps) {
    vec3 n;
    n.y = map_detailed(p);
    n.x = map_detailed(vec3(p.x+eps,p.y,p.z)) - n.y;
    n.z = map_detailed(vec3(p.x,p.y,p.z+eps)) - n.y;
    n.y = eps;
    return normalize(n);
}

float heightMapTracing(vec3 ori, vec3 dir, out vec3 p) {
    float tm = 0.0;
    float tx = 1000.0;
    float hx = map(ori + dir * tx);
    if(hx > 0.0) {
        p = ori + dir * tx;
        return tx;
    }
    float hm = map(ori + dir * tm);
    float tmid = 0.0;
    for(int i = 0; i < NUM_STEPS; i++) {
        tmid = mix(tm,tx, hm/(hm-hx));
        p = ori + dir * tmid;
        float hmid = map(p);
                if(hmid < 0.0) {
                tx = tmid;
            hx = hmid;
        } else {
            tm = tmid;
            hm = hmid;
        }
    }
    return tmid;
}

vec3 getPixel(in vec2 coord, float time, vec3 skyCol) {

    // ray

    vec3 ori = (camPos-vec3(0,5,0));
 //   ori.y = -5;//clamp(ori.y*0.1,-5,5);
    ori.y = clamp(ori.y,-15*8,-15*8);
//    vec3 dir = normalize(vec3(uv.xy,-2.0)); dir.z += length(uv) * 0.14;
    vec3 dir = normalize(ori - v_pos.xyz);
//    dir = normalize(dir) * fromEuler(ang);

    // tracing
    vec3 p;
    heightMapTracing(ori,dir,p);
    vec3 dist = p - camPos;
    vec3 n = getNormal(p, dot(dist,dist) * EPSILON_NRM);
    vec3 light = normalize(sun);

    // color
    return getSeaColor(p*1,n,light,dir,dist, pow(skyCol,vec3(0.2)));
/*
    return mix(
//        getSkyColor(dir),
        skyCol,
        getSeaColor(p,n,light,dir,dist),
        pow(smoothstep(0.0,-0.2,dir.y),0.2)-0.1);
        */
}

// main
void main() {
    float time = iTime * 5.0;
    SEA_BASE *= clamp(sun.y,0.1,1.);
    vec3 skyCol = getAtmosphere(sun, s_pos.xyz, v_pos);
    vec3 color = getPixel(v_pos.xz, time, skyCol);
    float dist = length(camPos - v_pos.xyz);
 //   float dist2 = clamp(dist*0.005,0.3,1.);
    dist = clamp(dist*0.005,0.,1.);
    color = color*(1.0-dist) + skyCol*dist;
    // post
    //fragColor = vec4(pow(color,vec3(0.65)), 0.6);
    fragColor = vec4(pow(color,vec3(0.65)), 0.8);
    fragColor.xyz *= v_light;
}

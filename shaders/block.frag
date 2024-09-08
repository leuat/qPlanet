#version 330
#ifdef GL_ES
// Set default precision to medium
precision mediump int;
precision mediump float;
#endif

vec3 saturate(vec3 rgb, float adjustment)
{
    // Algorithm from Chapter 16 of OpenGL Shading Language
    const vec3 W = vec3(0.2125, 0.7154, 0.0721);
    vec3 intensity = vec3(dot(rgb, W));
    return mix(intensity, rgb, adjustment);
}

float hash(vec3 p)  // replace this by something better
{
    p  = fract( p*0.3183099+.1 );
        p *= 17.0;
    return fract( p.x*p.y*p.z*(p.x+p.y+p.z) );
}

float noise( in vec3 x )
{
    vec3 i = floor(x);
    vec3 f = fract(x);
    f = f*f*(3.0-2.0*f);

    return mix(mix(mix( hash(i+vec3(0,0,0)),
                        hash(i+vec3(1,0,0)),f.x),
                   mix( hash(i+vec3(0,1,0)),
                        hash(i+vec3(1,1,0)),f.x),f.y),
               mix(mix( hash(i+vec3(0,0,1)),
                        hash(i+vec3(1,0,1)),f.x),
                   mix( hash(i+vec3(0,1,1)),
                        hash(i+vec3(1,1,1)),f.x),f.y),f.z);
}
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


out vec4 fragColor;
uniform vec3 sun;
uniform vec3 camPos;
uniform vec3 camForward;
uniform vec3 camRight;

const float R0 = 6360e3; // Earth surface radius
const float Ra = 6380e3; // Earth atmosphere top raduis
const vec3 bR = vec3(58e-7, 135e-7, 331e-7); // Rayleigh scattering coefficient
const vec3 bMs = vec3(2e-5); // Mie scattering coefficients
const vec3 bMe = bMs * 1.1;
const float I = 6.; // Sun intensity
const vec3 C = vec3(0., -R0, 0.); // Earth center point

// Calculate densities $\rho$.
// Returns vec2(rho_rayleigh, rho_mie)
// Note that intro version is more complicated and adds clouds by abusing Mie scattering density. That's why it's a separate function
vec2 densitiesRM(vec3 p) {
        float h = max(0., length(p - C) - R0); // calculate height from Earth surface
        return vec2(exp(-h/8e3), exp(-h/12e2));
}

// Basically a ray-sphere intersection. Find distance to where rays escapes a sphere with given radius.
// Used to calculate length at which ray escapes atmosphere
float escape(vec3 p, vec3 d, float R) {
        vec3 v = p - C;
        float b = dot(v, d);
        float det = b * b - dot(v, v) + R*R;
        if (det < 0.) return -1.;
        det = sqrt(det);
        float t1 = -b - det, t2 = -b + det;
        return (t1 >= 0.) ? t1 : t2;
}

// Calculate density integral for optical depth for ray starting at point `p` in direction `d` for length `L`
// Perform `steps` steps of integration
// Returns vec2(depth_int_rayleigh, depth_int_mie)
vec2 scatterDepthInt(vec3 o, vec3 d, float L, float steps) {
        // Accumulator
        vec2 depthRMs = vec2(0.);

        // Set L to be step distance and pre-multiply d with it
        L /= steps; d *= L;

        // Go from point P to A
        for (float i = 0.; i < steps; ++i)
                // Simply accumulate densities
                depthRMs += densitiesRM(o + d * i);

        return depthRMs * L;
}


// Global variables, needed for size
vec2 totalDepthRM;
vec3 I_R, I_M;

vec3 sundir;

// Calculate in-scattering for ray starting at point `o` in direction `d` for length `L`
// Perform `steps` steps of integration
void scatterIn(vec3 o, vec3 d, float L, float steps) {

        // Set L to be step distance and pre-multiply d with it
        L /= steps; d *= L;

        // Go from point O to B
        for (float i = 0.; i < steps; ++i) {

                // Calculate position of point P_i
                vec3 p = o + d * i;

                // Calculate densities
                vec2 dRM = densitiesRM(p) * L;

                // Accumulate T(P_i -> O) with the new P_i
                totalDepthRM += dRM;

                // Calculate sum of optical depths. totalDepthRM is T(P_i -> O)
                // scatterDepthInt calculates integral part for T(A -> P_i)
                // So depthRMSum becomes sum of both optical depths
                vec2 depthRMsum = totalDepthRM + scatterDepthInt(p, sundir, escape(p, sundir, Ra), 4.);

                // Calculate e^(T(A -> P_i) + T(P_i -> O)
                vec3 A = exp(-bR * depthRMsum.x - bMe * depthRMsum.y);

                // Accumulate I_R and I_M
                I_R += A * dRM.x;
                I_M += A * dRM.y;
        }
}

// Final scattering function
// O = o -- starting point
// B = o + d * L -- end point
// Lo -- end point color to calculate extinction for
vec3 scatter(vec3 o, vec3 d, float L, vec3 Lo) {

        // Zero T(P -> O) accumulator
        totalDepthRM = vec2(0.);

        // Zero I_M and I_R
        I_R = I_M = vec3(0.);

        // Compute T(P -> O) and I_M and I_R
        scatterIn(o, d, L, 16.);

        // mu = cos(alpha)
        float mu = dot(d, sundir);

        // Calculate Lo extinction
        return Lo * exp(-bR * totalDepthRM.x - bMe * totalDepthRM.y)

        // Add in-scattering
                + I * (1. + mu * mu) * (
                        I_R * bR * .0597 +
                        I_M * bMs * .0196 / pow(1.58 - 1.52 * mu, 1.5));
}


void main()
{
    sundir = normalize(sun);
//    vec2 fragCoord = vec2(-s_pos.x/1600.0, s_pos.y/1024.0);
    vec2 fragCoord = vec2(-s_pos.x/1600.0/4.0, s_pos.y/1024.0/4.0 -0.1);
    // Normalized pixel coordinates (from -1 to 1)
    vec2 uv = fragCoord;///iResolution.xy * 2. - 1.;

    // Fix aspect
 //   uv.y /= 1600.0/1024.0;

    vec3 O = vec3(v_pos.x, v_pos.y-5000, v_pos.z);
//    O = camPos-vec3(0.0, -v_pos.y, 0.0);
    O = vec3(0,0,0);
//    uv.x *= 1600.0/1024.0;

    vec3 cf = normalize((camPos - vec3(v_pos.x*1., v_pos.y, v_pos.z*1.)+vec3(0,-1.0,0)));
//    vec3 cf = normalize(camForward);
    vec3 U = normalize(cross(cf, camRight));

//    vec3 D = normalize(camForward*-1 + uv.y*normalize(camRight) + uv.x*U);
    vec3 D = normalize(cf*-1 + uv.y*normalize(camRight) + uv.x*U);

//    O = camPos;

    vec3 col = vec3(0.);
    float L = escape(O, D, Ra);

    col = scatter(O, D, L, col);

    col = sqrt(col);
//    if (length(col)<0.4) col = vec3(0.1,0.08,0.09)*2.0;

    float p = noise(v_pos*0.11);
    float p2 = int((noise(v_pos*40.1)*16))/16.0;

    vec3 color = saturate(u_color,p*0.8+0.2);


    if (abs(dot(v_normal,vec3(0,0,1)))>0.9 || abs(dot(v_normal,vec3(1,0,0)))>0.9) {
        color = saturate(color,0.7)*0.75;
    }
    else
        color = color*(0.75+p2*0.25);

    color = color*clamp(dot(normalize(u_light_dir),v_normal),0.15,1.0) *v_light;

    color = color*clamp(sqrt(sun.y)+0.1,0,1);

    vec3 fogColor = vec3(0.35,0.37,0.4)*clamp(sun.y,0,1);
    float dist = clamp(1-pow(length(v_pos-camPos),1.5)*0.0010,0.1,1.0);
//    dist = 0.;

    color = mix(color, fogColor, clamp(-(8+v_pos.y)*0.03,0.0,1.0));

//    col = mix(col, color*(col+vec3(1,1,1)), dist);
    col = mix(col, color, dist);
    fragColor = vec4(col,1.0);
}


//! [0]


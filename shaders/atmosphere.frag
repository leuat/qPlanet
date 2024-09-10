
in vec2 v_pos;

out vec4 fragColor;

void main( )
{
    sundir = normalize(sun);
    vec2 fragCoord = vec2(-v_pos.x, v_pos.y)*0.8;
    // Normalized pixel coordinates (from -1 to 1)
    vec2 uv = fragCoord;///iResolution.xy * 2. - 1.;

    // Fix aspect
    uv.x *= 1600.0/1024.0;

    vec3 O = vec3(0,0.0,0);
    vec3 U = normalize(cross(camForward, camRight));
    vec3 D = normalize(camForward*-1 + uv.y*normalize(camRight) + uv.x*U);

//    O = camPos;

    vec3 col = vec3(0.);
    float L = escape(O, D, Ra);
    col = scatter(O, D, L, col);
    fragColor = vec4(sqrt(col), 1.);
}

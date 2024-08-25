#version 330

in vec2 vPosition;
out vec2 v_pos;

void main()
{
    v_pos = vPosition.xy;
    gl_Position = vec4(vPosition.xy, 0.0, 1.0);
}

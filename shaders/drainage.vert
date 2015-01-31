#version 400
in vec3 position;
in vec3 valuecolor;
uniform mat4 mvp;
out vec3 color;
uniform float exag;

void main(void)
{
    color = valuecolor;
    gl_Position  = mvp*vec4(position.x, position.y, position.z*exag, 1.0);
}

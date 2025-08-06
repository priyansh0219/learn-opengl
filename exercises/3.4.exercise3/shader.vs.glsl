#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;

uniform float hOffset;

out vec3 ourColor;

void main()
{
    vec3 pos = vec3(aPos.x + hOffset, aPos.y, aPos.z);
    gl_Position = vec4(pos, 1.0f);
    ourColor = vec3(abs(pos.x), pos.y, pos.z);
}

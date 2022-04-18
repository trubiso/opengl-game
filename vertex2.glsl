#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;

uniform float time;
uniform float time2;
uniform float time3;

out vec3 ourColor;

void main() {
    gl_Position = vec4(aPos.x - (time2 / 2.0f), aPos.y * (time) + time3, aPos.z, 1.0);
    ourColor = aColor;
}

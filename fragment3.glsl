#version 330 core
out vec4 FragColor;
  
in vec3 ourColor;
in vec2 TexCoord;

uniform sampler2D texture1;
uniform sampler2D texture2;
uniform float time;

void main() {
    //FragColor = texture(texture1, TexCoord) / vec4(ourColor / (sin(time * 1.3) + 1.0), 1.0) - texture(texture2, TexCoord);
    FragColor = texture(texture1, TexCoord) * texture(texture2, TexCoord);
}

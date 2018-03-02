#version 330 core

out vec4 FragColor;

in vec3 Normal;
in vec3 TexCoords;

uniform samplerCube skybox;

void main() {
  FragColor = vec4( Normal, 1.0 );
}

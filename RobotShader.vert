#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

out vec3 Normal;
out vec3 TexCoords;

uniform mat4 projection;
uniform mat4 modelView;

void main() {
  gl_Position = projection * modelView * vec4( aPos, 1.0 );
  TexCoords   = aPos;
  Normal      = aNormal;
}

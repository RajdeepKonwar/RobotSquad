#version 330 core

layout (location = 0) in vec3 aPos;

out vec3 TexCoords;

uniform mat4 projection;
uniform mat4 modelView;

void main() {
  vec4 l_pos  = projection * modelView * vec4( aPos, 1.0 );
  gl_Position = l_pos.xyww;
  TexCoords   = aPos;
}

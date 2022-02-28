R"(
#version 460 core

layout (location = 0) in vec3 aPos;

uniform vec2 Pos;

out vec4 FinalColor;

void main() {
  gl_Position = vec4(aPos.x + Pos.x, aPos.y + Pos.y, aPos.z, 1.0);
}
)"

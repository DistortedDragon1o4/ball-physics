R"(
#version 460 core

layout (location = 0) in vec2 aPos;

//uniform vec4 Color;

//out vec4 FinalColor;

void main() {
  gl_Position = vec4(aPos.x, aPos.y, 0.0f, 1.0f);
}
)"

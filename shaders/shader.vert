#version 460 core

layout (location = 0) in vec3 aPos;

uniform vec2 Pos;
uniform vec4 Color;

out vec4 FinalColor;

void main() {
  //vec2 Pos = {sin(Data.x) * Data.y, cos(Data.x) * Data.y};
  gl_Position = vec4(aPos.x + Pos.x, aPos.y + Pos.y, aPos.z, 1.0);
  FinalColor = Color;
}

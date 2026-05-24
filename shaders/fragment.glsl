#version 330 core

uniform vec3 triColor;

out vec4 fragColor;

void main() {
  fragColor = vec4(triColor, 1.0);
}

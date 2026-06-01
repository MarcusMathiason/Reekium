#version 330 core

in vec3 Color;
in vec2 Texcoord;

out vec4 fragColor;

uniform sampler2D texKitten;
uniform sampler2D texPuppy;

void main() {
  vec4 colKitten = texture(texKitten, Texcoord);
  vec4 colPuppy = texture(texPuppy, Texcoord);
  //fragColor = texture(tex, Texcoord) * vec4(Color, 1.0);
  fragColor = mix(colKitten, colPuppy, 0.5);
}

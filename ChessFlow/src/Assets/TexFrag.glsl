#version 330 core

in vec2 texPos;

out vec4 fragColor;

uniform sampler2D tex;

void main() {
	fragColor = texture(tex, texPos);
	if(fragColor.a < 0.1)
		discard;
}
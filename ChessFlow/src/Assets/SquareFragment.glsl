#version 330 core

out vec4 FragColor;

in vec2 texCoords;

uniform vec3 sqCol;
uniform bool useColor;
uniform bool toInvertColor;
uniform sampler2D tex;

void main() {
	if(useColor)
		FragColor = vec4(sqCol, 1.0);
	else {
		FragColor = texture(tex, texCoords);
		if(FragColor.w < 0.1)
			discard;
	}
	if(toInvertColor) {
		FragColor = vec4(1, 1, 1, 0) - FragColor;
	}
}
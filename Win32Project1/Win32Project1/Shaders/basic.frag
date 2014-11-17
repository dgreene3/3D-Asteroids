#version 400

in vec2 UV;
in vec3 Normal;

out vec4 FragColor;


uniform int test;

uniform sampler2D tex;

void main() {

	if(test == 0) {
		FragColor = vec4(1.0f, 0.0f, 0.0f, 1.0f);
	}else {
		FragColor = texture(tex, UV);
	}
	
}
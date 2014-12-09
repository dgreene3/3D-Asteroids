#version 400


// INPUTS
in vec3 PosW;
in vec3 Normal;
in vec2 UV;



// OUTPUTS
out vec4 FragColor;



// UNIFORMS
uniform int test;
uniform sampler2D tex;

// Constants
vec3 lightPosition = vec3(0.0f, 0.0f, 0.0f); // sun is at the world origin


// No specular highlights
vec3 ads() {
	vec3 n = normalize( vec3(Normal) );
	vec3 s = normalize( lightPosition - PosW );

	// No v or r since we are not going to have any specular hightlights


	vec3 textureColor = texture(tex, UV).xyz; // used as diffuse color


	vec3 ambient = vec3(0.86f, 0.60f, 0.22f) * 0.06;
	vec3 diffuse = textureColor * max( dot(s, n), 0.0f);
	

	return clamp((ambient + diffuse), 0.0f, 1.0f);
}

void main() {

	if(test == 0) {
		FragColor = vec4(1.0f, 0.0f, 0.0f, 1.0f);
	}else if(test == 1) {
		FragColor = texture(tex, UV);
	}else if(test == 2) {
		FragColor = vec4(1.0f, 1.0f, 0.0f, 1.0f);
	}else if(test == 3) {
		FragColor = vec4(0.0f, 1.0f, 1.0f, 1.0f);
	}else if(test == 4) {
		FragColor = vec4(ads(), 1.0f);
	}
	
}
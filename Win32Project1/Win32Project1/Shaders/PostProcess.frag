#version 400


layout(location = 0) out vec4 FragColor;


subroutine void postProcessEffectType(vec2 pos);
subroutine uniform postProcessEffectType postProcessEffect;

uniform sampler2D ColorMap;

uniform vec2 size;








subroutine( postProcessEffectType )
void FXAA(vec2 pos)
{
	float FXAA_SPAN_MAX = 8.0f;
	float FXAA_REDUCE_MIN = 1.0f / 128.0f;
	float FXAA_REDUCE_MUL = 1.0f / 8.0f;


	// Window size is fixed at 800x600 so just hard code this
	vec2 inverseTextureSize = vec2(1.0f/800.0f, 1.0f/600.0f);

	vec3 luma = vec3(0.299f, 0.587f, 0.114f);


	float lumaTR = dot(luma, texture2D(ColorMap, (pos + vec2(-1.0f, -1.0f) * inverseTextureSize) ).xyz);
	float lumaTL = dot(luma, texture2D(ColorMap, (pos + vec2( 1.0f, -1.0f) * inverseTextureSize) ).xyz);
	float lumaBR = dot(luma, texture2D(ColorMap, (pos + vec2(-1.0f,  1.0f) * inverseTextureSize) ).xyz);
	float lumaBL = dot(luma, texture2D(ColorMap, (pos + vec2( 1.0f,  1.0f) * inverseTextureSize) ).xyz);
	float lumaM = dot(luma, texture2D(ColorMap, pos).xyz);
	

	vec2 dir;
	dir.x = -((lumaTL + lumaTR) - (lumaBL + lumaBR));
	dir.y = ((lumaTL + lumaBL) - (lumaTR + lumaBR));



	float dirR = max((lumaTL + lumaTR + lumaBL + lumaBR) * (0.25f * FXAA_REDUCE_MUL), FXAA_REDUCE_MIN);
	float inverseDirAdj = (1.0f / min(abs(dir.x), abs(dir.y) + dirR));


	dir = min( vec2(FXAA_SPAN_MAX, FXAA_SPAN_MAX), 
				max( vec2(-FXAA_SPAN_MAX, -FXAA_SPAN_MAX), dir * inverseDirAdj) ) * inverseTextureSize;
	
	dir = dir * 0.4f; // test this value to see what looks the best. blurring to far otherwise

	vec3 result1 = (1.0f/2.0f) * ( texture2D(ColorMap, pos + (dir * vec2(1.0f/3.0f - 0.5f))).xyz + 
								   texture2D(ColorMap, pos + (dir * vec2(2.0f/3.0f - 0.5f))).xyz);


	vec3 result2 = result1 * (1.0f/2.0f) + (1.0f/4.0f) * (
									texture2D(ColorMap, pos + (dir * vec2(0.0f/3.0f - 0.5f))).xyz + 
								    texture2D(ColorMap, pos + (dir * vec2(3.0f/3.0f - 0.5f))).xyz);


	
	float lumaMin = min(lumaM, min(min(lumaTL, lumaTR), min(lumaBL, lumaBR)));
	float lumaMax = max(lumaM, max(min(lumaTL, lumaTR), max(lumaBL, lumaBR)));
	
	float lumaResult2 = dot(luma, result2);

	if(lumaResult2 < lumaMin || lumaResult2 > lumaMax) {
		FragColor = vec4(result1, 1.0f);
	}else {
		FragColor = vec4(result2, 1.0f);
	}


}







subroutine( postProcessEffectType )
void drawTextureOnly(vec2 pos)
{
	FragColor = texture2D(ColorMap, pos);
}


subroutine( postProcessEffectType )
void drawPosterized(vec2 pos) {
    vec4 texColor = texture2D( ColorMap, pos);
	vec3 c = texColor.xyz;
	c = pow(c, vec3(0.6, 0.6, 0.6));
	c = c * 8;
	c = floor(c);
	c = c / 8;
	c = pow(c, vec3(1.0/0.6));
	texColor = vec4(c.x, c.y, c.z, 1.0);
    FragColor = texColor;
}


subroutine( postProcessEffectType )
void predatorVision(vec2 pos)
{
    vec4 texColor = texture2D( ColorMap, pos);
	vec3 tc = vec3(1.0, 1.0, 1.0);

	vec3 c = texColor.xyz;
	vec3 colors[3];
	colors[0] = vec3(0.0, 0.0, 1.0);
	colors[1] = vec3(1.0, 1.0, 0.0);
	colors[2] = vec3(1.0, 0.0, 0.0);
	float lum = dot(vec3(0.30, 0.59, 0.11), c.rgb); //(c.r + c.g + c.b) / 3.0;
	int ix = (lum < 0.5) ? 0:1;
	tc = mix(colors[ix], colors[ix + 1], (lum - float(ix) * 0.5) / 0.5);

    FragColor = vec4(tc, 1.0);
}


subroutine( postProcessEffectType )
void dreamVision(vec2 pos)
{

    vec4 texColor = texture2D( ColorMap, pos);

	texColor += texture(ColorMap, pos + 0.001);
	texColor += texture(ColorMap, pos + 0.003);
	texColor += texture(ColorMap, pos + 0.005);
	texColor += texture(ColorMap, pos + 0.007);
	texColor += texture(ColorMap, pos + 0.009);
	texColor += texture(ColorMap, pos + 0.011);

	texColor += texture(ColorMap, pos - 0.001);
	texColor += texture(ColorMap, pos - 0.003);
	texColor += texture(ColorMap, pos - 0.005);
	texColor += texture(ColorMap, pos - 0.007);
	texColor += texture(ColorMap, pos - 0.009);
	texColor += texture(ColorMap, pos - 0.011);

	texColor.rgb = vec3((texColor.r + texColor.g + texColor.b) / 3.0);
	texColor = texColor / 9.5;
	
    FragColor = texColor;
}


subroutine( postProcessEffectType )
void scanLinesChromatic(vec2 pos)
{
    float global_pos = (pos.y + 0.2) * 300.0;
    float wave_pos = cos((fract( global_pos ) - 0.5)*3.14);
    vec4 texColor = texture2D(ColorMap, pos );

	texColor.r = texture2D(ColorMap, pos).r;
    texColor.g = texture2D(ColorMap, pos + 0.004).g;
    texColor.b = texture2D(ColorMap, pos - 0.004).b;

    FragColor = mix(vec4(0,0,0,0), texColor, wave_pos);
}



subroutine( postProcessEffectType )
void basicNoise(vec2 pos)
{
    vec4 texColor = texture( ColorMap, pos);
	texColor = texColor * fract(sin(dot(pos.xy, vec2(12.9898,78.233))) * 43758.5453) * 4;
    FragColor = texColor;
}




void main() {
	vec2 pos = vec2(gl_FragCoord) / size; // Instead of passing in texCoord attribute!

	postProcessEffect(pos);
}
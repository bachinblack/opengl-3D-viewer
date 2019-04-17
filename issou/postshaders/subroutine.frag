#version 430

in vec2 uv;

out vec4 final_color;

uniform sampler2D tex;

subroutine vec4 filter_t();
subroutine uniform filter_t Filtering;

subroutine(filter_t)
vec4 Blurring()
{
	const float f = 1.0 / 300; 

	vec2 offsets[9] = vec2[](
		uv + vec2(-f, f),
		uv + vec2(0, f),
		uv + vec2(f, f),
		uv + vec2(-f, 0),
		uv,
		uv + vec2(f, 0),
		uv + vec2(-f, -f),
		uv + vec2(0, -f),
		uv + vec2(f, -f)
	);

	float kernelWeight[9] = float[](
		1, 2, 1,
		2, 4, 2,
		1, 2, 1
	);

	vec4 tmpColor = vec4(0, 0, 0, 0);
	for (int i = 0; i<9; ++i)
	{
		tmpColor += texture(tex, offsets[i]) * (kernelWeight[i] / 16);
	}
	return tmpColor;
}


subroutine(filter_t)
vec4 Sharpening()
{
	const float f = 1.0 / 300; 

	vec2 offsets[9] = vec2[](
		uv + vec2(-f, f),
		uv + vec2(0, f),
		uv + vec2(f, f),
		uv + vec2(-f, 0),
		uv,
		uv + vec2(f, 0),
		uv + vec2(-f, -f),
		uv + vec2(0, -f),
		uv + vec2(f, -f)
	);

	float kernel[9] = float[](
		-1, -1, -1,
		-1,  9, -1,
		-1, -1, -1
	);

	vec4 tmpColor = vec4(0, 0, 0, 0);
	for (int i = 0; i<9; ++i)
	{
		tmpColor += texture(tex, offsets[i]) * (kernel[i]);
	}
	return tmpColor;
}


subroutine(filter_t)
vec4 EdgeDetection()
{
	const float f = 1.0 / 300; 

	vec2 offsets[9] = vec2[](
		uv + vec2(-f, f),    uv + vec2(0, f),    uv + vec2(f, f),
		uv + vec2(-f, 0),          uv,           uv + vec2(f, 0),
		uv + vec2(-f, -f),   uv + vec2(0, -f),   uv + vec2(f, -f)
	);


	vec4 resX = (texture(tex, offsets[6])+(2*texture(tex, offsets[7]))+ texture(tex, offsets[8])) - (texture(tex, offsets[0])+(2*texture(tex, offsets[1]))+ texture(tex, offsets[2]));
	vec4 resY = (texture(tex, offsets[2])+(2*texture(tex, offsets[5]))+ texture(tex, offsets[8])) - (texture(tex, offsets[0])+(2*texture(tex, offsets[3]))+ texture(tex, offsets[6]));
		
	float tmp = sqrt(pow(resX.r, 2) + pow(resY.r, 2));
	final_color.r = tmp;
		
	tmp = sqrt(pow(resX.g, 2) + pow(resY.g, 2));
	final_color.g = tmp;
		
	tmp = sqrt(pow(resX.b, 2) + pow(resY.b, 2));
	final_color.b = tmp;

	return vec4(final_color.rrr * 0.5, 1);
}


// chosing one of the three available subroutines
void main() {
    final_color = Filtering();
}
#version 430

in vec2 uv;

out vec4 final_color;

uniform sampler2D tex;
uniform bool depth;


const float f = 1.0 / 300; 


float LinearizeDepth(in vec2 uv)
{
    float zNear = .1;    // TODO: Replace by the zNear of your perspective projection
    float zFar  = 5000.0; // TODO: Replace by the zFar  of your perspective projection
    float depth = texture(tex, uv).x;
    return (2.0 * zNear) / (zFar + zNear - depth * (zFar - zNear));
}



void main()
{

	if (depth) {
		
		final_color = texture(tex, uv).rrra;
		float d = LinearizeDepth(uv);
		final_color = vec4(d,d,d,1.0);
	} else {

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

		final_color = vec4(final_color.rrr * 0.5, 1);
	}
}
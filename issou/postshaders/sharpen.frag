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
		final_color = tmpColor;
	}
}
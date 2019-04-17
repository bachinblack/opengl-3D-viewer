#version 430

in vec2 uv;

out vec4 final_color;

uniform sampler2D tex;
uniform bool depth;


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
		
		vec4 tmpColor = texture(tex, uv).rgba;
		final_color.r = (tmpColor.r * .393) + (tmpColor.g *.769) + (tmpColor.b * .189);
		final_color.g = (tmpColor.r * .349) + (tmpColor.g *.686) + (tmpColor.b * .168);
		final_color.b = (tmpColor.r * .272) + (tmpColor.g *.534) + (tmpColor.b * .131);
		final_color.a = 1.0f;
	}
}
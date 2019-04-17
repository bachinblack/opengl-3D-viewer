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
		
		float grey = (tmpColor.r + tmpColor.g + tmpColor.b) / 3;
		
		final_color = vec4(grey, grey, grey, 1.0f);
	}
}
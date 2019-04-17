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
	//rrra because depth textures are not usual textures, they have only one channel
	final_color = (depth) ? texture(tex, uv).rrra : texture(tex, uv).rgba;

	float d;
	if (depth) {
	   d = LinearizeDepth(uv);
	   final_color = vec4(d,d,d,1.0);

	}
    //final_color = texture(tex, uv).rrra;
	//final_color = vec4(1.0,0.0,0.0,1.0);
}
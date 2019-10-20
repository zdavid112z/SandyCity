#include "lighting.glsl"

layout (location = 0) out vec4 outColor;

in VertexOutput
{
	vec3 position;
	vec2 texCoord;
	vec3 normal;
	vec3 tangent;
	vec3 bitangent;
	vec3 cameraPosition;
} v_input;


uniform MaterialUniformBuffer
{
	vec4 u_Color;
	float u_LightFactor;
	vec3 u_Padding;
};

uniform sampler2D u_Texture;

void main()
{
	vec4 c = texture(u_Texture, v_input.texCoord);
	float l = dot(normalize(vec3(0.2, 1, -0.5)), v_input.normal);
	if(l < 0.5)
		l = 0.5;
	outColor = c * u_Color;
	vec4 lit = vec4(outColor.rgb * l, outColor.a);
	outColor = mix(outColor, lit, u_LightFactor);
}
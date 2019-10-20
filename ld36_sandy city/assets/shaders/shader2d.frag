layout (location = 0) out vec4 outColor;

in VertexOutput
{
	vec2 texCoords;
	vec4 textureMaskAlphaLimits;
	vec4 color;
	vec4 borderColor;
	vec4 borderLimitsOffset;
} v_input;

uniform sampler2D u_Textures[32];

void main()
{
	vec4 finalColor = vec4(v_input.color.xyz, 1.0);
	//float aa = finalColor.a;
	if(v_input.textureMaskAlphaLimits.x != -1)
	{
		vec4 texColor = texture(u_Textures[int(v_input.textureMaskAlphaLimits.x)], v_input.texCoords);
		finalColor = finalColor * texColor;
		//aa = finalColor.a;
	}
	finalColor.a = smoothstep(v_input.textureMaskAlphaLimits.z, v_input.textureMaskAlphaLimits.w, finalColor.a);
	finalColor.a *= v_input.color.a;
	if(v_input.borderColor.a > 0)
	{
		float borderDistance =  texture(u_Textures[int(v_input.textureMaskAlphaLimits.x)], v_input.texCoords + v_input.borderLimitsOffset.zw).a;
		float borderAlpha = smoothstep(v_input.borderLimitsOffset.x, v_input.borderLimitsOffset.y, borderDistance) * v_input.borderColor.a;

		float alpha = (finalColor.a + (1 - finalColor.a) * borderAlpha) * v_input.color.a;
		//finalColor = vec4(alpha, alpha, alpha, 1);
		finalColor = vec4(mix(v_input.borderColor.xyz, finalColor.xyz, finalColor.a / alpha), alpha);
	}
	if(v_input.textureMaskAlphaLimits.y != -1)
	{
		float maskColor = texture(u_Textures[int(v_input.textureMaskAlphaLimits.y)], v_input.texCoords).r;
		finalColor.a *= maskColor;
	}
	//finalColor.a = aa;
	if(finalColor.a == 0)
		discard;
	outColor = finalColor;
}
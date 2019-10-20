layout (location = 0) in vec4 aMinMax;
layout (location = 1) in vec4 aTextureMaskAlphaLimits;
layout (location = 2) in vec4 aBorderColor;
layout (location = 3) in vec4 aBorderLimitsOffset;
layout (location = 4) in vec4 aSizePosition;
layout (location = 5) in vec4 aColor1;
layout (location = 6) in vec4 aColor2;
layout (location = 7) in vec4 aColor3;
layout (location = 8) in vec4 aColor4;
layout (location = 9) in mat4 aModel;

uniform UniformBuffer2D
{
	mat4 sys_ViewProjection;
};

out VertexOutput
{
	vec2 texCoords;
	vec4 textureMaskAlphaLimits;
	vec4 color;
	vec4 borderColor;
	vec4 borderLimitsOffset;
} v_output;


void main()
{
	mat4 mvp = sys_ViewProjection * aModel;
	if(gl_VertexID == 0)
	{
		v_output.texCoords = aMinMax.xw;
		v_output.color = aColor1;
		gl_Position = mvp * vec4(-aSizePosition.x + aSizePosition.z, aSizePosition.y + aSizePosition.w, 0, 1);
	}
	else if(gl_VertexID == 1)
	{
		v_output.texCoords = aMinMax.xy;
		v_output.color = aColor2;
		gl_Position = mvp * vec4(-aSizePosition.x + aSizePosition.z, -aSizePosition.y + aSizePosition.w, 0, 1);
	}
	else if(gl_VertexID == 2)
	{
		v_output.texCoords = aMinMax.zw;
		v_output.color = aColor3;
		gl_Position = mvp * vec4(aSizePosition.x + aSizePosition.z, aSizePosition.y + aSizePosition.w, 0, 1);
	}
	else
	{
		v_output.texCoords = aMinMax.zy;
		v_output.color = aColor4;
		gl_Position = mvp * vec4(aSizePosition.x + aSizePosition.z, -aSizePosition.y + aSizePosition.w, 0, 1);
	}
	v_output.textureMaskAlphaLimits = aTextureMaskAlphaLimits;
	v_output.borderColor = aBorderColor;
	v_output.borderLimitsOffset = aBorderLimitsOffset;
}

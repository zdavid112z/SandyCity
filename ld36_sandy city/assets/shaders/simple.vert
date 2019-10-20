layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec2 aTexCoord;
layout (location = 2) in vec3 aNormal;
layout (location = 3) in vec3 aTangent;
layout (location = 4) in vec3 aBitangent;

uniform MVPUniformBuffer
{
	mat4 sys_MVPMatrix;
	mat4 sys_ModelMatrix;
	vec4 sys_CameraPosition;
};

out VertexOutput
{
	vec3 position;
	vec2 texCoord;
	vec3 normal;
	vec3 tangent;
	vec3 bitangent;
	vec3 cameraPosition;
} v_output;


void main()
{
	gl_Position = sys_MVPMatrix * vec4(aPosition, 1);
	v_output.position = (sys_ModelMatrix * vec4(aPosition, 1)).xyz;
	v_output.texCoord = aTexCoord;
	v_output.normal = normalize((sys_ModelMatrix * vec4(aNormal, 0)).xyz);
	v_output.tangent = (sys_ModelMatrix * vec4(aTangent, 0)).xyz;
	v_output.bitangent = (sys_ModelMatrix * vec4(aBitangent, 0)).xyz;
	v_output.cameraPosition = vec3(sys_CameraPosition);
}

#version 430 core
layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 uv;

out VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
} vs_out;


uniform mat4 modelMatrix;
uniform mat4 viewProjMatrix;
uniform mat4 normalMatrix;
uniform vec4 clippingPlane;

void main()
{


	vs_out.Normal = normalize((normalMatrix * vec4(normal,1)).xyz);
	vs_out.TexCoords = uv;
	


	vec4 position_world_ = modelMatrix * vec4(position, 1);
	
	//world space
	vs_out.FragPos = position_world_.xyz;

	//camera space
	gl_Position = viewProjMatrix * position_world_;

	//clipping the rendered scene for the reflection and refraction textures of water
	gl_ClipDistance[0] = dot(position_world_,clippingPlane);



}
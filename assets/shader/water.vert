#version 430 core

in vec3 position;
out vec3 vecToCamera;
out vec3 vecFromSun;

out vec4 clipSpace;
out vec2 texCoords;
uniform mat4 viewProjMatrix;
uniform mat4 modelMatrix;
uniform vec3 cameraPos;
uniform vec3 sunPos;
uniform float tiling;


void main(void) {

	vec4 worldSpace = modelMatrix * vec4(position.x, position.y, position.z, 1.0);
	clipSpace = viewProjMatrix * worldSpace;
	gl_Position = clipSpace;

	vecToCamera = cameraPos-worldSpace.xyz;
	texCoords = vec2(position.x/2.0f+0.5f,position.z/2.0f+0.5f) * tiling;
	vecFromSun = worldSpace.xyz - sunPos;
	
 	
}


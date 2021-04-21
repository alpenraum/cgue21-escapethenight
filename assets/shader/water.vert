#version 430 core

in vec3 position;


out VS_OUT {
    vec3 vecToCamera;
    vec3 vecFromLight[10];
    vec3 lightColor[10];
	out vec4 clipSpace;
	out vec2 texCoords;
} vs_out;


uniform mat4 viewProjMatrix;
uniform mat4 modelMatrix;
uniform vec3 cameraPos;

struct Light{
	vec3 position;
	vec3 color;
};

uniform Light lights[10];
//how often the texture is repeated on the water surface
uniform float tiling;


void main(void) {

	vec4 worldSpace = modelMatrix * vec4(position, 1.0);
	vs_out.clipSpace = viewProjMatrix * worldSpace;
	gl_Position = vs_out.clipSpace;

	vs_out.vecToCamera = cameraPos-worldSpace.xyz;
	vs_out.texCoords = vec2(position.x/2.0f+0.5f,position.z/2.0f+0.5f) * tiling;

	for(int i=0;i<lights.length();i++){
		vs_out.vecFromLight[i] = worldSpace.xyz - lights[i].position;
		vs_out.lightColor[i] = lights[i].color;
	}
	
	
 	
}


#version 430 core
in vec3 position;
in vec2 uv;
in mat4 modelViewMatrix;
in vec3 hue;
in float alpha;

out vec2 texCoords;
out float alpha_P;
out vec3 hue_P;

uniform mat4 projectionMatrix;


void main(void){
	texCoords = uv;
	gl_Position = projectionMatrix * modelViewMatrix * vec4(position, 1.0);

	alpha_P = alpha;
	hue_P =hue;
}
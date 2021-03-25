#version 430 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec2 uv;

out vec2 texCoords;

uniform mat4 projectionMatrix;
uniform mat4 modelViewMatrix;

void main(void){
	texCoords = uv;
	gl_Position = projectionMatrix * modelViewMatrix * vec4(position, 1.0);

}
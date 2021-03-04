#version 430 core

layout (location = 0) in vec3 aPos;
layout (location = 2) in vec2 uv;


out vec2 TexCoords;
    


void main(){
	TexCoords = uv;
	gl_Position = vec4(aPos,1.0f);
}
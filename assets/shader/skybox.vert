#version 430 core

in vec3 aPos;

out vec3 TexCoords;

uniform mat4 viewProjMatrix;

void main(){
	TexCoords = aPos;
	vec4 pos = viewProjMatrix * vec4(aPos, 1.0);
	gl_Position = pos.xyww;
}

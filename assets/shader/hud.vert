#version 430 core


in vec3 position;
in vec2 textureCoords;

uniform mat4 transformationMatrix;

out VS_OUT{
    vec2 texCoords;
} vs_out;

void main(void){

	gl_Position = transformationMatrix * vec4(position, 1.0);
	vs_out.texCoords = vec2(textureCoords.x,1.0f-textureCoords.y);
}
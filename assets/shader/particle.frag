#version 430 core

in vec2 texCoords;

out vec4 FragColor;

uniform float alpha;
uniform vec3 hue;

uniform sampler2D particleTexture;

void main(void){
	vec4 color =texture(particleTexture, texCoords);
	
	color.rgb = color.rgb*hue;
	color.a = color.a * alpha;
	FragColor = color;

}
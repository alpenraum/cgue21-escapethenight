#version 430 core

out vec4 FragColor;

uniform float alpha;
uniform vec3 hue;

void main(void){
	vec3 color =vec3(1.0f);
	color = normalize(color+hue);
	FragColor = vec4(color,alpha);

}
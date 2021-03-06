#version 430 core

in vec2 texCoords;
in float alpha_P;
in vec3 hue_P;
out vec4 FragColor;


uniform sampler2D particleTexture;
uniform float gamma;

void main(void){
	vec4 color =texture(particleTexture, texCoords);
	
	color.rgb = color.rgb*hue_P;
	color.a = color.a * alpha_P;
	FragColor = color;

	FragColor.rgb = pow(FragColor.rgb, vec3(1.0/gamma));

}
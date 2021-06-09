#version 430 core
in vec2 TexCoords;

uniform sampler2D scene;
uniform sampler2D bloomBlur;
uniform sampler2D blurTexture;
uniform bool horizontal;
uniform float weight[5] = float[] (0.227027, 0.1945846,0.1216216,0.054054,0.016216);

uniform float gamma;
uniform int renderPass;

out vec4 FragColor;


void main(){

/*
0 = gaussian blur
1 = combine with texture
*/

vec3 result;
switch (renderPass){
    case 0:
        vec2 texOffest  = 1.0/textureSize(blurTexture,0);
	    result = texture(blurTexture,TexCoords).rgb * weight[0];

	    if(horizontal){
            for(int i=1;i<5;++i){
                result +=texture(blurTexture,TexCoords+vec2(texOffest.x*i,0.0)).rgb * weight[i];
                result +=texture(blurTexture,TexCoords-vec2(texOffest.x*i,0.0)).rgb * weight[i];
            }
        }else{
            for(int i=1;i<5;++i){
                result +=texture(blurTexture,TexCoords+vec2(0.0,texOffest.y*i)).rgb * weight[i];
                result +=texture(blurTexture,TexCoords-vec2(0.0,texOffest.y*i)).rgb * weight[i];
            }
        }

        FragColor = vec4(result,1.0);
    break;
    case 1:
        float exposure = 3.0; //can be changed. More = more Bloom
        const float gamma = 2.2;
        vec3 hdrColor  = texture(scene, TexCoords).rgb;
        vec3 bloomColor = texture(bloomBlur,TexCoords).rgb;

        hdrColor +=bloomColor;

        result = vec3(1.0f)- exp(-hdrColor*exposure);

        result = pow(result, vec3(1.0/gamma));
        FragColor = vec4(result,1.0);
        FragColor.rgb = hdrColor;//pow(FragColor.rgb, vec3(1.0f/gamma)); //linear -> srgb
    break;


}


}




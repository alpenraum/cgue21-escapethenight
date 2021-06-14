#version 430 core


in VS_OUT{
    vec2 texCoords;
} fs_in;

out vec4 FragColor;

uniform sampler2D texGUI;
uniform sampler2D texSCALE;
uniform float sanity;

void main(void){
    vec4 guiFrag = texture(texGUI,fs_in.texCoords).rgba;
	vec4 scaleFrag = texture(texSCALE,fs_in.texCoords).rgba;

    if(fs_in.texCoords.y<1.0f-sanity){
        scaleFrag.a = 0.0f;
    }


    if(scaleFrag.a ==0){
        FragColor = guiFrag;
    }else{
    FragColor = vec4(scaleFrag.rgb+guiFrag.rgb,scaleFrag.a);
    }

    if(fs_in.texCoords.x>0.95f){
        FragColor.a=0.0f;
    }
    

}
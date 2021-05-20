#version 430 core

const int MAX_JOINTS = 50;
const int MAX_WEIGHTS = 4;

layout(location=0) in vec3 position_in;
layout(location=1)in vec3 normal_in;
layout(location=2)in vec2 texCoords_in;
layout(location=3)in ivec4 jointIndices_in;
layout(location=4)in vec4 weights_in;

out VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
    vec2 lightMapCoords;
} vs_out;

uniform mat4 jointTransforms[MAX_JOINTS];
uniform mat4 viewProjMatrix;
uniform mat4 modelMatrix;
uniform vec4 clippingPlane;

void main(){
    vec4 totalLocalPos = vec4(0.0f,0.0f,0.0f,1.0f);
    vec4 totalNormal = vec4(0.0f);

    for (int i=0;i<MAX_WEIGHTS;i++){

        if(weights_in[i]<=0 ||jointIndices_in[i]<0){
            continue;
        }
        if(jointIndices_in[i] >= MAX_JOINTS) 
        {
            totalLocalPos = vec4(position_in,1.0f);
            break;
        }
        
        mat4 jointTransform = jointTransforms[jointIndices_in[i]];
        vec4 posePosition = jointTransform * vec4(position_in,1.0f);

        totalLocalPos += posePosition * weights_in[i];

        vec4 worldNormal = jointTransform * vec4(normal_in,0.0f);
        totalNormal+=worldNormal * weights_in[i];
    
    }
    vec4 worldPos = modelMatrix * totalLocalPos;
    vs_out.FragPos = worldPos.xyz;
    vs_out.Normal = totalNormal.xyz;
    vs_out.TexCoords = texCoords_in;
    
    gl_Position = viewProjMatrix * worldPos;

    gl_ClipDistance[0] = dot(worldPos,clippingPlane);
}
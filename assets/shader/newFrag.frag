#version 430 core

layout(location=0) out vec4 FragColor;
layout(location=1) out vec4 BrightColor;


in VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
} fs_in;



struct DirectionalLight {
    vec3 color;
    vec3 direction;
};

struct PointLight {
    vec3 color;
    vec3 position;
    vec3 attenuation;
    bool enabled;
    samplerCube depthMap;
    bool emittingShadows;
    float flickerOffset;
};



uniform sampler2D texture_diffuse1;
uniform PointLight pointLights[3];

uniform DirectionalLight dirLights[8];
uniform vec3 materialCoefficients; // x = ambient, y = diffuse, z = specular 
uniform float shininess;

uniform vec3 cameraWorld;
uniform float alpha;

uniform float gamma;


uniform float farPlane;


// array of offset direction for sampling
vec3 gridSamplingDisk[20] = vec3[]
(
   vec3(1, 1,  1), vec3( 1, -1,  1), vec3(-1, -1,  1), vec3(-1, 1,  1), 
   vec3(1, 1, -1), vec3( 1, -1, -1), vec3(-1, -1, -1), vec3(-1, 1, -1),
   vec3(1, 1,  0), vec3( 1, -1,  0), vec3(-1, -1,  0), vec3(-1, 1,  0),
   vec3(1, 0,  1), vec3(-1,  0,  1), vec3( 1,  0, -1), vec3(-1, 0, -1),
   vec3(0, 1,  1), vec3( 0, -1,  1), vec3( 0, -1, -1), vec3( 0, 1, -1)
);


float ShadowCalculation(vec3 fragPos, PointLight light, int index){
    vec3 fragToLight = fragPos - light.position; 
    float currentDepth = length(fragToLight);

    //pcf algorithm - https://learnopengl.com/Advanced-Lighting/Shadows/Point-Shadows

    float shadow = 0.0f;
    float bias = 0.25f;
    int samples = 20;
    float viewDistance = length(cameraWorld - fragPos);
    float diskRadius = (1.0f + (viewDistance / farPlane))/25.0;

    for (int i=0; i<samples; ++i){
        float closestDepth =texture(light.depthMap, fragToLight + gridSamplingDisk[i]*diskRadius).r;

        closestDepth *=farPlane;
        if(currentDepth - bias>closestDepth){
            shadow +=1.0f;
        }
       
    }

        shadow/=float(samples);

        return shadow;
}

vec3 blinnPhongPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir, vec4 color, int index){
    vec3 lightDir = (light.position-fragPos);
    float d= length(lightDir);

    
	    lightDir=normalize(lightDir);
	    // diffuse shading
	    float diff = max(dot(normal,lightDir), 0.0);
	   
	//blinn-phong
	    // specular shading
	    vec3 halfwayDir = normalize(lightDir + viewDir); 
	    float spec = pow(max(dot(normal, halfwayDir), 0.0), shininess);

	   
	    // combine results
	    vec3 specular = spec * materialCoefficients.z * light.color;
	    vec3 ambient  = materialCoefficients.x * light.color; 
	    vec3 diffuse  = diff*light.color * materialCoefficients.y;

	    
	    
	    
	    float att = 1.0f;   
	    if(light.attenuation.x>0.0){ 
	    att = 1.0f / (light.attenuation.x + d * light.attenuation.y + (d * d) * light.attenuation.z);
	    }else{
	        att = 1.0f / (1.0f + d * 0.045f + (d * d) * 0.0075f);
	    }
	    vec3 ambient1  = att * ambient;
	    vec3 diffuse1  = att * diffuse;
	    vec3 specular1 = att * specular;

        if(light.emittingShadows){
           float shadow = ShadowCalculation(fs_in.FragPos, light,index);
           diffuse1*= (1.0f-shadow);
           specular1*=(1.0f-shadow);
          
        }
	    
	    return ((ambient1 + diffuse1)* color.rgb + specular1);
    
  
}
vec3 blinnPhongDirLight(DirectionalLight light, vec3 normal, vec3 viewDir, vec4 color){
    vec3 lightDir = normalize(-light.direction);
    // diffuse shading
    float diff = max(dot(lightDir, normal), 0.0);
    // specular shading
    vec3 halfwayDir = normalize(lightDir + viewDir);  
    float spec = pow(max(dot(normal, halfwayDir), 0.0), shininess);

 
        
    vec3 specular = spec * materialCoefficients.z * light.color;
    vec3 ambient  = materialCoefficients.x * light.color; 
    vec3 diffuse  = diff*light.color * materialCoefficients.y;
    
    return ((ambient + diffuse)* color.rgb + specular);
}



void main()
{           

    vec4 texColor = texture(texture_diffuse1, fs_in.TexCoords).rgba;
    vec4 color = vec4(texColor.rgb * materialCoefficients.x,texColor.a);
    vec3 normal = normalize(fs_in.Normal); 
    vec3 viewDir = normalize(cameraWorld - fs_in.FragPos);

    
    FragColor = vec4(0.0f,0.0f,0.0f,color.a);

    for(int i=0;i<pointLights.length();i++){
        if(pointLights[i].enabled){
            FragColor.rgb+= blinnPhongPointLight(pointLights[i],normal,fs_in.FragPos, viewDir, color, i);
        }
    }
    
    FragColor.rgb = pow(FragColor.rgb, vec3(1.0/gamma));
    //FragColor.rgb = fs_in.FragPos;

    //Luma conversion
    
    float brightness = dot(FragColor.rgb, vec3(0.2126,0.7152,0.0722));
    
    if(brightness>1.2f){
    BrightColor = vec4(FragColor.rgb * brightness,1.0);
    }else{
    	BrightColor = vec4(0,0,0,1.0f);
    }

}




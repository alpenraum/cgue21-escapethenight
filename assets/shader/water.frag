#version 430 core

in VS_OUT {
    vec3 vecToCamera;
    vec3 vecFromLight[10];
    vec3 lightColor[10];
	vec4 clipSpace;
	vec2 texCoords;
} fs_in;

out vec4 out_Color;

uniform sampler2D reflectionTexture;
uniform sampler2D refractionTexture;
uniform sampler2D refractionDepthTexture;

uniform sampler2D dudvmap;
uniform sampler2D normalMap;


uniform float moveFactor;
uniform float near;
uniform float far;
uniform bool normalMapping;

const float shininess = 20.0f;
const float reflectivity = 0.5f;
const float waveStrength = 0.04f;

void main(void) {

	
	vec2 ndc = (fs_in.clipSpace.xy/fs_in.clipSpace.w)/2.0f+0.5f;
	vec2 refractionCoords = vec2(ndc.x,ndc.y);
	vec2 reflectionCoords = vec2(ndc.x, -ndc.y);
	

	//calc the real depth value at this point - source: http://web.archive.org/web/20130416194336/http://olivers.posterous.com/linear-depth-in-glsl-for-real
	float depth = texture(refractionDepthTexture, refractionCoords).r;
	float floorDist = 2.0 * near * far /(far+near-(2.0 * depth -1.0)* (far - near));

	depth = gl_FragCoord.z;
	float waterDist = 2.0 * near * far / (far + near - (2.0 * depth - 1.0) * (far - near));

	float waterDepth = floorDist - waterDist;
	

	vec2 distortedTexCoords = texture(dudvmap, vec2(fs_in.texCoords.x + moveFactor, fs_in.texCoords.y)).rg*0.1f;
	distortedTexCoords = fs_in.texCoords + vec2(distortedTexCoords.x, distortedTexCoords.y+moveFactor);
	vec2 distort = (texture(dudvmap, distortedTexCoords).rg * 2.0 - 1.0) * waveStrength * clamp(waterDepth/20.0f, 0.0f,1.0f);;

	refractionCoords += distort;
	refractionCoords = clamp(refractionCoords,0.001f, 0.999f);

	reflectionCoords += distort;
	reflectionCoords.x = clamp(reflectionCoords.x,0.001f, 0.999f);
	reflectionCoords.y = clamp(reflectionCoords.y,-0.999f, -0.001f);

	vec4 reflectColor = texture(reflectionTexture,reflectionCoords);
	vec4 refractColor = texture(refractionTexture,refractionCoords);

	

	vec4 normalC = vec4(1.0f);
	if(normalMapping){
		normalC = texture(normalMap, distortedTexCoords);
	}
	vec3 normal = vec3(normalC.r * 2.0f -1.0f, normalC.b*3.0f,normalC.g * 2.0f -1.0f);
	normal = normalize(normal);

	vec3 viewVec = normalize(fs_in.vecToCamera);
	float refractiveFactor = dot(viewVec, normal);
	refractiveFactor = pow(refractiveFactor, 1.5f);
	refractiveFactor = clamp(refractiveFactor, 0.0, 1.0);

// specular shading
	vec3 specular = vec3(0.0f);
	for (int i=0; i<fs_in.vecFromLight.length();i++){
	    vec3 reflectedLight =reflect( normalize(fs_in.vecFromLight[i] ), normal); 
	    float spec = pow(max(dot(reflectedLight, viewVec), 0.0), shininess);
		specular += fs_in.lightColor[i] * spec * reflectivity * clamp(waterDepth/5.0f, 0.0f,1.0f);
	}

	out_Color = mix (reflectColor,refractColor,refractiveFactor);
	out_Color = mix (out_Color, vec4(0.0f,0.2f,0.5f,1.0f), 0.2f) + vec4 (specular, 0.0f);
	out_Color.a =clamp(waterDepth/1.0f, 0.0f,1.0f); ;


}
#version 430


uniform sampler2D main_texture_id;
uniform sampler2D shadowMap;

in vec2 TexturePos;
in vec3 light_intensity;
in vec4 FragPosLightSpace;

out vec4 FragColor;



float ShadowCalculation(vec4 fragPosLightSpace)
{
    float bias = 0.0005;
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    projCoords = projCoords * 0.5 + 0.5;
    float closestDepth = texture(shadowMap, projCoords.xy).r; 
    float currentDepth = projCoords.z;
    float shadow = currentDepth - bias > closestDepth  ? 0.4 : 0.0;
 
    return shadow;
}  

void main()
{
	float shadow = ShadowCalculation(FragPosLightSpace);  
	FragColor = (0.15f * texture(main_texture_id, TexturePos.xy)) + (vec4(light_intensity.xyz, 1.0f) * texture(main_texture_id, TexturePos.xy)) * (1.0 - shadow);
}

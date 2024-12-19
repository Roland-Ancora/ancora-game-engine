#version 430


uniform sampler2D main_texture_id;
uniform sampler2D shadowMap;
uniform float default_fogging;
uniform float shadow_intensity;
uniform vec3 color;
uniform float color_intensity;

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
    float shadow = currentDepth - bias > closestDepth ? shadow_intensity : 0.0;
 
    return shadow;
}



subroutine vec4 color_processing();
subroutine uniform color_processing color_processing_type;

subroutine (color_processing)
vec4 default_color_only()
{
    vec4 color_default = vec4(texture(main_texture_id, TexturePos.xy) * (1.0f - color_intensity) + vec4(color, 1.0f) * color_intensity);
    return color_default * vec4(light_intensity.xyz, 1.0f);
}

subroutine (color_processing)
vec4 global_light()
{
    float shadow = ShadowCalculation(FragPosLightSpace); 
    vec4 default_color = texture(main_texture_id, TexturePos.xy) * (1.0f - color_intensity) + vec4(color, 1.0f) * color_intensity;
    return vec4(light_intensity.xyz, 1.0f) * default_color * (default_fogging - shadow);
}



void main()
{
	FragColor = color_processing_type();
}

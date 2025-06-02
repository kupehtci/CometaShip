#version 330 core
out vec4 FragColor;

struct Material {
    sampler2D diffuseMap;
    sampler2D specularMap;
    sampler2D emissionMap;

    bool hasDiffuseMap;
    bool hasSpecularMap;
    bool hasEmissionMap;

    vec3 diffuse;
    vec3 specular;

    vec3 color;
    vec3 ambient;
    float shininess;
};

// Shadow map samplers
uniform sampler2D shadowMap;         // For directional light
uniform samplerCube pointShadowMap;  // For point light
uniform float far_plane;             // Far plane for point light shadow mapping

struct Light {
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;
};

struct DirectionalLight {
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

// Position of the fragment
in vec3 FragPos;
// defines the normal of the fragment
in vec3 Normal;
// Texture coordinate
in vec2 texCoord;
// Fragment position in light space for shadow mapping
in vec4 FragPosLightSpace;

// Camera viewing position
uniform vec3 uViewPos;


uniform Material material;

// Light variables
#define MAX_LIGHTS_CONSTANT 16
uniform int number_lights;                                 // Number of lights to process
uniform Light lights[MAX_LIGHTS_CONSTANT];

uniform DirectionalLight directionalLight;

// Calculates the directional light sum of ambien, diffuse and specular
// param DirectionalLight dirLight:     directional light struct object
// param vec3 fragNormal:               Normal of the fragment
// param vec3 viewDirection:            view direction from the fragment to the viewing position (Camera)
vec3 CalculateDirectionalLight(DirectionalLight dirLight, vec3 fragNormal, vec3 viewDirection);


// Calculates the point light sum of the anbient, diffuse and specular
// param LIght pointLight:              Point light struct object
// param vec3 fragNormal:               Normal vector of the fragment
// param vec2 fragPos:                  Position vector of the fragment
// param vec2 viewDirection:            Viewing vector normalized. This vector is calculated using camera position and fragment position
vec3 CalculatePointLights(Light pointLight, vec3 fragNormal, vec3 fragPos,  vec3 viewDirection);


// Calculate shadow value (0.0 = in shadow, 1.0 = not in shadow) for directional light
// param vec4 fragPosLightSpace:        Fragment position in light space
float CalculateShadow(vec4 fragPosLightSpace);

// Calculate shadow value (0.0 = in shadow, 1.0 = not in shadow) for point light
// param vec3 fragPos:                  Fragment position in world space
// param vec3 lightPos:                 Light position in world space
float CalculatePointLightShadow(vec3 fragPos, vec3 lightPos);

void main()
{
    vec3 result = vec3(0.0, 0.0, 0.0);

    vec3 viewDir = normalize(uViewPos - FragPos);
    vec3 norm = normalize(Normal);

    for(int i = 0; i < number_lights; ++i){
        result += CalculatePointLights(lights[i], norm, FragPos, viewDir);
    }

    // emission
    vec3 emission = vec3(0.0, 0.0, 0.0);

    if(material.hasEmissionMap){
        emission = texture(material.emissionMap, texCoord).rgb;
    }
    result += emission;

    // Add directional light with shadow
    result += CalculateDirectionalLight(directionalLight, norm, viewDir);
    result *= material.color;

    FragColor = vec4(result, 1.0);
}


vec3 CalculateDirectionalLight(DirectionalLight dirLight, vec3 fragNormal, vec3 viewDirection)
{
    vec3 lightDirection = normalize(-dirLight.direction);
    float diff = max(dot(fragNormal, lightDirection), 0.0);

    // Blinn-Phong: Calculate halfway vector between light direction and view direction
    vec3 halfwayDir = normalize(lightDirection + viewDirection);
    // Calculate specular using the halfway vector and normal
    float spec = pow(max(dot(fragNormal, halfwayDir), 0.0), material.shininess);

    vec3 ambient = dirLight.ambient * vec3(texture(material.diffuseMap, texCoord));
    vec3 diffuse = dirLight.diffuse * diff * vec3(texture(material.diffuseMap, texCoord));
    vec3 specular = dirLight.specular * spec * vec3(texture(material.specularMap, texCoord));

    // Calculate shadow
    float shadow = CalculateShadow(FragPosLightSpace);
    
    // Apply shadow to diffuse and specular components only (ambient light still visible in shadows)
    vec3 result = ambient + shadow * (diffuse + specular);
    return result;
}

vec3 CalculatePointLights(Light pointLight, vec3 fragNormal, vec3 fragPos,  vec3 viewDirection)
{
    // ambient
    vec3 ambient = vec3(0.0, 0.0, 0.0);

    if(material.hasDiffuseMap){
        ambient = pointLight.ambient * vec3(texture(material.diffuseMap, texCoord));
    }
    else {
        ambient = pointLight.ambient * material.ambient;
    }

    // diffuse
    vec3 lightDir = normalize(pointLight.position - fragPos);
    float diff = max(dot(fragNormal, lightDir), 0.0);

    vec3 diffuse = vec3(0.0, 0.0, 0.0);

    if(material.hasDiffuseMap){
        diffuse = pointLight.diffuse * (diff * vec3(texture(material.diffuseMap, texCoord)));
    }
    else {
        diffuse = pointLight.diffuse * (diff * material.diffuse);
    }

    // specular - Blinn-Phong implementation
    vec3 halfwayDir = normalize(lightDir + viewDirection);
    float spec = pow(max(dot(fragNormal, halfwayDir), 0.0), material.shininess);
    
    vec3 specular = vec3(0.0, 0.0, 0.0);

    if(material.hasSpecularMap){
        specular = pointLight.specular * (spec * vec3(texture(material.specularMap, texCoord)));
    }
    else {
        specular = pointLight.specular * (spec * material.specular);
    }

    // Calculate shadow for point light
    float shadow = CalculatePointLightShadow(fragPos, pointLight.position);

    // atenuation of light
    float distance = length(pointLight.position - fragPos);
    float attenuation = 1.0 / (pointLight.constant + pointLight.linear * distance + pointLight.quadratic * (distance * distance));

    ambient *= attenuation;
    specular *= attenuation * shadow; // Apply shadow to specular
    diffuse *= attenuation * shadow;  // Apply shadow to diffuse

    return (ambient + diffuse + specular);
}

float CalculateShadow(vec4 fragPosLightSpace)
{
    // Perform perspective divide to get normalized device coordinates
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    
    // Transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;
    
    // Get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    float closestDepth = texture(shadowMap, projCoords.xy).r; 
    
    // Get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;
    
    // Calculate bias based on depth map resolution and angle
    vec3 normal = normalize(Normal);
    vec3 lightDir = normalize(-directionalLight.direction);
    float bias = max(0.05 * (1.0 - dot(normal, lightDir)), 0.005);
    
    // PCF (percentage-closer filtering) for softer shadows
    float shadow = 0.0;
    vec2 texelSize = 1.0 / textureSize(shadowMap, 0);
    for(int x = -1; x <= 1; ++x)
    {
        for(int y = -1; y <= 1; ++y)
        {
            float pcfDepth = texture(shadowMap, projCoords.xy + vec2(x, y) * texelSize).r; 
            shadow += currentDepth - bias > pcfDepth ? 1.0 : 0.0;        
        }
    }
    shadow /= 9.0;
    
    // Keep the shadow at 0.0 when outside the far plane region of the light's frustum
    if(projCoords.z > 1.0)
        shadow = 0.0;
        
    // Return inverted shadow value (1.0 = not in shadow, 0.0 = in shadow)
    return 1.0 - shadow;
}

float CalculatePointLightShadow(vec3 fragPos, vec3 lightPos)
{
    // Get vector between fragment position and light position
    vec3 fragToLight = fragPos - lightPos;
    
    // Use the fragment to light vector to sample from the depth map    
    float closestDepth = texture(pointShadowMap, fragToLight).r;
    
    // It is currently in linear range between [0,1]. Re-transform back to original depth value
    closestDepth *= far_plane;
    
    // Get current linear depth as the length between the fragment and light position
    float currentDepth = length(fragToLight);
    
    // Test for shadows with bias
    float bias = 0.05; // Adjust this value based on your scene
    float shadow = currentDepth - bias > closestDepth ? 1.0 : 0.0;
    
    // PCF (percentage-closer filtering) for softer shadows
    // This is a simplified version of PCF for cubemaps
    shadow = 0.0;
    float samples = 4.0;
    float offset = 0.1;
    for(float x = -offset; x < offset; x += offset / (samples * 0.5))
    {
        for(float y = -offset; y < offset; y += offset / (samples * 0.5))
        {
            for(float z = -offset; z < offset; z += offset / (samples * 0.5))
            {
                float closestDepth = texture(pointShadowMap, fragToLight + vec3(x, y, z)).r;
                closestDepth *= far_plane;   // Undo mapping [0;1]
                if(currentDepth - bias > closestDepth)
                    shadow += 1.0;
            }
        }
    }
    shadow /= (samples * samples * samples);
    
    // Return inverted shadow value (1.0 = not in shadow, 0.0 = in shadow)
    return 1.0 - shadow;
}
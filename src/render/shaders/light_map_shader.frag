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

// Main execution
//
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

    // results

    result += CalculateDirectionalLight(directionalLight, norm, viewDir);

    result *= material.color;

    FragColor = vec4(result, 1.0);

}


vec3 CalculateDirectionalLight(DirectionalLight dirLight, vec3 fragNormal, vec3 viewDirection)
{
    vec3 lightDirection = normalize(-dirLight.direction);
    float diff = max(dot(fragNormal, lightDirection), 0.0);

    vec3 reflectedDirection = reflect(-lightDirection, fragNormal);
    float spec = pow(max(dot(viewDirection, reflectedDirection), 0.0), material.shininess);

    vec3 ambient = dirLight.ambient * vec3(texture(material.diffuseMap, texCoord));
    vec3 diffuse = dirLight.diffuse * diff * vec3(texture(material.diffuseMap, texCoord));
    vec3 specular = dirLight.specular * spec * vec3(texture(material.specularMap, texCoord));

    vec3 result = ambient + diffuse + specular;
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

    // specular

    vec3 reflectDir = reflect(-lightDir, fragNormal);
    float spec = pow(max(dot(viewDirection, reflectDir), 0.0), material.shininess);

    vec3 specular = vec3(0.0, 0.0, 0.0);

    if(material.hasSpecularMap){
        specular = pointLight.specular * (spec * vec3(texture(material.specularMap, texCoord)));
    }
    else {
        specular = pointLight.specular * (spec * material.specular);
    }

    // atenuation of light
    float distance = length(pointLight.position - fragPos);
    float attenuation = 1.0 / (pointLight.constant + pointLight.linear * distance + pointLight.quadratic * (distance * distance));

    ambient *= attenuation;
    specular *= attenuation;
    diffuse *= attenuation;

    return (ambient + diffuse + specular);
}
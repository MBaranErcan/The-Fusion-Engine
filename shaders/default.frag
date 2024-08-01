#version 460 core

out vec4 fragColor;

in vec2 texCoord;
in vec3 fragPos;
in vec3 normal;

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;
uniform sampler2D texture_normal1;
uniform sampler2D texture_height1;
uniform sampler2D texture_ambientOcclusion1;
uniform sampler2D texture_roughness1;

struct DirectionalLight {
    vec3 color;
    vec3 direction;
};

struct PointLight {
    vec3 color;
    vec3 position;
    float constant;
    float linear;
    float quadratic;
};

struct SpotLight {
    vec3 color;
    vec3 position;
    vec3 direction;
    float cutOff;
    float outerCutOff;
};

#define MAX_DIR_LIGHTS 4
#define MAX_POINT_LIGHTS 4
#define MAX_SPOT_LIGHTS 4

uniform int numDirLights;
uniform int numPointLights;
uniform int numSpotLights;

uniform vec3 globalAmbientColor;
uniform float globalAmbientStrength;

uniform DirectionalLight dirLights[MAX_DIR_LIGHTS];
uniform PointLight pointLights[MAX_POINT_LIGHTS];
uniform SpotLight spotLights[MAX_SPOT_LIGHTS];

uniform vec3 viewPos; // Camera/view position
uniform vec3 objectColor;

// Function prototypes
vec3 CalculateDirectionalLight(DirectionalLight light, vec3 normal, vec3 viewDir);
vec3 CalculatePointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 CalculateSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 CalculateSpecular(vec3 specularColor, vec3 viewDir, vec3 normal, float shininess);

void main() {
    vec3 norm = normalize(normal);
    vec3 viewDir = normalize(viewPos - fragPos);
    
    vec3 tangentNormal = texture(texture_normal1, texCoord).xyz * 2.0 - 1.0;
    norm = normalize(norm + tangentNormal);

    vec3 result = vec3(0.0);

    for (int i = 0; i < numDirLights; i++) {
        result += CalculateDirectionalLight(dirLights[i], norm, viewDir);
    }

    for (int i = 0; i < numPointLights; i++) {
        result += CalculatePointLight(pointLights[i], norm, fragPos, viewDir);
    }

    for (int i = 0; i < numSpotLights; i++) {
        result += CalculateSpotLight(spotLights[i], norm, fragPos, viewDir);
    }

    vec4 texColor = texture(texture_diffuse1, texCoord);
    vec3 litColor = result * objectColor * texColor.rgb;

    vec3 specularColor = texture(texture_specular1, texCoord).rgb;
    vec3 specular = CalculateSpecular(specularColor, viewDir, norm, 32.0);

    fragColor = vec4(litColor + specular, texColor.a);
}

// Directional light
vec3 CalculateDirectionalLight(DirectionalLight light, vec3 normal, vec3 viewDir) {
    vec3 lightDir = normalize(-light.direction);

    float diff = max(dot(normal, lightDir), 0.0);

    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = 0.0;

    vec3 ambient = globalAmbientStrength * globalAmbientColor * texture(texture_ambientOcclusion1, texCoord).rgb;
    vec3 diffuse = diff * light.color;
    vec3 specular = spec * light.color;
    return (ambient + diffuse + specular);
}

// Point light
vec3 CalculatePointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir) {
    vec3 lightDir = normalize(light.position - fragPos);
    
    float diff = max(dot(normal, lightDir), 0.0);
    
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = 0.0; // Assuming no shininess for simplicity; add shininess if needed
    
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
    
    vec3 ambient = globalAmbientStrength * globalAmbientColor;
    vec3 diffuse = diff * light.color;
    vec3 specular = spec * light.color;
    return ambient + (diffuse + specular) * attenuation;
}

// Spot light
vec3 CalculateSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir) {
    vec3 lightDir = normalize(light.position - fragPos);
    float theta = dot(lightDir, normalize(-light.direction)); 
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);

    float diff = max(dot(normal, lightDir), 0.0);

    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = 0.0; // Assuming no shininess for simplicity; add shininess if needed
    
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (distance * distance);
    
    vec3 ambient = globalAmbientStrength * globalAmbientColor;
    vec3 diffuse = diff * light.color;
    vec3 specular = spec * light.color;
    return ambient + (diffuse + specular) * attenuation * intensity;
}

// Calculate specular
vec3 CalculateSpecular(vec3 specularColor, vec3 viewDir, vec3 normal, float shininess) {
	vec3 reflectDir = reflect(-viewDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
	return spec * specularColor;
}
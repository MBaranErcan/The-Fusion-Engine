#version 460 core

out vec4 fragColor;

in vec2 texCoord;
in vec3 fragPos;
in vec3 normal;

uniform sampler2D tex0;

// Point light properties
uniform vec3 lightPos;
uniform vec3 lightColor;
uniform float constant;
uniform float linear;
uniform float quadratic;

uniform vec3 ambientColor;
uniform vec3 viewPos; // Camera/view position

// Specular intensity and shininess parameters
uniform float specularIntensity;
uniform float shininess;

void main()
{
    vec3 norm = normalize(normal);
    vec3 lightDir = normalize(lightPos - fragPos);
    float distance = length(lightPos - fragPos);
    vec3 viewDir = normalize(viewPos - fragPos);
    
    // Diffuse
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;
    
    // Specular
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
    vec3 specular = specularIntensity * spec * lightColor;
    
    // Attenuation
    float attenuation = 1.0 / (constant + linear * distance + quadratic * (distance * distance));
    
    // Ambient
    vec3 ambient = ambientColor;
    
    vec3 resultColor = (ambient + (diffuse + specular) * attenuation);

    fragColor = vec4(resultColor, 1.0);
    fragColor = texture(tex0, texCoord) * fragColor;
}
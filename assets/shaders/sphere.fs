#version 460 core

out vec4 FragColor;
in vec3 Normal;
in vec2 TexCoord;
in vec3 FragPos; 

uniform sampler2D ourTexture;
uniform vec3 lightPos; 
uniform vec3 viewPos;

void main()
{
    vec4 textureColor = texture(ourTexture, TexCoord);

    // ambient
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * textureColor.rgb;

    // diffuse 
    vec3 norm = normalize(Normal); 
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * textureColor.rgb;

    // specular
    float specularStrength = 1.0;
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 16);
    vec3 specular = specularStrength * spec * textureColor.rgb; 

    // attenuation
    float distance = length(lightPos - Normal);
    float attenuation = 1.0 / (1.0 + 0.09 * distance + 0.032 * distance * distance);

    vec3 result = (ambient + diffuse + specular) * attenuation;
    FragColor = vec4(result, textureColor.a);
}

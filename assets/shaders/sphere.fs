#version 460 core
out vec4 FragColor;
in vec3 Normal;
in vec2 TexCoord;
uniform sampler2D ourTexture;
uniform float attenuation;
void main()
{
   vec3 lightDir = normalize(Normal);
   float diff = max(dot(lightDir, lightDir), 0.0);
   vec4 textureColor = texture(ourTexture, TexCoord);
   vec3 diffuse = diff * textureColor.rgb;
   vec3 result = diffuse * attenuation;
   FragColor = vec4(result, textureColor.a);
}
#version 400 core

struct Material {
    //IMG TEXTURE
    sampler2D diffuse;
    sampler2D specular;

    //COLOR TEXTURE
    vec3 ambient;
    vec3 diff;
    vec3 spec;
    
    float shininess;
    float transparency;
}; 

struct Light {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;
  
uniform vec3 viewPos;
uniform Material material;
uniform Light light;
uniform bool isTextured;
uniform float transition;

void main()
{
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light.position - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    
    if (transition > 0.0f && transition < 1.0f) {
        ambient = light.ambient * (material.ambient * transition) + light.ambient * (vec3(texture(material.diffuse, TexCoords)) * (1 - transition));
        diffuse = light.diffuse * diff * (material.diff * transition) + light.diffuse * diff * (vec3(texture(material.diffuse, TexCoords)) * (1 - transition));
        specular = light.specular * spec * (material.spec * transition) + light.specular * spec * (texture(material.specular, TexCoords).rgb * (1 - transition));
    }
    else if (isTextured == false) {
        
        // ambient
        ambient = light.ambient * material.ambient;

        // diffuse
        diffuse = light.diffuse * diff * material.diff;

        // specular
        specular = light.specular * (spec * material.spec);
    }
    else if (isTextured) {
        ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));

        diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));
    
        specular = light.specular * spec * texture(material.specular, TexCoords).rgb;
    
    }
    
    vec3 result = ambient + diffuse + specular;
    FragColor = vec4(result, material.transparency);
}


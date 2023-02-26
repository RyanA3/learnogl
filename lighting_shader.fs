#version 330 core

out vec4 FragColor;

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    sampler2D emission;
    float sheen;
};

struct Light {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};


in vec2 TexCoords;
in vec3 frag_normal;
in vec3 frag_pos;

uniform Material material;
uniform Light light;

uniform vec3 view_pos;




void main() {

    //Calculate ambient light
    vec3 ambient = light.ambient * (texture(material.diffuse, TexCoords).rgb); //The ambient color of the object at this point is also simply its diffuse color

    //Calculates the angle between the vector from the light source to the vertex and the normal of the vertex, and use it for diffusion lighting
    vec3 unit_normal = normalize(frag_normal);
    vec3 unit_light_dir = normalize(light.position - frag_pos);
    float diffusion_factor = max(dot(unit_normal, unit_light_dir), 0.0);
    vec3 diffuse = diffusion_factor * light.diffuse * (texture(material.diffuse, TexCoords).rgb);

    //Calculate reflectiveness at this fragment and represent it by lightening the object (or not) ((Specular lighting))
    vec3 reflected_light_dir = reflect(-unit_light_dir, unit_normal);
    vec3 view_dir = normalize(view_pos - frag_pos);
    float specular_factor = pow(max(dot(reflected_light_dir, view_dir), 0.0), material.sheen);
    vec3 specular = specular_factor * light.specular * vec3(texture(material.specular, TexCoords));

    //Total lighting result
    vec3 result = ambient + diffuse + specular + vec3(texture(material.emission, TexCoords));

    FragColor = vec4(result, 1.0);

}
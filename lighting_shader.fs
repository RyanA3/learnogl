#version 330 core

out vec4 FragColor;

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    sampler2D emission;
    float sheen;
};

struct Light {
    //vec3 position;    //for point lights
    //vec3 direction;     //for directional lights
    vec4 pos_or_dir;     //1.0w component = position, 0.0w component = direction

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    //For light attenuation (fading)
    float constant;
    float linear;
    float quadratic;
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
    vec3 unit_light_dir;
    if(light.pos_or_dir.w == 0.0f) {
        unit_light_dir = normalize(-light.pos_or_dir.rgb);
    } else {
        unit_light_dir = normalize(light.pos_or_dir.rgb - frag_pos);
    }

    float diffusion_factor = max(dot(unit_normal, unit_light_dir), 0.0);
    vec3 diffuse = diffusion_factor * light.diffuse * (texture(material.diffuse, TexCoords).rgb);

    //Calculate reflectiveness at this fragment and represent it by lightening the object (or not) ((Specular lighting))
    vec3 reflected_light_dir = reflect(-unit_light_dir, unit_normal);
    vec3 view_dir = normalize(view_pos - frag_pos);
    float specular_factor = pow(max(dot(reflected_light_dir, view_dir), 0.0), material.sheen);
    vec3 specular = specular_factor * light.specular * vec3(texture(material.specular, TexCoords));

    //Calculate emission
    float emission_factor = 0.15f;
    vec3 emission = emission_factor * vec3(texture(material.emission, TexCoords));

    //Calculate light intensity based on distance from object
    float attenuation = 1.0f;
    if(light.pos_or_dir.w != 0.0f) {
        float dist = distance(frag_pos, light.pos_or_dir.rbg);
        attenuation = 1.0f / (light.constant + (light.linear * dist) + (light.quadratic * dist * dist));
    }

    //Total lighting result
    vec3 result = (ambient + diffuse + specular) * attenuation + emission;

    FragColor = vec4(result, 1.0);

}
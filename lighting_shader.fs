#version 330 core
in vec3 leNormal;
in vec3 leFragPos;

uniform vec3 object_color;
uniform vec3 light_color;
uniform vec3 light_pos;
uniform vec3 view_pos;

out vec4 FragColor;

void main() {

    //Calculate ambient light
    float ambient_strength = 0.1;
    vec3 ambient = ambient_strength * light_color;

    //Calculates the angle between the vector from the light source to the vertex and the normal of the vertex, and use it for diffusion lighting
    vec3 unit_normal = normalize(leNormal);
    vec3 unit_light_dir = normalize(light_pos - leFragPos);
    float diffusion_factor = max(dot(unit_normal, unit_light_dir), 0.0);
    vec3 diffuse = diffusion_factor * light_color;

    //Calculate reflectiveness at this fragment and represent it by lightening the object (or not) ((Specular lighting))
    float specular_strength = 0.5;
    vec3 reflected_light_dir = reflect(-unit_light_dir, unit_normal);
    vec3 view_dir = normalize(view_pos - leFragPos);
    float specular_factor = pow(max(dot(reflected_light_dir, view_dir), 0.0), 32);
    vec3 specular = specular_strength * specular_factor * light_color;

    //Total lighting result
    vec3 result = (ambient + diffuse + specular) * object_color;

    FragColor = vec4(result, 1.0);

}
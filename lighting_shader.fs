#version 330 core

out vec4 FragColor;

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    sampler2D emission;
    float sheen;
};

struct DirectionalLight {
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct PointLight {
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;
};

struct SpotLight {
    vec3 position;
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;

    float inner_cutoff;
    float outer_cutoff;
};

#define NR_POINT_LIGHTS 1
#define NR_SPOT_LIGHTS 1

uniform DirectionalLight directional_light;
uniform PointLight point_lights[NR_POINT_LIGHTS];
uniform SpotLight spot_lights[NR_SPOT_LIGHTS];



in vec2 TexCoords;
in vec3 frag_normal;
in vec3 frag_pos;

uniform Material material;

uniform vec3 view_pos;



//Calculate the total output color of a fragment from a directional light
vec3 calcDirectionalLight(DirectionalLight light, vec3 normal, vec3 frag_pos, vec3 view_dir) {

    vec3 unit_light_dir = normalize(-light.direction);

    //ambient
    vec3 ambient = light.ambient * texture(material.diffuse, TexCoords).rgb;

    //diffusion
    float diffusion_factor = max(dot(normal, unit_light_dir), 0.0);
    vec3 diffuse = diffusion_factor * light.diffuse * texture(material.diffuse, TexCoords).rgb;

    //specular
    vec3 reflected_light_dir = normalize(reflect(-unit_light_dir, normal));
    float specular_factor = pow(max(dot(reflected_light_dir, view_dir), 0.0), material.sheen);
    vec3 specular = specular_factor * light.specular * texture(material.specular, TexCoords).rgb;

    return (ambient + diffuse + specular);

};


//Calculate the total output color of a fragment from a point light
vec3 calcPointLight(PointLight light, vec3 normal, vec3 frag_pos, vec3 view_dir) {

    vec3 unit_light_dir = normalize(light.position - frag_pos);

    //ambient
    vec3 ambient = light.ambient * texture(material.diffuse, TexCoords).rgb;

    //diffusion
    float diffusion_factor = max(dot(normal, unit_light_dir), 0.0);
    vec3 diffuse = diffusion_factor * light.diffuse * texture(material.diffuse, TexCoords).rgb;

    //specular
    vec3 reflected_light_dir = normalize(reflect(-unit_light_dir, normal));
    float specular_factor = pow(max(dot(view_dir, reflected_light_dir), 0.0), material.sheen);
    vec3 specular = specular_factor * light.specular * texture(material.specular, TexCoords).rgb;

    //attenuation
    float dist = distance(frag_pos, light.position.xyz);
    float attenuation = 1.0f / (light.constant + (light.linear * dist) + (light.quadratic * dist * dist));  

    return (diffuse + specular + ambient) * attenuation;

};


//Calculate the total output color of a fragment from a spot light
vec3 calcSpotLight(SpotLight light, vec3 normal, vec3 frag_pos, vec3 view_dir) {

    vec3 unit_light_dir = normalize(light.position.xyz - frag_pos);

    //Calculate angle between light-to-fragment and direction of spotlight for cutoffs
    float theta = dot(unit_light_dir, normalize(-light.direction));
    

    //ambient
    vec3 ambient = light.ambient * texture(material.diffuse, TexCoords).rgb;


    //Only do other lighting calculations if the fragment is in the spotlight's cone
    if(theta > light.outer_cutoff) {

        //Calculate difference in inner and outer cutoff angles
        float epsilon = light.inner_cutoff - light.outer_cutoff;

        //Calculate intensity of light (for smoothing the edges of the spot-light cone)
        float edge_intensity = clamp((theta - light.outer_cutoff) / epsilon, 0.0, 1.0);

        //diffusion
        float diffusion_factor = max(dot(normal, unit_light_dir), 0.0);
        vec3 diffuse = diffusion_factor * light.diffuse * texture(material.diffuse, TexCoords).rgb;

        //specular
        vec3 reflected_light_dir = reflect(-unit_light_dir, normal);
        float specular_factor = pow(max(dot(reflected_light_dir, view_dir), 0.0), material.sheen);
        vec3 specular = specular_factor * light.specular * texture(material.specular, TexCoords).rgb;

        //attenuation
        float dist = distance(frag_pos, light.position.xyz);
        float attenuation = 1.0f / (light.constant + (light.linear * dist) + (light.quadratic * dist * dist)); 

        return (ambient + diffuse + specular) * attenuation * edge_intensity;

    } else {

        return ambient;

    }

};





void main() {

    //Calculate some basic stuff
    vec3 view_dir = normalize(view_pos - frag_pos);
    vec3 unit_normal = normalize(frag_normal);

    //Calculate emission from the object
    vec3 emission = 0.15f * texture(material.emission, TexCoords).rgb;

    vec3 result = calcDirectionalLight(directional_light, unit_normal, frag_pos, view_dir) + emission;
    for(int i = 0; i < NR_POINT_LIGHTS; i++) {
         result += calcPointLight(point_lights[i], unit_normal, frag_pos, view_dir);   
    };

    for(int i = 0; i < NR_SPOT_LIGHTS; i++) {
        result += calcSpotLight(spot_lights[i], unit_normal, frag_pos, view_dir);
    };

    
    //Calculate ambient light
    //vec3 ambient = light.ambient * (texture(material.diffuse, TexCoords).rgb); //The ambient color of the object at this point is also simply its diffuse color

    //Calculate emission
    //float emission_factor = 0.15f;
    //vec3 emission = emission_factor * vec3(texture(material.emission, TexCoords));


    //Calculates the angle between the vector from the light source to the vertex and the normal of the vertex
    //vec3 unit_normal = normalize(frag_normal);
    //vec3 unit_light_dir = normalize(light.position.xyz - frag_pos);

    //Calculate angle between direction of light and light-to-fragment
    //float theta = dot(unit_light_dir, normalize(-light.direction));

    //Calculate the difference in inner cutoff and outer cutoff angles
    //float epsilon = light.cutoff - light.outer_cutoff;

    //Calculate intensity for smoothing edges
    //float edge_intensity = clamp((theta - light.outer_cutoff) / epsilon, 0.0, 1.0);

    //Only do lighting calculations if fragment is within the spotlight's cone of light
    //vec3 result;
    //if(theta > light.outer_cutoff) {

        //Calculate light on the fragment based on angle of the light to the surface, (diffusion)
    //    float diffusion_factor = max(dot(unit_normal, unit_light_dir), 0.0);
    //    vec3 diffuse = diffusion_factor * light.diffuse * (texture(material.diffuse, TexCoords).rgb);

        //Calculate reflectiveness at this fragment and represent it by lightening the object (or not) ((Specular lighting))
    //    vec3 reflected_light_dir = reflect(-unit_light_dir, unit_normal);
    //    vec3 view_dir = normalize(view_pos - frag_pos);
    //    float specular_factor = pow(max(dot(reflected_light_dir, view_dir), 0.0), material.sheen);
    //    vec3 specular = specular_factor * light.specular * vec3(texture(material.specular, TexCoords));

        //Calculate light intensity based on distance from object
    //    float attenuation = 1.0f;
    //    float dist = distance(frag_pos, light.position.xyz);
    //    attenuation = 1.0f / (light.constant + (light.linear * dist) + (light.quadratic * dist * dist));    

    //    result = (diffuse + specular) * attenuation * edge_intensity + emission + ambient;
    //} else {
    //    result = ambient + emission;
    //}

    FragColor = vec4(result, 1.0);

}
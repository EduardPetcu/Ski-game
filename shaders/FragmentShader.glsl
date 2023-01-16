#version 330

in vec3 world_position;
in vec3 world_normal;
in vec2 texcoord;

// Uniform properties
uniform sampler2D texture_1;
uniform float Ycoord;
uniform float Xcoord;
uniform bool turn;

uniform vec3 light_direction[80];
uniform vec3 light_position[80];
uniform ivec3 light_color[80];
uniform int  isGift[40];
uniform int  isSpot[80];
uniform vec3 eye_position;

uniform float material_kd;
uniform float material_ks;
uniform int material_shininess;

uniform float angleSpot;
uniform vec3 object_color;

uniform int nrOfLights;
// Output
layout(location = 0) out vec4 out_color;

float point_light_contribution(vec3 light_position, vec3 light_direction, int spot) 
{    
	vec3 L = normalize(light_position - world_position);
    //vec3 L; //= (0, -1, 0);
    //L[0] = 0;
    //L[1] = -1;
    //L[2] = 0;
    vec3 V = normalize(eye_position - world_position);
    vec3 H = normalize(L + V);
    vec3 N = normalize(world_normal);
    
    float specular_light = 0;
    float intensity = 2.5;
    float diffuse_light = material_kd * max(dot(world_normal, L), 0) * intensity;

  
    if (diffuse_light > 0)
    {
        specular_light = material_ks *  pow(max(dot(N, H), 0), material_shininess) * intensity;
    }

    float light	= 0;
    if (spot == 1) {
            // iluminare Spot-light
            float cut_off = radians(30);
            float spot_light = dot(-L, light_direction);
            float spot_light_limit = cos(cut_off + radians(angleSpot));
            if (spot_light > spot_light_limit) {
	            // fragmentul este iluminat de spot, deci se calculeaza valoarea luminii conform  modelului Phong
	            // se calculeaza atenuarea luminii
                float linear_att = (spot_light - spot_light_limit) / (1.0f - spot_light_limit);
                float light_att_factor = linear_att * linear_att;
                light = light_att_factor * (diffuse_light + specular_light);
            }   
        } else {
            // iluminare Phong
            float d	= distance(light_position, world_position);
	        float attenuation_factor = 1 / (1 + d * d);
	        light =  attenuation_factor * (diffuse_light * 2.5 + specular_light);
        }
    return light;
}

void main()
{
    int i;
    float ambient_light = 0.5;
    float lightR, lightG, lightB, lightSpot = 0;
    lightR = 0;
    lightG = 0;
    lightB = 0;
    vec4 colour1;

    for (i = 0; i < nrOfLights; i++) {
         if (light_position[i] == vec3(100, 0, 0))
             continue;
         if (isSpot[i] == 1) {
            lightSpot += point_light_contribution(light_position[i], light_direction[i], isSpot[i]);
            continue;
         } 
    }
    for (i = 0; i < 40; i++) {
         if (light_position[i] == vec3(100, 0, 0))
             continue;
         if (isSpot[i] % 2 == 0) {
            lightR += point_light_contribution(light_position[i], light_direction[i], 0);
         }
         if (isSpot[i] % 3 == 0) {
            lightG += point_light_contribution(light_position[i], light_direction[i], 0);
         }
         if (isSpot[i] % 5 == 0) {
            lightB += point_light_contribution(light_position[i], light_direction[i], 0);
         }
    }
    if (turn) {
        colour1 = texture2D(texture_1, texcoord + vec2(Xcoord / 50.0f, -Ycoord / 50.0f));
    }
    else {
        colour1 = texture2D(texture_1, texcoord);
    }
    colour1 = colour1 * 0.5;
    colour1[0] = colour1[0] * (ambient_light + lightR + lightSpot);
    colour1[1] = colour1[1] * (ambient_light + lightG + lightSpot);
    colour1[2] = colour1[2] * (ambient_light + lightB + lightSpot);

    out_color = colour1;

    if (out_color.a < 0.5f) {
        discard;
    }
}

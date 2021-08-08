#version 330 core

uniform vec3 material_colour;
uniform vec3 viewPos;

in vec3 normal;
in vec3 fragPos;

// Ouput data
out vec4 color;

void main(){
	vec3 lightPos = vec3(12.0f, 10.0f, 20.0f);
	vec3 lightColour = vec3(1,1,1);

	vec3 norm = normalize(normal);
	vec3 lightDir = normalize(lightPos - fragPos);

	//Using dot product to determine diffusal
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = diff * lightColour;
	//Using ambient variables to darken the model
	float ambientStrength = 0.8;
	vec3 ambient = ambientStrength * lightColour;

	float specularStrength = 0.5;
	vec3 viewDir = normalize(viewPos - fragPos);
	vec3 reflectDir = reflect(-lightDir, norm);  

	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
	vec3 specular = specularStrength * spec * lightColour;  

	vec3 result = (ambient + diffuse + specular) * material_colour;
	// Output Vector for colour
	if(gl_FragCoord.x < 400)
        color = vec4(result, 1.0);
    else
        color = vec4(abs(norm.x), abs(norm.y), abs(norm.z), 1.0);
	
}
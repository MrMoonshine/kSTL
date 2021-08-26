#version 330

in vec3 normal;
in vec3 fragPos;
in vec3 fragColor;
out vec4 outColor;

void main(void)
{
	vec3 lightPos = vec3(12.0f, 10.0f, 20.0f);
	vec3 viewPos = vec3(1200, 800, 800);
	vec3 lightColour = vec3(1,1,1);

	vec3 norm = normalize(normal);
	vec3 lightDir = normalize(lightPos - fragPos);

	//Using dot product to determine diffusal
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = diff * lightColour;
	//Using ambient variables to darken the model
	float ambientStrength = 0.7;
	vec3 ambient = ambientStrength * lightColour;

	float specularStrength = 0.5;
	vec3 viewDir = normalize(viewPos - fragPos);
	vec3 reflectDir = reflect(-lightDir, norm);  

	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
	vec3 specular = specularStrength * spec * lightColour;  

	vec3 result = (ambient + diffuse) * fragColor;
	// Output Vector for colour
	//if(gl_FragCoord.x < 400)
    outColor = vec4(normal, 1.0);
}

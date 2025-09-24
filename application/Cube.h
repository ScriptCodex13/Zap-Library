#pragma once
// Just a example
#include <Zap.h>


class Cube : public zap::Mesh
{
private:
	const char* vertexShaderSourcecube = R"glsl(#version 330 core
		layout (location = 0) in vec3 aPos;
		layout (location = 1) in vec3 aNormal;

		out vec3 FragPos;
		out vec3 Normal;

		uniform mat4 model;
		uniform mat4 view;
		uniform mat4 projection;

		void main()
		{
			FragPos = vec3(model * vec4(aPos, 1.0));
			Normal = mat3(transpose(inverse(model))) * aNormal;  
    
			gl_Position = projection * view * vec4(FragPos, 1.0);
		}

		)glsl";

	const char* fragmentShaderSourcecube = R"glsl(#version 330 core
		out vec4 FragColor;

		in vec3 Normal;  
		in vec3 FragPos;  
  
		uniform vec3 lightPos; 
		uniform vec3 viewPos; 
		uniform vec3 lightColor;
		uniform vec3 objectColor;

		void main()
		{
			// ambient
			float ambientStrength = 0.1;
			vec3 ambient = ambientStrength * lightColor;
  	
			// diffuse 
			vec3 norm = normalize(Normal);
			vec3 lightDir = normalize(lightPos - FragPos);
			float diff = max(dot(norm, lightDir), 0.0);
			vec3 diffuse = diff * lightColor;
    
			// specular
			float specularStrength = 0.5;
			vec3 viewDir = normalize(viewPos - FragPos);
			vec3 reflectDir = reflect(-lightDir, norm);  
			float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
			vec3 specular = specularStrength * spec * lightColor;  
        
			vec3 result = (ambient + diffuse + specular) * objectColor;
			FragColor = vec4(result, 1.0);
		} 

		)glsl";
private:
	unsigned int object_color_location;
	unsigned int light_color_location;
	unsigned int light_position_location;
	unsigned int view_position_location;

public:
	Cube();
	void setColorUniform3f(float r, float g, float b);
	void setLightColorUniform3f(float r, float g, float b);
	void setLightPositionUniform3fv(glm::vec3 pos);
	void setViewPositionUniform3fv(glm::vec3 pos);
	void MoveUpdate(const zap::SceneCamera& camera, glm::vec3 lightPos, double time);

};

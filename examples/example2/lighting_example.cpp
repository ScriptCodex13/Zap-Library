// Just a example

// PR = Prototyping -> for testing could be changed later on

//this is separate application and should treat the Zap library as external
//everyone using this library will do, this is a standard practice
#include <Zap.h>
#include <iostream>
#include <vector>

const char* vertexShaderSourcelight = R"glsl(#version 330 core
layout (location = 0) in vec3 aPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	gl_Position = projection * view * model * vec4(aPos, 1.0);
})glsl";

const char* fragmentShaderSourcelight = R"glsl(#version 330 core
out vec4 FragColor;

void main()
{
    FragColor = vec4(1.0); // set all 4 vector values to 1.0
})glsl";

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


int main()
{
	zap::Init();

	zap::Window window(1920, 1080, "Hello Window");

	zap::InitGlad();

	std::vector<float> vertices =
	{
		 -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,

		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
	};

	std::vector<unsigned int> indices =
	{
	
	};

	//Camera

	std::array<unsigned int, 2> size = window.GetSize(); // Not a Ref to the window size !

	zap::SceneCamera camera(size[0], size[1]);

	//


	//Mesh
	zap::Mesh2D cube(vertices, indices);

	cube.SetVertexShaderSource(vertexShaderSourcecube);
	cube.SetFragmentShaderSource(fragmentShaderSourcecube);

	cube.SetAttribPointer(0, 3, 6, 0);
	cube.SetAttribPointer(1, 3, 6, 3);

	cube.Finish();



	zap::Mesh2D light(vertices, indices);

	light.SetVertexShaderSource(vertexShaderSourcelight);
	light.SetFragmentShaderSource(fragmentShaderSourcelight);

	

	light.SetAttribPointer(0, 3, 6, 0);
	
	light.Finish();
	
	// Window settings

	window.SetVSync(true);

	window.UpdateViewport(true);

    //

	glEnable(GL_DEPTH_TEST); 

	glm::vec3 lightPos(1.0f, 0.0f, 2.0f);

	float rotation = 0.0f;
	
	while (window.Open())
	{
		if (window.isKeyPressed(zap::Key::ESC))
		{
			window.Close();
		}

		if (window.GetInput(zap::Key::left_arrow, zap::State::PRESSED))
		{
			camera.Rotate(-5.0f * window.GetDelta() * 20.0f, 0.0f, 0.0f);
		}
		if (window.GetInput(zap::Key::right_arrow, zap::State::PRESSED))
		{
			camera.Rotate(5.0f * window.GetDelta() * 20.0f, 0.0f, 0.0f);
		}
		glClear(GL_DEPTH_BUFFER_BIT); // PR

		
		cube.UseProgram();

		glUniform3f(glGetUniformLocation(cube.GetProgram(), "objectColor"), 1.0f, 0.5f, 0.31f);
		glUniform3f(glGetUniformLocation(cube.GetProgram(), "lightColor"), 1.0f, 1.0f, 1.0f);
		glUniform3fv(glGetUniformLocation(cube.GetProgram(), "lightPos"), 1, &lightPos[0]);
		glUniform3fv(glGetUniformLocation(cube.GetProgram(), "viewPos"), 1, &camera.GetPosition()[0]);

		camera.UpdateProjection(cube.GetProgram(), "projection");
		camera.UpdateView(cube.GetProgram(), "view");
		cube.GetModel() = glm::translate(glm::mat4(1.0), glm::vec3(1.4f, -0.1f, -2.0f));
		cube.GetModel() = glm::rotate(glm::mat4(1.0), (float)glfwGetTime(), glm::vec3(0.0f, rotation, 0.0f));
		cube.UpdateModel("model");

		camera.UpdateRotation();

        //here starts current VAO for current program draw
		cube.Bind();
		cube.Draw(36);
        //here draw ends

		light.UseProgram();
		camera.UpdateProjection(light.GetProgram(), "projection");
		camera.UpdateView(light.GetProgram(), "view");
		light.GetModel() = glm::mat4(1.0f);
		light.GetModel() = glm::translate(light.GetModel(), lightPos);
		light.GetModel() = glm::scale(light.GetModel(), glm::vec3(0.2f));
		light.UpdateModel("model");

		camera.UpdateRotation();

        //here starts current VAO for current program draw
		light.Bind(); //set current context before any draw routines, it prevents mess in more complex workflow
		light.Draw(36);
        //here draw ends


		zap::ShowWireFrame(window.isKeyPressed(zap::Key::F10));
		
		window.Update();
		window.Draw();

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // PR
		
		rotation += 1.0f;

        // std::cout << window.GetFPS() << std::endl;
	}

	zap::Delete();

}






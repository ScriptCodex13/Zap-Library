// Just a example
// PR = Prototyping -> only for testing 

//this is separate application and should treat the Zap library as external
//everyone using this library will do, this is a standard practice
#include <Zap.h> //TODO: This is not the correct way to use libraries
#include <iostream>
#include <vector>
#include <array>

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
	zap::Init(4, 6);

	zap::Window window(1920, 1080, "Hello Window");

	zap::InitGlad();

	//Controller 

	// zap::Device controller = zap::AssignController(); For later prototyping with the controller

	//

	glEnable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


	//Camera

	std::array<int, 2> size = window.GetSize(); // Not a Ref to the window size !

	zap::SceneCamera camera(size[0], size[1]);

	camera.SetRotationLimit(361.0f, 89.0f, 0.0f);

	camera.ActivateRotationLimit(true);

	//


	//Mesh
	zap::Mesh cube(zap::standard_cube::standardcubevertices, zap::standard_cube::standardcubeindices);

	cube.SetVertexShaderSource(vertexShaderSourcecube);
	cube.SetFragmentShaderSource(fragmentShaderSourcecube);

	cube.SetAttribPointer(0, 3, 6, 0);
	cube.SetAttribPointer(1, 3, 6, 3);

	cube.Finish();



	zap::Mesh light(zap::standard_cube::standardcubevertices, zap::standard_cube::standardcubeindices);

	light.SetVertexShaderSource(vertexShaderSourcelight);
	light.SetFragmentShaderSource(fragmentShaderSourcelight);



	light.SetAttribPointer(0, 3, 6, 0);
	//light.SetAttribPointer(1, 3, 6, 3);
	//mesh.SetAttribPointer(2, 2, 8, 6);

	//auto texture = light.AddTexture(0, "textures/texture.png", zap::TextureFilters::LINEAR, zap::MipmapSettings::LINEAR_MIPMAP_LINEAR, zap::TextureWrapping::CLAMP_TO_EDGE);

	light.Finish();

	//

	// Window settings

	window.UpdateViewport(true);
	window.SetVSync(true);
	window.Maximize();

	zap::Enable(zap::Instructions::DEPTH);

	zap::Enable(zap::Instructions::ANTIALIASING);

	//window.Maximize();
	window.SetCursorinCameraMode(true);


	zap::Text text("C:/Windows/Fonts/arial.ttf", "Text", window.GetSizeRef()); // It's better to use GetSizeRef here

	text.SetCharacterSize(32);
	text.SetColor(0.0f, 1.0f, 0.0f);
	text.SetPosition(100.0f, 100.0f); // A quick test


	glm::vec3 lightPos(1.0f, 0.0f, 2.0f);

	float rotation = 0.0f;

	float sensitivity = 0.1f;


	std::array<double, 2> oldPos = window.GetMousePosition();

	// Getting uniform locations

	//cube
	unsigned int view_location_cube = glGetUniformLocation(cube.GetProgram(), "view");
	unsigned int projection_location_cube = glGetUniformLocation(cube.GetProgram(), "projection");
	unsigned int model_uniform_location_cube = glGetUniformLocation(cube.GetProgram(), "model");

	unsigned int object_color_location_cube = glGetUniformLocation(cube.GetProgram(), "objectColor");
	unsigned int light_color_location_cube = glGetUniformLocation(cube.GetProgram(), "lightColor");
	unsigned int light_position_location_cube = glGetUniformLocation(cube.GetProgram(), "lightPos");
	unsigned int view_position_location_cube = glGetUniformLocation(cube.GetProgram(), "viewPos");

	//light

	unsigned int view_location_light = glGetUniformLocation(light.GetProgram(), "view");
	unsigned int projection_location_light = glGetUniformLocation(light.GetProgram(), "projection");
	unsigned int model_uniform_location_light = glGetUniformLocation(light.GetProgram(), "model");

	//

	glEnable(GL_CULL_FACE);

	while (window.Open())
	{
		// Mouse Input
		std::array<double, 2> newpos = window.GetMousePosition();

		float xoffset = (newpos[0] - oldPos[0]) * sensitivity;
		float yoffset = (oldPos[1] - newpos[1]) * sensitivity;

		oldPos = newpos;

		camera.Rotate(xoffset, yoffset, 0.0f);

		//


		//Input

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
		if (window.GetInput(zap::Key::W, zap::State::PRESSED))
		{
			camera.MoveForward(0.5f * window.GetDelta() * 20.0f);
		}
		if (window.GetInput(zap::Key::S, zap::State::PRESSED))
		{
			camera.MoveBackward(0.5f * window.GetDelta() * 20.0f);
		}
		if (window.GetInput(zap::Key::A, zap::State::PRESSED))
		{
			camera.MoveLeft(0.5f * window.GetDelta() * 20.0f);
		}
		if (window.GetInput(zap::Key::D, zap::State::PRESSED))
		{
			camera.MoveRight(0.5f * window.GetDelta() * 20.0f);
		}

		//


		glClear(GL_DEPTH_BUFFER_BIT); // PR
		zap::ClearBackground(0.2f, 0.3f, 0.3f, 1.0f);

		//Cube

		cube.UseProgram();

		camera.UpdateRotation();

		glUniform3f(object_color_location_cube, 1.0f, 0.5f, 0.31f);
		glUniform3f(light_color_location_cube, 1.0f, 1.0f, 1.0f);
		glUniform3fv(light_position_location_cube, 1, &lightPos[0]);
		glUniform3fv(view_position_location_cube, 1, &camera.GetPosition()[0]);


		glUniformMatrix4fv(projection_location_cube, 1, GL_FALSE, glm::value_ptr(camera.GetProjection()));
		glUniformMatrix4fv(view_location_cube, 1, GL_FALSE, glm::value_ptr(camera.GetView()));

		cube.GetModel() = glm::translate(glm::mat4(1.0), glm::vec3(1.4f, -0.1f, -2.0f));
		cube.GetModel() = glm::rotate(glm::mat4(1.0), (float)glfwGetTime(), glm::vec3(0.0f, rotation, 0.0f));
		cube.UpdateModel(model_uniform_location_cube);

		cube.Bind();
		cube.Draw(36);

		//


		//Light cube

		light.UseProgram();

		camera.UpdateRotation();

		glUniformMatrix4fv(projection_location_light, 1, GL_FALSE, glm::value_ptr(camera.GetProjection()));
		glUniformMatrix4fv(view_location_light, 1, GL_FALSE, glm::value_ptr(camera.GetView()));

		light.GetModel() = glm::mat4(1.0f);
		light.GetModel() = glm::translate(light.GetModel(), lightPos); // Does work without problems
		light.GetModel() = glm::scale(light.GetModel(), glm::vec3(0.2f));
		light.UpdateModel(model_uniform_location_light);

		light.Bind(); //set current context before any draw routines, it prevents mess in more complex workflow
		light.Draw(36);

		//*/

		zap::ShowWireFrame(window.isKeyPressed(zap::Key::F10));
		//zap::ClearBackground(0.2f, 0.3f, 0.3f, 1.0f);
		//here starts current VAO for current program draw
		//here draw ends

		text.SetContent(std::to_string(std::round(window.GetFPS())));

		text.Draw();

		window.Update();
		window.Draw();

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // PR

		rotation += 1.0f;
	}

	zap::Delete();

}








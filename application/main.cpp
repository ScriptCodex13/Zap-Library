// Just a example
// PR = Prototyping -> only for testing 

//this is separate application and should treat the Zap library as external
//everyone using this library will do, this is a standard practice
#include <Zap.h> //TODO: This is not the correct way to use libraries
#include <iostream>
#include <vector>
#include <array>


//TODO: Modularize: Move these classes to separate .h/.cpp modules | these are only example classes we are only going to focus on basic functionality for now. We can plan on some easy profile later on
class Cube : public zap::Mesh
{
	unsigned int object_color_location;
	unsigned int light_color_location;
	unsigned int light_position_location;
	unsigned int view_position_location;
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
public:
	Cube() : zap::Mesh (zap::standard_cube::standardcubevertices) {
		SetVertexShaderSource(vertexShaderSourcecube);
		SetFragmentShaderSource(fragmentShaderSourcecube);

		SetAttribPointer(0, 3, 6, 0);
		SetAttribPointer(1, 3, 6, 3);

		Finish();

		object_color_location = glGetUniformLocation(GetProgram(), "objectColor");
		light_color_location = glGetUniformLocation(GetProgram(), "lightColor");
		light_position_location = glGetUniformLocation(GetProgram(), "lightPos");
		view_position_location = glGetUniformLocation(GetProgram(), "viewPos");
	}
	void setColorUniform3f(float r, float g, float b) {
		glUniform3f(object_color_location, r, g, b);
	}
	void setLightColorUniform3f(float r, float g, float b) {
		glUniform3f(light_color_location, r, g, b);
	}
	void setLightPositionUniform3fv(glm::vec3 pos) {
		glUniform3fv(light_position_location, 1, &pos[0]);
	}
	void setViewPositionUniform3fv(glm::vec3 pos) {
		glUniform3fv(view_position_location, 1, &pos[0]);
	}
	void MoveUpdate(const zap::SceneCamera& camera, glm::vec3 lightPos, double time)
	{
		setLightPositionUniform3fv(lightPos);
		setViewPositionUniform3fv(camera.GetPosition());
		SetProjection(camera.GetProjection());
		SetView(camera.GetView());
		UpdateModel(glm::rotate(glm::mat4(1.0), (float)time, glm::vec3(0.0f, 1.0f, 0.0f)));
	}

};

//TODO: Modularize: Move these classes to separate .h/.cpp modules
class LightCube : public zap::Mesh
{
	const char* vertexShaderSource = R"glsl(#version 330 core
		layout (location = 0) in vec3 aPos;

		uniform mat4 model;
		uniform mat4 view;
		uniform mat4 projection;

		void main()
		{
			gl_Position = projection * view * model * vec4(aPos, 1.0);
		})glsl";

	const char* fragmentShaderSource = R"glsl(#version 330 core
		out vec4 FragColor;

		void main()
		{
			FragColor = vec4(1.0); // set all 4 vector values to 1.0
		})glsl";


public:
	LightCube() : zap::Mesh(zap::standard_cube::standardcubevertices) {
		SetVertexShaderSource(vertexShaderSource);
		SetFragmentShaderSource(fragmentShaderSource);
		SetAttribPointer(0, 3, 6, 0);
		Finish();
	}
	void MoveUpdate(const zap::SceneCamera& camera, glm::vec3 lightPos, double time)
	{
		SetProjection(camera.GetProjection());
		SetView(camera.GetView());
		glm::mat4 model = glm::translate(glm::mat4(1.0f), lightPos);
		UpdateModel(glm::scale(model, glm::vec3(0.2f)));
	}
};

//TODO: Modularize: Move these classes to separate .h/.cpp modules
template <typename T> class window_camera_invoker : public zap::util::callback_invoker<T>
{
public:
	inline window_camera_invoker(T _callback) : callback_invoker(_callback) {}
	template<typename A1, typename A2> void operator () (A1& a1, A2& a2) { callback(a1, a2); }
};
window_camera_invoker  cbi([](zap::Window& window, zap::SceneCamera& camera) {
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
});

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



	// Window settings

	window.UpdateViewport(true);
	window.SetVSync(true);
	//window.Maximize();

	zap::Enable(zap::Instructions::DEPTH);
	zap::Enable(zap::Instructions::ANTIALIASING);

	//window.Maximize();
	window.SetCursorinCameraMode(true);


	zap::Text text("C:/Windows/Fonts/arial.ttf", "Text", window.GetSizeRef()); // It's better to use GetSizeRef here

	text.SetCharacterSize(32);
	text.SetColor(0.0f, 1.0f, 0.0f);
	text.SetPosition(100.0f, 100.0f); // A quick test


	glm::vec3 lightPos(1.0f, 0.0f, 2.0f);

	float rotation = 0.0f, sensitivity = 0.1f;

	std::array<double, 2> oldPos = window.GetMousePosition();

	auto coord = zap::util::convert_pixel_to_window(window.GetSize(), 1.0f, 1.0f);

	std::cout << "x: " << coord[0] << ",y: " << coord[1] << std::endl;

	//////Mesh
	Cube cube;
	LightCube lightCube;

	zap::Button button(window);


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
		//All the window/camera magic happens here
		cbi(window, camera);

		//


		glClear(GL_DEPTH_BUFFER_BIT); // PR
		zap::ClearBackground(0.2f, 0.3f, 0.3f, 1.0f);

		//Cube

		camera.UpdateRotation();
	
		cube.Bind();
		cube.setColorUniform3f(1.0f, 0.5f, 0.31f);
		cube.setLightColorUniform3f(1.0f, 1.0f, 1.0f);
		cube.MoveUpdate(camera, lightPos, glfwGetTime());
		cube.Draw(36);

		//

		//Light cube

		lightCube.Bind();
		lightCube.MoveUpdate(camera, lightPos, glfwGetTime());
		lightCube.Draw(36);

		//*/

		zap::ShowWireFrame(window.isKeyPressed(zap::Key::F10));
		//zap::ClearBackground(0.2f, 0.3f, 0.3f, 1.0f);
		//here starts current VAO for current program draw
		//here draw ends

		text.SetContent(std::to_string(std::round(window.GetFPS())));

		text.Draw();

		button.Update();
		button.Draw();

		window.Update();
		window.Draw();

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // PR

		rotation += 1.0f;
	}

	zap::Delete();
	return 0;

}








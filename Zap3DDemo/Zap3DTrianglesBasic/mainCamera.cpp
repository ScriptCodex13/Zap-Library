#include "enabler.h"
#ifdef __MAIN_CAPERA_CPP__

// Just a example

#include <Zap.h>
#include <Mesh.h>
#include <Camera.h>
#include <iostream>
#include <vector>

#include <glm/glm.hpp>                  //basic glm math functions
#include <glm/gtc/matrix_transform.hpp> //matrix functions
#include <glm/gtc/type_ptr.hpp>         //convert glm types to opengl types

const char* vertexCameraShaderSource = R"glsl(#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aColor;
layout(location = 2) in vec2 aTexCoord;

uniform mat4 model;      //takes local coordinates for thing and moves it into world coordinates
uniform mat4 view;       //moves world space objects around based on camera
uniform mat4 projection; //converts values to normalised device coordinates (use sweet math for perspective)

out vec3 ourColor;
out vec2 TexCoord;

void main()
{
	gl_Position = projection * view * model * vec4(aPos, 1.0);
	ourColor = aColor;
})glsl";
const char* fragmentCameraShaderSource = R"glsl(#version 330
out vec4 FragColor;
  
in vec3 ourColor;
in vec2 TexCoord;

uniform sampler2D ourTexture;

void main()
{
    FragColor = texture(ourTexture, TexCoord);
})glsl";


float lastFrame = 0.f;
float deltaTime = 0.f;
void processInputs(GLFWwindow* window)
{
	using zap::Camera;
	using zap::config::camera;
	deltaTime = (float)glfwGetTime() - lastFrame;
	lastFrame += deltaTime;
	//CAMERA STUFF WASD movement
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.onKeyboard(Camera::Movement::FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.onKeyboard(Camera::Movement::BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.onKeyboard(Camera::Movement::LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.onKeyboard(Camera::Movement::RIGHT, deltaTime);
}
inline bool firstMouse = true;
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	using zap::config::camera;
	using zap::config::lastX;
	using zap::config::lastY;
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	//work out pixel difference between lastX and Y and current mouse X and Y
	float xOffset = xpos - lastX;
	float yOffset = lastY - ypos;//need to reverse to get correct movement

	lastX = xpos;
	lastY = ypos;

	camera.onMouseMove(xOffset, yOffset);

}
void scroll_callback(GLFWwindow* window, double xOffset, double yOffset)
{
	using zap::config::camera;
	camera.onMouseScroll(yOffset);//changes FOV of camera
}
int main()
{
	zap::Init();

	zap::Window zapWindow(1920, 1080, "Hello Window");

	zap::InitGlad();

	//zapWindow.SetFullscreen(true);
	//zapWindow.UpdateViewport();

	std::vector<float> vertices =
	{
		// positions          // colors           // texture coords
		 0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right 
		 0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right  This causes the bug
		-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
		-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // top left		And also this

	};

	std::vector<unsigned int> indices =
	{
		0, 1, 3,
		1, 2, 3
	};


	//Mesh

	zap::Mesh2D mesh(&vertices, &indices);

	mesh.SetVertexShaderSource(vertexCameraShaderSource);
	mesh.SetFragmentShaderSource(fragmentCameraShaderSource);

	mesh.SetAttribPointer(0, 3, 8, 0);
	mesh.SetAttribPointer(1, 3, 8, 3);
	mesh.SetAttribPointer(2, 2, 8, 6);

	unsigned int texture0Id = mesh.AddTexture(0, "textures/texture.png", zap::TextureFilters::NEAREST, zap::MipmapSettings::LINEAR_MIPMAP_LINEAR, zap::TextureWrapping::CLAMP_TO_BORDER).i_id;

	mesh.Finish();
	mesh.bind();

	unsigned int modelLocationId      = mesh.GetUniformLocation("model");
	unsigned int viewLocationId       = mesh.GetUniformLocation("view");
	unsigned int projectionLocationId = mesh.GetUniformLocation("projection");
	//
	float lastFrame = (float)glfwGetTime();
	float deltaTime = 0.f;
	const float& currentTime = lastFrame;  //time since last frame //time of last frame
	glm::vec3 pos(0.0f, 0.0f, 0.0f);

	zapWindow.UpdateViewport(); //This is a set callback. Once set == forever set
	GLFWwindow* window = zapWindow.GetNativeWindow();
	glfwSetCursorPosCallback (window, mouse_callback);
	glfwSetScrollCallback    (window, scroll_callback);
	//glfwSetKeyCallback(window, key_callback);

	using zap::config::camera;
	glm::mat4 model      = glm::translate(glm::mat4(1.0), glm::vec3(0.1f, -0.1f, 0.0f));
	glm::mat4 view       = camera.view();
	glm::mat4 projection = camera.perspective();
	mesh.bind();
	//initialize uniforms
	glUniformMatrix4fv (modelLocationId,      1, GL_FALSE, glm::value_ptr(model));
	glUniformMatrix4fv (projectionLocationId, 1, GL_FALSE, glm::value_ptr(projection));
	//glUniformMatrix4fv (viewLocationId,       1, GL_FALSE, glm::value_ptr(view));
	while (zapWindow.Open())
	{
		if (zapWindow.isKeyPressed(zap::Key::ESC))
			zapWindow.Close();

		deltaTime = (float)glfwGetTime() - lastFrame;

		lastFrame += deltaTime;
		//zapWindow.StartTime();
		//model      = glm::translate(glm::mat4(1.0), glm::vec3(0.1f, -0.1f, 0.0f));
		view       = camera.view();
		//projection = camera.perspective();

		//model = glm::translate(glm::mat4(1.0), pos);
		//model = glm::rotate   (model, currentTime, glm::vec3(0.0f, 1.0f, 0.0f));


		//from here draw starts
		//there starts general draw
		glClear(GL_DEPTH_BUFFER_BIT); // PR
		zapWindow.ShowWireFrame(zapWindow.isKeyPressed(zap::Key::F10));
		zapWindow.ClearBackground(zap::BackgroundColors::BLACK);
		//here starts current VAO for current program draw
		mesh.bind(); //set current context before any draw routines, it prevents mess in more complex programs
		//update uniforms
		//glUniformMatrix4fv ( modelLocationId,       1,  GL_FALSE, glm::value_ptr (model)  );
		glUniformMatrix4fv ( viewLocationId,        1,  GL_FALSE, glm::value_ptr (view)  );
		//glUniformMatrix4fv ( projectionLocationId,  1,  GL_FALSE, glm::value_ptr (projection)  );
		mesh.UseTexture(texture0Id); //return false if texture not found
		mesh.Draw();
		//here draw ends

		zapWindow.SetTitle(std::to_string(zapWindow.GetDelta()));

		zapWindow.Update();
		zapWindow.Draw();

		//std::cout << zapWindow.GetFPS() << std::endl;
	}

	zap::Delete();

}


#endif
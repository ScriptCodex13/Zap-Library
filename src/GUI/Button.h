#pragma once

#ifndef BUTTON_H
#define BUTTON_H

#include <memory>
#include <string>
#include <vector>

#include "../Graphics/Mesh.h"
#include "../GUI/GUIInterfaces.h"

#include <glad/glad.h>

#include "TextRenderer.h"

namespace zap
{
	//this is the stub, to prevent cascade interface changes whenever listener is engineered
	class IUIButtonEventListenerImpl : public IUIButtonEventListener
	{
		virtual void SetContainer(IUIButtonContainer* container) { }
		virtual bool HitTest(double x, double y) { return false; }
		virtual bool OnMouseMove(double x, double y) { return false; }
		virtual bool OnLMouseButtonDown(double x, double y) { return false; }
		virtual bool OnLMouseButtonUp(double x, double y) { return false; }
		virtual bool OnMouseEnter(double x, double y) { return false; }
		virtual bool OnMouseLeave(double x, double y) { return false; }
		virtual bool OnPress(double x, double y, int key) { return false; }
		virtual bool OnRelease(double x, double y, int key) { return false; }

		virtual bool OnLMouseClick(double x, double y) { return false; }
		virtual bool OnRMouseClick(double x, double y) { return false; }
		virtual bool OnMMouseClick(double x, double y) { return false; }
	};
	class Button : zap::Mesh, public IUIComponent
	{
		class EventListener : public IUIButtonEventListenerImpl
		{
			Button* button = nullptr;
			IUIButtonContainer* pButtonContainer = nullptr;
		public:
			EventListener(Button* btptr) : button(btptr) {}
			virtual void SetContainer(IUIButtonContainer* container) { pButtonContainer = container; }
			IUIButtonContainer* GetContainer() { return pButtonContainer; }
			virtual bool HitTest(double x, double y) {
				return zap::util::between(
					std::array<double, 2> {x, y},
					button->i_bounds);
			}
			virtual bool OnMouseMove(double x, double y)
			{
				std::wcout << "mouse move {" << x << ":" << y << "}" << std::endl;
				return false;
			}
			//when left mosue button is down.
			//attention!!! This is not the button click and should not trigger any action
			virtual bool OnLMouseButtonDown(double x, double y) {
				button->i_button_color = button->i_button_pressed_color;
				return false;
			}
			virtual bool OnLMouseButtonUp(double x, double y) {
				button->i_button_color = button->i_button_hover_color;
				return false;
			}
			virtual bool OnMouseEnter(double x, double y)
			{
				button->i_button_color = button->i_button_hover_color;
				std::wcout << "mouse enter {" << x << ":" << y << "}" << std::endl;
				return true;
			}
			virtual bool OnMouseLeave(double x, double y) {
				std::wcout << "mouse leave {" << x << ":" << y << "}" << std::endl;
				button->i_button_color = button->i_button_default_color;
				return true;
			}
			virtual bool OnPress(double x, double y, int key) { return false; }
			virtual bool OnRelease(double x, double y, int key) { return false; }

			virtual bool OnLMouseClick(double x, double y) {
				std::wcout << "mouse click {" << x << ":" << y << "}" << std::endl;
				return true;
			}

			//std::array<float, 4> getArray() { return button->i_bounds; }
		};
		EventListener listener;
	public:
		//We are supposed to use a portable container interface instead of a platform dependent window
		Button(
			IUIButtonContainer* buttonContainer,
			const char* button_text = nullptr, const char* const button_text_font_path = nullptr);
		Button(
			IUIButtonContainer* buttonContainer,
			const std::array<float, 4>& _bounds,
			const char* button_text = nullptr, 
			const char* const button_text_font_path = nullptr);
		~Button();
		//implement IUIButtonEventListener interface
		IUIButtonEventListener* GetUIListener() { return &listener; }
		//end implement IUIButtonEventListener interface


		int LoadTexture(unsigned int id, const char* const  path_to_texture, zap::TextureFilter filter, int shader_location = 1);
		void FinishMesh();
		void UseTextureShaders(bool state);
		void UseTextureShaders(const char* vertex_shader_source, const char* fragment_shader_source);

		zap::Text* GetTextObject();
		void UpdatePosition();

		void SetGlSize(float width, float height);
		void SetGlSize(std::array<float, 2>& width_height);
		void SetGlWidth(float width);
		void SetGlHeight(float height);
		void SetGlPosition(float xmin, float ymin); // Equivalent to move window to XY pos. Sets the xmin/ymin = bottom/left point to the given coordinates
		void SetGlPosition(std::array<float, 2>& gl_xy_min); // Equivalent to move window to XY pos. Sets the top left point to the given coordinates
		void SetGlPosition(float xmin, float ymin, float xmax, float ymax); // Fully sets position xmin/ymin=bottom/left  xmax/ymax=top/right
		void SetGlPosition(std::array<float, 2>& gl_xy_min, std::array<float, 2>& gl_xy_max); // Fully sets position xmin/ymin=bottom/left  xmax/ymax=top/right
		void SetGlPosition(std::array<float, 4>& gl_xy_min_xy_max); // Fully sets position xmin/ymin=bottom/left  xmax/ymax=top/right
		void SetColor(float RED, float GREEN, float BLUE, float ALPHA);
		
		void ActivateTexture(bool state);
		void UseText(bool state);
		void SetButtonText(const std::string text);
		void SetTextOffset(float x_offset, float y_offset);
		void SetTextColor(zap::TextColors color);
		void SetTextColor(float RED, float GREEN, float BLUE);
		//BUTTON is a billboard
		//   billboards by very definition do not need a Z coordinate
		//   billboards are not subject to MVP transformations
		//void SetZCoordinate(); // Don't know if you need it

		void Update();
		void Draw(int texture_id = 0);

	private:
		bool i_use_texture = false;
		bool texture_attribute_ptr_set = false;
		bool i_use_text = true;
		bool i_font_missing_flag = false;

		bool i_use_text_offset = false;

		std::unique_ptr<zap::Text> i_button_text;

		std::array<float, 2> i_text_offset = { 0.0f, 0.0f };
	private:
		//TODO: add getters/setters and all fancy fads
		glm::vec4 i_button_default_color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
		glm::vec4 i_button_hover_color = glm::vec4(1.0f, 1.0f, 1.0f, 0.8f);
		glm::vec4 i_button_pressed_color = glm::vec4(1.0f, 1.0f, 1.0f, 0.6f);
		//this actually to be used in shader
		glm::vec4 i_button_color = i_button_default_color;

	protected:
		unsigned int i_moveto_uniform_location = -1;
		unsigned int i_button_color_location = -1;

		std::array<float, 4> i_bounds; // x_min, x_max, y_min, y_max
		//std::array<int, 2> i_window_original_size {};

		const char* i_vertex_shader_source =
			R"glsl(
					#version 330 core
					layout(location = 0) in vec3 aPos;
					uniform mat4 moveto;
					void main()
					{
						gl_Position = moveto * vec4(aPos, 1.0);
					}
			)glsl";

		const char* i_fragment_shader_source =
			R"glsl(
					#version 330 core
					out vec4 fragColor;
					uniform vec4 button_color;
					void main()
					{
						fragColor = button_color;
					}
			)glsl";

		const char* i_vertex_shader_source_t =
			R"glsl(
					#version 330 core
					layout(location = 0) in vec3 aPos;
					layout(location = 1) in vec2 aTexCoord;
					uniform mat4 moveto;
					out vec2 TexCoord;
					void main()
					{
						gl_Position = moveto * vec4(aPos, 1.0);
						TexCoord = aTexCoord;
					}
			)glsl";

		const char* i_fragment_shader_source_t =
			R"glsl(
					#version 330 core
					out vec4 FragColor;
					in vec2 TexCoord;
					uniform vec4 button_color;
					uniform sampler2D texture1;
					void main()
					{
						FragColor = button_color * texture(texture1, TexCoord);
					}
			)glsl";

	};
	

	class ButtonText : public Mesh, public IUIComponent
	{
	protected:
		std::array<float, 4> i_bounds; // x_min, x_max, y_min, y_max
		class EventListener: public IUIButtonEventListenerImpl
		{
			ButtonText* buttonText = nullptr;
			IUIButtonContainer* pButtonContainer = nullptr;
		public:
			EventListener(ButtonText* btptr) : buttonText(btptr) {}
			virtual void SetContainer(IUIButtonContainer* container) { pButtonContainer = container; }
			virtual bool HitTest(double x, double y) {
				return zap::util::between(
					std::array<double, 2> {x, y},
					buttonText->i_bounds);
			}
			virtual bool OnMouseMove(double x, double y)
			{
				std::wcout << "mouse move {" << x << ":" << y << "}" << std::endl;
				return false;
			}
			//when left mosue button is down.
			//attention!!! This is not the button click and should not trigger any action
			virtual bool OnLMouseButtonDown(double x, double y) { 
				buttonText->i_button_color = buttonText->i_button_pressed_color;
				return false; 
			}
			virtual bool OnLMouseButtonUp(double x, double y) {
				buttonText->i_button_color = buttonText->i_button_hover_color;
				return false;
			}
			virtual bool OnMouseEnter(double x, double y)
			{
				buttonText->i_button_color = buttonText->i_button_hover_color;
				std::wcout << "mouse enter {" << x << ":" << y << "}" << std::endl;
				return true;
			}
			virtual bool OnMouseLeave(double x, double y) {
				std::wcout << "mouse leave {" << x << ":" << y << "}" << std::endl;
				buttonText->i_button_color = buttonText->i_button_default_color;
				return true;
			}
			virtual bool OnPress(double x, double y, int key) { return false; }
			virtual bool OnRelease(double x, double y,int key) { return false; }

			virtual bool OnLMouseClick(double x, double y) {
				std::wcout << "mouse click {" << x << ":" << y << "}" << std::endl;
				return true;
			}
			std::array<float, 4> getArray() { return buttonText->i_bounds; }
		} ;
		EventListener listener;

	public:

		ButtonText(const std::string button_text = "", const std::string button_text_font_path = "");
		ButtonText(const std::array<float, 4>& bounds, const std::string button_text = "", const std::string button_text_font_path = "");

		~ButtonText();
		//implement IUIButtonEventListener interface
		IUIButtonEventListener* GetUIListener() { return &listener; }

		//no interface methods
		zap::Text* GetTextObject();
		void UpdatePosition();

		void SetGlSize(float width, float height);
		void SetGlSize(std::array<float, 2>& width_height);
		void SetGlWidth(float width);
		void SetGlHeight(float height);
		void SetGlPosition(float xmin, float ymin); // Equivalent to move window to XY pos. Sets the xmin/ymin = bottom/left point to the given coordinates
		void SetGlPosition(std::array<float, 2>& gl_xy_min); // Equivalent to move window to XY pos. Sets the top left point to the given coordinates
		void SetGlPosition(float xmin, float ymin, float xmax, float ymax); // Fully sets position xmin/ymin=bottom/left  xmax/ymax=top/right
		void SetGlPosition(std::array<float, 2>& gl_xy_min, std::array<float, 2>& gl_xy_max); // Fully sets position xmin/ymin=bottom/left  xmax/ymax=top/right
		void SetGlPosition(std::array<float, 4>& gl_xy_min_xy_max); // Fully sets position xmin/ymin=bottom/left  xmax/ymax=top/right
		void SetColor(float RED, float GREEN, float BLUE, float ALPHA);

		void SetButtonText(const std::string text);
		void SetTextOffset(float x_offset, float y_offset);
		void SetTextColor(zap::TextColors color);
		void SetTextColor(float RED, float GREEN, float BLUE);

		void Update();
		void Draw(int texture_id = 0);



		size_t printf(const std::wstring content, ...);

	private: // Private functions
		std::vector<wchar_t> wprintf_buffer;

	private:
		zap::TextureText text;

		glm::vec4 i_button_default_color = glm::vec4(0.0f, 1.0f, 0.0f, 0.8f);
		glm::vec4 i_button_hover_color = glm::vec4(0.0f, 0.9f, 0.0f, 0.8f);
		glm::vec4 i_button_pressed_color = glm::vec4(0.0f, 0.7f, 0.0f, 0.8f);
		glm::vec4 i_button_color = i_button_default_color;
		unsigned int textureHash;
	protected:
		unsigned int i_moveto_uniform_location;
		unsigned int i_button_color_location;

	protected:

		const char* i_vertex_shader_source =
			R"glsl(
					#version 330 core
					layout(location = 0) in vec3 aPos;
					layout(location = 1) in vec2 aTexCoord;

					uniform mat4 moveto;

					out vec2 TexCoord;

					void main()
					{
						gl_Position = moveto * vec4(aPos, 1.0);
						TexCoord = aTexCoord;
					}
			)glsl";

		const char* i_fragment_shader_source =
			R"glsl(
					#version 330 core
					out vec4 FragColor;
					in vec2 TexCoord;

					uniform vec4 button_color;
					uniform sampler2D ourTexture;

					void main()
					{
                        vec4 texColor = texture(ourTexture, TexCoord);
						FragColor = button_color + texColor;
					}
			)glsl";

	};

}

#endif
#include "enabler.h"
#ifdef SAMPLE_MAIN_UI_TEXTURE_TEXT_BUTTON_CPP

// Just a example
// PR = Prototyping -> only for testing 
#include <Zap.h>
#include <iostream>
#include <array>

template <typename T> class window_invoker : public zap::util::callback_invoker<T>
{
public:
	inline window_invoker(T _callback) : callback_invoker(_callback) {}
	template<typename A1> void operator () (A1& a1) { callback(a1); }
};
window_invoker  cbi([](zap::Window& window) {
	if (window.isKeyPressed(zap::Key::ESC))
	{
		window.Close();
	}
});

int main()
{
	//ZAP_DISABLE_OUTPUTS(true);

	zap::Init(4, 6);

	zap::Window window(1920, 1080, "Hello Window");
	zap::util::scope_guard zapDeleter (zap::Delete);

	zap::InitGlad();


	glEnable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Window settings

	window.UpdateViewport(true);
	window.SetVSync(true);
	zap::Enable(zap::Instruction::DEPTH);
	zap::Enable(zap::Instruction::ANTIALIASING);


	zap::ButtonText buttonText(std::array<float, 4> {-0.5f, 0.6f, 0.2f, 0.8f }, L"Button", "C:/Windows/Fonts/arial.ttf");

	//
	//buttonText.SetTextOffset(0.31f, 0.075f);
	//buttonText.SetTextColor(zap::TextColors::RED);

	////buttonText.GetTextObject()->SetTextOffset(0.31f, 0.075f);
	////buttonText.GetTextObject()->SetTextColor(zap::TextColors::PURPLE);
	////buttonText.GetTextObject()->GenerateCharacters();
	////buttonText.SetGlPosition(std::array<float, 2> {- 0.9f, 0.7f});
	////buttonText.SetGlPosition(-0.9f, 0.7f, 0.2, 0.8 );
	////buttonText.SetGlSize(std::array<float, 2> {0.1f, 0.1f});
	////buttonText.SetGlWidth(0.7);
	////buttonText.SetGlHeight(0.28);
	////buttonText.SetGlHeight(0.10);
	//buttonText.SetColor(1.0f, 1.0f, 1.0f, 1.0f);

	glDisable(GL_DEPTH_TEST);

	window.GetMouseGlPosition();


	window.AddButtonEventHandler (buttonText.GetUIListener());
	float startTime = glfwGetTime(), prevTime = startTime;

	while (window.Open())
	{
		float time = glfwGetTime();
		float deltaTime = time - prevTime;
		prevTime = time;
		cbi(window);
		zap::ClearBuffers();
		glClear(GL_COLOR_BUFFER_BIT);
		zap::ClearBackground(0.2f, 0.3f, 0.3f, 1.0f);

		window.InvokeHandlers();


		buttonText.Bind();

		buttonText.MoveTextDelta(deltaTime / 4.f, 0.f);
		buttonText.printf(L"Button g %f   ", glfwGetTime() * glfwGetTime() * glfwGetTime() * glfwGetTime());
		buttonText.Draw();

		window.Update();
		window.Draw();
	}

	return 0;

}


#endif
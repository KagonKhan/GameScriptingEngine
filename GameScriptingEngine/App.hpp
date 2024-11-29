#include "Window.hpp"
#include <spdlog/spdlog.h>

class App {
public:
	void start() {
		while (!glfwWindowShouldClose(window) && isRunning) {

			glfwPollEvents();

			window.startFrame();

			renderApp();

			window.endFrame();
		}
	}

private:
	void renderApp() {

		if (ImGui::Begin("GameScriptingEngine", &isRunning)) {

		}
		else {
			spdlog::critical("Cannot write to main window!");
		}
		ImGui::End();
	}

	Window window;
	bool isRunning{ true };
};
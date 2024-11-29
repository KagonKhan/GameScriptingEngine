#pragma once
#include "Window.hpp"
#include "AutoClicker.hpp"
#include <spdlog/spdlog.h>
#include "Input.hpp"


class App {
public:
	void start() {

		while (!glfwWindowShouldClose(window) && isRunning) {
			glfwPollEvents();

			window.startFrame();

			renderApp();

			window.endFrame();
		}

		PostQuitMessage(0);


	}

private:
	struct DPIFixer {
		void operator()() {
			if (windowDPI == 0.0f) {
				windowDPI = ImGui::GetWindowDpiScale();
			}

			if (float current_dpi = ImGui::GetWindowDpiScale(); current_dpi != windowDPI) {
				spdlog::debug("DPI changed, resizing the window");
				auto size = ImGui::GetWindowSize();
				ImGui::SetWindowSize(size + ImVec2{ 1.f, 1.f });
				ImGui::SetWindowSize(size - ImVec2{ 1.f, 1.f });
				windowDPI = ImGui::GetWindowDpiScale();
			}

		}
	public:
		float windowDPI{ 0.0f };
	} dpiFixer;

	void renderApp() {
		static bool demo = false;
		if (ImGui::Begin("GameScriptingEngine", &isRunning)) {
			dpiFixer();

			ImGui::Checkbox("AutoClicker", &clicker.isVisible);
			
			if (clicker.isVisible) {
				clicker.render();
			}

			ImGui::Checkbox("Demo", &demo);
			if (demo) ImGui::ShowDemoWindow();

		}
		else {
			spdlog::critical("Cannot write to main window!");
		}
		ImGui::End();
	}


	Window window;
	bool isRunning{ true };

	AutoClicker clicker;
};
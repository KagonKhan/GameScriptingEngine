#pragma once
#include <GLFW/glfw3.h>

class Window {
public:
	Window();
	~Window();

	operator GLFWwindow* () const { return window; }

	void startFrame();
	void endFrame();

private:
	void setupGLFWWindow();
	void setupImGui();

private:
	GLFWwindow* window{ nullptr };
	const char* glsl_version = "#version 150";
};

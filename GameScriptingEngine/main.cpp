#include <print>
#include "App.hpp"




/*
	IDEAS:
	- OCR, ImageSearch (edges?) 
	- AI (NEAT algorithms support)
	- MOUSE, KEYBOARD facilities
	- SHORTCUTS to scripts
	- MINIMIZE TO TRAY
	- SCRIPTING engine (record mouse, kb, steps)
	- TIMERS

*/



int main() {
	spdlog::set_level(spdlog::level::debug);


	App app;

	app.start();

}
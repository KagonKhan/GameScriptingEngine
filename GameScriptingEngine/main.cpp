#include "App.hpp"
#include <spdlog/spdlog.h>




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

void initialize() { 
    spdlog::set_level(spdlog::level::debug);

}

int main() {
    initialize();

	App app;



	app.start();
}
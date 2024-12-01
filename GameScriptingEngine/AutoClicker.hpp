#pragma once
#include <algorithm>
#include <spdlog/spdlog.h>
#include "Input.hpp"
#include <ImGui/imgui_stdlib.h>

template <typename T>
concept DurationType = requires {
	[] <class Rep, class Period>(std::type_identity<std::chrono::duration<Rep, Period>>) {}(
		std::type_identity<T>());
};


// TODO: assert alignment works
struct Interval {
	int minutes{ 0 };
	int seconds{ 0 };
	int milli{ 0 };
	int micro{ 0 };


	std::chrono::microseconds asMicroseconds() {
		return
			std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::minutes{ minutes }) +
			std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::seconds{ seconds }) +
			std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::milliseconds{ milli }) +
			std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::microseconds{ micro });
	}
};


// Should this have multiple instances?
class AutoClicker {
public:
	bool isVisible = true;

	

	AutoClicker() {
		
		Input::Keyboard::keybinds.emplace(Input::Keyboard::Key::Q, [this] {
			isRunning? stopClicking() : startClicking();
		});
	}

	~AutoClicker() {
		stopClicking();
	}

	void render() {
		ImGui::Separator();
		// TODO: make this prettier without so much stupid code - youtube tutorials on imgui style or something

		ImGui::SetCursorScreenPos(ImGui::GetCursorScreenPos() + ImVec2(20, 0));

		std::string current_preview = Input::Keyboard::keyName(keybind);
		if (ImGui::BeginCombo("Keybind", current_preview.c_str(), ImGuiComboFlags_WidthFitPreview)) {
			for (const auto& [value, name] : magic_enum::enum_entries< Input::Keyboard::Key>()) {
				bool is_selected = value == keybind;
				if (ImGui::Selectable(Input::Keyboard::keyName(value).c_str(), is_selected)) {
					Input::Keyboard::keybinds.erase(keybind);
					keybind = value;
					Input::Keyboard::keybinds.emplace(keybind, [this] {
						isRunning ? stopClicking() : startClicking();
						});
				}

				if (is_selected) {
					ImGui::SetItemDefaultFocus();
				}
			}


			ImGui::EndCombo();
		}
		ImGui::Spacing();
		IntervalWidget();


		ImGui::SetCursorScreenPos(ImGui::GetCursorScreenPos() + ImVec2(20, 0));
		ImGui::Text("Current duration in ms: {%llu}", interval.asMicroseconds().count());

		ImGui::Separator();
	}

	void IntervalWidget()
	{
		ImGui::Text("Interval: ");
		ImGui::PushItemWidth(25);
		ImGui::SetCursorScreenPos(ImGui::GetCursorScreenPos() + ImVec2(40, 0));

		ImGui::InputInt("min", &interval.minutes, 0); clamped(interval.minutes, 0, 59); ImGui::SameLine();
		ImGui::InputInt("sec", &interval.seconds, 0); clamped(interval.seconds, 0, 59); ImGui::SameLine();
		ImGui::PopItemWidth();
		ImGui::PushItemWidth(45);
		ImGui::InputInt("milli", &interval.milli, 0); clamped(interval.milli, 0, 1000); ImGui::SameLine();
		ImGui::InputInt("micro", &interval.micro, 0); clamped(interval.micro, 0, 1000);
		ImGui::PopItemWidth();
	}

private:
	Input::Keyboard::Key keybind{ Input::Keyboard::Key::Q };
	Interval interval;

	void clamped(int& value, int low, int high) {
		value = std::clamp(value, low, high);
	}

	// CV to pause the thread instead of stopping it?

	void startClicking() {
		if (interval.asMicroseconds().count() == 0) return;
		isRunning = true;
		spdlog::debug("{} started clicking.", TAG);
		clicker = std::thread([this] {
			while (isRunning) {
				Input::Mouse::Click();
				std::this_thread::sleep_for(interval.asMicroseconds());
			}
			});
	};
	void stopClicking() {
		spdlog::debug("{} stopped clicking.", TAG);
		isRunning = false;
		clicker.join();
	}

	std::thread clicker;
	std::atomic<bool> isRunning;

	inline static const std::string TAG{ "[AutoClicker]" };
};




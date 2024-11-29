#pragma once
#include <algorithm>
#include <spdlog/spdlog.h>
#include "Input.hpp"

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

	template<DurationType T>
	T to() {
		return
			std::chrono::duration_cast<T>(std::chrono::minutes{ minutes }) +
			std::chrono::duration_cast<T>(std::chrono::seconds{ seconds }) +
			std::chrono::duration_cast<T>(std::chrono::milliseconds{ milli }) +
			std::chrono::duration_cast<T>(std::chrono::microseconds{ micro });
		
	}
};



class AutoClicker {
public:
	bool isVisible = true;

	AutoClicker() {
		
		Input::Keyboard::keybinds.emplace(Input::Keyboard::Key::Q, [this] {
			isClicking = !isClicking;
			isClicking ? startClicking() : stopClicking();
		});
	}

	void render() {

		ImGui::Separator();
		ImGui::SetCursorScreenPos(ImGui::GetCursorScreenPos() + ImVec2(20, 0));
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
		ImGui::SameLine();

		if (ImGui::Button(isClicking ? "Stop" : "Start", ImVec2(60, 0))) {
			isClicking = !isClicking;
			isClicking ? startClicking() : stopClicking();
		}

		ImGui::SetCursorScreenPos(ImGui::GetCursorScreenPos() + ImVec2(20, 0));
		ImGui::Text("Current duration in ms: {%llu}", interval.to<std::chrono::microseconds>().count());

		ImGui::Separator();
	}

private:
	Interval interval;

	void clamped(int& value, int low, int high) {
		value = std::clamp(value, low, high);
	}

	bool isClicking{ false };

	void startClicking() {
		spdlog::debug("{} started clicking.", TAG);
	};
	void stopClicking() {
		spdlog::debug("{} stopped clicking.", TAG);
	}


	inline static const std::string TAG{ "[AutoClicker]" };
};




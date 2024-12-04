#pragma once
#include "RectangleMarker.hpp"
#include "app/App.hpp"
#include "components/MonitorPixelReader.hpp"

#include <ImGui/imgui_stdlib.h>
#include <opencv2/imgcodecs.hpp>
#include <spdlog/spdlog.h>

// TODO: for now no need to be a class.
class ScreenImageSaver {
private:
    static constexpr char const* const TAG{"[ScreenImageSaver]"};

public:
    void save(cv::Mat const& image) {
        if (ImGui::Button("Save image")) {
            filename.clear();
            ImGui::OpenPopup("Save image popup");
        }


        ImVec2 center = ImGui::GetMainViewport()->GetCenter();
        ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
        if (ImGui::BeginPopupModal("Save image popup", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
            popupContents(image);
        }
    }
    // TODO: separator text
    // TODO: querying window status iswindowfocused / iswindowhovered
private:
    void popupContents(cv::Mat const& image) {
        ImGui::InputText("File name", &filename);
        ImGui::Separator();

        if (ImGui::Button("OK", buttonSize)) {
            bool result = cv::imwrite("W:\\repos\\GameScriptingEngine\\resources\\" + filename + ".png", image);
            // TODO: error handling...
            ImGui::CloseCurrentPopup();
        }

        ImGui::SetItemDefaultFocus();
        ImGui::SameLine();

        if (ImGui::Button("Cancel", buttonSize)) {
            ImGui::CloseCurrentPopup();
        }
        ImGui::EndPopup();
    }

private:
    std::string filename{};
    ImVec2      buttonSize{120, 0};
};
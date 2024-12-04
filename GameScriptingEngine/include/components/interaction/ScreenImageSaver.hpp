#pragma once
#include "RectangleMarker.hpp"
#include "app/App.hpp"
#include "components/MonitorPixelReader.hpp"

#include <opencv2/imgcodecs.hpp>
#include <spdlog/spdlog.h>


#include <ImGui/imgui_stdlib.h>

class ScreenImageSaver {
private:
    static constexpr char const* const TAG{"[ScreenImageSaver]"};

public:
    void render() {
        static bool isMarking = false;
        if (ImGui::Button("Capture screen")) {
            isMarking = true;
            spdlog::debug("Beginning to capture screen.");
        }

        if (isMarking) {
            spdlog::info("User is choosing area");
            isMarking = !marker.markArea();
            return;
        }

        if (auto rect = marker.get(); rect.GetArea() > 1.0f) {
            spdlog::warn("user chose area");
            reader.updateRegion(rect);
            reader.updateImage();
            static int frame{0}; // TODO: the issue here, is that marker draws on the screen, and that marker is seen on the image
            // TODO: an alternative, would be to make a big widget to operate on screen,
            // and instead of nesting components, just accept the results.
            if (frame++ < 2) {
                return;
            }
            
            image = reader.getImage();
            marker.reset();
        }

        if (image.has_value()) {
            if (App::IsOverlayMode())
                App::FlipMode();
            spdlog::critical("Showing popup");
            ImGui::OpenPopup("Save image");

            ImVec2 center = ImGui::GetMainViewport()->GetCenter();
            ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

            if (ImGui::BeginPopupModal("Save image", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
                static std::string filename;
                ImGui::InputText("File name", &filename);

                ImGui::Separator();


                if (ImGui::Button("OK", ImVec2(120, 0))) {
                    try {
                        bool result = cv::imwrite("W:\\repos\\GameScriptingEngine\\resources\\" + filename + ".png",
                                                  image.value());

                        spdlog::critical("Succeeded in writing?: {}", result);
                    } catch (const cv::Exception& ex) { spdlog::critical("Error: {}", ex.what()); }
                    filename.resize(0);
                    image.reset();
                    ImGui::CloseCurrentPopup();
                }

                ImGui::SetItemDefaultFocus();
                ImGui::SameLine();

                if (ImGui::Button("Cancel", ImVec2(120, 0))) {
                    filename.resize(0);
                    image.reset();

                    ImGui::CloseCurrentPopup();
                }
                ImGui::EndPopup();
            }
        }
    }
    // TODO: slider text
    // TODO: querying window status iswindowfocused / iswindowhovered

private:
    MonitorPixelReader     reader{};
    RectangleMarker        marker{};
    std::optional<cv::Mat> image{};
};
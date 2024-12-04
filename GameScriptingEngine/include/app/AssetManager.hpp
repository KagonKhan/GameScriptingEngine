#pragma once
#include <filesystem>
#include <iostream>
#include <opencv2/core/mat.hpp>
#include <opencv2/imgcodecs.hpp>
#include <spdlog/spdlog.h>
#include <string>
#include <unordered_map>

// File watcher / dir watcher?
// https://learn.microsoft.com/en-us/windows/win32/api/fileapi/nf-fileapi-findfirstchangenotificationa Memory pool /
// arena what's the best data type to hold these... cv::Mat for now. class for AssetImage?
enum class Asset : int {

};


class AssetManager {
private:
    static constexpr char const* const TAG{"[AssetManager]"};

public:
    static constexpr char const* const PATH{R"(W:\repos\GameScriptingEngine\resources\)"};

    static void ReadAll() {
        std::filesystem::path path{PATH};

        for (auto const& file : std::filesystem::directory_iterator{path}) {
            const std::string filename = file.path().filename().string();
            spdlog::critical("Reading file {} with path {}", filename, file.path().string());

            if (images.contains(filename))
                continue;

            Add(filename, file.path());
        }
    }

    static void Add(std::string const& name, std::filesystem::path const& path) {
        cv::Mat mat = cv::imread(path.string());
        spdlog::critical("Image info: channels {}, depth {}", mat.channels(), mat.depth());
        images.emplace(name, mat);
    }

    static cv::Mat Retrieve(std::string const& name) { return images[name]; }

private:
    inline static std::unordered_map<std::string, cv::Mat> images;
    // todo: memory pool, arena
};

// This is cool and all but doesn't support runtime

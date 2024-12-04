#pragma once

#include "app/App.hpp"

#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>


#include <spdlog/spdlog.h>


// TODO: template matching works, though im not sure about the fuzziness.
// TODO: make sure the gray scale conversion is absolutely necessary.
// TODO: asset manager with dynamic loading
// TODO: no need for this to be a class.
class TemplateMatcher {
private:
    inline static constexpr char const* const TAG{"[TemplateMatcher]"};

public:
    struct MatchResult {
        ImVec2 position;
        ImVec2 size;
    };


    static std::optional<MatchResult> templateMatch(const cv::Mat& screen, const cv::Mat& target,
                                                    double threshold = 0.3) {
        cv::Mat gray_screen, gray_target;
        cv::cvtColor(screen, gray_screen, cv::COLOR_BGRA2GRAY);
        cv::cvtColor(target, gray_target, cv::COLOR_BGRA2GRAY);

        try {
            cv::Mat result;
            cv::matchTemplate(gray_screen, gray_target, result, cv::TM_CCOEFF_NORMED);

            double    min_val, max_val;
            cv::Point min_loc, max_loc;

            cv::minMaxLoc(result, &min_val, &max_val, &min_loc, &max_loc);

            if (max_val >= threshold) {
                spdlog::debug("{} Found the target", TAG);
                return MatchResult{.position = ImVec2{static_cast<float>(max_loc.x), static_cast<float>(max_loc.y)},
                                   .size = ImVec2{static_cast<float>(target.cols), static_cast<float>(target.rows)}};
                // TODO: Extend imgui for <int> types? avoid unnecessary casting here.
            }
            spdlog::critical("{} confidence", max_val);

        } catch (...) {
            spdlog::critical("FAILURE");
        } // TODO: what to do with exceptions? forward them so imgui can parse?

        spdlog::debug("{} Target not found", TAG);
        return std::nullopt;
    }
};


// TODO:
// - Feature-Based Matching
// - Perceptual Hashing
// - Deep Learning - hopefully won't be necessary

/*
 *#include <opencv2/opencv.hpp>
#include <iostream>
#include <opencv2/xfeatures2d.hpp>

void featureMatching(const cv::Mat& screen, const cv::Mat& target) {
    auto sift = cv::xfeatures2d::SIFT::create();
    std::vector<cv::KeyPoint> screenKeypoints, targetKeypoints;
    cv::Mat screenDescriptors, targetDescriptors;

    // Detect and compute keypoints and descriptors
    sift->detectAndCompute(screen, cv::noArray(), screenKeypoints, screenDescriptors);
    sift->detectAndCompute(target, cv::noArray(), targetKeypoints, targetDescriptors);

    // Match descriptors using FLANN
    cv::FlannBasedMatcher matcher;
    std::vector<cv::DMatch> matches;
    matcher.match(targetDescriptors, screenDescriptors, matches);

    // Filter matches based on distance
    double maxDist = 0, minDist = 100;
    for (const auto& match : matches) {
        double dist = match.distance;
        if (dist < minDist) minDist = dist;
        if (dist > maxDist) maxDist = dist;
    }

    std::vector<cv::DMatch> goodMatches;
    for (const auto& match : matches) {
        if (match.distance < 2 * minDist) {
            goodMatches.push_back(match);
        }
    }

    // Draw matches
    cv::Mat imgMatches;
    cv::drawMatches(target, targetKeypoints, screen, screenKeypoints, goodMatches, imgMatches);
    cv::imshow("Matches", imgMatches);
    cv::waitKey(0);
}

int main() {
    cv::Mat screen = cv::imread("screen.jpg", cv::IMREAD_COLOR);
    cv::Mat target = cv::imread("target.jpg", cv::IMREAD_COLOR);

    if (screen.empty() || target.empty()) {
        std::cerr << "Failed to load images!" << std::endl;
        return -1;
    }

    featureMatching(screen, target);

    return 0;
}

 */
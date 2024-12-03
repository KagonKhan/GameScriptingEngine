#pragma once

// TODO: asset manager with dynamic loading
//class TemplateMatcher {
//    void templateMatch(const cv::Mat& screen, const cv::Mat& target) {
//        cv::Mat result;
//        cv::matchTemplate(screen, target, result, cv::TM_CCOEFF_NORMED);
//
//        double    minVal, maxVal;
//        cv::Point minLoc, maxLoc;
//        cv::minMaxLoc(result, &minVal, &maxVal, &minLoc, &maxLoc);
//
//        if (maxVal > 0.8) { // Adjust threshold for fuzziness
//            std::cout << "Target found at: " << maxLoc << " with confidence: " << maxVal << std::endl;
//            // Optionally draw a rectangle around the detected region
//            cv::rectangle(screen, maxLoc, cv::Point(maxLoc.x + target.cols, maxLoc.y + target.rows),
//                          cv::Scalar(0, 255, 0), 2);
//            cv::imshow("Result", screen);
//            cv::waitKey(0);
//        } else {
//            std::cout << "Target not found." << std::endl;
//        }
//
//           cv::Mat img(height, width, CV_8UC3);
//        GetDIBits(hdcMem, hBitmap, 0, height, img.data, &bmi, DIB_RGB_COLORS);
//    }
//};


// TODO:
// - Feature-Based Matching
// - Perceptual Hashing
// - Deep Learning - hopefully won't be necessary

/*
 *#include <opencv2/opencv.hpp>
#include <opencv2/xfeatures2d.hpp>
#include <iostream>

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
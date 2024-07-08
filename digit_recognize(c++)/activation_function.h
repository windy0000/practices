#pragma once
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <vector>

cv::Mat softmax(const cv::Mat& x);

cv::Mat sigmoid(const cv::Mat& x);
std::vector<cv::Mat> get_sigmoid(std::vector<cv::Mat>& x);
cv::Mat sigmoid_gradient(cv::Mat x);
std::vector<cv::Mat> get_sigmoid_gradient(std::vector<cv::Mat>& x);
std::vector<cv::Mat> mul(std::vector<cv::Mat>& x, std::vector<cv::Mat>& y);